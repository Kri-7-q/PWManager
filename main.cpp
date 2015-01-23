#include "options.h"
#include "consoleinterface.h"
#include "password.h"

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
        Password pw;
        int passwordLength = account.valueWithOption('l').toInt();
        QString characterDefinition = account.valueWithOption('s').toString();
        QString password = pw.passwordFromDefinition(passwordLength, characterDefinition);
        if (pw.hasError()) {
            userInterface.printError(pw.errorMessage());
            return -1;
        }
        account.insertWithOption('k', QVariant(password));
        userInterface.printSingleAccount(account);
        break;
    }
    default:
        break;
    }

    return 1;
}
