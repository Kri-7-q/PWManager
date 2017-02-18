#include "optionparser.h"

/**
 * Constructs an option parser object. It takes a list of option definitions.
 * The defined options can be parsed of that parser.
 * @param definitionList    A list of option definitions.
 */
OptionParser::OptionParser(const QList<OptionDefinition> &definitionList) :
    m_definitionList(definitionList)
{

}

/**
 * Takes the command line parameter an parses them for optiontions.
 * The options which can be parsed with this function must be defined.
 * Therefore use the OptionDefinition class.
 * This function takes the argument counter and array values from the
 * main() function. The third parameter specifize at which parameter
 * parsing should start.
 * @param argc          The argument counter value from the main() function.
 * @param argv[]        The argument array from main() function.
 * @param start         The index of the first parameter where to start parsing.
 * @return optionTable  A QHash<char,QVariant> table with all found options
 *                      and their values.
 */
OptionTable OptionParser::parseParameter(const int argc, const char * const argv[], const int start)
{
    OptionTable optionTable;
    ParseState state = None;
    int index = start;
    OptionDefinition lastOption;
    while (index < argc) {
        switch (state) {
        case None:
            state = stateFromParameter(argv[index], lastOption);
            if (lastOption.needValue() && state != Argument) {
                setErrorMsg(QString("A value to option '%1' is missing !").arg(lastOption.option()));
                return optionTable;
            }
            break;
        case Option: {
            char wrong = 0;
            if (! parseOptions(argv[index], optionTable, lastOption, wrong)) {
                setErrorMsg(QString("Found wrong symbol '%1'. Is unknown option !").arg(wrong));
                return optionTable;
            } else {
                state = None;
                ++index;
            }
            break;
        }
        case LongOption:
            if (! parseLongOption(argv[index], optionTable, lastOption))  {
                QString param(argv[index]);
                setErrorMsg(QString("The parameter '%1' is not a known option !").arg(param));
                return optionTable;
            } else {
                state = None;
                ++index;
            }
            break;
        case Argument:
            optionTable.insert(lastOption.option(), lastOption.convertValue(QString(argv[index])));
            lastOption = OptionDefinition();
            state = None;
            ++index;
            break;
        case NonOptArgument: {
            QStringList argumentList;
            if (optionTable.contains('?')) {
                argumentList = optionTable['?'].toStringList();
            }
            argumentList << QString(argv[index]);
            optionTable.insert('?', argumentList);
            state = None;
            ++index;
            break;
        }
        default:
            break;
        }
    }

    return optionTable;
}

/**
 * Checks if the parameter from command line contains options or arguments.
 * If a parameter starts with a hyphen '-' it must be parsed for options.
 * If parameter starts with two hyphens '--' then there must be a long name option.
 * Otherwise it is a argument to an option or a free argument.
 * @param param         A parameter from command line input. One of the argv array.
 * @param lastOption    The definition of the last found option or an empty definition.
 * @return              A state value. One of Option, LongOption, Argument and NonOptArgument.
 */
OptionParser::ParseState OptionParser::stateFromParameter(const char * const param, const OptionDefinition& lastOption) const
{
    if (param[0] == '-') {
        if (param[1] == '-') {
            return LongOption;
        }

        return Option;
    }

    return (lastOption.takesValue()) ? Argument : NonOptArgument;
}

/**
 * Searches for a option definition for a given character.
 * Returns the option definition to that character or if there is not
 * such an option defined it returns an empty definition object.
 * @param symbol        The character to search for in definitions.
 * @return              An OptionDefinition object with an option definition
 *                      or empty if there is not such an option.
 */
OptionDefinition OptionParser::definitionOf(const char symbol) const
{
    for (quint16 index=0; index<m_definitionList.size(); ++index) {
        if (m_definitionList[index].option() == symbol) {
            return m_definitionList[index];
        }
    }

    return OptionDefinition();
}

/**
 * Overload of function definitionOf() to find a long name option in
 * the list of definitions.
 * @param name      The long name of an option.
 * @return          An OptionDefinition object with an option definition
 *                  or empty if such an option doesn't exist.
 */
OptionDefinition OptionParser::definitionOf(const QString &name) const
{
    for (quint16 index=0; index<m_definitionList.size(); ++index) {
        if (m_definitionList[index].longOption() == name) {
            return m_definitionList[index];
        }
    }

    return OptionDefinition();
}

