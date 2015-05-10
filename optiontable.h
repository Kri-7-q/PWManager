#ifndef OPTIONTABLE_H
#define OPTIONTABLE_H

#include <QHash>
#include <QVariant>

class OptionTable : public QHash<char, QVariant>
{
public:
    OptionTable();
    ~OptionTable();

    void replaceOptionA();
    void insertStandardOptionForShow();
};

#endif // OPTIONTABLE_H
