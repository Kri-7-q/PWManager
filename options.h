#ifndef OPTIONS_H
#define OPTIONS_H

#include "account.h"
#include "persistence.h"
#include <getopt.h>

class Options
{
public:
    Options(char *parameter);

    enum Command { None, New, GeneratePW, Show, Remove, Modify, Help };

    // Methods
    Command command() const;
    void setCommand(const Command &command);
    Account parseOptions(const int argc, char **argv);

    bool hasError() const;
    void setHasError(bool hasError);
    QString errorMessage();
    void setErrorMessage(const QString &errorMessage);

private:
    Command m_command;
    bool m_needHelp;
    Account m_account;
    bool m_hasError;
    QString m_errorMessage;

    // Methods
    Command parseCommand(char *parameter);
    QString getCommandsOptions();
};

#endif // OPTIONS_H
