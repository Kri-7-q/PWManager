#ifndef CHARACTERDEFINITION_H
#define CHARACTERDEFINITION_H

#include <QString>
#include <QList>

class CharacterDefinition
{
public:
    CharacterDefinition();
    CharacterDefinition(unsigned short amount, QChar from, QChar to);
    CharacterDefinition(unsigned short amount, QList<QChar> characterList);

private:
    unsigned short m_amount;
    QList<QChar> m_characterList;

public:
    unsigned short amount() const                       { return m_amount; }
    void setAmount(const ushort amount)                 { m_amount = amount; }
    QList<QChar> characterSet() const                   { return m_characterList; }
    bool hasNoAmount() const                            { return m_amount < 1; }
};

#endif // CHARACTERDEFINITION_H
