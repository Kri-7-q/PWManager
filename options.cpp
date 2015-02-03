#include "options.h"

/**
 * Constructor
 * @param validOptions      A string with valid options.
 */
Options::Options(const QString &validOptions) :
    m_hasError(false),
    m_optionSeparater(' ')
{
    m_validOptionTable = validOptionTable(validOptions);
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
QHash<QString, QString> Options::parseOptions(const int argc, const char * const *argv, const int start)
{
    QHash<QString, QString> optionTable;
    QString lastOption;
    for (int index=start; index<argc; ++index) {
        qDebug() << "Index : " << index;
        QString parameter(argv[index]);
        qDebug() << "Parameter : " << parameter << "\tIndex : " << index;
        if (parameter.startsWith("--")) {
            // long option
            parameter.remove(0, 2);
            bool isOption = parseLongOption(parameter, optionTable, lastOption);
            if (! isOption) {
                return optionTable;
            }
            continue;
        }
        if (parameter.startsWith('-')) {
            // single option
            parameter.remove(0, 1);
            qDebug() << "without '-' : " << parameter;
            bool isMultiOption = isMultiOptionSet(parameter);
            if (isMultiOption) {
                lastOption = setMultiOption(parameter, optionTable);
            } else {
                lastOption = "";
                if (! setOptionAndValue(parameter, optionTable)) {
                    return optionTable;
                }
            }
            continue;
        }
        // Is value
        bool lastOptionTakeValue = m_validOptionTable.value(lastOption, false);
        bool hasNoValue = optionTable.value(lastOption, QString()).isNull();
        if (lastOptionTakeValue && hasNoValue) {
            optionTable.insert(lastOption, parameter);
        } else {
            m_hasError = true;
            // Error message ?????????????found value without option???????????????
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
 * Takes a string which contains all valid options.
 * Options are separated with '|'. Option name is followed
 * of a colon when it can take a value.
 * @param validOptions
 * @return
 */
QHash<QString, bool> Options::validOptionTable(const QString &validOptions)
{
    QHash<QString, bool> table;
    QStringList optionList = validOptions.split(m_optionSeparater);
    for (QString option : optionList) {
        bool hasValue = option.endsWith(':');
        if (hasValue) {
            option.remove(option.length()-1, 1);
        }
        table.insert(option, hasValue);
    }

    return table;
}

/**
 * Parse parameter for a long option.
 * A '=' can follow a long option. Than
 * the option fallows a value.
 * @param parameter         The command line argument.
 * @param optionTable
 * @return                  True if option and value are valid.
 */
bool Options::parseLongOption(const QString &parameter, QHash<QString, QString> &optionTable, QString &lastOption)
{
    QStringList paramList = parameter.split('=');
    if (!m_validOptionTable.contains(paramList[0])) {
        m_hasError = true;
        // Error message ??????????????wrong option??????????????????????????
        return false;
    }
    if (paramList.size() > 1) {
        optionTable.insert(paramList[0], paramList[1]);
        if (! m_validOptionTable.value(paramList[0], false)) {
            m_hasError = true;
            // Error message ????????????option does not take a value???????????????
        }
    } else {
        optionTable.insert(paramList[0], QString());
    }
    lastOption = paramList[0];

    return true;
}

/**
 * Checks if parameter contains a multi option set.
 * @param parameter
 * @return              True if all character are options.
 */
bool Options::isMultiOptionSet(const QString &parameter) const
{
    for (int index=0; index<parameter.length(); ++index) {
        QString option = parameter.mid(index, 1);
        if (! m_validOptionTable.contains(option)) {
            return false;
        }
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
QString Options::setMultiOption(const QString &parameter, QHash<QString, QString> &optionTable)
{
    QString option;
    for (int index=0; index<parameter.length(); ++index) {
        option = parameter.mid(index, 1);
        qDebug() << "Single option : " << option << "\tIndex : " << index;
        optionTable.insert(option, QString());
    }
    qDebug() << "Parameter length : " << parameter.length();

    return option;
}

/**
 * Splits the option charackter from the value.
 * Inserts if valid the option and its value.
 * @param parameter
 * @param optionTable
 * @return
 */
bool Options::setOptionAndValue(const QString &parameter, QHash<QString, QString> &optionTable)
{
    QString option = parameter.left(1);
    if (! m_validOptionTable.contains(option)) {
        m_hasError = true;
        // Error message ???????????is not a valid option????????????
        return false;
    }
    if (! m_validOptionTable.value(option)) {
        m_hasError = true;
        // Error mesage ????????????option does not take a value?????????
    }
    QString value = parameter.mid(1);
    optionTable.insert(option, value);

    return true;
}
