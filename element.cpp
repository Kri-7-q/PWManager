#include "element.h"

Element::Element() :
    isIdentifier(false)
{

}

Element::Element(QString columnName, QVariant value, bool isIdentifier) :
    isIdentifier(isIdentifier),
    columnName(columnName),
    value(value)
{

}

Element::Element(QString columnName, QString bindString, QVariant value, bool isIdentifier) :
    columnName(columnName),
    bindString(bindString),
    value(value),
    isIdentifier(isIdentifier)
{

}

Element::~Element()
{

}
