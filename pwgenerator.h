#ifndef PASSWORD_H
#define PASSWORD_H

#include "characterdefinition.h"
#include <QTime>
#include <QDebug>

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
    QList<CharacterDefinition> parseCharacterDefinitionString(const QString &definitionString);
    QList<CharacterDefinition> fixAmountValues(const ushort passwordLength, const QList<CharacterDefinition> &definitionList);
    QList<QChar> randomCharacterFromDefinition(const CharacterDefinition &definition);
    void setErrorMessage(const QString &message);
    int separarteCharacterAmountDefined(QList<CharacterDefinition> &definitionList, QList<CharacterDefinition> &undefinedList);
    void fillDefinitionToPasswordLength(QList<CharacterDefinition> &list, const ushort addtoEach, ushort rest);
};

#endif // PASSWORD_H
