#include "persistence.h"

Persistence::Persistence() :
    m_primaryKey("id"),
    m_unique(QStringList() << QString("provider") << QString("username")),
    m_tableName("Account"),
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
        m_errorString = db.lastError().text();
        return false;
    }
    ColumnValuePairs pairList = columnNameAndValuePairList(optionTable);
    QString queryString = sqlInsertInto(pairList);
    QSqlQuery query(db);
    bool result = query.prepare(queryString);
    if (!result) {
        m_errorString = db.lastError().text();
        db.close();
        return false;
    }
    for (QPair<QString, QVariant> pair : pairList) {
        query.addBindValue(pair.second);
    }
    result = query.exec();
    if (!result) {
        m_errorString = db.lastError().text();
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
        m_errorString = db.lastError().text();
        return QList<Account>();
    }
    ColumnValuePairs pairList = columnNameAndValuePairList(optionTable);
    QString queryColumns = sqlQueryForColumns(pairList);
    QVariantList valuesToBind;
    QString queryWhereClause = sqlWhereClauseFind(optionTable, valuesToBind);
    QString querySQL = QString("SELECT %1 FROM %2%3").arg(queryColumns).arg(m_tableName).arg(queryWhereClause);
    QSqlQuery query(db);
    bool result = query.prepare(querySQL);
    if (!result) {
        m_errorString = db.lastError().text();
        db.close();
        return QList<Account>();
    }
    for (QVariant value : valuesToBind) {
        query.addBindValue(value);
    }
    result = query.exec();
    if (!result) {
        m_errorString = db.lastError().text();
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
        m_errorString = db.lastError().text();
        return -1;
    }
    QString querySQL = QString("DELETE FROM %1").arg(m_tableName);
    QVariantList valuesToBind;
    QString queryWhereClause = sqlWhereClauseFind(optionTable, valuesToBind);
    querySQL.append(queryWhereClause);
    QSqlQuery query(db);
    bool result = query.prepare(querySQL);
    if (!result) {
        m_errorString = db.lastError().text();
        db.close();
        return 0;
    }
    for (QVariant value : valuesToBind) {
        query.addBindValue(value);
    }
    result = query.exec();
    if (!result) {
        m_errorString = db.lastError().text();
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
bool Persistence::modifyAccount(const OptionTable &optionTable)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = db.lastError().text();
        return false;
    }
    QList<char> optionList = optionTable.keys();
    QString whereClause = sqlWhereIdentify(optionTable, optionList);
    QString touple = sqlUpdateTouple(optionTable, optionList);
    QString querySQL = QString("UPDATE %1 SET %2").arg(m_tableName).arg(touple).append(whereClause);
    QSqlQuery query(db);
    bool result = query.prepare(querySQL);
    if (!result) {
        m_errorString = db.lastError().text();
        db.close();
        return false;
    }
    for (char option : optionTable.keys()) {
        QVariant value = optionTable.value(option);
        QString bindString = sqlBindingString(option);
        query.bindValue(bindString, value);
    }
    result = query.exec();
    if (!result) {
        m_errorString = db.lastError().text();
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
    QList<char> optionList = optionTable.keys();
    QString whereCondition = sqlWhereIdentify(optionTable, optionList);
    querySQL.append(whereCondition);
    QSqlQuery query(db);
    bool result = query.prepare(querySQL);
    if (!result) {
        m_errorString = db.lastError().text();
        db.close();
        return Account();
    }
    char *identifier = "ipu";
    for (int index=0; index<3; ++index) {
        if (optionTable.contains( identifier[index] )) {
            QVariant value = optionTable.value(identifier[index]);
            QString bindString = sqlBindingString(identifier[index]);
            query.bindValue(bindString, value);
        }
    }
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
 * Creates a string with all database column names.
 * The names are comma separated. If bindStrings is ture
 * each column name will have a leading colon.
 * @param account
 * @param bindStrings
 * @return
 */
QString Persistence::sqlQueryForColumns(const ColumnValuePairs &pairList)
{
    QString queryString;
    for (QPair<QString, QVariant> pair : pairList) {
        queryString.append(pair.first).append(',');
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
QString Persistence::sqlWhereClauseFind(const OptionTable &optionTable, QVariantList &toBind)
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
            QString condition = column + "=? AND";
            toBind << value;
            whereClause.append(condition);
            hasCondition = true;
        }
    }
    if (! hasCondition) {
        return QString();
    }
    whereClause.remove(whereClause.length()-4, 4);

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
        optionList.removeOne('i');
        return QString(" WHERE %1=:i").arg(m_primaryKey);
    }
    else if (optionList.contains('p') && optionList.contains('u')) {
        optionList.removeOne('p');
        optionList.removeOne('u');
        return QString(" WHERE %1=:p AND %3=:u").arg(m_unique[0]).arg(m_unique[1]);
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
    if (columnNames.isEmpty()) {
        columnNames << databaseNameOfOption('i') << databaseNameOfOption('p') << databaseNameOfOption('u');
    }

    return columnNames;
}

/**
 * Creates a string with SQL placeholders '?'.
 * @param columnCount
 * @return
 */
QString Persistence::sqlPlaceholderString(const QList<char> &optionList)
{
    if (optionList.isEmpty()) {
        return QString();
    }
    QString placeholder(sqlBindingString(optionList.first()));
    for (int i=1; i<optionList.size(); ++i) {
        QString bindString = QString(",%1").arg(sqlBindingString(optionList[i]));
        placeholder.append(bindString);
    }

    return placeholder;
}

/**
 * Build a placeholder string with '?' as placeholders.
 * @param amount                The amount of '?'.
 * @return placeholderString    A string like '?,?,?,?'.
 */
QString Persistence::sqlPlaceholderString(const int amount)
{
    QString placeholderString;
    for (int i=0; i<amount; ++i) {
        placeholderString.append("?,");
    }
    placeholderString.remove(placeholderString.length()-1, 1);

    return placeholderString;
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
        QString touple = QString(column + "=%1,").arg(sqlBindingString(option));
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
 * Build the SQL insert into query string.
 * @param pairList
 * @return
 */
QString Persistence::sqlInsertInto(const ColumnValuePairs &pairList)
{
    QString insertInto("INSERT INTO %1 (%2) VALUES (%3)");
    insertInto.arg(m_tableName);
    insertInto.arg(sqlQueryForColumns(pairList));
    insertInto.arg(sqlPlaceholderString(pairList.size()));

    return insertInto;
}

/**
 * Build pairs of column names and values from options.
 * @param optionTable
 * @return columnValueList      A list with pairs of column names and values.
 */
ColumnValuePairs Persistence::columnNameAndValuePairList(const OptionTable &optionTable)
{
    ColumnValuePairs columnValueList;
    QList<char> optionList = optionTable.keys();
    for (char option : optionList) {
        QString columnName = databaseNameOfOption(option);
        if (columnName.isEmpty()) {
            continue;
        }
        QPair<QString, QVariant> pair;
        pair.first = columnName;
        pair.second = optionTable.value(option);
        columnValueList << pair;
    }

    return columnValueList;
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
