#include "searchengine.h"

/**
 * Constructor
 * Creates and initializes a SearchAccount object.
 * @param accountList           A list of accounts in which is to search for.
 * @param searchParams          The search parameters in an Account object.
 */
SearchEngine::SearchEngine(const QVariantMap &accountObj, const QVariantMap &searchParams) :
    m_accountObj(accountObj),
    m_searchParams(searchParams),
    m_letterCount(0),
    m_inSequence(0),
    m_position(-1)
{

}

/**
 * Start the search for all account objects with are match the search parameter.
 * @return resultList       A list of account object which were found.
 */
void SearchEngine::run()
{
    QStringList keyList = m_searchParams.keys();
    // Loop through all search parameter.
    for (int index=0; keyList.size(); ++index) {
        QString key = keyList[index];
        QVariant searchParam = m_searchParams.value(key, QVariant());
        QVariant value = m_accountObj.value(key, QVariant());
        // Suchbegriff mit Parameter aus der Liste vergleichen.
        // Punktebewertung auswerten.
        //      - Position im Wort ?
        //      - Wieviele zusammenhängende Buchstaben ?
        //      - Wieviele Buchstaben sind enthalten ?
        // Wenn ausreichend Punkte, dann in Result-Liste aufnehmen.
    }
}
