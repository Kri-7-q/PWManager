#ifndef PERSISTENCEFACTORY_H
#define PERSISTENCEFACTORY_H

#include "postgresql.h"
#include "filepersistence.h"

class PersistenceFactory
{
public:
    PersistenceFactory();

    enum Type { SqlPostgre, File };


    static Persistence* createPersistence(const Type type);
};

#endif // PERSISTENCEFACTORY_H
