#include "optiontable.h"

OptionTable::OptionTable()
{

}

OptionTable::~OptionTable()
{

}

/**
 * @brief OptionTable::replaceOptionA
 */
void OptionTable::replaceOptionA()
{
    if (! contains('a')) {
        return;
    }
    remove('a');
    QList<char> optionList = QList<char>() << 'i' << 'p' << 'u' << 'k' << 'l' << 's' << 'q' << 'r';
    foreach (char option, optionList) {
        if (!contains(option)) {
            insert(option, QVariant());
        }
    }
}

/**
 * Insert ALL options whitout value.
 */
void OptionTable::replaceOptionE()
{
    if (! contains('e')) {
        return;
    }
    clear();
    QList<char> optionList = QList<char>() << 'i' << 'p' << 'u' << 'k' << 'l' << 's' << 'q' << 'r' << 't';
    setOptions(optionList);
}

/**
 * Insert a set of statandard options without a value.
 */
void OptionTable::insertStandardOptions()
{
    QList<char> optionList = QList<char>() << 'i' << 'p' << 'u' << 'k';
    setOptions(optionList);
}

/**
 * Tests if OptionTable has a valid value for a key.
 * @param key
 * @return
 */
bool OptionTable::hasValueForKey(const char key) const
{
    return value(key).isValid();
}

/**
 * Tests if OptionTable has valid values for a set of keys.
 * @param keySet
 * @return
 */
bool OptionTable::hasValuesForKeySet(const QByteArray &keySet) const
{
    foreach (char key, keySet) {
        if (!value(key).isValid()) {
            return false;
        }
    }

    return true;
}

/**
 * Insert all options of list without a value.
 * @param optionList
 */
void OptionTable::setOptions(const QList<char> &optionList)
{
    foreach (char option, optionList) {
        insert(option, QVariant());
    }
}

