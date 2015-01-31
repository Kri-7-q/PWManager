#ifndef OPTIONS_H
#define OPTIONS_H

#include <QStringList>
#include <QHash>

class Options
{
public:
    Options(const QString &validOptions);
    ~Options();

    bool hasError() const                   { return m_hasError; }
    QHash<QString, QString> parseOptions(const int argc, const char* const *argv, const int start);

private:
    QHash<QString, bool> m_validOptionTable;
    bool m_hasError;

    // Methods
    QHash<QString, bool> validOptionTable(const QString &validOptions);
    bool parseLongOption(QString &parameter, QHash<QString, QString> &optionTable);
};

#endif // OPTIONS_H
