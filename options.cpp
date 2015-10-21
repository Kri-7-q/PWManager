#include "options.h"

/**
 * Constructor
 * @param validOptions      A string with valid options.
 */
Options::Options(const QList<OptionDefinition> &definedOptions) :
    m_hasError(false)
{
    setDefinedOptions(definedOptions);
}

/**
 * Destructor
 */
Options::~Options()
{

}

/**
 * Parse command line input for options and its values.
 * Options and values are returned in a QHash object.
 * Where the option is key and value is options value.
 * @param argc
 * @param argv
 * @param start             At which position in programm parameter is to start. (zero based)
 * @return optionTable      A hash map with options and values.
 */
OptionTable Options::parseOptions(const int argc, const char * const *argv, const int start)
{
    OptionTable optionTable;
    char lastOption;
    for (int index=start; index<argc; ++index) {
        QString parameter(argv[index]);
        // Is long name option?
        if (parameter.startsWith("--")) {
            // parameter is a long option. It starts with '--'
            parameter.remove(0, 2);
            bool isOption = parseLongOption(parameter, optionTable, lastOption);
            if (! isOption) {
                return optionTable;
            }
            continue;
        }
        // Is normal option?
        if (parameter.startsWith('-')) {
            // param starts with '-' that signals one or more options.
            bool isMultiOption = isMultiOptionSet(argv[index]);
            if (isMultiOption) {
                lastOption = setMultiOption(argv[index], optionTable);
            } else {
                lastOption = '\0';
                if (! setOptionAndValue(argv[index], optionTable)) {
                    return optionTable;
                }
            }
            continue;
        }
        // Was not an option its a value.
        QVariant::Type expectedValueType = m_definedOption.value(lastOption, QVariant::Invalid);
        bool lastOptionTakeValue = (expectedValueType != QVariant::Invalid);
        bool hasNoValue = optionTable.hasValueForKey(lastOption);
        if (lastOptionTakeValue && hasNoValue) {
            optionTable.insert(lastOption, parameter);
        } else {
            setErrorMessage("Found value without option : " + parameter + "\n");
        }
    }

    return optionTable;
}

/**
 * Takes a list with option definitions.
 * Sets option information to Hashtables.
 * @param validOptions
 */
void Options::setDefinedOptions(const QList<OptionDefinition> &definedOptions)
{
    for (OptionDefinition definitionObj : definedOptions) {
        m_definedOption.insert(definitionObj.option(), definitionObj.dataType());
        if (definitionObj.hasLongName()) {
            m_definedLongOption.insert(definitionObj.longName(), definitionObj.option());
        }
    }
}

/**
 * Parse parameter for a long option.
 * A '=' can follow a long option. Than
 * the option is fallowed by a value.
 * Some options can take a value other do not.
 * @param parameter         The command line argument.
 * @param optionTable
 * @return                  True if option and value are valid.
 */
bool Options::parseLongOption(const QString &parameter, OptionTable &optionTable, char &lastOption)
{
    // posible option:     --file=/home/filename.txt
    QStringList paramList = parameter.split('=');
    QString longOption(paramList[0]);
    QString valueString;
    if (paramList.size() > 1) {
        valueString = paramList[1];
    }
    if (!m_definedLongOption.contains(longOption)) {
        setErrorMessage("Not a known option : " + longOption + "\n");
        return false;
    }
    char option = m_definedLongOption.value(longOption);
    if (! valueString.isEmpty()) {
        QVariant value = valueWithType(m_definedOption.value(option), valueString);
        optionTable.insert(option, value);
        QVariant::Type expectedValueType = m_definedOption.value(option, QVariant::Invalid);
        if (expectedValueType == QVariant::Invalid) {
            setErrorMessage("Option '" + longOption + "' does not take a value.\n");
        }
    } else {
        optionTable.insert(option, QVariant());
    }
    lastOption = option;

    return true;
}

/**
 * Checks if parameter contains a multi option set.
 * @param parameter
 * @return              True if all character are options.
 */
bool Options::isMultiOptionSet(const char *parameter) const
{
    int index = 1;
    while (parameter[index] != 0) {
        if (! m_definedOption.contains(parameter[index])) {
            return false;
        }
        ++index;
    }

    return true;
}

/**
 * Takes a string with multi option set.
 * Those options are inserted to hash table.
 * @param parameter
 * @param optionTable       Found options and values.
 * @return option           The last option in parameter string.
 */
char Options::setMultiOption(const char *parameter, OptionTable &optionTable)
{
    char option;
    int index = 1;
    while (parameter[index] != 0) {
        option = parameter[index];
        optionTable.insert(option, QVariant());
        ++index;
    }

    return option;
}

/**
 * Splits the option charackter from the value.
 * If option and value are one string.
 * Inserts if valid the option and its value.
 * @param parameter
 * @param optionTable
 * @return
 */
bool Options::setOptionAndValue(const char *parameter, OptionTable &optionTable)
{
    char option = parameter[1];
    if (! m_definedOption.contains(option)) {
        setErrorMessage(QString("Not a known option : %1\n").arg(option));
        return false;
    }
    QVariant::Type valueType = m_definedOption.value(option, QVariant::Invalid);
    if (valueType == QVariant::Invalid) {
        setErrorMessage(QString("Option '%1' does not take a value.\n").arg(option));
    }
    QString valueString = QString(parameter).mid(2);
    QVariant value = valueWithType(m_definedOption.value(option, QVariant::Invalid), valueString);
    optionTable.insert(option, value);

    return true;
}

/**
 * Converts a string value into another type.
 * It has to be a QVariant type. The value is
 * returned as a QVariant.
 * @param type
 * @param value
 * @return
 */
QVariant Options::valueWithType(const QVariant::Type type, const QString value) const
{
    switch (type) {
    case QVariant::Int:
        return QVariant(value.toInt());
        break;
    case QVariant::String:
        return QVariant(value);
    default:
        break;
    }

    return QVariant();
}

/**
 * Set a error message.
 * @param errorMsg
 */
void Options::setErrorMessage(const QString &errorMsg)
{
    m_errorMsg.append(errorMsg);
    m_hasError = true;
}
