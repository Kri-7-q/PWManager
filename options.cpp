#include "options.h"

/**
 * Constructor
 * @param parameter     The first parameter of command line. (After App name)
 */
Options::Options(char *parameter) :
    m_needHelp(false)
{
    m_command = parseCommand(parameter);
}

// Getter
Options::Command Options::command() const
{
    return m_command;
}

// Setter
void Options::setCommand(const Command &command)
{
    m_command = command;
}

/**
 * Parse command line options.
 * Returns all options and values in an Account object.
 * @param argc
 * @param argv
 * @return
 */
Account Options::parseOptions(const int argc, char **argv)
{
    struct option long_option[] = {
        {"help", no_argument, 0, 'h'},
        {"answer", (m_command == New || m_command == Modify) ? required_argument : no_argument, 0, 'r'},
        {0, 0, 0, 0}
    };

    Account account;
    int startIndex = 1;
    opterr = 0;

    while (true) {
        int foundOption = getopt_long(argc, argv, getCommandsOptions().toUtf8().data(), long_option, &startIndex);
        if (foundOption < 0) {
            break;
        }

        QString optionValue = (optarg == 0) ? QString() : QString(optarg);
        QString databaseName = Persistence::getDatabaseNameFor((char)foundOption);

        switch (foundOption) {
        case 'p':
        case 'u':
        case 'q':
        case 'r':
        case 's':
        case 'k':
            account.insert(databaseName, optionValue);
            break;
        case 'i':
        case 'l':
            account.insert(databaseName, optionValue.toInt());
            break;
        case '?':
            m_hasError = true;
            m_errorMessage.append((char)optopt).append(",");
            break;
        default:
            m_needHelp = true;
            break;
        }
    }

    return account;
}
bool Options::hasError() const
{
    return m_hasError;
}

void Options::setHasError(bool hasError)
{
    m_hasError = hasError;
}

QString Options::errorMessage()
{
    if (m_errorMessage.endsWith(',')) {
        m_errorMessage.remove(m_errorMessage.length()-1, 1);
    }
    return "Invalid options found : " + m_errorMessage + '\n';
}

void Options::setErrorMessage(const QString &errorMessage)
{
    m_errorMessage = errorMessage;
}




/**
 * Parse the first command line parameter.
 * It should fit to one of the commands.
 * @return
 */
Options::Command Options::parseCommand(char *parameter)
{
    QString commandString = QString(parameter).toLower();
    if (commandString == "new") {
        return New;
    }
    if (commandString == "generatepw") {
        return GeneratePW;
    }
    if (commandString == "show") {
        return Show;
    }
    if (commandString == "modify") {
        return Modify;
    }
    if (commandString == "remove") {
        return Remove;
    }
    if (commandString == "help") {
        return Help;
    }

    return None;
}

/**
 * Get c string with options which are allowed
 * for the current command.
 * @return
 */
QString Options::getCommandsOptions()
{
    switch (m_command) {
    case New:
        return "p:u:q:r:l:s:k:h";
        break;
    case GeneratePW:
        return "p:u:i:l:s:h";
        break;
    case Modify:
        return "p:u:i:q:r:l:s:k:h";
        break;
    case Show:
        return "p:u:i:qrlskah";
        break;
    case Remove:
        return "p:u:i:";
        break;
    default:
        return "h";
        break;
    }
}
