#ifndef MANAGERCOMMAND_H
#define MANAGERCOMMAND_H

/* -----------------------------------------------------------------
 * Class AppCommand
 * -----------------------------------------------------------------
 * The class takes the first parameter of command line input.
 * These parameter should contain a command for this application.
 * The following commands are available :
 * - new
 * - show
 * - modify
 * - generatepw
 * - remove
 * - file
 * - find
 * - help
 * - user
 *
 * Each of these commands takes a specified set of options. the
 * option set are given here.
 * Additionaly the class provides help texts to the commands.
 */

#include "optiondefinition.h"
#include <QStringList>

class AppCommand
{
public:
    AppCommand(const int argc, const char* const argv[]);
    ~AppCommand();

    enum Command { None, New, GeneratePW, Show, Remove, Modify, Help, File, Find, User };

private:
    Command m_command;
    bool m_optionAll;
    bool m_isHelpNeeded;
    QString m_appName;
    quint8 m_requiredParameter;
    quint8 m_allowedParameter;

public:
    Command command() const                 { return m_command; }
    bool isOptionAllSet() const             { return m_optionAll; }
    bool isHelpNeeded() const               { return m_isHelpNeeded; }
    QList<OptionDefinition> commandsOptions();
    QStringList getHelpText();
    quint8 requiredParam() const            { return m_requiredParameter; }
    quint8 allowedParam() const             { return m_allowedParameter; }

protected:
    Command parseCommand(const QString &parameter);
    QString applicationName(const char* const parameter);

    QStringList getHelpInGeneral();
    QStringList getHelpForNew();
    QStringList getHelpForGeneratePW();
    QStringList getHelpForModify();
    QStringList getHelpForShow();
    QStringList getHelpForRemove();
    QStringList getHelpForFile();
    QStringList getHelpForFind();
    QStringList getHelpForUser();
};

#endif // MANAGERCOMMAND_H
