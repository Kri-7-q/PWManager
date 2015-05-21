#include "dbvalue.h"


// Constructor
DBValue::DBValue() :
    isIdentifier(false)
{

}

/**
 * Constructor
 * Constructs an OptionInfo instance with values.
 * @param columnName        A database column name.
 * @param value             A value to a database column.
 * @param isIdentifier      True if column can alone or in combination with others identify an database entry.
 */
DBValue::DBValue(QString columnName, QVariant value, bool isIdentifier) :
    isIdentifier(isIdentifier),
    columnName(columnName),
    value(value)
{

}

/**
 * Constructor
 * @param columnName
 * @param bindString        A string as placeholder to bind values in prepared SQL statements.
 * @param value
 * @param isIdentifier
 */
DBValue::DBValue(QString columnName, QString bindString, QVariant value, bool isIdentifier) :
    columnName(columnName),
    bindString(bindString),
    value(value),
    isIdentifier(isIdentifier)
{

}

// Destructor
DBValue::~DBValue()
{

}
