#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <QHash>
#include <QStringList>
#include <QDebug>

class Credentials
{
public:
    Credentials();

    enum Key { Password, Username, DatabaseName, Hostname, Port, TableName };

    void addValue(const Key key, const QString &value);
    QString value(const Key key) const;
    QString errorMsg() const;
    bool hasError()                                 { return !m_errorMsg.isEmpty(); }

    // File operations
    bool storeCredentialsToFile(const QString &path);
    bool loadCredentialsFromFile(const QString &path);

    // Static funktions
    static Credentials credentialsFromFile(const QString &path);
    static QString usersHomePath();

private:
    QHash<Key, QString> m_credentials;
    QHash<QString, Key> m_keyMap;
    QString m_errorMsg;
};

#endif // CREDENTIALS_H
