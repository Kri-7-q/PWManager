#include "characterdefinition.h"

/**
 * Constructor
 * Standard
 */
CharacterDefinition::CharacterDefinition()
{
}

/**
 * Constructor
 * Creates a CharacterDefinition object with a range of characters.
 * @param amount
 * @param from
 * @param to
 */
CharacterDefinition::CharacterDefinition(unsigned short amount, QChar from, QChar to) :
    m_amount(amount)
{
    if (from.isLetter() && to.isLetter()) {
        if ((from.isUpper() && to.isLower()) ||
             (from.isLower() && to.isUpper())) {
            setCharacterRange(from.toUpper(), to.toUpper());
            setCharacterRange(from.toLower(), to.toLower());
        }
    } else {
        setCharacterRange(from, to);
    }
}

/**
 * Constructor
 * Creates a CharacterDefinition object with a set of character.
 * @param amount
 * @param characterList
 */
CharacterDefinition::CharacterDefinition(unsigned short amount, QList<QChar> characterList) :
    m_amount(amount)
{
    for (QChar character : characterList) {
        m_characterList << character;
    }
}

/**
 * Take two character and generate a list of character between from and to.
 * Both are included.
 * @param from
 * @param to
 */
void CharacterDefinition::setCharacterRange(const QChar from, const QChar to)
{
    uint current = from.unicode();
    m_characterList << from;
    do {
        m_characterList << QChar(++current);
    } while (current != to);
}
