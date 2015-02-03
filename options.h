#ifndef OPTIONS_H
#define OPTIONS_H

#include <QStringList>
#include <QHash>
#include <QDebug>

class Options
{
public:
    Options(const QString &validOptions);
    ~Options();

    bool hasError() const                   { return m_hasError; }
    QHash<QString, QString> parseOptions(const int argc, const char* const *argv, const int start);
    QChar optionSeparater() const;
    void setOptionSeparater(const QChar &optionSeparater);

private:
    QHash<QString, bool> m_validOptionTable;
    bool m_hasError;
    QChar m_optionSeparater;

    // Methods
    QHash<QString, bool> validOptionTable(const QString &validOptions);
    bool parseLongOption(const QString &parameter, QHash<QString, QString> &optionTable, QString &lastOption);
    bool isMultiOptionSet(const QString &parameter) const;
    QString setMultiOption(const QString &parameter, QHash<QString, QString> &optionTable);
    bool setOptionAndValue(const QString &parameter, QHash<QString, QString> &optionTable);
};

#endif // OPTIONS_H
