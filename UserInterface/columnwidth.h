#ifndef COLUMNWIDTH_H
#define COLUMNWIDTH_H

/* ---------------------------------------------------------------------------
 * Class ColumnWidth
 * ---------------------------------------------------------------------------
 * The class calculates the width of all output columns.
 * The width of a column depents on the longest entry in a
 * column of read data.
 */

#include <QHash>
#include <QStringList>
#include <QVariant>

class ColumnWidth
{
public:
    ColumnWidth();

    void insertWidthValue(const QString &column, const int width);
    void insertWidthValue(const QString &column, const QVariant &value);
    int widthOf(const QString &column) const;
    int totalWidth() const;
    int spaceToFillColumn(const QString &column, const QString &value) const;
    void clear()                        { m_widthTable.clear(); }

private:
    QHash<QString, int> m_widthTable;
};

#endif // COLUMNWIDTH_H
