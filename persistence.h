#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "columnwidth.h"
#include "element.h"
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
    QString sqlColumnsToQuery(const QList<Element> &elementList) const;
    QString bindStringList(QList<Element> &elementList) const;
    QString sqlWhere(QList<Element> &elementList, const bool identifyRecord = false) const;
    QList<Account> getAccountList(QSqlQuery &query);
    QStringList optionToDatabaseNames(QList<char> &optionList);
    QString sqlInsertInto(QList<Element> &elementList) const;
    QString sqlUpdateSet(QList<Element> &elementList) const;
    QList<Element> parseOptionTable(const OptionTable &optionTable) const;
    QString sqlSelectFrom(QList<Element> &elementList, const bool hasNoWhereClause) const;
    void bindValuesToQuery(QSqlQuery &query, const QList<Element> &elementList) const;
    QString sqlDeleteFrom(QList<Element> &elementList) const;
    QString updateToupleList(QList<Element> &elementList) const;

    void printElementList(const QList<Element> &elementList) const;

public:
    //Static
    static QString databaseNameOfOption(const char option);
};

#endif // PERSISTENCE_H
