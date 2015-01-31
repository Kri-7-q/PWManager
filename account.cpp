#include "account.h"

Account::Account()
{
}

/**
 * Insert a value with option as key.
 * Option will be translated to QString account key.
 * @param option
 * @param value
 */
void Account::insertWithOption(const char option, const QVariant &value)
{
    QString key = databaseNameOfOption(option);
    insert(key, value);
}

/**
 * Get a value with option char as key.
 * @param option
 * @return
 */
QVariant Account::valueWithOption(const QChar &option)
{
    QString key = databaseNameOfOption(option);

    return value(key, QVariant());
}

/**
 * Translates an option char to database name.
 * @param option
 * @return
 */
QString Account::databaseNameOfOption(const QChar &option)
{
    switch (option.toLatin1()) {
    case 'p':
        return "provider";
        break;
    case 'u':
        return "username";
        break;
    case 'i':
        return "id";
        break;
    case 'q':
        return "question";
        break;
    case 'r':
        return "answer";
        break;
    case 'l':
        return "passwordLength";
        break;
    case 's':
        return "definedCharacter";
        break;
    case 'k':
        return "password";
        break;
    case 't':
        return "lastModified";
        break;
    default:
        return QString(option);
        break;
    }
}
