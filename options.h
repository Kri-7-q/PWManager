#ifndef OPTIONS_H
#define OPTIONS_H

#include "optiondefinition.h"
#include <QStringList>
#include <QHash>

typedef QHash<char, QVariant> OptionTable;

class Options
{
public:
    Options(const QList<OptionDefinition> &validOptions);
    ~Options();

    bool hasError() const                   { return m_hasError; }
    QString errorMessage() const            { return m_errorMsg; }
    OptionTable parseOptions(const int argc, const char* const *argv, const int start);

private:
    QHash<char, QVariant::Type> m_validOption;
    QHash<QString, char> m_validLongOption;
    bool m_hasError;
    QString m_errorMsg;
    QChar m_optionSeparater;

    // Methods
    void setValidOptions(const QList<OptionDefinition> &validOptions);
    bool parseLongOption(const QString &parameter, OptionTable &optionTable, char &lastOption);
    bool isMultiOptionSet(const char *parameter) const;
    char setMultiOption(const char *parameter, OptionTable &optionTable);
    bool setOptionAndValue(const char *parameter, OptionTable &optionTable);
    QVariant valueWithType(const QVariant::Type type, const QString value) const ;
};

#endif // OPTIONS_H
