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
    void replaceOptionE();
    void insertStandardOptions();
    bool hasValueForKey(const char key) const;
    bool hasValuesForKeySet(const QByteArray &keySet) const;

private:
    void setOptions(const QList<char>& optionList);
};

#endif // OPTIONTABLE_H
