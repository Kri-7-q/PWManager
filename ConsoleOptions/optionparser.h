#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

#include "optiondefinition.h"
#include <QHash>

typedef QHash<char, QVariant> OptionTable;

class OptionParser
{
public:
    OptionParser(const QList<OptionDefinition>& definitionList, const quint8 requiredFreeArgs = 0, const quint8 freeArgsAllowed = 0);

    enum ParseState { None, Option, LongOption, Argument, OptionalArgument, NonOptArgument };

    OptionTable parseParameter(const int argc, const char* const argv[], const int start = 1);
    bool hasError() const                           { return ! m_errorMsg.isEmpty(); }
    QString errorMsg() const                        { return m_errorMsg; }

protected:
    ParseState stateFromParameter(const char* const param, const OptionDefinition &lastOption) const;
    OptionDefinition definitionOf(const char symbol) const;
    OptionDefinition definitionOf(const QString& name) const;
    bool parseOptions(const char* const param, OptionTable& optionTable, OptionDefinition &last, char& wrong) const;
    bool parseLongOption(const char* const param, OptionTable& optionTable, OptionDefinition& last) const;
    QString longOptionName(const QString &param) const;
    QString longOptionArgument(const QString &param) const;
    void setErrorMsg(const QString& msg);
    void checkFreeArgumentList(const OptionTable& optionTable);

private:
    const QList<OptionDefinition>& m_definitionList;
    QString m_errorMsg;
    quint8 m_requireFreeArgs;
    quint8 m_freeArgsAllowed;
};

#endif // OPTIONPARSER_H
