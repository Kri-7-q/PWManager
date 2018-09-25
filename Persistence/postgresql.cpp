#include "postgresql.h"
#include "credentials.h"
#include <QSqlDatabase>
#include <QSqlField>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>

PostgreSQL::PostgreSQL()
{
    initializeDatabase();
}

// Override
bool PostgreSQL::open(const QString &parameter)
{
    Q_UNUSED(parameter)
    QSqlDatabase db = QSqlDatabase::database("local", false);
    if (db.open()) {
        setOpen(true);
        return true;
    }
    setErrorDatabaseConectionFailed(db.lastError().databaseText(), db.lastError().driverText());

    return false;
}

// Override
void PostgreSQL::close()
{
    QSqlDatabase db = QSqlDatabase::database("local", false);
    db.close();
    setOpen(false);
}

// Override
bool PostgreSQL::persistAccountObject(const OptionTable &account)
{
    QSqlRecord record = recordFromOptionTable(account);
    QSqlDatabase db = QSqlDatabase::database(QString("local"));
    QString sqlInsert = db.driver()->sqlStatement(QSqlDriver::InsertStatement, m_tableName, record, true);
    QSqlQuery query(db);
    if (! query.prepare(sqlInsert)) {
        setErrorPrepareStatement(query.lastError().databaseText(), query.lastError().driverText());
        return false;
    }
    for (int index=0; index<record.count(); ++index) {
        query.addBindValue(record.value(index));
    }
    if (! query.exec()) {
        setErrorExecutionFailed(query.lastError().databaseText(), query.lastError().driverText());
        return false;
    }

    return true;
}

// Override
int PostgreSQL::deleteAccountObject(const OptionTable &account)
{
    QSqlRecord record = recordWithIdentifier(account);
    if (record.isEmpty()) {
        m_errorMsg.append(QString("Can not identify Account object in database!\n"));
        m_errorMsg.append(QString("It needs a 'id' value. Or 'provider' and 'username' to identify an Account object.\n"));
        return 0;
    }
    QSqlDatabase db = QSqlDatabase::database(QString("local"));
    QString sqlDelete = db.driver()->sqlStatement(QSqlDriver::DeleteStatement, m_tableName, record, false);
    QString whereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, record, true);
    sqlDelete.append(' ').append(whereClause);
    QSqlQuery query(db);
    if (! query.prepare(sqlDelete)) {
        setErrorPrepareStatement(query.lastError().databaseText(), query.lastError().driverText());
        return -1;
    }
    for (int index=0; index<record.count(); ++index) {
        query.addBindValue(record.value(index));
    }
    if (! query.exec()) {
        setErrorExecutionFailed(query.lastError().databaseText(), query.lastError().driverText());
        return -1;
    }

    return query.numRowsAffected();
}

// Override
bool PostgreSQL::modifyAccountObject(const OptionTable &modifications)
{
    QSqlRecord recordIdentifier = recordWithIdentifier(modifications);
    if (recordIdentifier.isEmpty()) {
        m_errorMsg.append(QString("Can not identify Account object in database!\n"));
        m_errorMsg.append(QString("It needs a 'id' value. Or 'provider' and 'username' to identify an Account object.\n"));
        return false;
    }
    QSqlRecord recordValues = recordWithoutIdentifier(modifications);
    QSqlDatabase db = QSqlDatabase::database(QString("local"));
    QString sqlUpdate = db.driver()->sqlStatement(QSqlDriver::UpdateStatement, m_tableName, recordValues, true);
    QString sqlWhereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, recordIdentifier, true);
    sqlUpdate.append(' ').append(sqlWhereClause);
    QSqlQuery query(db);
    if (! query.prepare(sqlUpdate)) {
        setErrorPrepareStatement(query.lastError().databaseText(), query.lastError().driverText());
        return false;
    }
    QSqlRecord record = recordConcardinate(recordValues, recordIdentifier);
    for (int index=0; index<record.count(); ++index) {
        query.addBindValue(record.value(index));
    }
    if (! query.exec()) {
        setErrorExecutionFailed(query.lastError().databaseText(), query.lastError().driverText());
        return false;
    }

    return true;
}

