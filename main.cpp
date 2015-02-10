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

    // Get command
    AppCommand appCommand(argv[1]);
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
        PwGenerator pwGenerator;
        qDebug() << "Get password length and definition ...";
        int passwordLength = optionTable.value('l').toInt();
        QString characterDefinition = optionTable.value('s').toString();
        qDebug() << "Length : " << passwordLength << "\tDefinition : " << characterDefinition << "\tIs NULL : " << characterDefinition.isNull();
        QString password = pwGenerator.passwordFromDefinition(passwordLength, characterDefinition);
        if (pwGenerator.hasError()) {
            userInterface.printError(pwGenerator.errorMessage());
            return -1;
        }
        optionTable.insert('k', QVariant(password));
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
        Persistence database;
        QList<Account> list = database.findAccount(optionTable);
        if (database.hasError()) {
            userInterface.printError(database.errorMessage());
            break;
        }
        ColumnWidth columnWidth = database.columnWidthTable();
        userInterface.printAccountList(list, columnWidth);
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
    default:
        break;
    }

    return 1;
}
