#include "persistence.h"

Persistence::Persistence() :
    m_primaryKey("id"),
    m_unique(QStringList() << QString("provider") << QString("username")),
    m_tableName("account"),
    m_databaseName("pwmanager")
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
bool Persistence::persistAccount(const OptionTable &optionTable)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = "Could not open Database !";
        return false;
    }
    QList<Element> elementList = parseOptionTable(optionTable);
    QString queryString = sqlInsertInto(elementList);
    QSqlQuery query(db);
    bool result = query.prepare(queryString);
    if (!result) {
        printElementList(elementList);
        qDebug() << "SQL query : " << queryString;
        m_errorString = "SQL query is NOT valid. Could NOT prepare query !";
        db.close();
        return false;
    }
    bindValuesToQuery(query, elementList);
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
QList<Account> Persistence::findAccount(const OptionTable &optionTable)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = "Could NOT open Database !";
        return QList<Account>();
    }
    QList<Element> elementList = parseOptionTable(optionTable);
    bool hasNoWhereClause = optionTable.contains('e');
    QString querySQL = sqlSelectFrom(elementList, hasNoWhereClause);
    QSqlQuery query(db);
    bool result = query.prepare(querySQL);
    if (!result) {
        printElementList(elementList);
        qDebug() << "Query string : " << querySQL;
        m_errorString = "SQL query is NOT valid. Could NOT prepare query !";
        db.close();
        return QList<Account>();
    }
    bindValuesToQuery(query, elementList);
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
int Persistence::deleteAccount(const OptionTable &optionTable)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = "Could NOT open database !";
        return -1;
    }
    QList<Element> elementList = parseOptionTable(optionTable);
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
bool Persistence::modifyAccount(OptionTable optionTable)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = "Could NOT open database !";
        return false;
    }
    QList<Element> elementList = parseOptionTable(optionTable);
    QString querySQL = sqlUpdateSet(elementList);
    QSqlQuery query(db);
    bool result = query.prepare(querySQL);
    if (!result) {
        printElementList(elementList);
        qDebug() << "SQL query : " << querySQL;
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
Account Persistence::passwordDefinition(const OptionTable &optionTable)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = db.lastError().text();
        return Account();
    }
    QString querySQL = QString("SELECT %1,%2 FROM %3").arg(databaseNameOfOption('l')).arg(databaseNameOfOption('s')).arg(m_tableName);
    QList<Element> elementList = parseOptionTable(optionTable);
    querySQL.append(sqlWhere(elementList, true));
    QSqlQuery query(db);
    bool result = query.prepare(querySQL);
    if (!result) {
        m_errorString = "SQL query is NOT vail. Could not prepare query !";
        db.close();
        return Account();
    }
    bindValuesToQuery(query, elementList);
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
QString Persistence::bindStringList(QList<Element> &elementList) const
{
    QString bindString;
    for (Element &element : elementList) {
        element.bindString = QString(":").append(element.columnName);
        bindString.append(element.bindString).append(',');
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
QString Persistence::sqlColumnsToQuery(const QList<Element> &elementList) const
{
    QString queryString;
    for (Element element : elementList) {
        queryString.append(element.columnName).append(',');
    }
    queryString.remove(queryString.length()-1, 1);

    return queryString;
}

/**
 * Get a SQL where for reading account objects from database.
 * @param account
 * @return
 */
QString Persistence::sqlWhere(QList<Element> &elementList, const bool identifyRecord) const
{
    QString whereClause;
    for (Element &element : elementList) {
        if (identifyRecord && !element.isIdentifier) {
            continue;
        }
        if (!element.value.isValid()) {
            continue;
        }
        element.bindString = QString(":").append(element.columnName);
        whereClause.append(element.columnName).append("=").append(element.bindString).append(" AND ");
    }
    if (!whereClause.isEmpty()) {
        whereClause.prepend(" WHERE ");
        whereClause.remove(whereClause.length()-4, 5);
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
QString Persistence::sqlInsertInto(QList<Element> &elementList) const
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
QString Persistence::sqlUpdateSet(QList<Element> &elementList) const
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
QList<Element> Persistence::parseOptionTable(const OptionTable &optionTable) const
{
    OptionTable copyOptionTable(optionTable);
    QList<Element> elementList;
    if (copyOptionTable.value('i').isValid()) {
        QVariant value = copyOptionTable.take('i');
        QString column = databaseNameOfOption('i');
        Element id(column, value, true);
        elementList << id;
    }
    else if (copyOptionTable.value('p').isValid() && copyOptionTable.value('u').isValid()) {
        QVariant value = copyOptionTable.take('p');
        QString column = databaseNameOfOption('p');
        Element provider(column, value, true);
        elementList << provider;
        value = copyOptionTable.take('u');
        column = databaseNameOfOption('u');
        Element username(column, value, true);
        elementList << username;
    }
    for (char option : copyOptionTable.keys()) {
        QString column = databaseNameOfOption(option);
        if (column.isEmpty()) {
            continue;
        }
        QVariant value = copyOptionTable.value(option);
        Element e(column, value, false);
        elementList << e;
    }

    return elementList;
}

/**
 * @brief Persistence::sqlSelectFrom
 * @param elementList
 * @return
 */
QString Persistence::sqlSelectFrom(QList<Element> &elementList, const bool hasNoWhereClause) const
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
void Persistence::bindValuesToQuery(QSqlQuery &query, const QList<Element> &elementList) const
{
    for (Element element : elementList) {
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
QString Persistence::sqlDeleteFrom(QList<Element> &elementList) const
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
QString Persistence::updateToupleList(QList<Element> &elementList) const
{
    QString touple;
    for (Element &element : elementList) {
        if (!element.isIdentifier) {
            element.bindString = QString(":").append(element.columnName);
            touple.append(element.columnName).append('=').append(element.bindString).append(",");
        }
    }
    touple.remove(touple.length()-1, 1);

    return touple;
}

/**
 * @brief Persistence::printElementList
 * @param elementList
 */
void Persistence::printElementList(const QList<Element> &elementList) const
{
    qDebug() << "Element list :" << endl << "Column name, bind String, value" << endl << "----------------------------------------------------";
    for (Element element : elementList) {
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
