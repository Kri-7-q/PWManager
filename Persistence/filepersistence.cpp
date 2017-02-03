#include "filepersistence.h"
#include <QTextStream>
#include <QDataStream>

/**
 * @brief FilePersistence::FilePersistence
 */
FilePersistence::FilePersistence() :
    m_pFile(NULL),
    m_isModified(false)
{

}

/**
 * @brief FilePersistence::~FilePersistence
 */
FilePersistence::~FilePersistence()
{
    close();
}

/**
 * @brief FilePersistence::error
 * @return
 */
QString FilePersistence::error() const
{
    return m_error;
}

/**
 * @brief FilePersistence::persistAccountObject
 * @param account
 * @return
 */
bool FilePersistence::persistAccountObject(const OptionTable &account)
{
    int index = findAccountObj(account);
    if (index > 0) {
        m_error.append(QString("There is a existing Account object with that keys !\n"));
        m_error.append(QString("Can not insert new Account object !\n"));

        return false;
    }
    m_isModified = true;
    QVariantMap object = variantMapFromOptionTable(account);
    m_fileContent << object;

    return true;
}

/**
 * @brief FilePersistence::deleteAccountObject
 * @param account
 * @return
 */
int FilePersistence::deleteAccountObject(const OptionTable &account)
{
    int index = findAccountObj(account);
    if (index < 0) {
        m_error.append(QString("Could not delete Account object !\n"));
        m_error.append(QString("There is no such Account object stored !\n"));
        return -1;
    }
    m_fileContent.removeAt(index);
    m_isModified = true;

    return 1;
}

/**
 * @brief FilePersistence::modifyAccountObject
 * @param modifications
 * @return
 */
bool FilePersistence::modifyAccountObject(const OptionTable &modifications)
{
    int index = findAccountObj(modifications);
    if (index < 0) {
        m_error.append(QString("Could not modify Account object !\n"));
        m_error.append(QString("There is no such an Account object.\n"));
        return false;
    }
    QVariantMap modifyValues = variantMapFromOptionTable(modifications);
    if (modifications.contains('i')) {
        modifyValues.remove(optionToRealName('i'));
    } else {
        modifyValues.remove(optionToRealName('p'));
        modifyValues.remove(optionToRealName('u'));
    }
    QVariantMap originObject = m_fileContent[index];
    QStringList keyList = modifyValues.keys();
    for (int index=0; index<keyList.size(); ++index) {
        QVariant value = modifyValues.value(keyList[index]);
        originObject.insert(keyList[index], value);
    }
    m_isModified = true;

    return true;
}

/**
 * @brief FilePersistence::findAccount
 * @param searchObj
 * @return
 */
QVariantMap FilePersistence::findAccount(const OptionTable &searchObj)
{
    int index = findAccountObj(searchObj);
    if (index < 0) {
        m_error.append(QString("Could not find Account object !\n"));
        m_error.append(QString("There is no such an Account object.\n"));
        return QVariantMap();
    }

    return m_fileContent[index];
}

/**
 * @brief FilePersistence::findAccountsLike
 * @param searchObj
 * @return
 */
QList<QVariantMap> FilePersistence::findAccountsLike(const OptionTable &searchObj)
{
    Q_UNUSED(searchObj)
    return QList<QVariantMap>();
}

/**
 * @brief FilePersistence::allPersistedAccounts
 * @return
 */
QList<QVariantMap> FilePersistence::allPersistedAccounts()
{
    return m_fileContent;
}

/**
 * @brief FilePersistence::optionToRealName
 * @param option
 * @return
 */
QString FilePersistence::optionToRealName(const char option) const
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
    case 'k':
        name = QString("password");
        break;
    case 'l':
        name = QString("passwordlength");
        break;
    case 's':
        name = QString("definedcharacter");
        break;
    case 'q':
        name = QString("question");
        break;
    case 'r':
        name = QString("answer");
        break;
    case 't':
        name = QString("lastmodify");
        break;
    default:
        break;
    }

    return name;
}

/**
 * @brief FilePersistence::hasError
 * @return
 */
