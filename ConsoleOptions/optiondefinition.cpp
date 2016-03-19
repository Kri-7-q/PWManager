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
 * @param dataType          A QVariant data type in witch a value is converted.
 * @param longOption        Optional. If option additional should be a word.
 */
OptionDefinition::OptionDefinition(const char option, const OptionType optionArgument,
                                   const QVariant::Type dataType, const QString &longOption) :
    m_option(option),
    m_dataType(dataType),
    m_longOption(longOption),
    m_optionType(optionArgument),
    m_pSwitch(NULL)
{

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

