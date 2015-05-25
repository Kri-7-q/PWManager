#include "appcommand.h"


/**
 * Constructor
 * @param parameter The first parameter of command line input.
 */
AppCommand::AppCommand(const QString &parameter)
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
 * Creates a set of options to the command given
 * throuh the constructor.
 * @return list     A list of all options which can be processed from the command.
 */
QList<OptionDefinition> AppCommand::commandsOptions()
{
    QList<OptionDefinition> list;
    switch (m_command) {
    case New:
        list << OptionDefinition('p', QVariant::String, QString());
        list << OptionDefinition('u', QVariant::String, QString());
        list << OptionDefinition('k', QVariant::String, QString());
        list << OptionDefinition('l', QVariant::Int, QString());
        list << OptionDefinition('s', QVariant::String, QString());
        list << OptionDefinition('q', QVariant::String, QString());
        list << OptionDefinition('r', QVariant::String, QString("answer"));
        list << OptionDefinition('h', QVariant::String, QString("help"));
        break;
    case GeneratePW:
        list << OptionDefinition('i', QVariant::Int, QString());
        list << OptionDefinition('p', QVariant::String, QString());
        list << OptionDefinition('u', QVariant::String, QString());
        list << OptionDefinition('l', QVariant::Int, QString());
        list << OptionDefinition('s', QVariant::String, QString());
        list << OptionDefinition('h', QVariant::String, QString("help"));
        break;
    case Show:
        list << OptionDefinition('i', QVariant::Int, QString());
        list << OptionDefinition('p', QVariant::String, QString());
        list << OptionDefinition('u', QVariant::String, QString());
        list << OptionDefinition('k', QVariant::Invalid, QString());
        list << OptionDefinition('l', QVariant::Invalid, QString());
        list << OptionDefinition('s', QVariant::Invalid, QString());
        list << OptionDefinition('q', QVariant::Invalid, QString());
        list << OptionDefinition('t', QVariant::Invalid, QString());
        list << OptionDefinition('r', QVariant::Invalid, QString("answer"));
        list << OptionDefinition('h', QVariant::Invalid, QString("help"));
        list << OptionDefinition('e', QVariant::Invalid, QString("all"));
        list << OptionDefinition('a', QVariant::Invalid, QString());
        break;
    case Modify:
        list << OptionDefinition('i', QVariant::Int, QString());
        list << OptionDefinition('p', QVariant::String, QString());
        list << OptionDefinition('u', QVariant::String, QString());
        list << OptionDefinition('k', QVariant::String, QString());
        list << OptionDefinition('l', QVariant::Int, QString());
        list << OptionDefinition('s', QVariant::String, QString());
        list << OptionDefinition('q', QVariant::String, QString());
        list << OptionDefinition('r', QVariant::String, QString("answer"));
        list << OptionDefinition('h', QVariant::Invalid, QString("help"));
        break;
    case Remove:
        list << OptionDefinition('i', QVariant::Int, QString());
        list << OptionDefinition('p', QVariant::String, QString());
        list << OptionDefinition('u', QVariant::String, QString());
        list << OptionDefinition('h', QVariant::Invalid, QString("help"));
        break;
    case File:
        list << OptionDefinition('f', QVariant::String, QString("file"));
        list << OptionDefinition('o', QVariant::Invalid, QString("out"));
        list << OptionDefinition('g', QVariant::Invalid, QString("in"));
    default:
        break;
    }

    return list;
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
    case File:
        return getHelpForFile();
    default:
        return getHelpInGeneral();
    }
}

/**
 * Parse the first command line parameter.
 * It should fit to one of the applications commands.
 * @return
 */
AppCommand::Command AppCommand::parseCommand(const QString &parameter)
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
    if (commandString == "file") {
        return File;
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
    help << "pwmanager <command> <options>\n";
    help << "pwmanager <command> --help\n";
    help << "   Commands :  Help, New, GeneratePW, Show, Modify, Remove\n\n";
    help << "   help        Shows this help text.\n";
    help << "   new         Adds a new account to database.\n";
    help << "   generatepw  Generates a new password for an existing account.\n";
    help << "   show        To show one or more accounts.\n";
    help << "   modify      Modifies an existing account.\n";
    help << "   remove      Removes an existing account from database.\n";
    help << "   file        Write database content to file. Or read from file.\n";
    help << "   --help      Shows a help text to the command.\n";

    return help;
}

/**
 * Help text for command new.
 * @return
 */
