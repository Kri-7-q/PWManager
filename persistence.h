#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "columnwidth.h"
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

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
    int deleteAccount(const OptionTable &optionTable);
    bool modifyAccount(const OptionTable &optionTable);
    Account passwordDefinition(const OptionTable &optionTable);
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
    QString sqlQueryColumns(const QStringList &columnsToQuery);
    QStringList tableColumnNames(QSqlDatabase &db, const QString &table);
    QString sqlWhereClauseFind(const OptionTable &optionTable, QList<char> &toBind);
    QString sqlWhereIdentify(const OptionTable &optionTable, QList<char> &optionList);
    QList<Account> getAccountList(QSqlQuery &query);
    QStringList optionToDatabaseNames(QList<char> optionList);
    QString sqlStringOfValue(const QVariant &value);
    QString sqlPlaceholderString(const QList<char> &optionList);
    QString sqlUpdateTouple(const OptionTable &optionTable, QList<char> optionList);
    QString sqlBindingString(const char option);

public:
    //Static
    static QString databaseNameOfOption(const char option);
};

#endif // PERSISTENCE_H