bool FilePersistence::hasError() const
{
    return ! m_error.isEmpty();
}

/**
 * @brief FilePersistence::persistContent
 * @return
 */
bool FilePersistence::persistContent()
{
    if (! m_isModified) {
        return false;
    }
    bool result = m_pFile->resize(0);
    if (!result) {
        m_error.append(QString("Could not resize the file !\n"));
        m_error.append(m_pFile->errorString()).append('\n');

        return false;
    }
    QDataStream outStream(m_pFile);
    while (m_fileContent.size() > 0) {
        QVariantMap object = m_fileContent.takeFirst();
        outStream << object;
    }

    return true;
}

/**
 * Protected
 * @param primaryKey
 * @return
 */
int FilePersistence::findWithPrimaryKey(const QVariant& primaryKey) const
{
    for (int index=0; index<m_fileContent.size(); ++index) {
        QVariant key = m_fileContent[index].value("id");
        if (key == primaryKey) {
            return index;
        }
    }

    return -1;
}

/**
 * Protected
 * @param provider
 * @param username
 * @return
 */
int FilePersistence::findWithUnique(const QVariant &provider, const QVariant &username) const
{
    for (int index=0; index<m_fileContent.size(); ++index) {
        QVariant existProvider = m_fileContent[index].value("provider");
        QVariant existUsername = m_fileContent[index].value("username");
        if (existProvider == provider && existUsername == username) {
            return index;
        }
    }

    return -1;
}

/**
 * @brief FilePersistence::findAccountObj
 * @param account
 * @return
 */
int FilePersistence::findAccountObj(const OptionTable &account) const
{
    int index = -1;
    if (account.contains('i')) {
        index = findWithPrimaryKey(account.value('i'));
    } else {
        index = findWithUnique(account.value('p'), account.value('u'));
    }

    return index;
}

/**
 * @brief FilePersistence::variantMapFromOptionTable
 * @param account
 * @return
 */
QVariantMap FilePersistence::variantMapFromOptionTable(const OptionTable &account) const
{
    QVariantMap object;
    QList<char> keyList = account.keys();
    for (int index=0; index<keyList.size(); ++index) {
        QString key = optionToRealName(keyList[index]);
        QVariant value = account.value(keyList[index]);
        object.insert(key, value);
    }

    return object;
}

/**
 * @brief FilePersistence::persistReadableFile
 * @param filePath
 * @return
 */
bool FilePersistence::persistReadableFile(const QString &filePath, const QList<QVariantMap>& accountList)
{
    if (! open(filePath)) {
        m_error += QString("Could not open file !\n");
        m_error += m_pFile->errorString().append('\n');
        return false;
    }
    QTextStream outStream(m_pFile);
    for (int index=0; index<accountList.size(); ++index) {
        const QVariantMap& account = accountList.at(index);
        QStringList keyList = account.keys();
        for (int keyIndex=0; keyIndex<keyList.size(); ++keyIndex) {
            const QVariant value = account.value(keyList[keyIndex]);
            if (! value.isValid()) {
                continue;
            }
            QString line = keyList[keyIndex] + ": " + value.toString();
            outStream << line << endl;
        }
        outStream << QString("-----------------------------------------") << endl;
    }
    close();

    return true;
}

/**
 * @brief FilePersistence::open
 * @param parameter
 * @return
 */
bool FilePersistence::open(const QString &parameter)
{
    m_pFile = new QFile(parameter);
    if (! m_pFile->open(QIODevice::ReadWrite)) {
        m_error = QString("Could not open file !\n");
        m_error.append(m_pFile->errorString()).append('\n');
        return false;
    }
    setOpen(true);
    QDataStream inStream(m_pFile);
    QVariantMap object;
    while (! inStream.atEnd()) {
        inStream >> object;
        m_fileContent << object;
    }

    return true;
}

/**
 * @brief FilePersistence::close
 */
void FilePersistence::close()
{
    if (isOpen()) {
        persistContent();
        m_pFile->close();
        setOpen(false);
    }
    if (m_pFile != NULL) {
        delete m_pFile;
        m_pFile = NULL;
    }
}
