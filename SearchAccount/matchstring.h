#ifndef MATCHSTRING_H
#define MATCHSTRING_H

#include "Utility/range.h"
#include <QHash>
#include <QVector>

class MatchString
{
public:
    MatchString(const QString& searchMask);

    void matchSymbol(const QChar& symbol);
    int matchText(const QString& text);
    int matchResult();
    void reset();

protected:
    void createSearchMask(const QString& mask);
    QVector<quint16> incrementVector(const QVector<quint16>& vector) const;
    QVector<quint16> intersection(const QVector<quint16>& first, const QVector<quint16>& second) const;
    void takeMatchesAndCleanup();

private:
    QHash<QChar, QVector<quint16>> m_characterMap;
    QVector<quint16> m_currentMatch;
    quint16 m_matchCount;
    QVector<Range> m_rangeVector;
};

#endif // MATCHSTRING_H
