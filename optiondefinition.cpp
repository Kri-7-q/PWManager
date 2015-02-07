#include "optiondefinition.h"

/**
 * Constructor
 * @param option
 * @param dataType
 * @param longName
 */
OptionDefinition::OptionDefinition(const char option, const QVariant::Type dataType, const QString &longName) :
    m_option(option),
    m_dataType(dataType),
    m_longName(longName)
{

}

OptionDefinition::~OptionDefinition()
{

}

