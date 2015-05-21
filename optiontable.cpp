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
    remove('a');
    QList<char> optionList = QList<char>() << 'i' << 'p' << 'u' << 'k' << 'l' << 's' << 'q' << 'r';
    foreach (char option, optionList) {
        if (!contains(option)) {
            insert(option, QVariant());
        }
    }
}

/**
 * @brief OptionTable::insertStandardOptionForShow
 */
void OptionTable::insertStandardOptionForShow()
{
    insert('i', QVariant());
    insert('p', QVariant());
    insert('u', QVariant());
    insert('k', QVariant());
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

