#ifndef ELEMENT_H
#define ELEMENT_H

#include <QString>
#include <QVariant>

class OptionInfo
{
public:
    OptionInfo();
    OptionInfo(QString columnName, QVariant value, bool isIdentifier = false);
    OptionInfo(QString columnName, QString bindString, QVariant value, bool isIdentifier = false);
    ~OptionInfo();

    // elements
    bool isIdentifier;
    QString columnName;
    QString bindString;
    QVariant value;
};

#endif // ELEMENT_H
