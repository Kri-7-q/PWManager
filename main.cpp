#include "options.h"
#include "consoleinterface.h"
#include "pwgenerator.h"
#include "persistence.h"
#include <QDebug>
#include <QDateTime>


int main(int argc, char *argv[])
{
    ConsoleInterface userInterface;

    // Get options from command line input.
    Options options(argv[1]);
    Account account = options.parseOptions(argc, argv);
    if (options.hasError()) {
        userInterface.printError(options.errorMessage());
        return -1;
    }

    // User needs help?
    if (options.needsHelp()) {
        userInterface.printHelp(options.getHelpText());
        return 1;
    }

    // Execute command
    Options::Command command = options.command();
    switch (command) {
    case Options::New: {
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
    default:
        break;
    }

    return 1;
}
