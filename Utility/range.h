#ifndef RANGE_H
#define RANGE_H

#include <QtGlobal>

class Range
{
public:
    Range();
    Range(const quint16 start, const quint16 end);

    void setStart(const quint16 start)                      { m_start = start; }
    void setEnd(const quint16 end)                          { m_end = end; }
    quint16 start() const                                   { return m_start; }
    quint16 end() const                                     { return m_end; }
    quint16 size() const                                    { return m_end - m_start; }
    quint16 distance(const Range &range) const;

    // Operator overload.
    bool operator < (const Range& rhs) const;
    bool operator > (const Range& rhs) const;

private:
    quint16 m_start;
    quint16 m_end;
};

#endif // RANGE_H