/**
 * Takes a command line argument which starts with a hyphen '-'.
 * It parses this argument for defined options. The argument can
 * contain options and its values.
 * All found options will be added to a QHash<char,QVariant> map.
 * It takes a reference to a OptionDefinition object. The definition
 * of the last found option will be set to that reference. It is
 * necessary to add a value to that option if there occures one.
 * To provide an appropriate error message it take a reference to
 * a char value. A found character which was not defined as option
 * will be set to that reference.
 * @param param             A command line argument to parse for options.
 * @param optionTable       A reference to the hash map with found options.
 * @param last              A reference to a OptionDefinition object to store the last found option.
 * @param wrong             To set a wrong character which was not defined as option for error message.
 * @return                  True if successfully parsed the command line argument (param).
 *                          Returns false if a wrong character occurse and set it to the 'wrong'
 *                          reference.
 */
bool OptionParser::parseOptions(const char * const param, OptionTable &optionTable, OptionDefinition& last, char &wrong) const
{
    int index = 1;
    ParseState state = Option;
    OptionDefinition lastOption;
    while (param[index] != 0) {
        OptionDefinition definition = definitionOf(param[index]);
        switch(state) {
        case Option:
            if (definition.isValid()) {                     // Found valid option.
                if (definition.isSwitch()) {
                    definition.setSwitchOn();               // Option switches a boolean to true.
                } else {
                    lastOption = definition;                // Is normal option.
                    optionTable.insert(definition.option(), QVariant());
                    if (definition.needValue()) {
                        state = Argument;                   // Option require an argument.
                    }
                }
                ++index;
            } else {                                        // Found NONE option character.
                if (lastOption.takesValue()) {
                    state = Argument;                       // Last option takes a value.
                } else {
                    wrong = param[index];                   // Provide wrong character for error msg.
                    return false;
                }
            }
            break;
        case Argument: {
            QString paramaeter(param);
            QString stringVal = paramaeter.right(paramaeter.length()-index);
            optionTable.insert(lastOption.option(), lastOption.convertValue(stringVal));
            last = OptionDefinition();
            return true;
            break;
        }
        default:
            break;
        }
    }
    last = lastOption;

    return true;
}

/**
 * Takes a command line argument if it starts with two hyphen '--'.
 * It checks the argument for a long name option. If the option is
 * combined with a value like '--file=/Home/Horst/Data' then the
 * option and its value is taken and set to the QHash map (optionTable).
 * If argument doesn't contain a value '--file' but option takes a
 * value then the definition of that option is set to 'last'. This
 * is required to set a following value to that option.
 * @param param             A command line argument from the 'argv[]' array.
 * @param optionTable       A QHash<char,QVarinat> map reference to store found options.
 * @param last              Reference to an option definition object.
 * @return                  True if the argument contains a valid long name option.
 *                          Other wise it returns false if the option is unknown.
 */
bool OptionParser::parseLongOption(const char * const param, OptionTable &optionTable, OptionDefinition &last) const
{
    QString parameter(param);
    QString optionName = longOptionName(parameter);
    OptionDefinition definition = definitionOf(optionName);
    if (! definition.isValid()) {
        // ERROR: Is not an option.
        return false;
    }
    if (definition.isSwitch()) {
        definition.setSwitchOn();           // This option is just a switch which turns a boolean to true.
        last = OptionDefinition();
        return true;
    }
    int index = parameter.indexOf('=');     // Console parameter may contain long option and argument.
    if (index > 0) {
        QString stringVal = longOptionArgument(parameter);
        optionTable.insert(definition.option(), definition.convertValue(stringVal));
        last = OptionDefinition();
        return true;
    }
    if (definition.takesValue()) {
        last = definition;
    }
    optionTable.insert(definition.option(), QVariant());

    return true;
}

/**
 * Extracts the name of a long name option from the command line argument
 * like '--file=/Home/Horst/Data'.
 * @param param     A string with the command line argument.
 * @return          The found name of the long name option as a QString.
 */
QString OptionParser::longOptionName(const QString& param) const
{
    int index = param.indexOf('=');
    if (index < 0) {
        return param.right(param.length() - 2);
    }

    return param.mid(2, index - 2);
}

/**
 * Extracts the option value from a long name option combined
 * with a value like '--file=/home/Horst/Data'.
 * @param param     The command line argument with the option and its value.
 * @return          The value of that option as a QString.
 */
QString OptionParser::longOptionArgument(const QString& param) const
{
    int index = param.indexOf('=');
    if (index < 0) {
        return QString();
    }

    return param.right(param.length() - index - 1);
}

/**
 * Setter function to set an error message.
 * @param msg       The message text.
 */
void OptionParser::setErrorMsg(const QString &msg)
{
    m_errorMsg.append(msg).append('\n');
}
