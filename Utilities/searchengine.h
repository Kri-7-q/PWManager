#ifndef SEARCH_H
#define SEARCH_H

#include <QList>
#include <QVariantMap>

class SearchEngine
{
public:
    SearchEngine(const QVariantMap& accountObj, const QVariantMap& searchParams);

    void run();

private:
    const QVariantMap& m_accountObj;
    const QVariantMap& m_searchParams;
    quint16 m_letterCount;
    quint8 m_inSequence;
    qint16 m_position;
};

#endif // SEARCH_H
