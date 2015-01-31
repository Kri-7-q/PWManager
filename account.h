#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QVariantHash>

class Account : public QVariantHash
{
public:
    Account();

    void insertWithOption(const char option, const QVariant &value);
    QVariant valueWithOption(const QChar &option);

    // Static methods
    static QString databaseNameOfOption(const QChar &option);
};

#endif // ACCOUNT_H
