#include "credentials.h"
#include <QFile>
#include <QTextStream>
#include <QProcessEnvironment>

Credentials::Credentials()
{
    m_keyNames = QStringList() << QString("password") << QString("username") << QString("databasename")
                               << QString("hostname") << QString("port");
}

/**
 * Add a value to the credentials.
 * @param key
 * @param value
 */
void Credentials::addValue(const Credentials::Key key, const QString &value)
{
    m_credentials.insert(key, value);
}

/**
 * Get a value of credentials.
 * @param key
 * @return
 */
QString Credentials::value(const Credentials::Key key) const
{
    return m_credentials.value(key, QString());
}

/**
 * Store credentials to file.
 * The current content of credentials is stored to a file
 * with the given path. If file exists it will be overwritten.
 * If file do not exist a new file is created.
 * @param path
 * @return
 */
bool Credentials::storeCredentialsToFile(const QString &path)
{
    QFile file(path);
    if (! file.open(QFile::WriteOnly)) {
        m_errorMsg = QString("Could not open file !");
        return false;
    }
    QString seperator(":");
    QTextStream outStream(&file);
    QList<Key> keyList = m_credentials.keys();
    for (int index=0; index<keyList.size(); ++index) {
        Key key = keyList[index];
        QString keyString = m_keyNames[key];
        QString value = m_credentials.value(keyList[index], QString());
        outStream << keyString << seperator << value << endl;
    }
    file.close();

    return true;
}

/**
 * Loads Credentials from a file with given path.
 * Credentials stored like:
 * password:pwofhorst
 * username:horst
 * @param path
 * @return      True is done.
 */
bool Credentials::loadCredentialsFromFile(const QString &path)
{
    QFile file(path);
    if (! file.exists()) {
        m_errorMsg = QString("File does not exist !");
        return false;
    }
    if (! file.open(QFile::ReadOnly)) {
        m_errorMsg = QString("Could not open file !");
        return false;
    }
    QTextStream inStream(&file);
    while (! inStream.atEnd()) {
        QString line = inStream.readLine();
        QStringList keyValueList = line.split(QChar(':'), QString::SkipEmptyParts);
        Key key = (Key)m_keyNames.indexOf(keyValueList[0]);
        QString value = keyValueList[1];
        m_credentials.insert(key, value);
    }
    file.close();

    return true;
}

/**
 * Read Credentials from file.
 * @param path
 * @return
 */
Credentials Credentials::credentialsFromFile(const QString &path)
{
    Credentials credentials;
    credentials.loadCredentialsFromFile(path);

    return credentials;
}

/**
 * Get the path to the users home directory.
 * @return
 */
QString Credentials::usersHomePath()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    return env.value(QString("HOME"));
}

/**
 * Get error message string.
 * @return
 */
QString Credentials::errorMsg() const
{
    return m_errorMsg;
}

