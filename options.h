#ifndef OPTIONS_H
#define OPTIONS_H

#include "optiondefinition.h"
#include "optiontable.h"
#include <QStringList>


class Options
{
public:
    Options(const QList<OptionDefinition> &definedOptions);
    ~Options();

    bool hasError() const                   { return m_hasError; }
    QString errorMessage() const            { return m_errorMsg; }
    OptionTable parseOptions(const int argc, const char* const *argv, const int start);

private:
    QHash<char, QVariant::Type> m_definedOption;
    QHash<QString, char> m_definedLongOption;
    bool m_hasError;
    QString m_errorMsg;
    QChar m_optionSeparater;

    // Methods
    void setDefinedOptions(const QList<OptionDefinition> &definedOptions);
    bool parseLongOption(const QString &parameter, OptionTable &optionTable, char &lastOption);
    bool isMultiOptionSet(const char *parameter) const;
    char setMultiOption(const char *parameter, OptionTable &optionTable);
    bool setOptionAndValue(const char *parameter, OptionTable &optionTable);
    QVariant valueWithType(const QVariant::Type type, const QString value) const;
    void setErrorMessage(const QString& errorMsg);
};

#endif // OPTIONS_H
