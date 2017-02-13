#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "UserInterface/consoleinterface.h"
#include "ConsoleOptions/appcommand.h"
#include "PasswordGenerator/pwgenerator.h"
#include "Persistence/persistence.h"

class CommandProcessor
{
public:
    CommandProcessor(ConsoleInterface& iface, Persistence* database);

    void process(AppCommand::Command command, OptionTable& optionTable);

private:
    ConsoleInterface& m_userInterface;
    Persistence* m_pDatabase;
};

#endif // COMMANDPROCESSOR_H
