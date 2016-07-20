#include "xmlpersistence.h"
#include <QProcessEnvironment>
#include <QDomDocument>
#include <QDateTime>
#include <QTextStream>

XmlPersistence::XmlPersistence() :
    m_filename(".account/content.xml"),
    m_nextId(1),
    m_isModified(false)
{

}

/**
 * Open XML file with stored Accounts.
 * Reads the file and parses the XML content.
 * All content is kept in memory until it is
 * stored with close() function.
 * @return      True if file content could be read.
 */
bool XmlPersistence::open()
{
    QString filePath = getUsersHomePath() + "/" + m_filename;
    QFile xmlFile(filePath);
    if (! xmlFile.open(QFile::ReadOnly)) {
        m_error = QString("Could not open file !");
        return false;
    }
    bool result = parseXmlContentFile(xmlFile);
    xmlFile.close();

    return result;
}

/**
 * Store the content list to a XML file.
 */
void XmlPersistence::close()
{
    if (! m_isModified) {
        m_contentList.clear();
        m_nextId = 1;
        return;
    }
    // Document is modified and must be stored.
    QDomDocument doc("pwmanager");
    QDomElement root = doc.createElement("accountList");
    root.setAttribute("nextId", m_nextId);
    doc.appendChild(root);
    for (int index=0; index<m_contentList.size(); ++index) {
        QVariantMap account = m_contentList[index];
        QDomElement xmlAccount = accountObjectToXml(account, doc);
        root.appendChild(xmlAccount);
    }
    QString filePath = getUsersHomePath() + "/" + m_filename;
    QFile file(filePath);
    if (! file.open(QFile::WriteOnly)) {
        m_error = QString("Can not open file to write content !\nModifications not stored !");
        return;
    }
    QTextStream fileStream(&file);
    fileStream << doc.toString();
    file.close();

    m_contentList.clear();
    m_nextId = 1;
}

// Override
bool XmlPersistence::persistAccountObject(const OptionTable &account)
{
    for (int index=0; index<m_contentList.size(); ++index) {
        if (equalsByUniqueConstraint(account, m_contentList[index])) {
            m_error = QString("An account object with this credentials exists all ready !");
            return false;
        }
    }
    QVariantMap persistObj = optionTableToVariantMap(account);
    persistObj.insert("id", getNextId());
    m_contentList << persistObj;
    m_isModified = true;

    return true;
}

// Override
int XmlPersistence::deleteAccountObject(const OptionTable &account)
{
    int removed = 0;
    int index = m_contentList.size() - 1;
    for ( ; index>=0; --index) {
        if (equals(account, m_contentList[index])) {
            m_contentList.removeAt(index);
            ++removed;
        }
    }
    if (removed > 0) {
        m_isModified = true;
    }

    return removed;
}

// Override
bool XmlPersistence::modifyAccountObject(const OptionTable &modifications)
{
    QVariantMap* account = NULL;
    for (int index=0 ; index<m_contentList.size(); ++index) {
        if (equalsByUniqueConstraint(modifications, m_contentList[index])) {
            account = &m_contentList[index];
            break;
        }
    }
    if (account == NULL) {
        m_error = QString("Could not identify account object to modify !");
        return false;
    }
    QList<char> keyList = modifications.keys();
    if (keyList.contains('i')) {
        keyList.removeAll('i');
    } else {
        keyList.removeAll('p');
        keyList.removeAll('u');
    }
    for (int index = 0; index<keyList.size(); ++index) {
        QVariant value = modifications.value(keyList[index]);
        QString tagName = optionToRealName(keyList[index]);
        account->insert(tagName, value);
    }
    m_isModified = true;

    return true;
}

// Override
QVariantMap XmlPersistence::findAccount(const OptionTable &searchObj)
{
    for (int index=0; index<m_contentList.size(); ++index) {
        if (equals(searchObj, m_contentList[index])) {
            return accountFromIndexPartially(index, searchObj.keys());
        }
    }

    return QVariantMap();
}

// Override
QList<QVariantMap> XmlPersistence::findAccountsLike(const OptionTable &searchObj)
{
    QList<QVariantMap> list;
    QList<char> optionList = searchObj.keys();
    for (int index=0; index<m_contentList.size(); ++index) {
        if (equals(searchObj, m_contentList[index])) {
            list << accountFromIndexPartially(index, optionList);
        }
    }

    return list;
}

