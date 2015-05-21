#include "persistence.h"

Persistence::Persistence() :
    m_tableName("account"),
    m_databaseName("pwmanager"),
    m_primaryKey('i'),
    m_uniqueKey(QByteArray().append('p').append('u'))
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
    return m_errorString;
}

/**
 * Persist an account into database.
 * @param account
 * @return
 */
bool Persistence::persistAccount(QList<DBValue> &valueList)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = "Could not open Database !";
        return false;
    }
    QString queryString = sqlInsertInto(valueList);
    QSqlQuery query(db);
    bool result = query.prepare(queryString);
    if (!result) {
        printElementList(valueList);
        qDebug() << "SQL query : " << queryString;
        m_errorString = "SQL query is NOT valid. Could NOT prepare query !";
        db.close();
        return false;
    }
    bindValuesToQuery(query, valueList);
    result = query.exec();
    if (!result) {
        m_errorString = "Could not store Data !";
    }
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
QList<Account> Persistence::findAccount(QList<DBValue> &valueList, const bool hasNoWhereClause)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = "Could NOT open Database !";
        return QList<Account>();
    }
    QString querySQL = sqlSelectFrom(valueList, hasNoWhereClause);
    QSqlQuery query(db);
    bool result = query.prepare(querySQL);
    if (!result) {
        printElementList(valueList);
        qDebug() << "Query string : " << querySQL;
        m_errorString = "SQL query is NOT valid. Could NOT prepare query !";
        db.close();
        return QList<Account>();
    }
    bindValuesToQuery(query, valueList);
    result = query.exec();
    if (!result) {
        m_errorString = "Could NOT read Data !";
        db.close();
        return QList<Account>();
    }
    QList<Account> accountList = getAccountList(query);
    db.close();

    return accountList;
}

/**
 * Delete one or more Accounts from database.
 * OptionTable should contain values to identify
 * the rows to delete.
 * An empty OptionTable object will cause an empty
 * database.
 * @param optionTable
 * @return
 */
int Persistence::deleteAccount(QList<DBValue> &elementList)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = "Could NOT open database !";
        return -1;
    }
    QString querySQL = sqlDeleteFrom(elementList);
    QSqlQuery query(db);
    bool result = query.prepare(querySQL);
    if (!result) {
        printElementList(elementList);
        qDebug() << "SQL query : " << querySQL;
        m_errorString = "SQL query is NOT valid. Could not prepare query !";
        db.close();
        return 0;
    }
    bindValuesToQuery(query, elementList);
    result = query.exec();
    if (!result) {
        m_errorString = "Could NOT delete data !";
        db.close();
        return 0;
    }
    int numRowsAffected = query.numRowsAffected();
    db.close();

    return numRowsAffected;
}

/**
 * Modify an Account object in database.
 * @param optionTable
 * @return
 */
bool Persistence::modifyAccount(QList<DBValue> &elementList)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = "Could NOT open database !";
        return false;
    }
    QString querySQL = sqlUpdateSet(elementList);
    QSqlQuery query(db);
    bool result = query.prepare(querySQL);
    if (!result) {
        m_errorString = "SQL query is not valid. Could NOT prepare query !";
        db.close();
        return false;
    }
    bindValuesToQuery(query, elementList);
    result = query.exec();
    if (!result) {
        m_errorString = "Could NOT store data !";
    }
    db.close();

    return result;
}

/**
 * Read password definition from an Account object.
 * @param optionTable
 * @return              Account object with keys : length, definition.
 */
Account Persistence::passwordDefinition(QList<DBValue> &valueList)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = db.lastError().text();
        return Account();
    }
    QString querySQL = QString("SELECT %1,%2 FROM %3").arg(databaseNameOfOption('l')).arg(databaseNameOfOption('s')).arg(m_tableName);
    querySQL.append(sqlWhere(valueList, true));
    QSqlQuery query(db);
    bool result = query.prepare(querySQL);
    if (!result) {
        m_errorString = "SQL query is NOT vail. Could not prepare query !";
        db.close();
        return Account();
    }
    bindValuesToQuery(query, valueList);
    result = query.exec();
    if (!result) {
        m_errorString = db.lastError().text();
        db.close();
        return Account();
    }
    result = query.next();
    Account definiton;
    if (result) {
        QVariant value = query.value(0);
        definiton.insert("length", value);
        value = query.value(1);
        definiton.insert("definition", value);
    }
    db.close();

    return definiton;
}

/**
 * @brief Persistence::bindStringList
 * @param elementList
 * @return
 */
QString Persistence::bindStringList(QList<DBValue> &valueList) const
{
    QString bindString;
    for (int i=0; i<valueList.size(); ++i) {
        DBValue &value = valueList[i];
        value.bindString = QString(":").append(value.columnName);
        bindString.append(value.bindString).append(',');
    }
    bindString.remove(bindString.length()-1, 1);

    return bindString;
}

/**
 * Creates a string with all database column names.
 * The names are comma separated.
 * @param optionTable
 * @return
 */
QString Persistence::sqlColumnsToQuery(const QList<DBValue> &valueList) const
{
    QString queryString;
    for (int i=0; i<valueList.size(); ++i) {
        queryString.append(valueList[i].columnName).append(',');
    }
    queryString.remove(queryString.length()-1, 1);

    return queryString;
}

/**
 * Get a SQL where for reading account objects from database.
 * @param account
 * @return
 */
