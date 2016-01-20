#ifndef PERSISTENCE_H
#define PERSISTENCE_H

/* ------------------------------------------------------------------------------
 * Class Persistence
 *
 * Its an abstract class with most methods are pure virtual. This is just a
 * interface for a concret persistence class. There are a factory which creates
 * a instance of the concret class.
 * To use your own persistence you should subclass this interface and modify
 * the PersistenceFactory.
 * ------------------------------------------------------------------------------
 */

#include <QVariantMap>

typedef QHash<char, QVariant> OptionTable;

class Persistence
{
public:
    Persistence();
    virtual ~Persistence();

    // Open and close persistence
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool isOpen() const;

    // Modify persistent objects.
    // Database connection must be made before call one of these methods.
    virtual bool persistAccountObject(const OptionTable& account) = 0;
    virtual int deleteAccountObject(const OptionTable& account) = 0;
    virtual bool modifyAccountObject(const OptionTable& modifications) = 0;
    virtual QVariantMap findAccount(const OptionTable& searchObj) = 0;
    virtual QList<QVariantMap> findAccountsLike(const OptionTable& searchObj) = 0;

    // Read from persistence.
    // These methods open database connection by it self and close it afterwarts.
    virtual QList<QVariantMap> allPersistedAccounts() = 0;

    // Translate option char into real name.
    virtual QString optionToRealName(const char option) const = 0;

    // Error messages
    virtual QString error() const = 0;
    virtual bool hasError() const = 0;

protected:
    void setOpen(const bool isOpen)                 { m_isOpen = isOpen; }

private:
    bool m_isOpen;
};

#endif // PERSISTENCE_H
