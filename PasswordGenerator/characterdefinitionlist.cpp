#include "characterdefinitionlist.h"

/**
 * Standard constructor
 */
CharacterDefinitionList::CharacterDefinitionList(const int passwordLength) :
    m_totalShareCount(0),
    m_shareCount(passwordLength)
{

}

/**
 * Append a new character definition object to the definition list.
 * If a definition has not amount value set then the character count
 * will be added to count value of all characters from defintions
 * without a amount value. The total amount of characters from a
 * undefined definition is neccessary to calculate the percentage
 * of amount.
 * @param definition
 * @return
 */
bool CharacterDefinitionList::appendDefinition(const CharacterDefinition &definition)
{
    if (definition.hasNoAmount()) {
        m_totalShareCount += definition.characterCount();
    } else {
        m_shareCount -= definition.amount();
    }

    m_definitionList << definition;

    return true;
}

/**
 * Some definitions in the list can be undefined in the amount of
 * used characters for the password. Therefore the amount is calculated
 * by percentage. The definitions which contain the most character
 * will provide the most character for the password.
 * @return      True if done.
 */
bool CharacterDefinitionList::defineAmountForUndefined()
{
    if (m_shareCount < 1) {
        return false;
    }
    int distributedAmount = 0;
    CharacterDefinition& greatestDefinition = m_definitionList[0];
    for (int index=0; index<m_definitionList.size(); ++index) {
        CharacterDefinition& definition = m_definitionList[index];
        if (definition.hasNoAmount()) {
            if (greatestDefinition.characterCount() < definition.characterCount()) {
                greatestDefinition = definition;
            }
            int characterCount = definition.characterCount();
            float amount = (float)characterCount / (float)m_totalShareCount * (float)m_shareCount;
            definition.setAmount((int)amount);
            distributedAmount += amount;
        }
    }
    if (distributedAmount < m_shareCount) {
        int tmpAmount = greatestDefinition.amount();
        int toAdd = m_shareCount - distributedAmount + tmpAmount;
        greatestDefinition.setAmount(toAdd);
    }

    return true;
}

/**
 * Get a random set of characters to generate a password out of it.
 * The character list is randomly taken from the definitions.
 * @return list     A list with characters to create a password.
 */
QList<QChar> CharacterDefinitionList::getPasswordCharacterList() const
{
    QList<QChar> list;
    for (int index=0; index<m_definitionList.size(); ++index) {
        CharacterDefinition definition = m_definitionList[index];
        list << definition.getRandomSet();
    }

    return list;
}
