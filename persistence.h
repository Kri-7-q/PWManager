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
typedef QList<QPair<QString, QVariant> > ColumnValuePairs;

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
    bool hasError() const                                   { return !m_errorString.isEmpty(); }

private:
    const QString m_primaryKey;
    const QStringList m_unique;
    const QString m_tableName;
    const QString m_databaseName;
    ColumnWidth m_columnWidth;
    QString m_errorString;

    // Methods
    QString sqlQueryForColumns(const ColumnValuePairs &pairList);
    QStringList tableColumnNames(QSqlDatabase &db, const QString &table);
    QString sqlWhereClauseFind(const OptionTable &optionTable, QVariantList &toBind);
    QString sqlWhereIdentify(const OptionTable &optionTable, QList<char> &optionList);
    QList<Account> getAccountList(QSqlQuery &query);
    QStringList optionToDatabaseNames(QList<char> optionList);
    QString sqlPlaceholderString(const QList<char> &optionList);
    QString sqlPlaceholderString(const int amount);
    QString sqlUpdateTouple(const OptionTable &optionTable, QList<char> optionList);
    QString sqlBindingString(const char option);
    QString sqlInsertInto(const ColumnValuePairs &pairList);
    ColumnValuePairs columnNameAndValuePairList(const OptionTable &optionTable);

public:
    //Static
    static QString databaseNameOfOption(const char option);
};

#endif // PERSISTENCE_H
