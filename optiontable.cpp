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
    for (char option : optionList) {
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

