#ifndef PERSISTENCEFACTORY_H
#define PERSISTENCEFACTORY_H

#include "postgresql.h"
#include "xmlpersistence.h"

class PersistenceFactory
{
public:
    PersistenceFactory();

    enum Type { SqlPostgre, Xml };


    static Persistence* createPersistence(const Type type);
};

#endif // PERSISTENCEFACTORY_H
