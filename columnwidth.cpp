#include "columnwidth.h"

// Constructor
ColumnWidth::ColumnWidth()
{
}

/**
 * Takes a width value to a column.
 * If width value is greater then the current
 * value then the width will be set.
 * The columns name is the minimum width for a
 * column.
 * Includes a leading and following space.
 * @param column        The database column names.
 * @param width
 */
void ColumnWidth::insertWidthValue(const QString &column, const int width)
{
    int minWidth = 1;
    if (column.length() > width) {
        minWidth += column.length() + 1;
    } else {
        minWidth += width + 1;
    }
    int currentWidth = m_widthTable.value(column, 0);
    if (currentWidth < minWidth) {
        m_widthTable.insert(column, minWidth);
    }
}

/**
 * OVERLOAD
 * Takes a QVariant value and converts it to a string.
 * If the string length is greater than any forther string
 * that length value will be stored as current Column
 * width.
 * @param column
 * @param value
 */
void ColumnWidth::insertWidthValue(const QString &column, const QVariant &value)
{
    QString text = value.toString();
    int width = text.length();
    insertWidthValue(column, width);
}

/**
 * Return the width of a column.
 * @param column
 * @return
 */
int ColumnWidth::widthOf(const QString &column) const
{
    return m_widthTable.value(column, 0);
}

/**
 * Caluculates the total width of table.
 * @return
 */
int ColumnWidth::totalWidth() const
{
    int total = 1;
    QStringList columnList = m_widthTable.keys();
    for (QString column : columnList) {
        total += m_widthTable.value(column, 0);
        ++total;
    }

    return total;
}

/**
 * Get the amount of space which is needed to fill the value string
 * to fill the column to its width.
 * Calculation for one space as left margin.
 * @param column
 * @param value
 * @return          The amount of white space to fill the column.
 */
int ColumnWidth::spaceToFillColumn(const QString &column, const QString &value) const
{
    return m_widthTable.value(column, 0) - (value.length() + 1);
}
