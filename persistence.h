#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "optiontable.h"
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlDriver>
#include <QDebug>

typedef QHash<QString, QVariant> Account;

class Persistence
{
public:
    Persistence();

    QString errorMessage();
    bool persistAccount(const OptionTable& optiontTable);
    QList<Account> findAccount(const OptionTable& optionTable);
    int deleteAccount(const OptionTable& optionTable);
    bool modifyAccount(const OptionTable& optionTable);
    Account passwordDefinition(const OptionTable& optionTable);
    bool hasError() const                                   { return !m_errorString.isEmpty(); }

private:
    const char m_primaryKey;
    const QByteArray m_uniqueKey;
    QString m_tableName;
    QString m_errorString;
    QSqlRecord m_record;

    // Methods
    QSqlRecord recordFromOptionTable(const OptionTable& optionTable) const;
    QSqlRecord keepFieldsWithValues(const QSqlRecord& record) const;
    QSqlRecord removeUniqueConstraintFields(QSqlRecord& record) const;
    QSqlRecord recordWithFields(const QStringList& columnList) const;
    QList<Account> getAccountList(QSqlQuery &query);
    void bindValuesToQuery(QSqlQuery &query, const QSqlRecord& record) const;
    // Initialization
    void initializeDatabase(QSqlDatabase &db);

public:
    //Static
    static QString columnNameOfOption(const char option);
};

#endif // PERSISTENCE_H
