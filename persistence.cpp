#include "persistence.h"
#include "credentials.h"

Persistence::Persistence() :
    m_primaryKey('i'),
    m_uniqueKey(QByteArray().append('p').append('u'))
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "local");
    initializeDatabase(db);
    if (db.open()) {
        m_record = db.record(m_tableName);
        db.close();
    }
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
bool Persistence::persistAccount(const OptionTable &optiontTable)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = QString("Could not open Database !");
        return false;
    }
    QSqlRecord insertRecord = recordFromOptionTable(optiontTable);
    QString selectSql = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, insertRecord, true);
    QSqlQuery query(db);
    bool result = query.prepare(selectSql);
    if (!result) {
        qDebug() << "SQL query : " << selectSql;
        m_errorString = QString("Persistence:persistAccount() --> SQL query is NOT valid. Could NOT prepare query !");
        db.close();
        return false;
    }
    bindValuesToQuery(query, insertRecord);
    result = query.exec();
    if (!result) {
        m_errorString = QString("Could not store Data !");
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
QList<Account> Persistence::findAccount(const OptionTable& optionTable)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = "Could NOT open Database !";
        return QList<Account>();
    }
    QSqlRecord selectRecord = recordFromOptionTable(optionTable);
    QSqlRecord whereRecord = keepFieldsWithValues(selectRecord);
    QString selectSql = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, selectRecord, false);
    QString whereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, whereRecord, true);
    if (! whereClause.isEmpty()) {
        selectSql.append(' ').append(whereClause);
    }
    QSqlQuery query(db);
    bool result = query.prepare(selectSql);
    if (!result) {
        qDebug() << "Query string : " << selectSql;
        m_errorString = "Persistence:findAccount() --> SQL query is NOT valid. Could NOT prepare query !";
        db.close();
        return QList<Account>();
    }
    bindValuesToQuery(query, whereRecord);
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
    QSqlRecord deleteRecord = recordFromOptionTable(optionTable);
    QString deleteSql = db.driver()->sqlStatement(QSqlDriver::DeleteStatement, m_tableName, deleteRecord, false);
    QString whereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, deleteRecord, true);
    deleteSql.append(' ').append(whereClause);
    QSqlQuery query(db);
    bool result = query.prepare(deleteSql);
    if (!result) {
        qDebug() << "SQL query : " << deleteSql;
        m_errorString = "Persistence:deleteAccount() --> SQL query is NOT valid. Could not prepare query !";
        db.close();
        return 0;
    }
    bindValuesToQuery(query, deleteRecord);
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
bool Persistence::modifyAccount(const OptionTable &optionTable)
{
    QSqlDatabase db = QSqlDatabase::database("local");
    if (! db.open()) {
        m_errorString = "Could NOT open database !";
        return false;
    }
    QSqlRecord modifyRecord = recordFromOptionTable(optionTable);
    QSqlRecord whereRecord = removeUniqueConstraintFields(modifyRecord);
    QString modifySql = db.driver()->sqlStatement(QSqlDriver::UpdateStatement, m_tableName, modifyRecord, true);
    QString whereSql = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, whereRecord, true);
    modifySql.append(' ').append(whereSql);
    QSqlQuery query(db);
    bool result = query.prepare(modifySql);
    if (!result) {
        qDebug() << "Sql: " << modifySql;
        m_errorString = "Persistence:modifyAccount() --> SQL query is not valid. Could NOT prepare query !";
        db.close();
        return false;
    }
    bindValuesToQuery(query, modifyRecord);
    bindValuesToQuery(query, whereRecord);
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
    QSqlRecord selectRecord = recordFromOptionTable(optionTable);
    QSqlRecord whereRecord = removeUniqueConstraintFields(selectRecord);
    QStringList columnList = QStringList() << columnNameOfOption('l') << columnNameOfOption('s');
    selectRecord = recordWithFields(columnList);
    QString selectSql = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, selectRecord, false);
    QString whereSql = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, whereRecord, true);
    selectSql.append(' ').append(whereSql);
    QSqlQuery query(db);
    bool result = query.prepare(selectSql);
    if (!result) {
        qDebug() << "Sql: " << selectSql;
        m_errorString = "SQL query is NOT vail. Could not prepare query !";
        db.close();
        return Account();
    }
    bindValuesToQuery(query, whereRecord);
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
        definiton.insert(columnList[0], value);
        value = query.value(1);
        definiton.insert(columnList[1], value);
    }
    db.close();

    return definiton;
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
 * Create a QSqlRecord from an OptionTable object.
 * Take QSqlField obects from member record an set value of OptionTable.
 * Append this fields to a QSqlRecord object.
 * @param optionTable       All options given from the user.
 * @return record           A QSqlRecord object with fields for named columns in OptionTable.
 */
