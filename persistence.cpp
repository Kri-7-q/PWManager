#include "persistence.h"

Persistence::Persistence() :
    m_primaryKey("id"),
    m_unique(QStringList() << QString("provider") << QString("username")),
    m_tableName("Account"),
    m_databaseName("pwmanager"),
    m_hasError(false)
{
    addDatabase("local", m_databaseName, "localhost", 3306, "root", "postgres");
}

/**
 * Add a MySql database with a identifier name.
 * All attributes are set. Database is ready to open.
 * @param identifier
 * @param dbName
 * @param host
 * @param port
 * @param pw
 * @param username
 */
void Persistence::addDatabase(const QString &identifier, const QString &dbName, const QString &host, const ushort port, const QString &pw, const QString &username)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", identifier);
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    db.setPassword(pw);
    db.setUserName(username);
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
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        return false;
    }
    QStringList columnNameList = columnNames(db, m_tableName);
    QString columns = queryColumnString(columnNameList, account);
    QString bindStrings = queryColumnString(columnNameList, account, true);
    QString queryString = QString("INSERT INTO %1 (%2) VALUES(%3)").arg(m_tableName).arg(columns).arg(bindStrings);
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
 * Reads one or more Account objects from database.
 * Funktion searches for primary key or uniqe attributes
 * in the Account object.
 * If option '--all' (e) is set than all stored Account
 * objects are read.
 * @param account
 * @return
 */
QList<Account> Persistence::findAccount(const Account &account)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    qDebug() << "DB : " << db;
    if (! db.open()) {
        m_hasError = true;
        return QList<Account>();
    }
    QStringList columnNameList = columnNames(db, m_tableName);
    QString queryColumns = queryColumnString(columnNameList, account);
    QString queryWhereClause = sqlWhereClauseFind(account);
    QString querySQL = QString("SELECT %1 FROM %2%3").arg(queryColumns).arg(m_tableName).arg(queryWhereClause);
    qDebug() << "Query String : " << querySQL;
    QSqlQuery query = db.exec(querySQL);
    QList<Account> accountList = getAccountList(query);
    db.close();

    return accountList;
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

/**
 * Get a SQL where for reading account objects from database.
 * @param account
 * @return
 */
QString Persistence::sqlWhereClauseFind(const Account &account)
{
    if (account.contains("e")) {
        return QString();
    }
    QString whereClause;
    QVariant primaryKeyValue = account.value(m_primaryKey, QVariant());
    if (! primaryKeyValue.isNull()) {
        whereClause  = QString(" WHERE %1=%2");
        whereClause.arg(m_primaryKey).arg(primaryKeyValue.toInt());
    } else {
        whereClause = QString(" WHERE %1=%2 AND %3=%4");
        whereClause.arg(m_unique[0]).arg(account.value(m_unique[0]).toString()).arg(m_unique[1]).arg(account.value(m_unique[1]).toString());
    }

    return whereClause;
}

/**
 * Get a list of Account object from a database query.
 * @param query
 * @return
 */
QList<Account> Persistence::getAccountList(QSqlQuery &query)
{
    QList<Account> list;
    QStringList columnList;
    for (int i=0; i<query.record().count(); ++i) {
        columnList << query.record().fieldName(i);
    }
    while (query.next()) {
        Account account;
        for (QString column : columnList) {
            QVariant value = query.value(column);
            m_columnWidth.insertWidthValue(column, value);
            account.insert(column, value);
        }
        list << account;
    }

    return list;
}
