#ifndef ELEMENT_H
#define ELEMENT_H

#include <QString>
#include <QVariant>

class DBValue
{
public:
    DBValue();
    DBValue(QString columnName, QVariant value, bool isIdentifier = false);
    DBValue(QString columnName, QString bindString, QVariant value, bool isIdentifier = false);
    ~DBValue();

    // elements
    bool isIdentifier;
    QString columnName;
    QString bindString;
    QVariant value;
};

#endif // ELEMENT_H