QSqlRecord Persistence::recordFromOptionTable(const OptionTable &optionTable) const
{
    QSqlRecord record;
    QList<char> keyList = optionTable.keys();
    foreach (char option, keyList) {
        QString columnName = columnNameOfOption(option);
        int index = m_record.indexOf(columnName);
        if (index < 0) {
            continue;
        }
        QSqlField field = m_record.field(index);
        QVariant value = optionTable.value(option);
        if (value.isValid()) {      // Field is NULL by default.
            field.setValue(value);  // Set just valid QVariant values.
        }
        record.append(field);
    }

    return record;
}

/**
 * Finds fields in record having a value which is not null.
 * @param record                The record with all fields.
 * @return recordWithValues     the field of record which have a value is not null.
 */
QSqlRecord Persistence::keepFieldsWithValues(const QSqlRecord &record) const
{
    QSqlRecord recordWithValues;
    for (int index=0; index<record.count(); ++index) {
        if (! record.isNull(index)) {
            QSqlField field = record.field(index);
            recordWithValues.append(field);
        }
    }

    return recordWithValues;
}

/**
 * Remove field with a UNIQUE constraint from given record.
 * Return a QSqlRecord with removed unique constraint fields.
 * Remove either Primary Key or Unique constraint.
 * @param record            All fields for a table row action.
 * @return uniqueFields     Database fields with a unique constraint.
 */
QSqlRecord Persistence::removeUniqueConstraintFields(QSqlRecord &record) const
{
    QSqlRecord uniqueFields;
    QString primaryKey = columnNameOfOption(m_primaryKey);
    int index = record.indexOf(primaryKey);
    if (index >= 0 && !record.isNull(index)) {
        QSqlField field = record.field(index);
        uniqueFields.append(field);
        record.remove(index);

        return uniqueFields;
    }
    int indexFirst = record.indexOf(columnNameOfOption(m_uniqueKey[0]));
    int indexSecond = record.indexOf(columnNameOfOption(m_uniqueKey[1]));
    if (indexFirst >= 0 && indexSecond >= 0 && !record.isNull(indexFirst) && !record.isNull(indexSecond)) {
        QSqlField firstField = record.field(indexFirst);
        uniqueFields.append(firstField);
        QSqlField secondField = record.field(indexSecond);
        uniqueFields.append(secondField);
        record.remove(indexFirst);
        record.remove(indexSecond);
    }

    return uniqueFields;
}

/**
 * Creates a QSqlRecord object with fields of column names.
 * Fields are taken from member record object.
 * @param columnList        A list of column names.
 * @return record           A record object containing all fields of column names.
 */
QSqlRecord Persistence::recordWithFields(const QStringList &columnList) const
{
    QSqlRecord record;
    foreach (QString column, columnList) {
        int index = m_record.indexOf(column);
        if (index < 0) {
            continue;
        }
        record.append(m_record.field(index));
    }

    return record;
}

/**
 * @brief Persistence::bindValuesToQuery
 * @param query
 * @param elementList
 */
void Persistence::bindValuesToQuery(QSqlQuery &query, const QSqlRecord &record) const
{
    for (int index=0; index<record.count(); ++index) {
        query.addBindValue(record.value(index));
    }
}


/**
 * Initializes a database with credentials from file.
 * @param db
 */
void Persistence::initializeDatabase(QSqlDatabase &db)
{
    QString homeDir = Credentials::usersHomePath();
    QString filePath = homeDir.append(QString("/.pwmanager"));
    Credentials credentials = Credentials::credentialsFromFile(filePath);
    db.setDatabaseName(credentials.value(Credentials::DatabaseName));
    db.setHostName(credentials.value(Credentials::Hostname));
    db.setUserName(credentials.value(Credentials::Username));
    db.setPassword(credentials.value(Credentials::Password));
    db.setPort(credentials.value(Credentials::Port).toInt());
    m_tableName = credentials.value(Credentials::TableName);
}


/**
 * Translates an option char to database name.
 * @param option
 * @return
 */
QString Persistence::columnNameOfOption(const char option)
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
