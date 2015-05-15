#ifndef OPTIONDEFINITION_H
#define OPTIONDEFINITION_H

/* ------------------------------------------------------
 * Class OptionDefinition
 * ------------------------------------------------------
 * Class to define one option of this appliication.
 * Option needs a character to set it on the command line.
 * These option can be a single character or a long name.
 * The options value type can be chosen. Posible value
 * typse are the typse of QVariant.
 */

#include <QVariant>
#include <QString>


class OptionDefinition
{
public:
    OptionDefinition(const char option, const QVariant::Type dataType, const QString &longName);
    ~OptionDefinition();

    char option() const                         { return m_option; }
    QVariant::Type dataType() const             { return m_dataType; }
    bool hasLongName() const                    { return !m_longName.isNull(); }
    QString longName() const                    { return m_longName; }

private:
    char m_option;
    QVariant::Type m_dataType;
    QString m_longName;
};

#endif // OPTIONDEFINITION_H
