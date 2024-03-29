#include "optiondefinition.h"

/**
 * Standard constructor
 */
OptionDefinition::OptionDefinition() :
    m_option(0),
    m_dataType(QVariant::Invalid),
    m_optionType(InvalidDefinition),
    m_pSwitch(nullptr)
{

}

/**
 * Constructs a initialized OptionDefinition object.
 * @param option            The option character.
 * @param longOption        Optional. If option additional should be a word.
 * @param optionArgument    NeedArgument, NoArgument, SwitchOn and OptionalArgument.
 * @param dataType          A QVariant data type in witch a value is converted.
 * @param formatString      A string to describe how a value should be parsed. QDate, QDateTime and QTime.
 * @param longOption        A long name for a option.
 */
OptionDefinition::OptionDefinition(const char option, const OptionType optionArgument,
                                   const QVariant::Type dataType, const QString &formatString, const QString &longOption) :
    m_option(option),
    m_dataType(dataType),
    m_longOption(longOption),
    m_optionType(optionArgument),
    m_pSwitch(nullptr),
    m_formatString(formatString)
{
    if (formatString.isEmpty()) {
        switch (dataType) {
        case QVariant::Date:
            m_formatString = QString("d-M-yyyy");
            break;
        case QVariant::DateTime:
            m_formatString = QString("d-M-yyyy+h:m");
            break;
        case QVariant::Time:
            m_formatString = QString("h:m");
            break;
        default:
            break;
        }
    }
}

/**
 * Constructs a initialized OptionDefinition object.
 * This definition takes a pointer to a boolean to switch it on (to true)
 * if this option is chosen from the user.
 * @param option
 * @param pSwitch
 * @param longOption
 */
OptionDefinition::OptionDefinition(const char option, bool *pSwitch, const QString &longOption) :
    m_option(option),
    m_dataType(QVariant::Invalid),
    m_longOption(longOption),
    m_optionType(SwitchOn),
    m_pSwitch(pSwitch)
{

}

/**
 * @brief OptionDefinition::convertValue
 * @param stringValue
 * @return
 */
QVariant OptionDefinition::convertValue(const QString &stringValue) const
{
    QVariant value(QVariant::Invalid);
    switch (m_dataType) {
    case QVariant::Bool:
        value.setValue(convertToBool(stringValue));
        break;
    case QVariant::Date:
        value.setValue(convertToDate(stringValue));
        break;
    case QVariant::DateTime:
        value.setValue(convertToDateTime(stringValue));
        break;
    case QVariant::Double:
        value.setValue(convertToDouble(stringValue));
        break;
    case QVariant::Int:
        value.setValue(convertToInt(stringValue));
        break;
    case QVariant::LongLong:
        value.setValue(convertToLongLong(stringValue));
        break;
    case QVariant::String:
        value.setValue(stringValue);
        break;
    case QVariant::Time:
        value.setValue(convertToTime(stringValue));
        break;
    case QVariant::UInt:
        value.setValue(convertToUInt(stringValue));
        break;
    case QVariant::ULongLong:
        value.setValue(convertToULongLong(stringValue));
        break;
    default:
        break;
    }

    return value;
}

/**
 * Add a line of help text for this option.
 * @param line      String with a line of help text. Without endline symbol.
 * @return this     A reference of this OptionDefinition object.
 */
OptionDefinition& OptionDefinition::addHelpTextLine(const QString &line)
{
    m_helpTextLines << line;

    return *this;
}

/**
 * Set a string list with help text. A list of text lines.
 * @param helpText          A list of text lines.
 * @return this             Reference of this OptionDefinition object.
 */
OptionDefinition &OptionDefinition::setHelpTextLines(const QStringList &helpText)
{
    m_helpTextLines = helpText;

    return *this;
}

/**
 * Constructs a help text to this option.
 * @return text         A String with multiple lines of help text.
 */
QString OptionDefinition::helpText() const
{
    QString text;
    int line = 1;
    text.append( helpTextOptionWithArgument(QString("value")) ).append( helpTextLine(0) );
    if (! m_longOption.isEmpty()) {
        text.append( helpTextOptionWithArgument(QString("value"), true) ).append( helpTextLine(1) );
        ++line;
    }
    for (; line<m_helpTextLines.size(); ++line) {
        text.append( QString(m_maxLength, QChar(' ')) ).append( helpTextLine(line) );
    }

    return text;
}

