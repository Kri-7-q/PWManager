#include "appcommand.h"


/**
 * Constructor
 * @param parameter The first parameter of command line input.
 */
AppCommand::AppCommand(const int argc, const char * const argv[]) :
    m_optionAll(false),
    m_isHelpNeeded(false),
    m_requiredParameter(0),
    m_allowedParameter(0)
{
    if (argc < 2) {
        m_command = Help;
    } else {
        m_command = parseCommand(QString(argv[1]));
    }
    m_appName = applicationName(argv[0]);
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
    QHash<char, QStringList> helpMap = optionsHelpTextMap();
    QList<OptionDefinition> list;
    list << OptionDefinition('h', &m_isHelpNeeded, QString("help")).setHelpTextLines(helpMap.value('h'));
    switch (m_command) {
    case New:
        list << OptionDefinition('p', NeedArgument).setHelpTextLines(helpMap.value('p'));
        list << OptionDefinition('u', NeedArgument).setHelpTextLines(helpMap.value('u'));
        list << OptionDefinition('k', OptionalArgument).setHelpTextLines(helpMap.value('k'));
        list << OptionDefinition('l', NeedArgument).setHelpTextLines(helpMap.value('l'));
        list << OptionDefinition('s', NeedArgument).setHelpTextLines(helpMap.value('s'));
        list << OptionDefinition('q', OptionalArgument).setHelpTextLines(helpMap.value('p'));
        list << OptionDefinition('r', OptionalArgument, QVariant::String, QString(), QString("answer"))
                .setHelpTextLines(helpMap.value('r'));
        break;
    case GeneratePW:
        list << OptionDefinition('i', NeedArgument, QVariant::Int).setHelpTextLines(helpMap.value('i'));
        list << OptionDefinition('p', NeedArgument).setHelpTextLines(helpMap.value('p'));
        list << OptionDefinition('u', NeedArgument).setHelpTextLines(helpMap.value('u'));
        list << OptionDefinition('l', NeedArgument, QVariant::Int).setHelpTextLines(helpMap.value('l'));
        list << OptionDefinition('s', NeedArgument).setHelpTextLines(helpMap.value('s'));
        break;
    case Show:
        list << OptionDefinition('i', OptionalArgument, QVariant::Int).setHelpTextLines(helpMap.value('i'));
        list << OptionDefinition('p', OptionalArgument).setHelpTextLines(helpMap.value('p'));
        list << OptionDefinition('u', OptionalArgument).setHelpTextLines(helpMap.value('u'));
        list << OptionDefinition('k', OptionalArgument).setHelpTextLines(helpMap.value('k'));
        list << OptionDefinition('l', OptionalArgument, QVariant::Int).setHelpTextLines(helpMap.value('l'));
        list << OptionDefinition('s', OptionalArgument).setHelpTextLines(helpMap.value('s'));
        list << OptionDefinition('q', OptionalArgument).setHelpTextLines(helpMap.value('q'));
        list << OptionDefinition('t', OptionalArgument, QVariant::DateTime).setHelpTextLines(helpMap.value('t'));
        list << OptionDefinition('r', OptionalArgument, QVariant::String, QString(), QString("answer"))
                .setHelpTextLines(helpMap.value('r'));
        list << OptionDefinition('a', &m_optionAll, QString("all")).setHelpTextLines(helpMap.value('a'));
        break;
    case Modify:
        list << OptionDefinition('i', NeedArgument, QVariant::Int).setHelpTextLines(helpMap.value('i'));
        list << OptionDefinition('p', NeedArgument).setHelpTextLines(helpMap.value('p'));
        list << OptionDefinition('u', NeedArgument).setHelpTextLines(helpMap.value('u'));
        list << OptionDefinition('k', NeedArgument).setHelpTextLines(helpMap.value('k'));
        list << OptionDefinition('l', NeedArgument, QVariant::Int).setHelpTextLines(helpMap.value('l'));
        list << OptionDefinition('s', NeedArgument).setHelpTextLines(helpMap.value('s'));
        list << OptionDefinition('q', NeedArgument).setHelpTextLines(helpMap.value('q'));
        list << OptionDefinition('r', NeedArgument, QVariant::String, QString(), QString("answer"))
                .setHelpTextLines(helpMap.value('r'));
        break;
    case Remove:
        list << OptionDefinition('i', NeedArgument).setHelpTextLines(helpMap.value('i'));
        list << OptionDefinition('p', NeedArgument).setHelpTextLines(helpMap.value('p'));
        list << OptionDefinition('u', NeedArgument).setHelpTextLines(helpMap.value('u'));
        break;
    case File:
        list << OptionDefinition('f', NeedArgument, QVariant::String, QString(), QString("file"))
                .setHelpTextLines(helpMap.value('i'));
        list << OptionDefinition('o', NoArgument, QVariant::Invalid, QString(), QString("out"))
                .setHelpTextLines(helpMap.value('o'));
        list << OptionDefinition('g', NoArgument, QVariant::Invalid, QString(), QString("in"))
                .setHelpTextLines(helpMap.value('g'));
        list << OptionDefinition('v', NoArgument, QVariant::Invalid, QString(), QString("readable"))
                .setHelpTextLines(helpMap.value('v'));
        break;
    case Find:
        m_requiredParameter = 1;
        m_allowedParameter = 1;
        break;
    case User:
        list << OptionDefinition('n', NoArgument, QVariant::Invalid, QString(), QString("name"))
                .setHelpTextLines(helpMap.value('n'));
        list << OptionDefinition('i', NoArgument, QVariant::Invalid, QString(), QString("id"))
                .setHelpTextLines(helpMap.value('i'));
        list << OptionDefinition('m', NoArgument, QVariant::Invalid, QString(), QString("email"))
                .setHelpTextLines(helpMap.value('m'));
        list << OptionDefinition('x', NoArgument, QVariant::Invalid, QString(), QString("active"))
                .setHelpTextLines(helpMap.value('x'));
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
QStringList AppCommand::getHelpText(const QList<OptionDefinition>& optionList) const
{
    QStringList list;
    switch (m_command) {
    case New:
        list << QString(m_appName).append(" new -p <value> -u <value> [other options]\n");
        list << "Insert a new account into database.\n\n";
        break;
    case GeneratePW:
        list << QString(m_appName).append(" generatepw -i <value> [other options]\n");
        list << QString(m_appName).append(" generatepw -p <value> -u <value> [other options]\n");
        list << "Generates a new passwort for an given account.\n";
        list << "If no length and character set is given application will try\n";
        list << "to read them from database.\n\n";
        break;
    case Modify:
        list << QString(m_appName).append(" modify -i <value> [other options]\n");
        list << QString(m_appName).append(" modify -p <value> -u <value> [other options]\n");
        list << "Can modify some values of an account.\n\n";
        break;
    case Show:
        list << QString(m_appName).append(" show [options]\n");
        list << "Shows chosen infomation about one or more accounts.\n";
        list << "If to any option a value is given then this value will be used to find a database entry.\n\n";
        break;
    case Remove:
        list << QString(m_appName).append(" remove -i <value>\n");
        list << QString(m_appName).append(" remove -p <value>\n");
        list << "Removes an account from database.\n\n";
        break;
    case File:
        list << QString(m_appName).append(" file [options]\n");
        list << "Writes data from database into a file. Or reads from file into database.\n\n";
        break;
    case Find:
        list << QString(m_appName).append(" find <searchMask>\n");
        list << "Searches the database for all provider names and matches the 'searchMask'.\n";
        list << "It is a fuzzy string compare to find a provider with a search mask.\n";
        break;
    case User:
        list << QString(m_appName).append(" user [options]\n");
        list << "Shows infomation about the current computer user.\n";
        list << "The user who is logged on currently will must be registered for this application.\n";
        list << "Information to registered users are stored in database.\n\n";
        break;
    default:
        list << QString(m_appName).append(" <command> <options>\n");
        list << QString(m_appName).append(" <command> --help\n");
        list << "   Commands :  Help, New, GeneratePW, Show, Modify, Remove\n\n";
        list << "   help        Shows this help text.\n";
        list << "   new         Adds a new account to database.\n";
        list << "   generatepw  Generates a new password for an existing account.\n";
        list << "   show        To show one or more accounts.\n";
        list << "   modify      Modifies an existing account.\n";
        list << "   remove      Removes an existing account from database.\n";
        list << "   file        Write database content to file. Or read from file.\n";
        list << "   user        Get information about the current user.\n";
        list << "   --help      Shows a help text to the command.\n";
        break;
    }
    foreach(OptionDefinition definition, optionList) {
        list << definition.helpText();
    }

    return list;
}

/**
 * Parse the first command line parameter.
 * It should fit to one of the applications commands.
 * @return
 */
AppCommand::Command AppCommand::parseCommand(const QString &parameter)
{
    QString commandString = QString(parameter).toLower();
    if (commandString == "show") {
        return Show;
    }
    if (commandString == "new") {
        return New;
    }
    if (commandString == "generatepw") {
        return GeneratePW;
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
    if (commandString == "user") {
        return User;
    }

    return Help;
}

/**
 * Get the application name from command line parameter.
 * @param parameter     The first command line parameter.
 * @return              A string with the application name.
 */
QString AppCommand::applicationName(const char * const parameter)
{
    QString path(parameter);
    int index = path.lastIndexOf('/');
    if (index < 0) {
        return path;
    }

    return path.right(path.length() - index - 1);
}

/**
 * Hash map with help texts to each option.
 * @return
 */
QHash<char, QStringList> AppCommand::optionsHelpTextMap() const
{
    QHash<char, QStringList> helpTextMap;
    helpTextMap.insert('i', QStringList() << "The id (primary key) to identify database entry.\n"
                                          << "This can be an id of an account entry or the id of a\n"
                                          << "registered user in database.\n");
    helpTextMap.insert('p', QStringList() << "The name of a provider, Webpage, Device ...\n"
                                          << "The information to where the login data belongs.\n");
    helpTextMap.insert('u', QStringList() << "A username to login.\n");
    helpTextMap.insert('k', QStringList() << "An existing password to store.\n");
    helpTextMap.insert('q', QStringList() << "A security question to restore the password\n"
                                          << "If you can't recall your password you may can get a new one\n"
                                          << "with a security question and its answer.\n");
    helpTextMap.insert('r', QStringList() << "An answer (replay) to the security question.\n"
                                          << "If you have to recover a lost password.\n");
    helpTextMap.insert('l', QStringList() << "The length of passwort used when generate any.\n");
    helpTextMap.insert('s', QStringList() << "A definition string with characters to generate a passwort.\n"
                                          << "For instance : 6[A-Z]8[a-z]*[0-9]4{+#-.,<>()}\n"
                                          << "[] Range of characters, {} set of characters, * wildcard for amount\n"
                                          << "5[a-z] means 5 characters in the range from a to z.\n"
                                          << "The amount of defined characters MUST fit the password length.\n");
    helpTextMap.insert('t', QStringList() << "The date of last modify.");
    helpTextMap.insert('a', QStringList() << "Set all available options.");
    helpTextMap.insert('e', QStringList() << "Select all accounts in database.");
    helpTextMap.insert('f', QStringList() << "A full path to the file.\n");
    helpTextMap.insert('o', QStringList() << "To write database content into a file.\n");
    helpTextMap.insert('g', QStringList() << "Read (get) data from a file and store it to database.\n");
    helpTextMap.insert('v', QStringList() << "Visual human readable file out put with all account information.\n"
                                          << "(only for file out put.)\n");
    helpTextMap.insert('n', QStringList() << "Show the name of user.\n");
    helpTextMap.insert('m', QStringList() << "Show the users email.\n");
    helpTextMap.insert('x', QStringList() << "Show if the user is active.\n");
    helpTextMap.insert('h', QStringList() << "Shows help to a command.\n");


    return helpTextMap;
}

/**
 * General help text.
 * @return
 */
QStringList AppCommand::getHelpInGeneral()
{
    QStringList help;
    help << QString(m_appName).append(" <command> <options>\n");
    help << QString(m_appName).append(" <command> --help\n");
    help << "   Commands :  Help, New, GeneratePW, Show, Modify, Remove\n\n";
    help << "   help        Shows this help text.\n";
    help << "   new         Adds a new account to database.\n";
    help << "   generatepw  Generates a new password for an existing account.\n";
    help << "   show        To show one or more accounts.\n";
    help << "   modify      Modifies an existing account.\n";
    help << "   remove      Removes an existing account from database.\n";
    help << "   file        Write database content to file. Or read from file.\n";
    help << "   user        Get information about the current user.\n";
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
    help << QString(m_appName).append(" new <options>\n");
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
    help << QString(m_appName).append(" generatepw <options>\n");
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
    help << QString(m_appName).append(" modify <options>\n");
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
    help << QString(m_appName).append(" show <options>\n");
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
    help << QString(m_appName).append(" remove <options>\n");
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
    help << QString(m_appName).append(" file <options>\n");
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
    help << QString(m_appName).append(" find <searchMask>\n");
    help << "Searches the database for all provider names and matches the 'searchMask'.\n";
    help << "It is a fuzzy string compare to find a provider with a search mask.\n";

    return help;
}

/**
 * Help text for command user.
 * @return
 */
QStringList AppCommand::getHelpForUser()
{
    QStringList help;
    help << QString(m_appName).append(" user <options>\n");
    help << "Shows infomation about the current computer user.\n";
    help << "The user who is logged on currently will must be registered for this application.\n";
    help << "Information to registered users are stored in database.\n\n";
    help << "  -i | --id        Show id of user.\n";
    help << "  -n | --name      Show the name of user.\n";
    help << "  -m | --email     Show the users email.\n";
    help << "  -x | --active    Show if the user is active.\n";

    return help;
}
