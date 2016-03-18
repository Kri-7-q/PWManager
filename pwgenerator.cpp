#include "pwgenerator.h"
#include <QTime>

/**
 * Constructor
 * Standard
 */
PwGenerator::PwGenerator() :
    m_hasError(false),
    m_standardDefinition("*[a-Z]*[0-9]"),
    m_standardLength(12)
{
}

/**
 * Creates a password from a given definition and length.
 * @param passwordLength
 * @param definitionString
 * @return
 */
QString PwGenerator::passwordFromDefinition(const ushort passwordLength, const QString &definitionString)
{
    int pwLength = passwordLength;
    QString pwDefinition(definitionString);
    if (definitionString.isEmpty()) {
        pwDefinition = m_standardDefinition;
    }

    if (pwLength < 1) {
        setErrorMessage(QString("Password length is %1 this is to less !").arg(pwLength));
        pwLength = m_standardLength;
    }
    CharacterDefinitionList definitionList(passwordLength);
    parseCharacterDefinitionString(pwDefinition, definitionList);
    if (hasError()) {
        return QString();
    }
    if (! definitionList.defineAmountForUndefined()) {
        setErrorMessage(QString("Could not define the amount values of character definitions !"));
        return QString();
    }
    QList<QChar> characterList = definitionList.getPasswordCharacterList();
    QString password;
    qsrand(QTime::currentTime().msec());
    while (! characterList.isEmpty()) {
        int index = qrand() % characterList.size();
        QChar current = characterList.takeAt(index);
        password.append(current);
    }

    return password;
}

/**
 * Takes a string containing the password definition.
 * @param definition
 * @return
 */
CharacterDefinitionList PwGenerator::parseCharacterDefinitionString(const QString &definitionString, CharacterDefinitionList &definitionList)
{
    unsigned short charAmount = 0;
    QChar rangeBegin;
    QChar rangeEnd;
    QList<QChar> charList;

    State state = Number;
    int index = 0;
    while (index < definitionString.length() && ! hasError()) {
        QChar currentChar = definitionString.at(index);
        switch (state) {
        case Number:
            if (currentChar == '*') {
                state = Asterisk;
                break;
            }
            if (currentChar.isDigit()) {
                charAmount = charAmount * 10 + currentChar.digitValue();
                break;
            }
            if (currentChar == '[') {
                state = RangeBegin;
                break;
            }
            if (currentChar == '{') {
                state = Set;
                break;
            }
            setErrorMessage(QString("Number was expected but found : ").append(currentChar));
            break;
        case Asterisk:
            if (currentChar == '[') {
                state = RangeBegin;
                break;
            }
            if (currentChar == '{') {
                state = Set;
                break;
            }
            setErrorMessage(QString("Expected '[' or '{' but found : ").append(currentChar));
            break;
        case RangeBegin:
            rangeBegin = currentChar;
            state = RangeTo;
            break;
        case RangeTo:
            if (currentChar == '-') {
                state = RangeEnd;
            } else {
                setErrorMessage(QString("Expected '-' sign but found : ").append(currentChar));
            }
            break;
        case RangeEnd:
            rangeEnd = currentChar;
            state = RangeClose;
            break;
        case RangeClose:
            if (currentChar == ']') {
                CharacterDefinition definition(charAmount, rangeBegin, rangeEnd);
                definitionList.appendDefinition(definition);
                state = Number;
                charAmount = 0;
                break;
            }
            setErrorMessage(QString("Expected ']' symbol but found : ").append(currentChar));
            break;
        case Set:
            if (currentChar == '}') {
                CharacterDefinition definition(charAmount, charList);
                definitionList.appendDefinition(definition);
                state = Number;
                charAmount = 0;
                charList.clear();
                break;
            }
            charList << currentChar;
            break;
        }
        ++index;
    }
    if (state != Number && ! hasError()) {
        setErrorMessage(QString("Password definition has corrupt end !"));
    }

    return definitionList;
}

// PRIVATE - Setter
void PwGenerator::setErrorMessage(const QString &message)
{
    m_hasError = true;
    m_errorMessage = message;
}
