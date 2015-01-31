#include "appcommand.h"

/**
 * Constructor
 * @param parameter
 */
AppCommand::AppCommand(const char *parameter)
{
    m_command = parseCommand(parameter);
}

/**
 * Destructor
 */
AppCommand::~AppCommand()
{

}

/**
 * Return a string containing all options of a command.
 * Options seperated by a pipe symbol '|'.
 * @return
 */
QString AppCommand::commandsOptions()
{
    switch (m_command) {
    case New:
        return "p:|u:|k:|l:|s:|q:|answer:|help";
    case GeneratePW:
        return "i:|u:|p:|l:|s:|help";
    case Show:
        return "i:|p:|u:|all|a|k|l|s|q|answer|t|help";
    case Modify:
        return "i:|p:|u:|k:|l:|s:|q:|answer:|help";
    case Remove:
        return "i:|p:|u:";
    default:
        break;
    }
}

/**
 * Get help text to command or in general.
 * @return
 */
QStringList AppCommand::getHelpText()
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
AppCommand::Command AppCommand::parseCommand(const char *parameter)
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

    return Help;
}

/**
 * General help text.
 * @return
 */
QStringList AppCommand::getHelpInGeneral()
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
QStringList AppCommand::getHelpForNew()
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
QStringList AppCommand::getHelpForGeneratePW()
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
QStringList AppCommand::getHelpForModify()
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
QStringList AppCommand::getHelpForShow()
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
QStringList AppCommand::getHelpForRemove()
{
    QStringList help;
    help << "pwmanager remove [options]\n";
    help << "Removes an account from database.\n\n";
    help << optI << "or\n" << optP << optU;

    return help;
}
