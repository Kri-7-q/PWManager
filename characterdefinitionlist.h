#ifndef CHARACTERDEFINITIONLIST_H
#define CHARACTERDEFINITIONLIST_H

#include <characterdefinition.h>

class CharacterDefinitionList
{
public:
    CharacterDefinitionList(const int passwordLength);

    bool appendDefinition(const CharacterDefinition& definition);
    bool defineAmountForUndefined();
    QList<QChar> getPasswordCharacterList() const;

private:
    QList<CharacterDefinition> m_definitionList;
    unsigned int m_totalShareCount;
    int m_shareCount;
};

#endif // CHARACTERDEFINITIONLIST_H
