#include "optiondefinition.h"

/**
 * Standard constructor
 */
OptionDefinition::OptionDefinition() :
    m_option(0),
    m_dataType(QVariant::Invalid),
    m_optionType(InvalidDefinition),
    m_pSwitch(NULL)
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
    m_pSwitch(NULL),
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
    m_pSwitch(pSwitch),
    m_longOption(longOption),
    m_optionType(SwitchOn),
    m_dataType(QVariant::Invalid)
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

