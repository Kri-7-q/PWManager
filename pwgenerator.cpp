#include "pwgenerator.h"

/**
 * Constructor
 * Standard
 */
PwGenerator::PwGenerator() :
    m_hasError(false),
    m_standardDefinition("*[a-z]*[A-Z]*[0-9]"),
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
    if (definitionString.isNull()) {
        pwDefinition = m_standardDefinition;
    }

    if (pwLength < 1) {
        setErrorMessage(QString("Password length is %1 this is to less !").arg(pwLength));
        pwLength = m_standardLength;
    }
    QList<CharacterDefinition> definitionList = parseCharacterDefinitionString(pwDefinition);
    if (hasError()) {
        return QString();
    }
    definitionList = fixAmountValues(pwLength, definitionList);
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
QList<CharacterDefinition> PwGenerator::parseCharacterDefinitionString(const QString &definitionString)
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
QList<CharacterDefinition> PwGenerator::fixAmountValues(const ushort passwordLength, const QList<CharacterDefinition> &definitionList)
{
    QList<CharacterDefinition> withoutAmountList;
    QList<CharacterDefinition> hasAmountList(definitionList);
    int definedAmount = separarteCharacterAmountDefined(hasAmountList, withoutAmountList);
    if (definedAmount > passwordLength) {
        setErrorMessage(QString("%1 character are defined but length of password is %2 !").arg(definedAmount).arg(passwordLength));
    }
    if (withoutAmountList.isEmpty()) {
        if (definedAmount < passwordLength) {
            // Definition of password character is less the password length.
            ushort addAmount = (passwordLength - definedAmount) / hasAmountList.size();
            ushort rest = (passwordLength - definedAmount) % hasAmountList.size();
            fillDefinitionToPasswordLength(hasAmountList, addAmount, rest);
        }
        return hasAmountList;
    }
    ushort calulatedAmount = (passwordLength - definedAmount) / withoutAmountList.size();
    ushort rest = (passwordLength - definedAmount) % withoutAmountList.size();
    fillDefinitionToPasswordLength(withoutAmountList, calulatedAmount, rest);

    hasAmountList << withoutAmountList;

    return hasAmountList;
}

/**
 * Get a randomly list of characters from a CharacterDefinition object.
 * @param definition
 * @return
 */
QList<QChar> PwGenerator::randomCharacterFromDefinition(const CharacterDefinition &definition)
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
void PwGenerator::setErrorMessage(const QString &message)
{
    m_hasError = true;
    m_errorMessage = message;
}

/**
 * Takes a list with CharacterDefinition objects and separartes
 * them. There are some with a set of character but not defined
 * how many of those character should be used in password. These
 * objects are moved to the undefined list.
 * The method counts the total amount of characters to use in
 * password which are defined.
 * @param definitionList
 * @param undefinedList
 * @return definedAmount        How many characters are defined in definition objects.
 */
int PwGenerator::separarteCharacterAmountDefined(QList<CharacterDefinition> &definitionList, QList<CharacterDefinition> &undefinedList)
{
    ushort definedAmount = 0;
    int index = 0;
    while (index < definitionList.size()) {
        CharacterDefinition definition = definitionList[index];
        if (definition.hasNoAmount()) {
            definitionList.removeAt(index);
            undefinedList << definition;
        } else {
            definedAmount += definition.amount();
            ++index;
        }
    }

    return definedAmount;
}

/**
 * Password length is higher the defined amount of character.
 * Fills the definition the password length.
 * @param list
 * @param addtoEach
 * @param rest
 */
void PwGenerator::fillDefinitionToPasswordLength(QList<CharacterDefinition> &list, const ushort addtoEach, ushort rest)
{
    for (CharacterDefinition &definition : list) {
        int amount;
        if (definition.hasNoAmount()) {
            amount = addtoEach;
        } else {
            amount = definition.amount() + addtoEach;
        }

        if (rest > 0) {     // Add one as long as available. Is not enough for each object.
            definition.setAmount(amount + 1);
            --rest;
        } else {
            definition.setAmount(amount);
        }
    }
}
