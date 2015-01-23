#ifndef OPTIONS_H
#define OPTIONS_H

#include "account.h"
#include "persistence.h"
#include <getopt.h>
#include <QHash>

class Options
{
public:
    Options(char *parameter);

    enum Command { None, New, GeneratePW, Show, Remove, Modify, Help };
    enum ErrorType { InvalidOption, LeftArgument };

    // Methods
    Command command() const;
    void setCommand(const Command &command);
    Account parseOptions(const int argc, char **argv);

    bool needsHelp() const                  { return m_needHelp; }
    bool hasError() const;
    void setHasError(bool hasError);
    QString errorMessage();
    QStringList getHelpText();

private:
    Command m_command;
    bool m_needHelp;
    Account m_account;
    bool m_hasError;
    QHash<ErrorType, QStringList> m_errorHashTable;

    // Methods
    Command parseCommand(char *parameter);
    QString getCommandsOptions();
    QString getInvalidOptionMsg() const;
    QString getLeftArgumentMsg() const;
    void setFailedValue(const ErrorType errorType, const QString &value);


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

#endif // OPTIONS_H
