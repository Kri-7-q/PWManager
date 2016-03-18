#ifndef PASSWORD_H
#define PASSWORD_H

#include "characterdefinition.h"
#include "characterdefinitionlist.h"

class PwGenerator
{
public:
    PwGenerator();

    enum State { Number, Asterisk, RangeBegin, RangeTo, RangeEnd, RangeClose, Set };

private:
    QString m_errorMessage;
    bool m_hasError;
    QString m_standardDefinition;
    int m_standardLength;

public:
    bool hasError() const                       { return m_hasError; }
    QString errorMessage() const                { return m_errorMessage; }
    QString passwordFromDefinition(const ushort passwordLength, const QString &definitionString);

private:
    CharacterDefinitionList parseCharacterDefinitionString(const QString &definitionString, CharacterDefinitionList& definitionList);
    void setErrorMessage(const QString &message);
};

#endif // PASSWORD_H
