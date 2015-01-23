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
    QList<QChar> characterSet() const                   { return m_characterList; }
};

#endif // CHARACTERDEFINITION_H
