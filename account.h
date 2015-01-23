#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "persistence.h"
#include <QVariantHash>

class Account : public QVariantHash
{
public:
    Account();

    void insertWithOption(const char option, const QVariant &value);
    QVariant valueWithOption(const char option);
};

#endif // ACCOUNT_H
