#include "persistencefactory.h"

PersistenceFactory::PersistenceFactory()
{

}

/**
 * @brief PersistenceFactory::createPersistence
 * @param type
 * @return
 */
Persistence* PersistenceFactory::createPersistence(const PersistenceFactory::Type type)
{
    Persistence* object = nullptr;
    switch (type) {
    case SqlPostgre:
        object = new PostgreSQL();
        break;
    case File:
        object = new FilePersistence();
        break;
    }

    return object;
}
