#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <QString>


class Persistence
{
public:
    Persistence();

    // Static function
    static QString getDatabaseNameFor(const char option);
};

#endif // PERSISTENCE_H
