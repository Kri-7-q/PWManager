#include "password.h"

/**
 * Constructor
 * Standard
 */
Password::Password() :
    m_hasError(false)
{
}

/**
 * Creates a password from a given definition and length.
 * @param passwordLength
 * @param definitionString
 * @return
 */
QString Password::passwordFromDefinition(const ushort passwordLength, const QString &definitionString)
{
    if (passwordLength < 1) {
        setErrorMessage(QString("Password length is %1 this is to less !").arg(passwordLength));
        return QString();
    }
    QList<CharacterDefinition> definitionList = parseCharacterDefinitionString(definitionString);
    if (hasError()) {
        return QString();
    }
    definitionList = fixAmountValues(passwordLength, definitionList);
    if (hasError()) {
        return QString();
    }
    QList<QChar> characterList;
    for (CharacterDefinition definition : definitionList) {
        characterList << randomCharacterFromDefinition(definition);
    }
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
QList<CharacterDefinition> Password::parseCharacterDefinitionString(const QString &definitionString)
{
    QList<CharacterDefinition> definitionList;

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
                definitionList << definition;
                state = Number;
                charAmount = 0;
                break;
            }
            setErrorMessage(QString("Expected ']' symbol but found : ").append(currentChar));
            break;
        case Set:
            if (currentChar == '}') {
                CharacterDefinition definition(charAmount, charList);
                definitionList << definition;
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

/**
 * Some definitions can have no amount value.
 * Those definitions get a calculated amount value.
 * Calculation achives password length.
 * @param passwordLength
 * @param definitionList
 * @return
 */
QList<CharacterDefinition> Password::fixAmountValues(const ushort passwordLength, const QList<CharacterDefinition> &definitionList)
{
    QList<CharacterDefinition> withoutAmountList;
    QList<CharacterDefinition> hasAmountList;
    ushort definedAmount = 0;
    for (CharacterDefinition definition : definitionList) {
        if (definition.hasNoAmount()) {
            withoutAmountList << definition;
        } else {
            definedAmount += definition.amount();
            hasAmountList << definition;
        }
    }
    if (definedAmount > passwordLength) {
        setErrorMessage(QString("%1 character are defined but length of password is %2 !").arg(definedAmount).arg(passwordLength));
    }
    if (withoutAmountList.isEmpty()) {
        return hasAmountList;
    }
    ushort calulatedAmount = (passwordLength - definedAmount) / withoutAmountList.size();
    ushort rest = (passwordLength - definedAmount) % withoutAmountList.size();
    for (CharacterDefinition &definition : withoutAmountList) {
        if (rest > 0) {
            definition.setAmount(calulatedAmount + 1);
            --rest;
        } else {
            definition.setAmount(calulatedAmount);
        }
    }
    hasAmountList << withoutAmountList;

    return hasAmountList;
}

/**
 * Get a randomly list of characters from a CharacterDefinition object.
 * @param definition
 * @return
 */
QList<QChar> Password::randomCharacterFromDefinition(const CharacterDefinition &definition)
{
    QList<QChar> list;
    qsrand(QTime::currentTime().msec());
    for (int i=0; i<definition.amount(); ++i) {
        int index = qrand() % definition.characterSet().size();
        list << definition.characterSet().takeAt(index);
    }

    return list;
}

// PRIVATE - Setter
void Password::setErrorMessage(const QString &message)
{
    m_hasError = true;
    m_errorMessage = message;
}
