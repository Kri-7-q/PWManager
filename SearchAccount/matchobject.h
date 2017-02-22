#ifndef MATCHOBJECT_H
#define MATCHOBJECT_H


class MatchObject
{
public:
    MatchObject(const int index, const int result);

    int index() const                                       { return m_index; }

    // operator overload
    bool operator < (const MatchObject& rhs) const          { return m_result < rhs.m_result; }
    bool operator == (const MatchObject& rhs) const         { return m_result == rhs.m_result; }

private:
    int m_index;
    int m_result;
};

#endif // MATCHOBJECT_H
