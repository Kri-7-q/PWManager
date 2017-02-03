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
 *
 * Converts string values from command line into appropriate Qvariant values.
 * Therefor this class has some virtual functions to convert strings to other
 * data types. These functions should be overriden for custom conversion.
 * -------------------------------------------------------------------------------
 */

#include <QVariant>
#include <QDateTime>


enum OptionType { InvalidDefinition, OptionalArgument, NeedArgument, NoArgument, SwitchOn };


class OptionDefinition
{
public:
    OptionDefinition();
    OptionDefinition(const char option, const OptionType optionArgument, const QVariant::Type dataType = QVariant::String,
                     const QString& formatString = QString(), const QString& longOption = QString());
    OptionDefinition(const char option, bool* pSwitch, const QString& longOption = QString());

    virtual ~OptionDefinition() {}

    char option() const             { return m_option; }
    QVariant::Type dataType() const { return m_dataType; }
    QString longOption() const      { return m_longOption; }
    QString formatString() const    { return m_formatString; }

    bool needValue() const          { return m_optionType == NeedArgument; }
    bool takesValue() const         { return m_optionType == OptionalArgument || m_optionType == NeedArgument; }
    bool isValid() const            { return m_optionType != InvalidDefinition; }
    bool isSwitch() const           { return m_optionType == SwitchOn; }
    void setSwitchOn()              { *m_pSwitch = true; }

    QVariant convertValue(const QString& stringValue) const;

protected:
    // Virtual converter functions
    virtual bool convertToBool(const QString& stringValue) const;
    virtual QDate convertToDate(const QString& stringValue) const;
    virtual QDateTime convertToDateTime(const QString& stringValue) const;
    virtual double convertToDouble(const QString& stringValue) const;
    virtual int convertToInt(const QString& stringValue) const;
    virtual qlonglong convertToLongLong(const QString& stringValue) const;
    virtual QTime convertToTime(const QString& stringValue) const;
    virtual uint convertToUInt(const QString& stringValue) const;
    virtual qulonglong convertToULongLong(const QString& stringValue) const;

private:
    char m_option;
    QVariant::Type m_dataType;
    QString m_longOption;
    OptionType m_optionType;
    bool* m_pSwitch;
    QString m_formatString;
};

#endif // OPTIONDEFINITION_H
