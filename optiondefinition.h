#ifndef OPTIONDEFINITION_H
#define OPTIONDEFINITION_H

/* -------------------------------------------------------------------------------
 * OptionDefinition class
 *
 * A class to define console options which can be taken from a application.
 * An object of this class must be fully initialized by one of the constructors.
 * The standard constructor is used to indicate errors.
 *
 * The first constructor can create options for all the following examoles:
 *  AppName -x -file=/users/horst/text.txt -l12 -ntr
 *  AppName -nHorst -p password
 * The parser will create a QHash table with all found options and its values.
 * Options can be marked to take a value as a MUST or optional. They can be
 * marked to do not take a value as well. But if a value is found with an option
 * which should not take any then the value is taken and a warning is generated.
 *
 * The second constructor creates an option with takes a pointer to a boolean
 * which will be set to true if this option occures. Such an option do not take
 * a value.
 * -------------------------------------------------------------------------------
 */

#include <QVariant>


enum OptionType { InvalidDefinition, OptionalArgument, NeedArgument, NoArgument, SwitchOn };


class OptionDefinition
{
public:
    OptionDefinition();
    OptionDefinition(const char option, const OptionType optionArgument,
                     const QVariant::Type dataType = QVariant::String, const QString& longOption = QString());
    OptionDefinition(const char option, bool* pSwitch, const QString& longOption = QString());

    char option() const             { return m_option; }
    QVariant::Type dataType() const { return m_dataType; }
    QString longOption() const      { return m_longOption; }

    bool needValue() const          { return m_optionType == NeedArgument; }
    bool takesValue() const         { return m_optionType == OptionalArgument || m_optionType == NeedArgument; }
    bool isValid() const            { return m_optionType != InvalidDefinition; }
    bool isSwitch() const           { return m_optionType == SwitchOn; }
    void setSwitchOn()              { *m_pSwitch = true; }

private:
    char m_option;
    QVariant::Type m_dataType;
    QString m_longOption;
    OptionType m_optionType;
    bool* m_pSwitch;
};

#endif // OPTIONDEFINITION_H
