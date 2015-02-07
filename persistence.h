#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "columnwidth.h"
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

typedef QHash<QString, QVariant> Account;
typedef QHash<char, QVariant> OptionTable;

class Persistence
{
public:
    Persistence();

    void addDatabase(const QString &identifier, const QString &dbName, const QString &host, const ushort port, const QString &pw, const QString &username);
    QString errorMessage();
    bool persistAccount(const OptionTable &optionTable);
    QList<Account> findAccount(const OptionTable &optionTable);
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
    QString queryColumnString(const QStringList &columnsToQuery, const bool bindStrings = false);
    QStringList columnNames(QSqlDatabase &db, const QString &table);
    QString sqlWhereClauseFind(const OptionTable &optionTable);
    QList<Account> getAccountList(QSqlQuery &query);
    QStringList optionToDatabaseNames(QList<char> optionList);
    QString sqlStringOfValue(const QVariant &value);
    QString sqlPlaceholderString(const int columnCount);

public:
    //Static
    static QString databaseNameOfOption(const char option);
};

#endif // PERSISTENCE_H
