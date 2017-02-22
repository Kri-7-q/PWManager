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
MatchObject::MatchObject(const int index, const int result) :
    m_index(index),
    m_result(result)
{

}
