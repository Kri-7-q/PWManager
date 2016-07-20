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
    Persistence* persistence = NULL;

    switch (type) {
    case SqlPostgre:
        persistence = new PostgreSQL();
        break;
    case Xml:
        persistence = new XmlPersistence();
        break;
    default:
        break;
    }

    return persistence;
}