/**
 * Find a Account object in database.
 * @param searchObj
 * @return
 */
QVariantMap PostgreSQL::findAccount(const OptionTable &searchObj)
{
    QSqlDatabase db = QSqlDatabase::database(QString("local"));
    QSqlRecord record = recordFromOptionTable(searchObj);
    QString sqlSelect = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, record, false);
    QSqlRecord recordIdentifier = recordWithIdentifier(searchObj);
    QString sqlWhereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, recordIdentifier, true);
    sqlSelect.append(' ').append(sqlWhereClause);
    QSqlQuery query(db);
    if (! query.prepare(sqlSelect)) {
        setErrorPrepareStatement(query.lastError().databaseText(), query.lastError().driverText());
        return QVariantMap();
    }
    for (int index=0; index<recordIdentifier.count(); ++index) {
        query.addBindValue(recordIdentifier.value(index));
    }
    if (! query.exec()) {
        setErrorExecutionFailed(query.lastError().databaseText(), query.lastError().driverText());
        return QVariantMap();
    }
    if (! query.next()) {
        return QVariantMap();
    }

    return accountObject(query.record());
}

/**
 * Find Account objects which fits to the search values of search object.
 * @param searchObj
 * @return
 */
QList<QVariantMap> PostgreSQL::findAccountsLike(const OptionTable &searchObj)
{
    QSqlDatabase db = QSqlDatabase::database(QString("local"));
    QSqlRecord record = recordFromOptionTable(searchObj);
    QString sqlSelect = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, record, false);
    QSqlRecord recordSearch = recordFieldsWithValues(searchObj);
    if (! recordSearch.isEmpty()) {
        QString sqlWhereClause = db.driver()->sqlStatement(QSqlDriver::WhereStatement, m_tableName, recordSearch, true);
        sqlSelect.append(' ').append(sqlWhereClause);
    }
    QSqlQuery query(db);
    if (! query.prepare(sqlSelect)) {
        setErrorPrepareStatement(query.lastError().databaseText(), query.lastError().driverText());
        return QList<QVariantMap>();
    }
    for (int index=0; index<recordSearch.count(); ++index) {
        query.addBindValue(recordSearch.value(index));
    }
    if (! query.exec()) {
        setErrorExecutionFailed(query.lastError().databaseText(), query.lastError().driverText());
        return QList<QVariantMap>();
    }
    QList<QVariantMap> accountList;
    while (query.next()) {
        accountList << accountObject(query.record());
    }

    return accountList;
}

/**
 * Reads the whole database table. All data is returned as a list of
 * Account objects (QVariantMap).
 * @return accountList      A list of accounts stored in database.
 */
QList<QVariantMap> PostgreSQL::allPersistedAccounts()
{
    QSqlDatabase db = QSqlDatabase::database(QString("local"), false);
    if (! db.open()) {
        setErrorDatabaseConectionFailed(db.lastError().databaseText(), db.lastError().driverText());
        return QList<QVariantMap>();
    }
    QSqlRecord record = db.record(m_tableName);
    QString sqlSelect = db.driver()->sqlStatement(QSqlDriver::SelectStatement, m_tableName, record, false);
    QSqlQuery query(sqlSelect, db);
    if (query.lastError().isValid()) {
        setErrorExecutionFailed(query.lastError().databaseText(), query.lastError().driverText());
        return QList<QVariantMap>();
    }
    QList<QVariantMap> accountList;
    while (query.next()) {
        accountList << accountObject(query.record());
    }

    return accountList;
}

/**
 * @brief Find a user in database 'user' table.
 * @param userInfo
 * @return
 */
QVariantMap PostgreSQL::findUser(const OptionTable& userInfo)
{
    QSqlDatabase db = QSqlDatabase::database(QString("local"));
    QSqlRecord record = recordFromOptionTable(userInfo);
    QString sqlSelect = db.driver()->sqlStatement(QSqlDriver::SelectStatement, QString("public.user"), record, false);
    QSqlRecord whereRecord = recordFieldsWithValues(userInfo);
    QString sqlWhere = db.driver()->sqlStatement(QSqlDriver::WhereStatement, QString("public.user"), whereRecord, false);
    sqlSelect.append(' ').append(sqlWhere);
    QSqlQuery query(sqlSelect, db);
    if (query.lastError().isValid()) {
        setErrorExecutionFailed(query.lastError().databaseText(), query.lastError().driverText());
        return QVariantMap();
    }
    if (! query.next()) {
        return QVariantMap();
    }

    return accountObject(query.record());
}

