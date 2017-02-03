#include "optionparser.h"

/**
 * Constructs an OptionParser object.
 * @param definition    A list of available options.
 */
OptionParser::OptionParser(const QList<OptionDefinition> &definition)
{
    initializeDefinitionTable(definition);
}

/**
 * Parses the arguments given through the console.
 * @param argc              The argument count value from main().
 * @param argv              The char pointer array from main().
 * @param startIndex        A index from where to start in the array.
 * @return argumentTable    A QHash table with all found arguments and its values.
 */
Arguments OptionParser::parseOptions(const int argc, char** argv, const int startIndex)
{
    Arguments argumentTable;
    OptionDefinition lastOption;
    for (int index=startIndex; index<argc; ++index) {
        QString parameter(argv[index]);
        // Has long option?
        // ------------------------------------
        if (parameter.startsWith(QString("--"))) {
            if (lastOption.needValue()) {
                // New option found but the last option need a value.
                setErrorOptionNeedValue(lastOption.option());
            }
            QString longOption = getLongOptionString(parameter);
            QString valueString = getLongOptionValue(parameter);
            lastOption = findLongOption(longOption);
            if (! lastOption.isValid()) {
                setErrorUnknownOption(longOption);
                return argumentTable;
            }
            // Set option and value.
            setOption(argumentTable, lastOption, valueString);
            // Need a value or not.
            if (! valueString.isEmpty()) {
                if (!lastOption.takesValue()) {
                    setWarningUnexpectedValue(longOption, valueString);
                }
                // Option has a value and is satified.
                lastOption = OptionDefinition();
            }
            continue;
        }
        // Has one or more options?
        // ------------------------------------
        if (parameter.startsWith(QString("-"))) {
            if (lastOption.needValue()) {
                // New option found but last option need a value.
                setErrorOptionNeedValue(lastOption.option());
            }
            QList<char> optionSet;
            QList<char> needValueList;
            lastOption = parseForOptionSet(parameter, optionSet, needValueList);
            if (lastOption.isValid()) {
                if (! needValueList.isEmpty()) {
                    // Error. Some options need a value.
                    setErrorOptionNeedValue(needValueList);
                    return argumentTable;
                }
                // Valid option set were found. Take it.
                setOption(argumentTable, optionSet);
                continue;
            }
            if (optionSet.isEmpty()) {
                // Must contain at least one option.
                setErrorUnknownOption(parameter);
                return argumentTable;
            } else {
                char option = parameter.toLocal8Bit().at(1);
                lastOption = m_definitionTable.value(option, OptionDefinition());
                QString valueString = parameter.mid(2);
                setOption(argumentTable, lastOption, valueString);
                if (! lastOption.takesValue()) {
                    setWarningUnexpectedValue(QString(option), valueString);
                }
                lastOption = OptionDefinition();
            }
            continue;
        }

        // No option. Is Value.
        // ------------------------------------
        if (! lastOption.isValid()) {
            setErrorValueWithoutOption(parameter);
            return argumentTable;
        }
        if (! lastOption.takesValue()) {
            setWarningUnexpectedValue(QString(lastOption.option()), parameter);
        }
        setOption(argumentTable, lastOption, parameter);
        lastOption = OptionDefinition();
    }

    return argumentTable;
}

/**
 * Extract the long option name from the console parameter.
 * For instance:
 * --file                       Return 'file'.
 * --file=/User/text.txt        Return 'file'.
 * @param parameter             A console parameter.
 * @return                      The long option name.
 */
QString OptionParser::getLongOptionString(const QString &parameter) const
{
    int posEqualSymbol = parameter.indexOf(QChar('='));
    int length = -1;
    if (posEqualSymbol >= 0) {
        length = posEqualSymbol - 2;
    }

    return parameter.mid(2, length);
}

/**
 * Extract the value of a long option console paramater.
 * For instance:
 * --file=/User/text.txt            Return '/User/text.txt'.
 * --file                           Return an empty string.
 * @param parameter
 * @return
 */
QString OptionParser::getLongOptionValue(const QString &parameter) const
{
    int posEqualSymbol = parameter.indexOf(QChar('='));
    if (posEqualSymbol <= 0) {
        return QString();
    }

    return parameter.mid(posEqualSymbol+1, -1);
}

/**
 * Find long option in list of defined options.
 * @param longOption        The name of a long option.
 * @return index            The index of the option in the list. Or -1 if not it exists.
 */
