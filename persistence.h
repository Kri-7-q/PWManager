#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "account.h"
#include "columnwidth.h"
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>


class Persistence
{
public:
    Persistence();

    void addDatabase(const QString &identifier, const QString &dbName, const QString &host, const ushort port, const QString &pw, const QString &username);
    QString errorMessage();
    bool persistAccount(const Account &account);
    QList<Account> findAccount(const Account &account);
    ColumnWidth columnWidthTable() const                   { return m_columnWidth; }
    bool hasError() const                                   { return m_hasError; }

private:
    const QString m_primaryKey;
    const QStringList m_unique;
    const QString m_tableName;
    const QString m_databaseName;
    ColumnWidth m_columnWidth;
    bool m_hasError;

    // Methods
    QString queryColumnString(const QStringList columnNames, const Account &account, const bool bindStrings = false);
    QStringList columnNames(QSqlDatabase &db, const QString &table);
    QString sqlWhereClauseFind(const Account &account);
    QList<Account> getAccountList(QSqlQuery &query);
};

#endif // PERSISTENCE_H
