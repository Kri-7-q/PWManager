#include "options.h"
#include "consoleinterface.h"
#include "pwgenerator.h"
#include "persistence.h"
#include "appcommand.h"
#include <QDebug>
#include <QDateTime>


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
    QHash<QString, QString> table = options.parseOptions(argc, argv, 2);
    userInterface.printOptionTable(table, 10);
    return 1;

    Account account;
    // User needs help?

    // Execute command
    qDebug() << "Get command from class Options";
    qDebug() << "Switch command ...";
    switch (command) {
    case AppCommand::New: {
        PwGenerator pwGenerator;
        int passwordLength = account.valueWithOption('l').toInt();
        QString characterDefinition = account.valueWithOption('s').toString();
        QString password = pwGenerator.passwordFromDefinition(passwordLength, characterDefinition);
        if (pwGenerator.hasError()) {
            userInterface.printError(pwGenerator.errorMessage());
            return -1;
        }
        account.insertWithOption('k', QVariant(password));
        userInterface.printSingleAccount(account);
        Persistence database;
        account.insertWithOption('t', QVariant(QDateTime::currentDateTime()));
        bool result = database.persistAccount(account);
        qDebug() << "Write into database successfully : " << result;
        if (result) {
            userInterface.printSuccessMsg("Account successfully persisted.\n");
        } else {
            userInterface.printError(database.errorMessage());
        }
        break;
    }
    case AppCommand::Show: {
        qDebug() << "Account before replace 'a' ...";
        userInterface.printSingleAccount(account);
        qDebug() << "Account after replace 'a' ...";
        userInterface.printSingleAccount(account);
        Persistence database;
        QList<Account> list = database.findAccount(account);
        if (database.hasError()) {
            qDebug() << "Database error ...";
            userInterface.printError(database.errorMessage());
            break;
        }
        qDebug() << "Get column width table from class Database ...";
        ColumnWidth columnWidth = database.columnWidthTable();
        userInterface.printAccountList(list, columnWidth);
        break;
    }
    default:
        break;
    }

    return 1;
}
