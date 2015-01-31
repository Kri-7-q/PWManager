#include "options.h"

/**
 * Constructor
 * @param validOptions      A string with valid options.
 */
Options::Options(const QString &validOptions) :
    m_hasError(false)
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
    for (int index=start; index<argc; ++index) {
        QString parameter(argv[index]);
        if (parameter.startsWith("--")) {
            // long option
            bool isOption = parseLongOption(parameter, optionTable);
            if (! isOption) {
                return optionTable;
            }
        }
        else if (parameter.startsWith('-')) {

        } else {

        }
    }
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
    QStringList optionList = validOptions.split('|');
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
bool Options::parseLongOption(QString &parameter, QHash<QString, QString> &optionTable)
{
    parameter.remove(0, 2);
    int index = parameter.indexOf('=');
    QString option(parameter), value;
    if (index > 0) {
        QStringList list = parameter.split('=');
        option = list[0];
        value = list[1];
        if (! m_validOptionTable.value(option)) {
            m_hasError = true;
            // Error message ???????????does not take a value???????????????????
        }
    }
    if (m_validOptionTable.contains(option)) {
        optionTable.insert(option, value);
        return true;
    }
    m_hasError = true;
    // Error message ????????????????is not a valid option??????????????

    return false;
}
