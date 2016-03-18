#ifndef CHARACTERDEFINITION_H
#define CHARACTERDEFINITION_H

/* -----------------------------------------------------------------------
 * Class CharacterDefinition
 * -----------------------------------------------------------------------
 * The application needs a set of characters to generate a password out
 * of these characters.
 * The characters can be defined through to different mechanism. The user
 * can define a range of characters. The string to define a range would look
 * like '3[A-Z]' or '*[a-z]'. The number gives the amount of characters
 * which will be used of this range for the password.
 * The other kind of definition is a set of characters. A String to define
 * a set would look like '6{<>$%()=?+#*-_}'.
 * These two mechanisms can be combined to define all characters for a
 * password.
 * This class takes one subset of characters for the password.
 */

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
    int characterCount() const                          { return m_characterList.size(); }
    QList<QChar> getRandomSet() const;

private:
    void setCharacterRange(const QChar from, const QChar to);
};

#endif // CHARACTERDEFINITION_H
