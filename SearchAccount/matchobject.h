#ifndef MATCHOBJECT_H
#define MATCHOBJECT_H

#include <QString>

class MatchObject
{
public:
    MatchObject(const int id, const QString& text, const int result);
    QString toString(bool showMatchResult = false) const;

    // operator overload
    bool operator < (const MatchObject& rhs) const          { return m_result < rhs.m_result; }
    bool operator == (const MatchObject& rhs) const         { return m_result == rhs.m_result; }

private:
    QString m_text;
    int m_id;
    int m_result;
};

#endif // MATCHOBJECT_H
