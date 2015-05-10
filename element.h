#ifndef ELEMENT_H
#define ELEMENT_H

#include <QString>
#include <QVariant>

class Element
{
public:
    Element();
    Element(QString columnName, QVariant value, bool isIdentifier = false);
    Element(QString columnName, QString bindString, QVariant value, bool isIdentifier = false);
    ~Element();

    // elements
    bool isIdentifier;
    QString columnName;
    QString bindString;
    QVariant value;
};

#endif // ELEMENT_H
