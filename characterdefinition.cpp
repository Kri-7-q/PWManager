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
    ushort current = from.unicode();
    m_characterList << from;
    do {
        m_characterList << QChar(++current);
    } while (current != to);
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