/**
 * Private
 * Reads credentials from a file and initializes the database.
 */
void PostgreSQL::initializeDatabase()
{
    QString path = Credentials::usersHomePath() + QString("/.pwmanager");
    Credentials credentials = Credentials::credentialsFromFile(path);
    QSqlDatabase db = QSqlDatabase::addDatabase(QString("QPSQL"), "local");
    db.setHostName(credentials.value(Credentials::Hostname));
    db.setDatabaseName(credentials.value(Credentials::DatabaseName));
    db.setPort(credentials.value(Credentials::Port).toInt());
    db.setUserName(credentials.value(Credentials::Username));
    db.setPassword(credentials.value(Credentials::Password));
    m_tableName = credentials.value(Credentials::TableName);
}

/**
 * Private
 * Creates a QSqlRecord object from the information stored in the OptionTable object.
 * The option char is translated to a column name and the values are set to the record.
 * If an option keeps a value then the field in record is set to generated.
 * @param optionTable       The OptionTable object from the option parser.
 * @return record           A QSqlRecord object with all relevant columns and values.
 */
QSqlRecord PostgreSQL::recordFromOptionTable(const OptionTable &optionTable) const
{
    QSqlRecord record;
    QList<char> optionList = optionTable.keys();
    for (int index=0; index<optionList.size(); ++index) {
        char option = optionList[index];
        QString columnName = optionToRealName(option);
        if (columnName.isEmpty()) {
            continue;
        }
        QVariant value = optionTable.value(option, QVariant());
        QSqlField field(columnName, value.type());
        field.setValue(value);
        record.append(field);
    }

    return record;
}

/**
 * Private
 * Creates a QSqlRecord object from a OptionTable object (QHash).
 * Created fields in record are just identifier fields. This are
 * either primary key (id) or unique values (provider, username).
 * @param optionTable
 * @return record           A record object with identifier fields.
 */
QSqlRecord PostgreSQL::recordWithIdentifier(const OptionTable &optionTable) const
{
    QSqlRecord record;
    QVariant userId = optionTable.value('U');
    recordAppendField(record, optionToRealName('U'), userId);
    QVariant valueId = optionTable.value('i', QVariant(QVariant::Invalid));
    if (valueId.isValid()) {
        recordAppendField(record, optionToRealName('i'), valueId);
        return record;
    }
    QVariant valueProvider = optionTable.value('p' ,QVariant(QVariant::Invalid));
    QVariant valueUsername = optionTable.value('u', QVariant(QVariant::Invalid));
    if (valueProvider.isValid() && valueUsername.isValid()) {
        recordAppendField(record, optionToRealName('p'), valueProvider);
        recordAppendField(record, optionToRealName('u'), valueUsername);
    } else {
        return QSqlRecord();
    }

    return record;
}

/**
 * Private
 * Creates a QSqlRecord object with fields which are non identifiers.
 * This function takes a copy of the option table and removes either
 * the primary key (id) value or the unique constraint values
 * (provider, username) from the copy of option table.
 * @param optionTable
 * @return                  A record object whitout identifier fields.
 */
QSqlRecord PostgreSQL::recordWithoutIdentifier(OptionTable optionTable) const
{
    optionTable.take('U');
    if (optionTable.value('i', QVariant(QVariant::Invalid)).isValid()) {
        optionTable.take('i');
        return recordFromOptionTable(optionTable);
    }
    if (optionTable.value('p', QVariant(QVariant::Invalid)).isValid() &&
        optionTable.value('u', QVariant(QVariant::Invalid)).isValid()   ) {
        optionTable.take('p');
        optionTable.take('u');
    }

    return recordFromOptionTable(optionTable);
}

/**
 * Creates a QSqlRecord object with fields which takes a value. Empty
 * fields are dropped.
 * @param optionTable
 * @return record       A record object with fields containing a value only.
 */
