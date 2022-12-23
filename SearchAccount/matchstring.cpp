#include "matchstring.h"

/**
 * Constructor
 * Creates a MatchString object and initializes the obect with a
 * search mask sting.
 * Puts each symbol of the search mask into a QHash map. The symbol
 * is the key. The index of that symbol builds the value of hash map.
 * The index is set into a QVector object. If a symbol appears again
 * its index is set to the former appearence.
 * @param searchMask        A search mask string.
 */
MatchString::MatchString(const QString &searchMask) :
    m_matchCount(0)
{
    createSearchMask(searchMask);
}

/**
 * Take a symbol from the text which is to search in the search mask.
 * Search mask is in a QHash map. If the symbol is part of the search mask
 * the hash map will return a QVector with index values.
 * The first match is stored into a QVector member attribute.
 * If a second match appears then the first index values are incremented.
 * If the symbol index and the incremented index has some in common then
 * a second match has been proved.
 * @param symbol
 * @return
 */
void MatchString::matchSymbol(const QChar &symbol)
{
    QVector<quint16> position = m_characterMap.value(symbol, QVector<quint16>());
    if (position.isEmpty()) {
        // Cleanup. Character didn't match.
        takeMatchesAndCleanup();
        return;
    }
    if (m_currentMatch.isEmpty()) {
        // Character matches but there are no former matches.
        ++m_matchCount;
        m_currentMatch = position;
        return;
    }
    QVector<quint16> nextPosition = incrementVector(m_currentMatch);
    QVector<quint16> match = intersection(position, nextPosition);
    if (match.isEmpty()) {
        // Did not match. Cleanup.
        takeMatchesAndCleanup();
    } else {
        // Matches.
        ++m_matchCount;
        m_currentMatch = match;
    }
}

/**
 * Takes a text to match it against the search mask.
 * There must be three symbol matches at least to be
 * counted. Each appereance of three matches and more
 * are counted.
 * @param text      A text to search for the mask.
 * @return          The number of symbol which mtached.
 */
int MatchString::matchText(const QString &text)
{
    for (int index=0; index<text.length(); ++index) {
        matchSymbol(text.at(index));
    }

    return matchResult();
}

/**
 * Calculates how much matches were found.
 * @return      The number of symbols which matched.
 */
int MatchString::matchResult()
{
    takeMatchesAndCleanup();
    int result = 0;
    for (int index=0; index<m_rangeVector.size(); ++index) {
        Range range = m_rangeVector[index];
        result += range.size();
    }

    return result;
}

/**
 * Resets the object to match another text.
 */
void MatchString::reset()
{
    m_currentMatch.clear();
    m_rangeVector.clear();
    m_matchCount = 0;
}

/**
 * Creates the QHash map from the serach mask string.
 * Collects characters of the mask and stores them with its positions
 * in the map.
 * @param mask
 */
void MatchString::createSearchMask(const QString &mask)
{
    for (quint16 index=0; index<mask.length(); ++index) {
        QChar symbol = mask.at(index).toLower();
        QVector<quint16> position = m_characterMap.value(symbol, QVector<quint16>());
        position << index;
        m_characterMap.insert(symbol, position);
    }
}

/**
 * Increments a QVector object. Adds 1 to each value in the vector.
 * @param vector        A vector object to increment.
 * @return increment    The vector object of which each value is greater
 *                      then the value of the given vector object.
 */
QVector<quint16> MatchString::incrementVector(const QVector<quint16> &vector) const
{
    QVector<quint16> increment;
    for (int index=0; index<vector.size(); ++index) {
        increment << (vector[index] + 1);
    }

    return increment;
}

/**
 * Takes two QVector objects and find the value which are both object have in common.
 * Returns a vector object containing all values which are equal in both
 * given vector objects. It make an intersection of two sets.
 * @param first         One vector object to intersect with another one.
 * @param second        The other vector object to intersect.
 * @return section      A vector with all the common values.
 */
QVector<quint16> MatchString::intersection(const QVector<quint16> &first, const QVector<quint16> &second) const
{
    QVector<quint16> section;
    for (int index=0; index<first.size(); ++index) {
        quint16 val = first[index];
        if (second.contains(val)) {
            section << val;
        }
    }

    return section;
}

/**
 * Is called after a symbol appears which do not match with the search mask.
 * If there was at least three matches then it stores that matches.
 * A Range object is created which holds the start and end index of the match
 * within the search mask string.
 */
void MatchString::takeMatchesAndCleanup()
{
    if (m_matchCount > 2) {
        quint16 end = m_currentMatch.first();
        quint16 start = end - m_matchCount;
        Range range(start, end);
        m_rangeVector << range;
    }
    m_currentMatch.clear();
    m_matchCount = 0;
}
