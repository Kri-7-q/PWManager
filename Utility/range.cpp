#include "range.h"

/**
 * Constructor
 * Creates a range object with start and end value null.
 */
Range::Range() :
    m_start(0),
    m_end(0)
{

}

/**
 * Constructor
 * Creates a range object with a given start and end value.
 * @param start     The lower value of a range object.
 * @param end       The higher value of a range object.
 */
Range::Range(const quint16 start, const quint16 end) :
    m_start(start),
    m_end(end)
{

}

/**
 * Return the distance between two Rage objects.
 * @return
 */
quint16 Range::distance(const Range& range) const
{
    if (*this < range) {
        return range.m_start - m_end;
    }
    if (*this > range) {
        return m_start - range.m_end;
    }

    return 0;
}

/**
 * Operator overload
 * Compares this Range object to another one.
 * Returns true if this is lower then rhs.
 * @param rhs       A Range object to compare with this.
 * @return          True if this is lower then the other one.
 */
bool Range::operator < (const Range &rhs) const
{
    if (m_end < rhs.m_start) {
        return true;
    }

    return false;
}

/**
 * Operator overload
 * Compares this Range object to another one.
 * Returns true if this is greater then rhs.
 * @param rhs       A Range object to compare with this.
 * @return          True if this is greater then the other one.
 */
bool Range::operator > (const Range &rhs) const
{
    if (m_start > rhs.end()) {
        return true;
    }

    return false;
}
