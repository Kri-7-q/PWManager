#include "persistence.h"

/**
 * Constructor
 */
Persistence::Persistence() :
    m_isOpen(false)
{

}

/**
 * Virtual
 * Destructor
 */
Persistence::~Persistence()
{

}

/**
 * Virtual public
 * Returns if the persistence is open to read and write or not.
 * Method 'setOpen(bool)' must be used in 'open()' and 'close()'
 * otherwise this methode returns allways false;
 * @return          True if persistence is open. Otherwise false.
 */
bool Persistence::isOpen() const
{
    return m_isOpen;
}
