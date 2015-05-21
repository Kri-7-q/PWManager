#ifndef ELEMENT_H
#define ELEMENT_H

/* ----------------------------------------------------------------------
 * Class DBValue
 * ----------------------------------------------------------------------
 * Takes a value to store in database and keeps additional information
 * to each value.
 * Additional information :
 * - database column name
 * - marks values which can identify a database record
 * - takes a bind string for prepared statements
 * Do not set a bind string manually. Bind strings are set while creating
 * SQL statememts.
 */

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
