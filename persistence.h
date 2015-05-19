#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "optioninfo.h"
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
    bool persistAccount(const OptionTable &optionTable);
    QList<Account> findAccount(const OptionTable &optionTable);
    int deleteAccount(const OptionTable &optionTable);
    bool modifyAccount(OptionTable optionTable);
    Account passwordDefinition(const OptionTable &optionTable);
    bool hasError() const                                   { return !m_errorString.isEmpty(); }

private:
    const QString m_tableName;
    const QString m_databaseName;
    QString m_errorString;

    // Methods
    QString sqlColumnsToQuery(const QList<OptionInfo> &elementList) const;
    QString bindStringList(QList<OptionInfo> &elementList) const;
    QString sqlWhere(QList<OptionInfo> &elementList, const bool identifyRecord = false) const;
    QList<Account> getAccountList(QSqlQuery &query);
    QStringList optionToDatabaseNames(QList<char> &optionList);
    QString sqlInsertInto(QList<OptionInfo> &elementList) const;
    QString sqlUpdateSet(QList<OptionInfo> &elementList) const;
    QList<OptionInfo> parseOptionTable(const OptionTable &optionTable) const;
    QString sqlSelectFrom(QList<OptionInfo> &elementList, const bool hasNoWhereClause) const;
    void bindValuesToQuery(QSqlQuery &query, const QList<OptionInfo> &elementList) const;
    QString sqlDeleteFrom(QList<OptionInfo> &elementList) const;
    QString updateToupleList(QList<OptionInfo> &elementList) const;

    void printElementList(const QList<OptionInfo> &elementList) const;

public:
    //Static
    static QString databaseNameOfOption(const char option);
};

#endif // PERSISTENCE_H
