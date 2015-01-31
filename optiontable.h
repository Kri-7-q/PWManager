#ifndef OPTIONTABLE_H
#define OPTIONTABLE_H

#include <QHash>
#include <QString>

enum OptionValueType { NoValue, String, Integer, Float, Double, Date, Time, DateTime };


class OptionTable : public QHash<QString, OptionValueType>
{
public:
    OptionTable(const QString &validOptions, const int argc, const char * const *argv);
    ~OptionTable();

};

#endif // OPTIONTABLE_H
