#include "ConsoleOptions/optionparser.h"
#include "commandprocessor.h"
#include "Persistence/persistencefactory.h"
#include <QDebug>
#include <QDateTime>



// Global method
void setAllOptions(OptionTable& optionTable);
QVariantMap variantMapFromOptionTable(const OptionTable& table, const Persistence* db,
                                      const bool onlyValues = false, const QList<char> &removeList = QList<char>());
OptionTable removeAllValuesExceptOf(const QList<char>& exceptionKeyList, const OptionTable& table);
void setAttributePrintOrder(ConsoleInterface& iface, Persistence* database);


int main(int argc, char *argv[])
{
    ConsoleInterface userInterface;

    // Get command (first parameter after application name)
    // and its options.
    AppCommand appCommand(argc, argv);
    AppCommand::Command command = appCommand.command();

    // Get options from command line input.
    QList<OptionDefinition> optionDefinitionList = appCommand.commandsOptions();
    OptionParser parser(optionDefinitionList);
    OptionTable optionTable = parser.parseOptions(argc, argv, 2);
    if (parser.hasError()) {
        userInterface.printError(parser.errorMsg());
        userInterface.printHelp(appCommand.getHelpText());
        return 0;
    }
    if (parser.hasWarnings()) {
        userInterface.printWarnings(parser.warnings());
    }

    // Need help?
    // Handle help before open database because it is not required for help text.
    if (command == AppCommand::Help || appCommand.isHelpNeeded()) {
        userInterface.printHelp(appCommand.getHelpText());
        return 0;
    }

    // Open database
    Persistence* database = PersistenceFactory::createPersistence(PersistenceFactory::SqlPostgre);
    if (! database->open()) {
        userInterface.printError(database->error());
        return 0;
    }

    // Get print order for console interface.
    setAttributePrintOrder(userInterface, database);

    // If option '-a' is set.
    // This option must be replaced with all available options for the command.
    if (appCommand.isOptionAllSet()) {
        setAllOptions(optionTable);
    }

    // Execute command
    CommandProcessor processor(userInterface, database);
    processor.process(command, optionTable);

    // Close open persistence.
    database->close();
    delete database;

    return 1;
}



/**
 * If option 'a' or 'all' was used by the user then there must be inserted
 * some missing options into the OptionTable.
 * @param optionTable       Result of the option parser.
 * @param definitionList    The currently used option definition.
 */
void setAllOptions(OptionTable &optionTable)
{
    QList<char> optionList = QList<char>() << 'i' << 'p' << 'u' << 'k' << 'q' << 'r' << 'l' << 's' << 't';
    for (int index=0; index<optionList.size(); ++index) {
        QVariant value = optionTable.value(optionList[index], QVariant());
        optionTable.insert(optionList[index], value);
    }
}

/**
 * User interface needs a ordered list of attributes to print output in that order.
 * @param iface
 * @param database
 */
void setAttributePrintOrder(ConsoleInterface& iface, Persistence* database) {
    QList<char> optionList = QList<char>() << 'i' << 'p' << 'u' << 'k' << 'l' << 's' << 'q' << 'r' << 't';
    QStringList printOrder;
    for (int index=0; index<optionList.size(); ++index) {
        printOrder << database->optionToRealName(optionList[index]);
    }
    iface.setPrintOrderList(printOrder);
}
