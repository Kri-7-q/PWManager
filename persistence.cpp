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
bool Persistence::persistAccount(const OptionTable &optionTable)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        return false;
    }
    QList<char> optionList = optionTable.keys();
    QStringList columnList = optionToDatabaseNames(optionList);
    QString queryColumns = sqlQueryColumns(columnList);
    QString placeholder = sqlPlaceholderString(columnList.size());
    QString queryString = QString("INSERT INTO %1 (%2) VALUES(%3)").arg(m_tableName).arg(queryColumns).arg(placeholder);
    qDebug() << "SQL : " << queryString;
    QSqlQuery query(db);
    query.prepare(queryString);
    for (char option : optionList) {
        QVariant value = optionTable.value(option);
        qDebug() << "Value : " << option << " ---> " << value;
        query.addBindValue(value);
    }
    bool result = query.exec();
    query.finish();
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
        m_hasError = true;
        return QList<Account>();
    }
    QStringList columnList = optionToDatabaseNames(optionTable.keys());
    QString queryColumns = sqlQueryColumns(columnList);
    QList<char> valuesToBind;
    QString queryWhereClause = sqlWhereClauseFind(optionTable, valuesToBind);
    QString querySQL = QString("SELECT %1 FROM %2%3").arg(queryColumns).arg(m_tableName).arg(queryWhereClause);
    qDebug() << "Query String : " << querySQL;
    QSqlQuery query(db);
    query.prepare(querySQL);
    for (char option : valuesToBind) {
        QVariant value = optionTable.value(option);
        QString bindString = sqlBindingString(option);
        query.bindValue(bindString, value);
    }
    query.exec();
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
        m_hasError = true;
        return -1;
    }
    QString querySQL = QString("DELETE FROM %1").arg(m_tableName);
    QList<char> valuesToBind;
    QString queryWhereClause = sqlWhereClauseFind(optionTable, valuesToBind);
    querySQL.append(queryWhereClause);
    qDebug() << "SQL : " << querySQL;
    QSqlQuery query(db);
    query.prepare(querySQL);
    for (char option : valuesToBind) {
        QVariant value = optionTable.value(option);
        QString bindString = sqlBindingString(option);
        query.bindValue(bindString, value);
    }
    query.exec();
    int numRowsAffected = query.numRowsAffected();
    db.close();

    return numRowsAffected;
}

/**
 * Modify an Account object in database.
 * @param optionTable
 * @return
 */
bool Persistence::modifyAccount(const OptionTable &optionTable)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_hasError = true;
        return false;
    }
    QList<char> optionList = optionTable.keys();
    QString whereClause = sqlWhereIdentify(optionTable, optionList);
    QString touple = sqlUpdateTouple(optionTable, optionList);
    QString querySQL = QString("UPDATE %1 SET %2").arg(m_tableName).arg(touple).append(whereClause);
    qDebug() << "SQL : " << querySQL;
    QSqlQuery query(db);
    query.prepare(querySQL);
    for (char option : optionList) {
        QVariant value = optionTable.value(option);
        query.addBindValue(value);
    }
    bool result = query.exec();
    db.close();

    return result;
}

/**
 * Creates a string with all database column names.
 * The names are comma separated. If bindStrings is ture
 * each column name will have a leading colon.
 * @param account
 * @param bindStrings
 * @return
 */
QString Persistence::sqlQueryColumns(const QStringList &columnsToQuery)
{
    QString queryString;
    for (QString column : columnsToQuery) {
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
QStringList Persistence::tableColumnNames(QSqlDatabase &db, const QString &table)
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
QString Persistence::sqlWhereClauseFind(const OptionTable &optionTable, QList<char> &toBind)
{
    if (optionTable.contains('e')) {
        return QString();
    }
    bool hasCondition = false;
    QString whereClause(" WHERE ");
    QList<char> keyList = optionTable.keys();
    for (char option : keyList) {
        QVariant value = optionTable.value(option);
        if (! value.isNull()) {
            QString column = databaseNameOfOption(option);
            QString bindString = sqlBindingString(option);
            QString condition = column + "=" + bindString;
            toBind << option;
            whereClause.append(condition);
            hasCondition = true;
        }
    }
    if (! hasCondition) {
        return QString();
    }
    if (whereClause.endsWith("AND ")) {
        whereClause.remove(whereClause.length()-4, 4);
    }

    return whereClause;
}

/**
 * Creates a SQL WHERE clause to identify one Account in database.
 * To identify there are a primary key and a unique constraint.
 * It removes the identifier from OptionTable.
 * @param optionTable
 * @return
 */
QString Persistence::sqlWhereIdentify(const OptionTable &optionTable, QList<char> &optionList)
{
    if (optionList.contains('i')) {
        int primaryKey = optionTable.value('i').toInt();
        optionList.removeOne('i');
        return QString(" WHERE %1=%2").arg(m_primaryKey).arg(primaryKey);
    }
    else if (optionList.contains('p') && optionList.contains('u')) {
        QString provider = sqlStringOfValue(optionTable.value('p'));
        QString username = sqlStringOfValue(optionTable.value('u'));
        optionList.removeOne('p');
        optionList.removeOne('u');
        return QString(" WHERE %1=%2 AND %3=%4").arg(m_unique[0]).arg(provider).arg(m_unique[1]).arg(username);
    }

    return QString();
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
QStringList Persistence::optionToDatabaseNames(QList<char> optionList)
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

    return columnNames;
}

/**
 * Get a string value of a QVariant. For SQL query it
 * is neccassary to sourround strings with quations (').
 * @param value
 * @return
 */
QString Persistence::sqlStringOfValue(const QVariant &value)
{
    switch (value.type()) {
    case QVariant::Int:
    case QVariant::Double:
    case QVariant::LongLong:
        return value.toString();
        break;
    case QVariant::String:
    case QVariant::Char:
    case QVariant::Date:
    case QVariant::DateTime:
    case QVariant::Time:
        return QString("'").append(value.toString()).append("'");
    default:
        break;
    }

    return QString(value.toString());
}

/**
 * Creates a string with SQL placeholders '?'.
 * @param columnCount
 * @return
 */
QString Persistence::sqlPlaceholderString(const int columnCount)
{
    qDebug() << "Placeholder count : " << columnCount;
    if (columnCount < 1) {
        return QString();
    }
    QString placeholder("?");
    for (int i=1; i<columnCount; ++i) {
        placeholder.append(",?");
    }

    return placeholder;
}

/**
 * Creates a string with komma separarted touples of column names
 * and values for a update SQL instruction.
 * For instance :   id=?,provider=?,password=?
 * @param optionTable
 * @param optionList
 * @return
 */
QString Persistence::sqlUpdateTouple(const OptionTable &optionTable, QList<char> optionList)
{
    QString sqlUpdate;
    for (char option : optionList) {
        QString column = databaseNameOfOption(option);
        QString touple = column + "=?,";
        sqlUpdate.append(touple);
    }
    if (sqlUpdate.endsWith(',')) {
        sqlUpdate.remove(sqlUpdate.length()-1, 1);
    }

    return sqlUpdate;
}

/**
 * Creates a binding string out of an option.
 * @param option
 * @return
 */
QString Persistence::sqlBindingString(const char option)
{
    return QString(":").append(option);
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
        return "qustion";
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
