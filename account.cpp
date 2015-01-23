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
    QString key = Persistence::getDatabaseNameFor(option);
    insert(key, value);
}

/**
 * Get a value with option char as key.
 * @param option
 * @return
 */
QVariant Account::valueWithOption(const char option)
{
    QString key = Persistence::getDatabaseNameFor(option);

    return value(key, QVariant());
}
