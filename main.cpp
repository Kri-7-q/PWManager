#include "options.h"
#include "consoleinterface.h"
#include "pwgenerator.h"
#include "persistence.h"
#include "appcommand.h"
#include <QDebug>
#include <QDateTime>

typedef QHash<QString, QVariant> Account;

int main(int argc, char *argv[])
{
    ConsoleInterface userInterface;

    // Make sure that a parameter is given.
    QString commandString("help");
    if (argc > 1) {
        commandString = QString(argv[1]);
    }

    // Get command (first parameter after application name)
    AppCommand appCommand(commandString);
    AppCommand::Command command = appCommand.command();
    if (command == AppCommand::Help) {
        userInterface.printHelp(appCommand.getHelpText());
        return -1;
    }

    // Get options from command line input.
    Options options(appCommand.commandsOptions());
    OptionTable optionTable = options.parseOptions(argc, argv, 2);
    if (options.hasError()) {
        userInterface.printError(options.errorMessage());
        userInterface.printHelp(appCommand.getHelpText());
        return 1;
    }

    // Need help?
    if (optionTable.contains('h')) {
        userInterface.printHelp(appCommand.getHelpText());
        return 1;
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
        userInterface.printOptionTable(optionTable);
        Persistence database;
        optionTable.insert('t', QVariant(QDateTime::currentDateTime()));
        bool result = database.persistAccount(optionTable);
        if (result) {
            userInterface.printSuccessMsg("Account successfully persisted.\n");
        } else {
            userInterface.printError("Could not store new Account !");
            userInterface.printError(database.errorMessage());
        }
        break;
    }
    case AppCommand::Show: {
        if (optionTable.contains('a')) {
            optionTable.replaceOptionA();
        }
        if (optionTable.contains('e') && optionTable.size() < 2) {
            optionTable.insertStandardOptionForShow();
        }
        Persistence database;
        QList<Account> list = database.findAccount(optionTable);
        if (database.hasError()) {
            userInterface.printError(database.errorMessage());
            break;
        }
        userInterface.printAccountList(list);
        break;
    }
    case AppCommand::Remove: {
        Persistence database;
        int rowsRemoved = database.deleteAccount(optionTable);
        if (database.hasError()) {
            userInterface.printError(database.errorMessage());
        } else {
            QString msg = QString("%1 accounts removed from database.\n").arg(rowsRemoved);
            userInterface.printSuccessMsg(msg);
        }
        break;
    }
    case AppCommand::Modify: {
        Persistence database;
        optionTable.insert('t', QDateTime::currentDateTime());
        if (database.modifyAccount(optionTable)) {
            userInterface.printSuccessMsg("Account object successfully updated.\n");
        } else {
            userInterface.printError("Account could not be updated !\n");
            userInterface.printError(database.errorMessage());
        }
        break;
    }
    case AppCommand::GeneratePW: {
        Persistence database;
        // If not have new password definition then get it from database.
        if (! optionTable.contains('l') && ! optionTable.contains('s')) {
            Account pwDefinition = database.passwordDefinition(optionTable);
            if (pwDefinition.isEmpty()) {
                userInterface.printError("Could not read password definition.\n");
                return -1;
            }
            if (! optionTable.contains('l')) {
                optionTable.insert('l', pwDefinition.value("length"));
            }
            if (! optionTable.contains('s')) {
                optionTable.insert('s', pwDefinition.value("definition"));
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
        if (database.modifyAccount(optionTable)) {
            userInterface.printSuccessMsg("New password generated and stored into database.\n");
        } else {
            userInterface.printError("Could not store new password into database !\n");
            userInterface.printError(database.errorMessage());
        }
        break;
    }
    default:
        break;
    }

    return 1;
}
