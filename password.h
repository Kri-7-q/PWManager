#ifndef PASSWORD_H
#define PASSWORD_H

#include "characterdefinition.h"
#include <QTime>

class Password
{
public:
    Password();

    enum State { Number, Asterisk, RangeBegin, RangeTo, RangeEnd, RangeClose, Set };

private:
    QString m_errorMessage;
    bool m_hasError;

public:
    bool hasError() const                       { return m_hasError; }
    QString errorMessage() const                { return m_errorMessage; }
    QString passwordFromDefinition(const ushort passwordLength, const QString &definitionString);

private:
    QList<CharacterDefinition> parseCharacterDefinitionString(const QString &definitionString);
    QList<CharacterDefinition> fixAmountValues(const ushort passwordLength, const QList<CharacterDefinition> &definitionList);
    QList<QChar> randomCharacterFromDefinition(const CharacterDefinition &definition);
    void setErrorMessage(const QString &message);
};

#endif // PASSWORD_H
