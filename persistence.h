#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "account.h"
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


class Persistence
{
public:
    Persistence();

    QString errorMessage();
    bool persistAccount(const Account &account);

private:
    const QString primaryKey;
    const QStringList unique;
    const QString tableName;
    const QString databaseName;

    // Methods
    bool openLocalDatabase(const QString &username, const QString &password);
    QString queryColumnString(const QStringList columnNames, const Account &account, const bool bindStrings = false);
    QStringList columnNames(QSqlDatabase &db, const QString &table);
};

#endif // PERSISTENCE_H
