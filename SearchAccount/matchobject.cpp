#include "matchobject.h"

/**
 * Constructor
 * Creates a MatchObject object. It takes a id value, the text which was to
 * match against the search mask and the match result.
 * This object is necassary to sort the matches with their match result.
 * @param id        The objects id.
 * @param text      The text which was to search for the mask.
 * @param result    A match result value.
 */
MatchObject::MatchObject(const int id, const QString &text, const int result) :
    m_id(id),
    m_text(text),
    m_result(result)
{

}

/**
 * Returns a string representation of this object.
 * @return
 */
QString MatchObject::toString(bool showMatchResult) const
{
    QString space(2, ' ');

    QString stringId = QString::number(m_id);
    QString fillString(3 - stringId.length(), ' ');
    stringId = fillString.append(stringId);

    fillString = QString(30 - m_text.length(), ' ');
    QString text = m_text + fillString;

    QString objectString(stringId);
    objectString.append(space).append(text);

    if (showMatchResult) {
        QString resultString = QString::number(m_result);
        fillString = QString(3 - resultString.length(), ' ');
        resultString = fillString.append(resultString);
        objectString.append(space).append(resultString);
    }

    return objectString;
}
