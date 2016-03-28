#ifndef XMLPERSISTENCE_H
#define XMLPERSISTENCE_H

/**
  * Class XmlPersistence
  * -----------------------
  * Implements the Persistence interface.
  * An XML file at the location <home>/.aacount/content.xml is read when open() was called.
  * If the content was modified the the content list is written to file as XML when close()
  * is called.
  */

#include "Persistence/persistence.h"
#include <QFile>
#include <QList>
#include <QVariantMap>
#include <QDomElement>

class XmlPersistence : public Persistence
{
public:
    XmlPersistence();

    // Persistence interface
public:
    bool open() override;
    void close() override;
    bool persistAccountObject(const OptionTable &account) override;
    int deleteAccountObject(const OptionTable &account) override;
    bool modifyAccountObject(const OptionTable &modifications) override;
    QVariantMap findAccount(const OptionTable &searchObj) override;
    QList<QVariantMap> findAccountsLike(const OptionTable &searchObj) override;
    QList<QVariantMap> allPersistedAccounts() override;

    QString optionToRealName(const char option) const override;
    QString error() const override;
    bool hasError() const override;

private:
    QString m_filename;
    QString m_error;
    QList<QVariantMap> m_contentList;
    int m_nextId;
    bool m_isModified;

    // Private methods
    QString getUsersHomePath() const;
    int getNextId()                     { return m_nextId++; }
    // Parsing
    bool parseXmlContentFile(QFile& file);
    bool parseXmlAccountObject(const QDomElement& xmlElement, QVariantMap& account);
    // Cast
    QVariant castStringToVariant(const QString dataType, const QString value) const;
    QDomElement accountObjectToXml(QVariantMap account, QDomDocument& doc) const;
    QVariantMap optionTableToVariantMap(const OptionTable& optionTbl) const;
    // Test
    bool equalsByUniqueConstraint(const OptionTable& optionTblObj, const QVariantMap& accountObj) const;
    bool equals(const OptionTable& optionTblObj, const QVariantMap& accountObj) const;
};

#endif // XMLPERSISTENCE_H
