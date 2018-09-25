#ifndef POSTGRESQL_H
#define POSTGRESQL_H

#include "persistence.h"
#include <QSqlRecord>

class PostgreSQL : public Persistence
{
public:
    PostgreSQL();

    // Persistence interface
public:
    bool open(const QString& parameter = QString());
    void close();
    // Call following function with open database connection.
    bool persistAccountObject(const OptionTable &account);
    int deleteAccountObject(const OptionTable &account);
    bool modifyAccountObject(const OptionTable &modifications);
    QVariantMap findAccount(const OptionTable &searchObj);
    QList<QVariantMap> findAccountsLike(const OptionTable &searchObj);
    // Can be called without open database connection. (Reads the whole table)
    QList<QVariantMap> allPersistedAccounts();
    // User management
    QVariantMap findUser(const OptionTable &userInfo);
    // Error messages
    QString error() const               { return m_errorMsg; }
    bool hasError() const               { return !m_errorMsg.isEmpty(); }
    // Translation
    QString optionToRealName(const char option) const;

private:
    QString m_tableName;
    QString m_errorMsg;

    // Initialization
    void initializeDatabase();
    // Translation
    QSqlRecord recordFromOptionTable(const OptionTable& optionTable) const;
    QSqlRecord recordWithIdentifier(const OptionTable &optionTable) const;
    QSqlRecord recordWithoutIdentifier(OptionTable optionTable) const;
    QSqlRecord recordFieldsWithValues(const OptionTable& optionTable) const;
    // Error messages
    void setErrorDatabaseConectionFailed(const QString& database, const QString& driver);
    void setErrorPrepareStatement(const QString& database, const QString& driver);
    void setErrorExecutionFailed(const QString& database, const QString& driver);
    // Creation
    void recordAppendField(QSqlRecord& record, const QString& name, const QVariant& value) const;
    QSqlRecord recordConcardinate(const QSqlRecord& first, const QSqlRecord& second) const;
    QVariantMap accountObject(const QSqlRecord& record) const;
};

#endif // POSTGRESQL_H