QString Persistence::sqlWhere(QList<DBValue> &valueList, const bool identifyRecord) const
{
    QString whereClause;
    for (int i=0; i<valueList.size(); ++i) {
        DBValue &value = valueList[i];
        if ( (identifyRecord && !value.isIdentifier) || !value.value.isValid() ) {
            continue;
        }
        value.bindString = QString(":").append(value.columnName);
        whereClause.append(value.columnName).append("=").append(value.bindString).append(" AND ");
    }
    if (!whereClause.isEmpty()) {
        whereClause.prepend(" WHERE ");
        whereClause.remove(whereClause.length()-4, 5);
    }
    if (whereClause.isEmpty() && identifyRecord) {
        whereClause = QString(" WHERE FALSE");
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
        foreach (const QString column, columnList) {
            QVariant value = query.value(column);
            account.insert(column, value);
        }
        list << account;
    }

    return list;
}

/**
 * Translates option char into database column name string.
 * @param optionList
 * @return
 */
QStringList Persistence::optionToDatabaseNames(QList<char> &optionList)
{
    QStringList columnNames;
    int index = 0;
    while (index < optionList.size()) {
        char option = optionList[index];
        QString column = databaseNameOfOption(option);
        if (column.isNull()) {
            optionList.removeAt(index);
        } else {
            columnNames << column;
            ++index;
        }
    }
    if (columnNames.isEmpty()) {
        columnNames << databaseNameOfOption('i') << databaseNameOfOption('p') << databaseNameOfOption('u');
    }

    return columnNames;
}

/**
 * Build the SQL insert into query string.
 * @param pairList
 * @return
 */
QString Persistence::sqlInsertInto(QList<DBValue> &elementList) const
{
    QString columns = sqlColumnsToQuery(elementList);
    QString bindString = bindStringList(elementList);
    QString insertInto = QString("INSERT INTO %1 (%2) VALUES (%3)").arg(m_tableName).arg(columns).arg(bindString);

    return insertInto;
}

/**
 * @brief Persistence::sqlUpdateSet
 * @param elementList
 * @return
 */
QString Persistence::sqlUpdateSet(QList<DBValue> &elementList) const
{
    QString touples = updateToupleList(elementList);
    QString sqlUpdate = QString("UPDATE %1 SET %2").arg(m_tableName).arg(touples);
    sqlUpdate.append(sqlWhere(elementList, true));

    return sqlUpdate;
}

/**
 * @brief Persistence::queryElementList
 * @param optionTable
 * @return
 */
QList<DBValue> Persistence::valueListFromOptionTable(const OptionTable &optionTable) const
{
    QList<DBValue> valueList;
    bool hasPrimaryKey = optionTable.hasValueForKey(m_primaryKey);
    bool hasUnique = optionTable.hasValuesForKeySet(m_uniqueKey);

    foreach (const char option, optionTable.keys()) {
        QString column = databaseNameOfOption(option);
        if (column.isEmpty()) {
            continue;
        }
        QVariant value = optionTable.value(option);
        DBValue dbValue(column, value, false);
        switch (option) {
        case 'i':
            dbValue.isIdentifier = hasPrimaryKey;
            break;
        case 'p':
        case 'u':
            dbValue.isIdentifier = hasUnique && !hasPrimaryKey;
            break;
        default:
            break;
        }
        valueList << dbValue;
    }

    return valueList;
}

/**
 * @brief Persistence::sqlSelectFrom
 * @param elementList
 * @return
 */
QString Persistence::sqlSelectFrom(QList<DBValue> &elementList, const bool hasNoWhereClause) const
{
    QString columnList = sqlColumnsToQuery(elementList);
    QString selectQuery = QString("SELECT %1 FROM %2").arg(columnList).arg(m_tableName);
    if (hasNoWhereClause) {
        return selectQuery;
    }
    QString whereClause = sqlWhere(elementList);
    selectQuery.append(whereClause);

    return selectQuery;
}

/**
 * @brief Persistence::bindValuesToQuery
 * @param query
 * @param elementList
 */
void Persistence::bindValuesToQuery(QSqlQuery &query, const QList<DBValue> &elementList) const
{
    foreach (const DBValue element, elementList) {
        if (element.bindString.isEmpty()) {
            continue;
        }
        query.bindValue(element.bindString, element.value);
    }
}

/**
 * @brief Persistence::sqlDeleteFrom
 * @param elementList
 * @return
 */
QString Persistence::sqlDeleteFrom(QList<DBValue> &elementList) const
{
    QString sqlDelete = QString("DELETE FROM %1").arg(m_tableName);
    sqlDelete.append(sqlWhere(elementList));

    return sqlDelete;
}

/**
 * @brief Persistence::updateToupleList
 * @param elementList
 * @return
 */
QString Persistence::updateToupleList(QList<DBValue> &valueList) const
{
    QString touple;
    for (int i=0; i<valueList.size(); ++i) {
        DBValue &value = valueList[i];
        if (!value.isIdentifier) {
            value.bindString = QString(":").append(value.columnName);
            touple.append(value.columnName).append('=').append(value.bindString).append(",");
        }
    }
    touple.remove(touple.length()-1, 1);

    return touple;
}

/**
 * @brief Persistence::printElementList
 * @param elementList
 */
void Persistence::printElementList(const QList<DBValue> &elementList) const
{
    qDebug() << "Element list :" << endl << "Column name, bind String, value" << endl << "----------------------------------------------------";
    foreach (const DBValue element, elementList) {
        qDebug() << element.columnName << ", " << element.bindString << ", " << element.value;
    }
}

/**
 * Translates an option char to database name.
 * @param option
 * @return
 */
QString Persistence::databaseNameOfOption(const char option)
{
    switch (option) {
    case 'p':
        return "provider";
    case 'u':
        return "username";
    case 'i':
        return "id";
    case 'q':
        return "question";
    case 'r':
        return "answer";
    case 'l':
        return "passwordlength";
    case 's':
        return "definedcharacter";
    case 'k':
        return "password";
    case 't':
        return "lastmodify";
    default:
        break;
    }

    return QString();
}
