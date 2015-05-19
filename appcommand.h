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
 * - help
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
    AppCommand(const QString &parameter);
    ~AppCommand();

    enum Command { None, New, GeneratePW, Show, Remove, Modify, Help };

private:
    Command m_command;

public:
    Command command() const                 { return m_command; }
    QList<OptionDefinition> commandsOptions();
    QStringList getHelpText();

private:
    Command parseCommand(const QString &parameter);

    QStringList getHelpInGeneral();
    QStringList getHelpForNew();
    QStringList getHelpForGeneratePW();
    QStringList getHelpForModify();
    QStringList getHelpForShow();
    QStringList getHelpForRemove();

    // Help text constants
    static const QString optI;
    static const QString optP;
    static const QString optU;
    static const QString optK;
    static const QString optQ;
    static const QString optAnswer;
    static const QString optA;
    static const QString optL;
    static const QString optS;
    static const QString optAll;

};

#endif // MANAGERCOMMAND_H
