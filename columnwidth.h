#ifndef COLUMNWIDTH_H
#define COLUMNWIDTH_H

#include <QHash>
#include <QStringList>

class ColumnWidth
{
public:
    ColumnWidth();

    void widthValue(const QString &column, const int width);
    int widthOf(const QString &column) const;
    int totalWidth() const;
    int spaceToFillColumn(const QString &column, const QString &value) const;

private:
    QHash<QString, int> m_widthTable;
};

#endif // COLUMNWIDTH_H
