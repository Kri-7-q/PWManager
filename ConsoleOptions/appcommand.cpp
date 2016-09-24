#include "appcommand.h"


/**
 * Constructor
 * @param parameter The first parameter of command line input.
 */
AppCommand::AppCommand(const QString &parameter) :
    m_optionAll(false)
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
        list << OptionDefinition('p', NeedArgument);
        list << OptionDefinition('u', NeedArgument);
        list << OptionDefinition('k', OptionalArgument);
        list << OptionDefinition('l', NeedArgument);
        list << OptionDefinition('s', NeedArgument);
        list << OptionDefinition('q', OptionalArgument);
        list << OptionDefinition('r', OptionalArgument, QVariant::String, QString("answer"));
        break;
    case GeneratePW:
        list << OptionDefinition('i', NeedArgument, QVariant::Int);
        list << OptionDefinition('p', NeedArgument);
        list << OptionDefinition('u', NeedArgument);
        list << OptionDefinition('l', NeedArgument, QVariant::Int);
        list << OptionDefinition('s', NeedArgument);
        break;
    case Show:
        list << OptionDefinition('i', OptionalArgument, QVariant::Int);
        list << OptionDefinition('p', OptionalArgument);
        list << OptionDefinition('u', OptionalArgument);
        list << OptionDefinition('k', OptionalArgument);
        list << OptionDefinition('l', OptionalArgument, QVariant::Int);
        list << OptionDefinition('s', OptionalArgument);
        list << OptionDefinition('q', OptionalArgument);
        list << OptionDefinition('t', OptionalArgument, QVariant::DateTime);
        list << OptionDefinition('r', OptionalArgument, QVariant::String, QString("answer"));
        list << OptionDefinition('a', &m_optionAll, QString("all"));
        break;
    case Modify:
        list << OptionDefinition('i', NeedArgument, QVariant::Int);
        list << OptionDefinition('p', NeedArgument);
        list << OptionDefinition('u', NeedArgument);
        list << OptionDefinition('k', NeedArgument);
        list << OptionDefinition('l', NeedArgument, QVariant::Int);
        list << OptionDefinition('s', NeedArgument);
        list << OptionDefinition('q', NeedArgument);
        list << OptionDefinition('r', NeedArgument, QVariant::String, QString("answer"));
        break;
    case Remove:
        list << OptionDefinition('i', NeedArgument);
        list << OptionDefinition('p', NeedArgument);
        list << OptionDefinition('u', NeedArgument);
        break;
    case File:
        list << OptionDefinition('f', NeedArgument, QVariant::String, QString("file"));
        list << OptionDefinition('o', NoArgument, QVariant::Invalid, QString("out"));
        list << OptionDefinition('g', NoArgument, QVariant::Invalid, QString("in"));
        list << OptionDefinition('v', NoArgument, QVariant::Invalid, QString("readable"));
        break;
    case Find:
        list << OptionDefinition('i', NoArgument, QVariant::Invalid);
        list << OptionDefinition('p', OptionalArgument);
        list << OptionDefinition('u', OptionalArgument);
        list << OptionDefinition('k', OptionalArgument);
        list << OptionDefinition('l', NoArgument, QVariant::Int);
        list << OptionDefinition('s', OptionalArgument);
        list << OptionDefinition('q', OptionalArgument);
        list << OptionDefinition('t', OptionalArgument, QVariant::DateTime);
        list << OptionDefinition('r', OptionalArgument, QVariant::String, QString("answer"));
        list << OptionDefinition('a', &m_optionAll, QString("all"));
        break;
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
    case Find:
        return getHelpForFind();
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
    if (commandString == "find") {
        return Find;
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
    help << "  -v               Visual human readable file out put with all account information.\n";
    help << "  --readable       (only for file out put.)\n";

    return help;
}

/**
 * Help text for command find.
 * @return
 */
QStringList AppCommand::getHelpForFind()
{
    QStringList help;
    help << "pwmanager find <options>\n";
    help << "Searches in all entries for the given parameter. Options with no value will cause an output\n";
    help << "of that parameter in search result.";
    help << "If to any option a value is given then this value will be used to find a database entry.\n\n";
    help << "  -i               Id of database entry or give an id to identify a database entry.\n";
    help << "  -p [name]        The provider name of an account.\n";
    help << "  -u [name]        The username of an account.\n";
    help << "  -k [password]    Password of an account.\n";
    help << "  -q [question]    Security question of an account.\n";
    help << "  -r [answer]      The answer (replay) to the security question.\n";
    help << "  --answer <answer> | --answer=\"answer\" | --answer=<answer>\n";
    help << "  -l               The length of password when generate.\n";
    help << "  -s [definition]  Defined character which are used to generate a password.\n";
    help << "  -t               The time when account was modified last time.\n";
    help << "  -a               All values of an account.\n";

    return help;
}
