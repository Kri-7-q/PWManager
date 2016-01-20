#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

/* ------------------------------------------------------------------------------------
 * OptionParser class
 *
 * This class is meant to parse arguments given through the console to an application.
 * It needs a list of option definitions. The parser takes the definition list to
 * parse for the defined options.
 * All found options from a console input are stored in a QHash table with its values.
 * The QHash table keeps the short name of an option (char) as key to its value.
 * There can be defined options with a switch. The definition of such an option keeps
 * a pointer to a boolean. If such an option occures in the console input then the
 * boolean will be switched to true and nothing is inserted in the QHash table.
 *
 * Each console parameter will be checked first to be a long name option.
 * For instance: AppName --file fileName, AppName --file=fileName
 * Long name options allways start with '--'. If the parameter do not start this way
 * it must be something else.
 *
 * If the parameter starts with '-' then it must be a normal option. Or some options.
 * The parse tries to find multiple options. If this fails it takes the first char
 * and the rest as a value.
 * If the option should not take a value even so the value is taken. It will not cause
 * an error but a warning is generated.
 *
 * If a parameter do not start with '--' or '-' then it will be taken as value to
 * the last option. If there is no option which can take this value then an error
 * message is generated. If there is an option but this option should not take a
 * value then even so the value is appended to this option. No error message will
 * be generated but a warning.
 * ------------------------------------------------------------------------------------
 */

#include "optiondefinition.h"
#include <QList>

typedef QHash<char, QVariant> Arguments;

class OptionParser
{
public:
    OptionParser(const QList<OptionDefinition>& definition);

    // Parse method
    Arguments parseOptions(const int argc, char** argv, const int startIndex);

    // Status requests.
    bool hasError() const                   { return !m_errorMessages.isEmpty(); }
    bool hasWarnings() const                { return !m_warningMessages.isEmpty(); }
    QString errorMsg() const                { return m_errorMessages; }
    QString warnings() const                { return m_warningMessages; }

private:
    QHash<char, OptionDefinition> m_definitionTable;
    QString m_errorMessages;
    QString m_warningMessages;

    // Private methods
    QString getLongOptionString(const QString& parameter) const;
    QString getLongOptionValue(const QString& parameter) const;
    OptionDefinition findLongOption(const QString& longOption) const;
    OptionDefinition parseForOptionSet(const QString& parameter, QList<char>& args, QList<char> &needValueList) const;
    void setOption(Arguments& arguments, const OptionDefinition& definition, const QString& valueString = QString()) const;
    void setOption(Arguments &arguments, const QList<char>& optionList) const;
    // Error and warnings.
    void setErrorUnknownOption(const QString& longOption);
    void setErrorValueWithoutOption(const QString& value);
    void setWarningUnexpectedValue(const QString& option, const QString &value);
    void setErrorOptionNeedValue(const QList<char>& optionList);
    void setErrorOptionNeedValue(const char option);
    // Data conversion.
    QVariant convertValueToVariant(const QString& value, const QVariant::Type dataType) const;
    // Initialization
    void initializeDefinitionTable(const QList<OptionDefinition>& definitionList);
};

#endif // OPTIONPARSER_H
