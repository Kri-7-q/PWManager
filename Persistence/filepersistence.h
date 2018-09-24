#ifndef FILEPERSISTENCE_H
#define FILEPERSISTENCE_H

#include <QFile>
#include "Persistence/persistence.h"

class FilePersistence : public Persistence
{
public:
    FilePersistence();
    ~FilePersistence() override;

    // Opens the file and writes the content.
    bool persistReadableFile(const QString& filePath, const QList<QVariantMap> &accountList);

    // Persistence interface
    bool open(const QString &parameter) override;
    void close() override;
    QString error() const override;
    bool persistAccountObject(const OptionTable &account) override;
    int deleteAccountObject(const OptionTable &account) override;
    bool modifyAccountObject(const OptionTable &modifications) override;
    QVariantMap findAccount(const OptionTable &searchObj) override;
    QVariantMap findUser(const OptionTable &userInfo) override;
    QList<QVariantMap> findAccountsLike(const OptionTable &searchObj) override;
    QList<QVariantMap> allPersistedAccounts() override;
    QString optionToRealName(const char option) const override;
    bool hasError() const override;

protected:
    bool persistContent();
    int findWithPrimaryKey(const QVariant &primaryKey) const;
    int findWithUnique(const QVariant &provider, const QVariant &username) const;
    int findAccountObj(const OptionTable& account) const;
    QVariantMap variantMapFromOptionTable(const OptionTable& account) const;

private:
    QFile* m_pFile;
    QString m_error;
    QList<QVariantMap> m_fileContent;
    bool m_isModified;
};

#endif // FILEPERSISTENCE_H
