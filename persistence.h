#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "dbvalue.h"
#include "optiontable.h"
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

typedef QHash<QString, QVariant> Account;

class Persistence
{
public:
    Persistence();

    void addDatabase(const QString &identifier, const QString &dbName, const QString &host, const ushort port, const QString &pw, const QString &username);
    QString errorMessage();
    bool persistAccount(QList<DBValue> &valueList);
    QList<Account> findAccount(QList<DBValue> &valueList, const bool hasNoWhereClause);
    int deleteAccount(QList<DBValue> &elementList);
    bool modifyAccount(QList<DBValue> &elementList);
    Account passwordDefinition(QList<DBValue> &valueList);
    QList<DBValue> valueListFromOptionTable(const OptionTable &optionTable) const;
    bool hasError() const                                   { return !m_errorString.isEmpty(); }

private:
    const char m_primaryKey;
    const QByteArray m_uniqueKey;
    const QString m_tableName;
    const QString m_databaseName;
    QString m_errorString;

    // Methods
    QString sqlColumnsToQuery(const QList<DBValue> &valueList) const;
    QString bindStringList(QList<DBValue> &valueList) const;
    QString sqlWhere(QList<DBValue> &valueList, const bool identifyRecord = false) const;
    QList<Account> getAccountList(QSqlQuery &query);
    QStringList optionToDatabaseNames(QList<char> &optionList);
    QString sqlInsertInto(QList<DBValue> &elementList) const;
    QString sqlUpdateSet(QList<DBValue> &elementList) const;
    QString sqlSelectFrom(QList<DBValue> &elementList, const bool hasNoWhereClause) const;
    void bindValuesToQuery(QSqlQuery &query, const QList<DBValue> &elementList) const;
    QString sqlDeleteFrom(QList<DBValue> &elementList) const;
    QString updateToupleList(QList<DBValue> &elementList) const;

    // Debug method
    void printElementList(const QList<DBValue> &elementList) const;

public:
    //Static
    static QString databaseNameOfOption(const char option);
};

#endif // PERSISTENCE_H
