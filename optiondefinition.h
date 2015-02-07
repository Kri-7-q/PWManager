#ifndef OPTIONDEFINITION_H
#define OPTIONDEFINITION_H

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
