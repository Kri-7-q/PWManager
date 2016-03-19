#include "optionparser.h"
#include "consoleinterface.h"
#include "pwgenerator.h"
#include "postgresql.h"
#include "appcommand.h"
#include <QDebug>
#include <QDateTime>



// Global method
void setAllOptions(OptionTable& optionTable);




int main(int argc, char *argv[])
{
    ConsoleInterface userInterface;

    // Make sure that a parameter is given.
    QString commandString("help");
    if (argc > 1) {
        commandString = QString(argv[1]);
    }

    // Get command (first parameter after application name)
    // and its options.
    AppCommand appCommand(commandString);
    AppCommand::Command command = appCommand.command();
    if (command == AppCommand::Help) {
        userInterface.printHelp(appCommand.getHelpText());
        return -1;
    }

    // Get available option and add switches for 'help' and 'all'.
    bool needHelp = false;
    QList<OptionDefinition> optionDefinitionList = appCommand.commandsOptions();
    optionDefinitionList << OptionDefinition('h', &needHelp, QString("help"));

    // Get options from command line input.
    OptionParser parser(optionDefinitionList);
    OptionTable optionTable = parser.parseOptions(argc, argv, 2);
    if (parser.hasError()) {
        userInterface.printError(parser.errorMsg());
        userInterface.printHelp(appCommand.getHelpText());
        return 1;
    }
    if (parser.hasWarnings()) {
        userInterface.printWarnings(parser.warnings());
    }

    // Need help?
    if (needHelp) {
        userInterface.printHelp(appCommand.getHelpText());
        return 1;
    }

    // Open database
    Persistence* database = new PostgreSQL();
    if (! database->open()) {
        userInterface.printError(database->error());
        return -1;
    }

    // Execute command
    switch (command) {
    case AppCommand::New: {
        if (!optionTable.contains('k')) {
            PwGenerator pwGenerator;
            int passwordLength = optionTable.value('l').toInt();
            QString characterDefinition = optionTable.value('s').toString();
            QString password = pwGenerator.passwordFromDefinition(passwordLength, characterDefinition);
            if (pwGenerator.hasError()) {
                userInterface.printError(pwGenerator.errorMessage());
                return -1;
            }
            optionTable.insert('k', QVariant(password));
        }
        optionTable.insert('t', QVariant(QDateTime::currentDateTime()));
        bool result = database->persistAccountObject(optionTable);
        if (result) {
            userInterface.printSuccessMsg("Account successfully persisted.\n");
            QVariantMap account = database->findAccount(optionTable);
            userInterface.printSingleAccount(account);
        } else {
            userInterface.printError("Could not store new Account !");
            userInterface.printError(database->error());
        }
        break;
    }
    case AppCommand::Show: {
        if (appCommand.isOptionAllSet()) {
            setAllOptions(optionTable);
        }
        QList<QVariantMap> list = database->findAccountsLike(optionTable);
        if (database->hasError()) {
            userInterface.printError(database->error());
            break;
        }
        userInterface.printAccountList(list);
        break;
    }
    case AppCommand::Remove: {
        int rowsRemoved = database->deleteAccountObject(optionTable);
        if (database->hasError()) {
            userInterface.printError(database->error());
        } else {
            QString msg = QString("%1 accounts removed from database.\n").arg(rowsRemoved);
            userInterface.printSuccessMsg(msg);
        }
        break;
    }
    case AppCommand::Modify: {
        optionTable.insert('t', QDateTime::currentDateTime());
        if (database->modifyAccountObject(optionTable)) {
            userInterface.printSuccessMsg("Account object successfully updated.\n");
            QVariantMap account = database->findAccount(optionTable);
            userInterface.printSingleAccount(account);
        } else {
            userInterface.printError("Account could not be updated !\n");
            userInterface.printError(database->error());
        }
        break;
    }
    case AppCommand::GeneratePW: {
        // If not have new password definition then get it from database.
        if (! optionTable.contains('l') || ! optionTable.contains('s')) {
            OptionTable searchObj(optionTable);
            searchObj.insert('l', QVariant());
            searchObj.insert('s', QVariant());
            QVariantMap pwDefinition = database->findAccount(searchObj);
            if (pwDefinition.isEmpty()) {
                userInterface.printError("Could not read password definition.\n");
                return -1;
            }
            if (! optionTable.contains('l')) {
                optionTable.insert('l', pwDefinition.value(database->optionToRealName('l')));
            }
            if (! optionTable.contains('s')) {
                optionTable.insert('s', pwDefinition.value(database->optionToRealName('s')));
            }
        }
        int length = optionTable.value('l').toInt();
        QString definition = optionTable.value('s').toString();
        PwGenerator generator;
        QString password = generator.passwordFromDefinition(length, definition);
        if (generator.hasError()) {
            userInterface.printError(generator.errorMessage());
            return -1;
        }
        optionTable.insert('k', password);
        optionTable.insert('t', QDateTime::currentDateTime());
        if (database->modifyAccountObject(optionTable)) {
            userInterface.printSuccessMsg("New password generated and stored into database.\n");
        } else {
            userInterface.printError("Could not store new password into database !\n");
            userInterface.printError(database->error());
        }
        break;
    }
    case AppCommand::File:
        if (optionTable.contains('o')) {
            QList<QVariantMap> accountList = database->allPersistedAccounts();
            userInterface.writeToFile(accountList, optionTable.value('f').toString());
        }
        break;
    default:
        break;
    }

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
