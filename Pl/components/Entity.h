#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

class Entity
{
public:
    Entity(char type, std::size_t id, std::size_t unit, std::size_t offset);

    char type;

    std::size_t id;
    std::size_t unit;
    std::size_t offset;
};

#endif // ENTITY_H