/**
 * Virtual
 * Converts a string value to a boolean value.
 * Recognized strings are '1', 't', 'T', 'ture' and 'True'. These strings will
 * lead to a return value of true. All other content in the string
 * will lead to a return value of false.
 * @param stringValue
 * @return
 */
bool OptionDefinition::convertToBool(const QString &stringValue) const
{
    QString string = stringValue.toLower();
    if (string == QString("1")) {
        return true;
    }
    if (string == QString("t")) {
        return true;
    }
    if (string == QString("true")) {
        return true;
    }

    return false;
}

/**
 * Convert a string value to a QDate value.
 * A string like 3-5-2016 will lead to a date of '3.March 2016'.
 * Warning: A string like 3-5-16 will lead to a date of '3.March 1916'.
 * @param stringValue
 * @return A valid date if possible. Otherwise a invalid date.
 */
QDate OptionDefinition::convertToDate(const QString &stringValue) const
{
    return QDate::fromString(stringValue, m_formatString);
}

/**
 * Converts a string value to a QDateTime value.
 * The string must have a specific format. It must start with the number of day in
 * month. Next if the number of month and than the number of year with four digits.
 * All seperated with a '-'. The date and time value must be seperated with a '+'.
 * Than follows the houre and minute seperated with a colon.
 * E.g. 13-7-2016+14:30     13.July 2016 at 2:30 p.m.
 * @param stringValue
 * @return                  A valid QDateTime object if possible.
 */
QDateTime OptionDefinition::convertToDateTime(const QString &stringValue) const
{
    return QDateTime::fromString(stringValue, m_formatString);
}

/**
 * Converts a string value to a double value.
 * The toDouble() function of QString is used for this task.
 * @param stringValue
 * @return
 */
double OptionDefinition::convertToDouble(const QString &stringValue) const
{
    return stringValue.toDouble();
}

/**
 * Converts a string value to an int value.
 * The toInt() function of QString is used for this task.
 * @param stringValue
 * @return
 */
int OptionDefinition::convertToInt(const QString &stringValue) const
{
    return stringValue.toInt();
}

/**
 * Converts a string value to a qlonglong value.
 * The toLongLong() function of QString is used for this task.
 * @param stringValue
 * @return
 */
qlonglong OptionDefinition::convertToLongLong(const QString &stringValue) const
{
    return stringValue.toLongLong();
}

QTime OptionDefinition::convertToTime(const QString &stringValue) const
{
    return QTime::fromString(stringValue, m_formatString);
}

/**
 * Converts a string value to a uint value.
 * The toUInt() function of QString is used for this task.
 * @param stringValue
 * @return
 */
uint OptionDefinition::convertToUInt(const QString &stringValue) const
{
    return stringValue.toUInt();
}

/**
 * Converts a string value to a qulonglong value.
 * The toULongLong() function of QString is used for this task.
 * @param stringValue
 * @return
 */
qulonglong OptionDefinition::convertToULongLong(const QString &stringValue) const
{
    return stringValue.toULongLong();
}

/**
 * Get help string of option and long option with option argument. String will be of length 35.
 * For instance:    -i <value>      or      --index [value]
 * @param name          The name of options argument for help text.
 * @return argument     Argument description for help text.
 */
QString OptionDefinition::helpTextOptionWithArgument(const QString &name, const bool longOption) const
{
    QString argument('-');
    if (longOption) {
        argument.append('-').append(m_longOption);
    } else {
        argument.append(m_option);
    }
    argument.append(' ');
    switch (m_optionType) {
    case NeedArgument:
        argument.append('<').append(name).append('>');
        break;
    case OptionalArgument:
        argument.append('[').append(name).append(']');
        break;
    default:
        break;
    }
    if (argument.length() < m_maxLength) {
        argument.append(QString(m_maxLength - argument.length(), QChar(' ')));
    }

    return argument;
}

/**
 * Get a line of option help text. If there is no more line an empty string is returned.
 * @param line          The line of help text list.
 * @return
 */
QString OptionDefinition::helpTextLine(const int line) const
{
    if (line >= m_helpTextLines.size()) {
        return QString('\n');
    }

    return m_helpTextLines[line];
}