OptionDefinition OptionParser::findLongOption(const QString &longOption) const
{
    QList<char> keyList = m_definitionTable.keys();
    for (int index=0; index<keyList.size(); ++index) {
        char key = keyList[index];
        OptionDefinition definition = m_definitionTable[key];
        if (definition.longOption() == longOption) {
            return definition;
        }
    }

    return OptionDefinition();
}

/**
 * Set an error message. Unknown option was found.
 * @param longOption
 */
void OptionParser::setErrorUnknownOption(const QString &longOption)
{
    m_errorMessages += QString("Unknown option found: %1\n").arg(longOption);
}

/**
 * Set an error message. Value found with no option.
 * @param value
 */
void OptionParser::setErrorValueWithoutOption(const QString &value)
{
    m_errorMessages += QString("Found a value without an option ! Found: %1").arg(value);
}

/**
 * Set a warning when a value was found where no value should be placed.
 * @param longOption
 * @param value
 */
void OptionParser::setWarningUnexpectedValue(const QString &option, const QString& value)
{
    m_warningMessages += QString("Option '%1' should not take a value! Found: %2\n").arg(option).arg(value);
}

/**
 * Set error message. Some options were found which should have a value but
 * they does not have any.
 * @param optionList
 */
void OptionParser::setErrorOptionNeedValue(const QList<char> &optionList)
{
    QString options;
    for (int index=0; index<optionList.size(); ++index) {
        options += QString("%1,").arg(optionList[index]);
    }
    options.chop(1);
    m_errorMessages += QString("Options '%1' must have a value !").arg(options);
}

/**
 * Set error message. An option were found which should have a value but
 * it does not have any.
 * @param option
 */
void OptionParser::setErrorOptionNeedValue(const char option)
{
    m_errorMessages += QString("Option '%1' must have a value !").arg(option);
}

/**
 * Convert string value to QVariant with defined data type.
 * @param value
 * @param dataType
 * @return
 */
QVariant OptionParser::convertValueToVariant(const QString &value, const QVariant::Type dataType) const
{
    if (value.isEmpty()) {
        return QVariant();
    }
    switch (dataType) {
    case QVariant::String:
        return QVariant(value);
        break;
    case QVariant::Int:
        return QVariant(value.toInt());
        break;
    default:
        break;
    }

    return QVariant(value);
}

/**
 * Initialize option definition table.
 * Set the definitions from a list to a hash table.
 * @param definitionList
 */
void OptionParser::initializeDefinitionTable(const QList<OptionDefinition> &definitionList)
{
    for (int index=0; index<definitionList.size(); ++index) {
        OptionDefinition definition = definitionList[index];
        m_definitionTable.insert(definition.option(), definition);
    }
}

/**
 * Parse for one or more options in a console parameter.
 * Returns an option definition object of the last found option.
 * If any character is not a option than the definition will
 * be a invalid object.
 * @param parameter     A console parameter like: '-avL'
 * @param args          A argument table to store found options.
 * @return              A option definition object. Can be invalid.
 */
OptionDefinition OptionParser::parseForOptionSet(const QString &parameter, QList<char> &args, QList<char>& needValueList) const
{
    OptionDefinition definition;
    QByteArray array = parameter.toLocal8Bit();
    int lastOption = array.size() - 1;
    for (int index=1; index<array.size(); ++index) {
        char option = array.at(index);
        definition = m_definitionTable.value(option, OptionDefinition());
        if (definition.isValid()) {
            args << option;
            if (definition.needValue() && (index < lastOption)) {
                needValueList << option;
            }
        } else {
            return OptionDefinition();
        }
    }

    return definition;
}

/**
 * Set a found option and its value to the Arguments table.
 * If option is a Switch then the boolean is set to true and
 * nothing is set to Arguments table.
 * @param arguments
 * @param definition
 * @param value
 */
void OptionParser::setOption(Arguments &arguments, const OptionDefinition &definition, const QString &valueString) const
{
    if (definition.isSwitch()) {
        const_cast<OptionDefinition*>(&definition)->setSwitchOn();
        return;
    }
    QVariant value = definition.convertValue(valueString);
    arguments.insert(definition.option(), value);
}

/**
 * Takes a list of options without a value to insert them into
 * the Arguments table. Or if an option is a Switch then the
 * boolean is set to true and nothing is inserted into Arguments
 * table.
 * @param arguments
 * @param optionList
 */
void OptionParser::setOption(Arguments &arguments, const QList<char> &optionList) const
{
    for (int index=0; index<optionList.size(); ++index) {
        OptionDefinition definition = m_definitionTable.value(optionList[index]);
        if (definition.isSwitch()) {
            definition.setSwitchOn();
        } else {
            arguments.insert(definition.option(), QVariant());
        }
    }
}