/**
 * Read whole XML file and return a list of its content.
 * @return      A list of variant maps with account objects.
 */
QList<QVariantMap> XmlPersistence::allPersistedAccounts()
{
    if (! open()) {
        // Error message provided by open().
        return QList<QVariantMap>();
    }

    return m_contentList;
}

/**
 * Translates an option charackter into a Xml tag name.
 * @param option        An option character.
 * @return name         The name of an XML tag.
 */
QString XmlPersistence::optionToRealName(const char option) const
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
        name = QString("passwordLength");
        break;
    case 's':
        name = QString("definedCharacter");
        break;
    case 'q':
        name = QString("question");
        break;
    case 'r':
        name = QString("answer");
        break;
    case 't':
        name = QString("lastModify");
        break;
    default:
        break;
    }

    return name;
}

/**
 * Get error message.
 * @return
 */
QString XmlPersistence::error() const
{
    return m_error;
}

/**
 * Query persistence from error massages.
 * @return      True if an error occured. Otherwise false.
 */
bool XmlPersistence::hasError() const
{
    return !m_error.isEmpty();
}

/**
 * Get users Home path from system evironment.
 * @return      The path to users home.
 */
QString XmlPersistence::getUsersHomePath() const
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    return env.value(QString("HOME"));
}

/**
 * Get an Account object from the list with a list of options.
 * The options singnals which attributes are requested.
 * @param index
 * @param optionList
 * @return
 */
QVariantMap XmlPersistence::accountFromIndexPartially(const int index, const QList<char> &optionList)
{
    QVariantMap& persistentAccount = m_contentList[index];
    QVariantMap account;
    for (int i=0; i<optionList.size(); ++i) {
        QString key = optionToRealName(optionList[i]);
        QVariant value = persistentAccount.value(key);
        account.insert(key, value);
    }

    return account;
}

/**
 * Parses the XML file and reads the content into memory.
 * @param file      A file to read from.
 * @return          True if file was read.
 */
bool XmlPersistence::parseXmlContentFile(QFile &file)
{
    QDomDocument doc("pwmanager");
    QString errorMsg;
    int errorLine = -1, errorColumn = -1;
    if (! doc.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        m_error = QString("Found an error while parsing file at line: %1 and column: %2 !").arg(errorLine).arg(errorColumn);
        return false;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() != "accountList") {
        m_error = QString("Wrong document ! Root node is '%1' but must be 'accountList'.").arg(root.tagName());
        return false;
    }
    m_nextId = root.attribute("nextId").toInt();
    QDomNode objNode = root.firstChild();
    while (! objNode.isNull()) {
        QDomElement accountXml = objNode.toElement();
        if (accountXml.tagName() != "account") {
            m_error = QString("Wrong document ! Node is '%1' but must be 'account'.").arg(accountXml.tagName());
            return false;
        }
        QVariantMap account;
        bool result = parseXmlAccountObject(accountXml, account);
        if (! result) {
            return false;
        }
        m_contentList << account;
        objNode = objNode.nextSibling();
    }

    return true;
}

/**
 * Parses a node of the DOM tree for an Account object.
 * @param xmlElement    The DOM node.
 * @param account       An empty QVariantMap to receive the values from DOM.
 * @return              True if done.
 */
bool XmlPersistence::parseXmlAccountObject(const QDomElement &xmlElement, QVariantMap &account)
{
    QString attribute = xmlElement.attribute(QString("id"), QString("-1"));
    bool result = true;
    int id = attribute.toInt(&result);
    if (! result) {
        m_error = QString("Tag attribute does not contain Id value (integer) value !");
        return false;
    }
    account.insert(QString("id"), QVariant(id));
    QDomNode elementNode = xmlElement.firstChild();
    while (! elementNode.isNull()) {
        QDomElement xmlElementChild = elementNode.toElement();
        QString dataType = xmlElementChild.attribute(QString("type"));
        QVariant value = castStringToVariant(dataType, xmlElementChild.text());
        account.insert(xmlElementChild.tagName(), value);
        elementNode = elementNode.nextSibling();
    }

    return true;
}

