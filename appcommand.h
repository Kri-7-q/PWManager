#ifndef MANAGERCOMMAND_H
#define MANAGERCOMMAND_H

#include "optiondefinition.h"
#include <QStringList>

class AppCommand
{
public:
    AppCommand(const char *parameter);
    ~AppCommand();

    enum Command { None, New, GeneratePW, Show, Remove, Modify, Help };

private:
    Command m_command;

public:
    Command command() const                 { return m_command; }
    QList<OptionDefinition> commandsOptions();
    QStringList getHelpText();

private:
    Command parseCommand(const char *parameter);

    QStringList getHelpInGeneral();
    QStringList getHelpForNew();
    QStringList getHelpForGeneratePW();
    QStringList getHelpForModify();
    QStringList getHelpForShow();
    QStringList getHelpForRemove();

    // Help text constants
    const QString optI = "  -i              The index of an account in the database.\n";
    const QString optP = "  -p              The name of a provider where the account is used.\n";
    const QString optU = "  -u              The user name which is used for login.\n";
    const QString optK = "  -k              Password (Keyword) which is used for login.\n";
    const QString optQ = "  -q              A question to indentify when forgot the password.\n";
    const QString optAnswer = "  --answer -r     The answer to the question.\n";
    const QString optA = "  -a              To show all of an account.\n";
    const QString optL = "  -l              Set the length of the password.\n";
    const QString optS = "  -s              Define the characters to use in password generation.\n                  for instance : 3[a-z]7{235njbccsv#+<>}6[A-Z]\n";
    const QString optAll = "  --all -e        To show all stored accounts.\n";

};

#endif // MANAGERCOMMAND_H
