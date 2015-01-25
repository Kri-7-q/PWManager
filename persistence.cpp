#include "persistence.h"

Persistence::Persistence() :
    primaryKey("id"),
    unique(QStringList() << QString("provider") << QString("username")),
    tableName("Account"),
    databaseName("PWManager")
{

}

/**
 * Provide an error message.
 * @return
 */
QString Persistence::errorMessage()
{
    QSqlDatabase db = QSqlDatabase::database("local");

    return db.lastError().databaseText();
}

/**
 * Persist an account into database.
 * @param account
 * @return
 */
bool Persistence::persistAccount(const Account &account)
{
    if (! openLocalDatabase("root", "root")) {
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database("local");
    QStringList columnNameList = columnNames(db, tableName);
    QString columns = queryColumnString(columnNameList, account);
    QString bindStrings = queryColumnString(columnNameList, account, true);
    QString queryString = QString("INSERT INTO %1 (%2) VALUES(%3)").arg(tableName).arg(columns).arg(bindStrings);
    QSqlQuery query(db);
    query.prepare(queryString);
    QStringList columnList = account.keys();
    for (QString column : columnList) {
        QVariant value = account.value(column);
        column.prepend(':');
        query.bindValue(column, value);
    }
    bool result = query.exec();
    db.close();

    return result;
}

/**
 * Open a connection to local database.
 * @param username
 * @param password
 * @return
 */
bool Persistence::openLocalDatabase(const QString &username, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "local");
    db.setDatabaseName(databaseName);
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setUserName(username);
    db.setPassword(password);

    return db.open();
}

/**
 * Creates a string with all database column names.
 * The names are comma separated. If bindStrings is ture
 * each column name will have a leading colon.
 * @param account
 * @param bindStrings
 * @return
 */
QString Persistence::queryColumnString(const QStringList columnNameList, const Account &account, const bool bindStrings)
{
    QString queryString;
    for (QString column : columnNameList) {
        if (! account.contains(column)) {
            continue;
        }
        if (bindStrings) {
            column.prepend(':');
        }
        queryString.append(column).append(',');
    }
    queryString.remove(queryString.length()-1, 1);

    return queryString;
}

/**
 * Return a list of all column names from a adatabase table.
 * @param db
 * @return
 */
QStringList Persistence::columnNames(QSqlDatabase &db, const QString &table)
{
    QStringList columnName;
    QSqlRecord record = db.record(table);
    for (int i=0; i<record.count(); ++i) {
        columnName << record.fieldName(i);
    }

    return columnName;
}