QSqlRecord PostgreSQL::recordFieldsWithValues(const OptionTable &optionTable) const
{
    QSqlRecord record;
    QList<char> keyList = optionTable.keys();
    for (int index=0; index<keyList.size(); ++index) {
        QVariant value = optionTable.value(keyList[index], QVariant(QVariant::Invalid));
        QString columnName = optionToRealName(keyList[index]);
        if (! columnName.isEmpty() && value.isValid()) {
            recordAppendField(record, columnName, value);
        }
    }

    return record;
}

/**
 * Public
 * Translates the option char into an string. The string is the
 * real name of the persistence.
 * @param option        The option character.
 * @return              A database column name. Or an emty string.
 */
QString PostgreSQL::optionToRealName(const char option) const
{
    QString name;
    switch (option) {
    case 'i':
        name = QString("id");
        break;
    case 'p':
        name = QString("provider");
        break;
    case 'u':
        name = QString("username");
        break;
    case 'q':
        name = QString("question");
        break;
    case 'r':
        name = QString("answer");
        break;
    case 'k':
        name = QString("password");
        break;
    case 'l':
        name = QString("passwordlength");
        break;
    case 's':
        name = QString("definedcharacter");
        break;
    case 't':
        name = QString("lastmodify");
        break;
    case 'U':
        name = QString("userid");
        break;
    case 'n':
        name = QString("name");
        break;
    case 'm':
        name = QString("email");
        break;
    case 'x':
        name = QString("active");
        break;
    default:
        break;
    }

    return name;
}

/**
 * Private
 * Set an error message if database connection fails.
 * @param database      The error message of the database object.
 * @param driver        The error message of the QSqlDriver object.
 */
void PostgreSQL::setErrorDatabaseConectionFailed(const QString &database, const QString &driver)
{
    m_errorMsg.append(QString("Could not open Database !\n"));
    m_errorMsg.append(database).append('\n');
    m_errorMsg.append(driver).append('\n');
}

/**
 * Private
 * Set an error message when could not prepare SQL statement.
 * @param database      The error message of the database object.
 * @param driver        The error message of the QSqlDriver object.
 */
void PostgreSQL::setErrorPrepareStatement(const QString &database, const QString &driver)
{
    m_errorMsg.append(QString("Could not prepare SQL statement !\n"));
    m_errorMsg.append(database).append('\n');
    m_errorMsg.append(driver).append('\n');
}

/**
 * Private
 * Set an error message when could not prepare SQL statement.
 * @param database      The error message of the database object.
 * @param driver        The error message of the QSqlDriver object.
 */
void PostgreSQL::setErrorExecutionFailed(const QString &database, const QString &driver)
{
    m_errorMsg.append(QString("Could not execute SQL statement !\n"));
    m_errorMsg.append(database).append('\n');
    m_errorMsg.append(driver).append('\n');
}

/**
 * Private, Inline
 * Append a new field to a QSqlRecord object.
 * @param record        The record object which gets a new field.
 * @param name          A field name.
 * @param value         Fields value.
 */
void PostgreSQL::recordAppendField(QSqlRecord &record, const QString &name, const QVariant &value) const
{
    QSqlField field(name, value.type());
    field.setValue(value);
    record.append(field);
}

/**
 * Private
 * Concardinates two record object. The result will be that the first record
 * is copied and second record appended to the first. The order of fields in
 * the record objects is kept.
 * @param first
 * @param second
 * @return record       The concardination of first and second record object.
 */
QSqlRecord PostgreSQL::recordConcardinate(const QSqlRecord &first, const QSqlRecord &second) const
{
    QSqlRecord record(first);
    for (int index=0; index<second.count(); ++index) {
        record.append(second.field(index));
    }

    return record;
}

/**
 * Creates a QVariantMap object from a QSqlRecord object. Uses the column names
 * as keys. This builds a account object.
 * @param record        A record object from the SQL query.
 * @return account      An Account object.
 */
QVariantMap PostgreSQL::accountObject(const QSqlRecord &record) const
{
    QVariantMap account;
    for (int index=0; index<record.count(); ++index) {
        QString name = record.fieldName(index);
        QVariant value = record.value(index);
        account.insert(name, value);
    }

    return account;
}
