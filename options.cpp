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
        {"all", no_argument, 0, 'e'},
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
        QString databaseName = Account::databaseNameOfOption((char)foundOption);

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
            setFailedValue(InvalidOption, QString((char)optopt));
            break;
        default:
            m_needHelp = true;
            break;
        }
    }
    if (optind < argc-1) {
        m_hasError = true;
        ++optind;
        while (optind < argc) {
            setFailedValue(LeftArgument, QString(argv[optind++]));
        }
    }

    return account;
}

// Setter and Getter
bool Options::hasError() const
{
    return m_hasError;
}

void Options::setHasError(bool hasError)
{
    m_hasError = hasError;
}

/**
 * Return error messages.
 * @return
 */
QString Options::errorMessage()
{
    if(! hasError()) {
        return QString("No errors occured.");
    }
    QString errorMsg = getInvalidOptionMsg();
    errorMsg.append(getLeftArgumentMsg());

    return errorMsg;
}

/**
 * Get help text to command or in general.
 * @return
 */
QStringList Options::getHelpText()
{
    switch (m_command) {
    case New:
        return getHelpForNew();
    case GeneratePW:
        return getHelpForGeneratePW();
    case Modify:
        return getHelpForModify();
    case Show:
        return getHelpForShow();
    case Remove:
        return getHelpForRemove();
    default:
        return getHelpInGeneral();
    }
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

    m_needHelp = true;

    return Help;
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
        return "p:u:i:qrlskaeh";
        break;
    case Remove:
        return "p:u:i:";
        break;
    default:
        return "h";
        break;
    }
}

/**
 * Creates a error message when invalid options are found.
 * @return
 */
QString Options::getInvalidOptionMsg() const
{
    QStringList optionList = m_errorHashTable.value(InvalidOption);
    if (optionList.isEmpty()) {
        return QString();
    }
    QString errorMsg("Invalid option found : ");
    for (QString wrongOption : optionList) {
        errorMsg.append(wrongOption).append(", ");
    }
    errorMsg.remove(errorMsg.length()-2, 2).append('\n');

    return errorMsg;
}

/**
 * Creates a error message when arguments left.
 * @return
 */
QString Options::getLeftArgumentMsg() const
{
    QStringList argumentList = m_errorHashTable.value(LeftArgument);
    if (argumentList.isEmpty()) {
        return QString();
    }
    QString errorMsg("Arguments left : ");
    for (QString argument : argumentList) {
        errorMsg.append(argument).append(", ");
    }
    errorMsg.remove(errorMsg.length()-2, 2).append('\n');

    return errorMsg;
}

/**
 * Set a wrong option or argument to the error list.
 * @param errorType
 * @param value
 */
void Options::setFailedValue(const Options::ErrorType errorType, const QString &value)
{
    QStringList list = m_errorHashTable.take(errorType);
    list << value;
    m_errorHashTable.insert(errorType, list);
}

/**
 * General help text.
 * @return
 */
QStringList Options::getHelpInGeneral()
{
    QStringList help;
    help << "pwmanager [command] [options]\n";
    help << "pwmanager [command] --help\n";
    help << "   Commands :  Help, New, GeneratePW, Show, Modify, Remove\n\n";
    help << "   help        Shows this help text.\n";
    help << "               --help has the same effect.\n";
    help << "   new         Adds a new account to database.\n";
    help << "   generatepw  Generates a new password for an existing account.\n";
    help << "   show        To show one or more accounts.\n";
    help << "   modify      Modifies an existing account.\n";
    help << "   remove      Removes an existing account from database.\n";

    return help;
}

/**
 * Help text for command new.
 * @return
 */
QStringList Options::getHelpForNew()
{
    QStringList help;
    help << "pwmanager new [options]\n";
    help << "Insert a new account into database.\n\n";
    help << optP << optU << "optional\n" << optK << optQ << optAnswer << optL << optS;

    return help;
}

/**
 * Help text for command generatePW.
 * @return
 */
QStringList Options::getHelpForGeneratePW()
{
    QStringList help;
    help << "pwmanager generatepw [options]\n";
    help << "Generates a new passwort for an given account.\n\n";
    help << optI << optP << optU << "optional\n" << optL << optS;

    return help;
}

/**
 * Help text for command modify.
 * @return
 */
QStringList Options::getHelpForModify()
{
    QStringList help;
    help << "pwmanager modify [options]\n";
    help << "Can modify some values of an account.\n\n";
    help << optI << optP << optU << "optional\n" << optQ << optAnswer << optK << optL << optS;

    return help;
}

/**
 * Help text for command show.
 * @return
 */
QStringList Options::getHelpForShow()
{
    QStringList help;
    help << "pwmanager show [options]\n";
    help << "Shows chosen infomation about one or more accounts.\n\n";
    help << optP<< optAll << "optional\n" << optU << optI << optK << optQ << optAnswer << optL << optS << optA;

    return help;
}

/**
 * Help text for command remove.
 * @return
 */
QStringList Options::getHelpForRemove()
{
    QStringList help;
    help << "pwmanager remove [options]\n";
    help << "Removes an account from database.\n\n";
    help << optI << "or\n" << optP << optU;

    return help;
}