QStringList AppCommand::getHelpForNew()
{
    QStringList help;
    help << "pwmanager new <options>\n";
    help << "Insert a new account into database.\n\n";
    help << "  -p <name>        The name of a provider, Webpage, Device ...\n";
    help << "  -u <name>        A username to login.\n";
    help << "optional\n";
    help << "  -k <password>    An existing password to store.\n";
    help << "  -q <quetion>     Security question if you can't remember the password.\n";
    help << "  -r <answer>      An answer (replay) to the security question.\n";
    help << "  --answer <answer> | --answer=\"answer\" | --answer=<answer>\n";
    help << "  -l <length>      The length of passwort when generate any.\n";
    help << "  -s <definition>  A definition string with characters to generate a passwort.\n";
    help << "                   For instance : 6[A-Z]8[a-z]*[0-9]4{+#-.,<>()}\n";
    help << "                   [] Range of characters, {} set of characters, * wildcard for amount\n";

    return help;
}

/**
 * Help text for command generatePW.
 * @return
 */
QStringList AppCommand::getHelpForGeneratePW()
{
    QStringList help;
    help << "pwmanager generatepw <options>\n";
    help << "Generates a new passwort for an given account.\n";
    help << "If no length an character set is given application will try\n\n";
    help << "to read them from database.\n";
    help << "  -i <id>          The id (primary key) to identify database entry.\n";
    help << "or\n";
    help << "  -p <name>        The name of a provider, Webpage, Device ...\n";
    help << "  -u <name>        A username to login.\n";
    help << "optional\n";
    help << "  -l <length>      The length of generated password.\n";
    help << "  -s <definition>  A character definition. Characters which will be uesd to gnerate password.\n";
    help << "                   For instance : 6[A-Z]8[a-z]*[0-9]4{+#-.,<>()}\n";
    help << "                   [] Range of characters, {} set of characters, * wildcard for amount\n";

    return help;
}

/**
 * Help text for command modify.
 * @return
 */
QStringList AppCommand::getHelpForModify()
{
    QStringList help;
    help << "pwmanager modify <options>\n";
    help << "Can modify some values of an account.\n\n";
    help << "  -i <id>          The id (primary key) to identify a database entry.\n";
    help << "or\n";
    help << "  -p <name>        The name of a provider, Webpage, Device ...\n";
    help << "  -u <name>        A username to login.\n";
    help << "optional\n";
    help << "  -k <password>    An existing password to store.\n";
    help << "  -q <quetion>     Security question if you can't remember the password.\n";
    help << "  -r <answer>      An answer to the security question.\n";
    help << "  --answer <answer> | --answer=\"answer\" | --answer=<answer>\n";
    help << "  -l <length>      The length of passwort when generate any.\n";
    help << "  -s <definition>  A definition string with characters to generate a passwort.\n";
    help << "                   For instance : 6[A-Z]8[a-z]*[0-9]4{+#-.,<>()}\n";
    help << "                   [] Range of characters, {} set of characters, * wildcard for amount\n";

    return help;
}

/**
 * Help text for command show.
 * @return
 */
QStringList AppCommand::getHelpForShow()
{
    QStringList help;
    help << "pwmanager show <options>\n";
    help << "Shows chosen infomation about one or more accounts.\n";
    help << "If to any option a value is given then this value will be used to find a database entry.\n\n";
    help << "  -i [id]          Show id of database entry or give an id to identify a database entry.\n";
    help << "  -p [name]        Show the provider name of an account.\n";
    help << "  -u [name]        Show the username of an account.\n";
    help << "  -k [password]    Show password of an account.\n";
    help << "  -q [question]    Show security question of an account.\n";
    help << "  -r [answer]      Show the answer (replay) to the security question.\n";
    help << "  --answer <answer> | --answer=\"answer\" | --answer=<answer>\n";
    help << "  -l [length]      Show the length of password when generate.\n";
    help << "  -s [definition]  Show defined character which are used to generate a password.\n";
    help << "  -t               Show the time when account was modified last time.\n";
    help << "  -a               Show all values of an account.\n";
    help << "  --all | -e       Show all accounts which are stored in database.\n";

    return help;
}

/**
 * Help text for command remove.
 * @return
 */
QStringList AppCommand::getHelpForRemove()
{
    QStringList help;
    help << "pwmanager remove <options>\n";
    help << "Removes an account from database.\n\n";
    help << "  -i <id>          The id (primary key) to identify a database entry.\n";
    help << "or\n";
    help << "  -p <name>        The name of a provider, Webpage, Device ...\n";
    help << "  -u <name>        A username to login.\n";

    return help;
}

/**
 * Help text for command file.
 * @return
 */
QStringList AppCommand::getHelpForFile()
{
    QStringList help;
    help << "pwmanager file <options>\n";
    help << "Writes data from database into a file. Or reads from file into database.\n\n";
    help << "  -f <path>        A full path to the file.\n";
    help << "  --file <path>  or  --file=<path>\n";
    help << "  -o               To write database content into a file.\n";
    help << "  --out\n";
    help << "  -g               Read (get) data from a file and store it to database.\n";
    help << "  --in\n";

    return help;
}
