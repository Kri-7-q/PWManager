#include "options.h"

/**
 * Constructor
 * @param validOptions      A string with valid options.
 */
Options::Options(const QList<OptionDefinition> &validOptions) :
    m_hasError(false),
    m_optionSeparater(' ')
{
    setValidOptions(validOptions);
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
 * Where the option is key and value if options value.
 * @param argc
 * @param argv
 * @param start     At which position in programm parameter is to start.
 * @return
 */
OptionTable Options::parseOptions(const int argc, const char * const *argv, const int start)
{
    OptionTable optionTable;
    char lastOption;
    for (int index=start; index<argc; ++index) {
        QString parameter(argv[index]);
        if (parameter.startsWith("--")) {
            // long option
            parameter.remove(0, 2);
            bool isOption = parseLongOption(parameter, optionTable, lastOption);
            if (! isOption) {
                return optionTable;
            }
            continue;
        }
        if (argv[index][0] == '-') {
            // single option
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
        // Is value
        bool lastOptionTakeValue = (m_validOption.value(lastOption, QVariant::Invalid) != QVariant::Invalid);
        bool hasNoValue = optionTable.value(lastOption, QString()).isNull();
        if (lastOptionTakeValue && hasNoValue) {
            optionTable.insert(lastOption, parameter);
        } else {
            m_hasError = true;
            m_errorMsg.append("Found value without option : " + parameter + "\n");
        }
    }

    return optionTable;
}

// Getter
QChar Options::optionSeparater() const
{
    return m_optionSeparater;
}
/**
 * SETTER
 * The separater character must not be a colon.
 * The default sparater is '|'.
 * @param optionSeparater
 */
void Options::setOptionSeparater(const QChar &optionSeparater)
{
    if (optionSeparater == ':') {
        return;
    }
    m_optionSeparater = optionSeparater;
}


/**
 * Takes a list with option definitions.
 * Sets option information to Hashtables.
 * @param validOptions
 */
void Options::setValidOptions(const QList<OptionDefinition> &validOptions)
{
    for (OptionDefinition definition : validOptions) {
        m_validOption.insert(definition.option(), definition.dataType());
        if (definition.hasLongName()) {
            m_validLongOption.insert(definition.longName(), definition.option());
        }
    }
}

/**
 * Parse parameter for a long option.
 * A '=' can follow a long option. Than
 * the option is fallowed by a value.
 * @param parameter         The command line argument.
 * @param optionTable
 * @return                  True if option and value are valid.
 */
bool Options::parseLongOption(const QString &parameter, OptionTable &optionTable, char &lastOption)
{
    QStringList paramList = parameter.split('=');
    if (!m_validLongOption.contains(paramList[0])) {
        m_hasError = true;
        m_errorMsg.append("Not a known optiond : " + paramList[0] + "\n");
        return false;
    }
    char option = m_validLongOption.value(paramList[0]);
    if (paramList.size() > 1) {
        QVariant value = valueWithType(m_validOption.value(option), paramList[1]);
        optionTable.insert(option, value);
        if (m_validOption.value(option, QVariant::Invalid) == QVariant::Invalid) {
            m_hasError = true;
            m_errorMsg.append("Option '" + paramList[0] + "' does not take a value.\n");
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
        if (! m_validOption.contains(parameter[index])) {
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
 * Inserts if valid the option and its value.
 * @param parameter
 * @param optionTable
 * @return
 */
bool Options::setOptionAndValue(const char *parameter, OptionTable &optionTable)
{
    char option = parameter[1];
    if (! m_validOption.contains(option)) {
        m_hasError = true;
        m_errorMsg.append("Not a known option : %1\n").arg(option);
        return false;
    }
    if (m_validOption.value(option, QVariant::Invalid) == QVariant::Invalid) {
        m_hasError = true;
        m_errorMsg.append("Option '%1' does not take a value.\n").arg(option);
    }
    QString valueString = QString(parameter).mid(2);
    QVariant value = valueWithType(m_validOption.value(option, QVariant::Invalid), valueString);
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