/**
 * Cast the string value from XML to a QVariant value with a given type.
 * If the data type is not known by this function then a QVariant containing
 * the given string value is returned.
 * @param dataType      The type of the value.
 * @param value         A string value to cast into variant.
 * @return var          A QVariant contaning the value from DOM.
 */
QVariant XmlPersistence::castStringToVariant(const QString dataType, const QString value) const
{
    QVariant::Type type = QVariant::nameToType(dataType.toLocal8Bit().data());
    QVariant var(type);
    switch (type) {
    case QVariant::Int:
        var.setValue(value.toInt());
        break;
    case QVariant::DateTime:
        var.setValue(QDateTime::fromString(value, Qt::ISODate));
        break;
    case QVariant::String:
        var.setValue(value);
        break;
    default:
        var = QVariant();
        break;
    }

    return var;
}

/**
 * Turns a QVariantMap object into an XML node.
 * It uses the keys of the map as tag names.
 * @param account           A map which is to turn into XML.
 * @param doc               The XML document reference.
 * @return xmlAccount       A DOM node with the values of 'account'.
 */
QDomElement XmlPersistence::accountObjectToXml(QVariantMap account, QDomDocument& doc) const
{
    QDomElement xmlAccount = doc.createElement("account");
    int id = account.take(QString("id")).toInt();
    xmlAccount.setAttribute(QString("id"), id);
    QStringList keyList = account.keys();
    for (int index=0; index<keyList.size(); ++index) {
        QString tagName = keyList[index];
        QString value = account.value(tagName).toString();
        QDomElement tag = doc.createElement(tagName);
        QDomText tagText = doc.createTextNode(value);
        tag.appendChild(tagText);
        xmlAccount.appendChild(tag);
    }

    return xmlAccount;
}

/**
 * Translate an OptionTable object to a QVariantMap object.
 * Option char's are translated to names.
 * @param optionTbl
 * @return
 */
QVariantMap XmlPersistence::optionTableToVariantMap(const OptionTable &optionTbl) const
{
    QList<char> keyList = optionTbl.keys();
    QVariantMap account;
    for (int index=0; index<keyList.size(); ++index) {
        QString keyName = optionToRealName(keyList[index]);
        QVariant value = optionTbl.value(keyList[index]);
        account.insert(keyName, value);
    }

    return account;
}

/**
 * Compares an OptionTable object with an Account object (QVariantMap).
 * These both types are equal. The difference is just the key value.
 * OptionTable uses a char value wereas the QVariantMap key are strings.
 * @param accountObj    New inserted object (OptionTable).
 * @param listObj       An Account object from persistence list.
 * @return              True if unique values are equal. (id, provider, username)
 */
bool XmlPersistence::equalsByUniqueConstraint(const OptionTable &optionTblObj, const QVariantMap &accountObj) const
{
    if (optionTblObj.isEmpty()) {
        return false;
    }
    if (optionTblObj.contains('i') && optionTblObj.value('i') == accountObj.value("id")) {
        return true;
    }
    if (optionTblObj.value('p') != accountObj.value("provider")) {
        return false;
    }
    if (optionTblObj.value('u') != accountObj.value("username")) {
        return false;
    }

    return true;
}

/**
 * Compares the values in the OptionTable object with the values in a Account object (QVariantMap).
 * OptionTable objects have single char's as keys which must be translated to persistence keys to
 * get the value of a persistent account object.
 * @param optionTblObj      OptionTable object with option char's as key value.
 * @param accountObj        Persistent Account object.
 * @return                  True if the values in OptionTable object equals with that in the Account object.
 */
bool XmlPersistence::equals(const OptionTable &optionTblObj, const QVariantMap &accountObj) const
{
    if (optionTblObj.isEmpty()) {
        return false;
    }
    QList<char> keyList = optionTblObj.keys();
    for (int index=0; index<keyList.size(); ++index) {
        QVariant searchValue = optionTblObj.value(keyList[index]);
        if (searchValue.isNull()) {
            continue;
        }
        QVariant accountValue = accountObj.value(optionToRealName(keyList[index]));
        if (searchValue != accountValue) {
            return false;
        }
    }

    return true;
}

