#ifndef PASSWORD_H
#define PASSWORD_H

#include "characterdefinition.h"

class Password
{
public:
    Password();

private:
    QList<CharacterDefinition> parseCharacterDefinitionString(const QString definition);
};

#endif // PASSWORD_H
