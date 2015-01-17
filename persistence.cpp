#include "persistence.h"

Persistence::Persistence()
{
}

/**
 * Static
 * Translates an option charakter to a database name.
 * @param option
 * @return
 */
QString Persistence::getDatabaseNameFor(const char option)
{
    switch (option) {
    case 'p':
        return "provider";
        break;
    case 'u':
        return "username";
        break;
    case 'i':
        return "id";
        break;
    case 'q':
        return "question";
        break;
    case 'r':
        return "answer";
        break;
    case 'l':
        return "passwordLength";
        break;
    case 's':
        return "definedCharacter";
        break;
    case 'k':
        return "password";
        break;
    default:
        return "";
        break;
    }
}
