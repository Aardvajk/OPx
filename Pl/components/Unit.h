#ifndef UNIT_H
#define UNIT_H

#include "components/Entity.h"

#include <string>

class Unit
{
public:
    Unit();

    std::vector<std::string> strings;
    std::vector<Entity> entities;
};

#endif // UNIT_H
