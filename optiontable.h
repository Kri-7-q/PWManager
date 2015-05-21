#ifndef OPTIONTABLE_H
#define OPTIONTABLE_H

#include <QHash>
#include <QVariant>

class OptionTable : public QHash<char, QVariant>
{
public:
    OptionTable();
    ~OptionTable();

    // Application specific methods
    void replaceOptionA();
    void insertStandardOptionForShow();
    bool hasValueForKey(const char key) const;
    bool hasValuesForKeySet(const QByteArray &keySet) const;
};

#endif // OPTIONTABLE_H
