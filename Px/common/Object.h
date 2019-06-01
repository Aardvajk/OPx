#ifndef OBJECT_H
#define OBJECT_H

#include "framework/InputStream.h"

#include <pcx/callback.h>

#include <vector>
#include <string>
#include <iostream>

namespace Object
{

class Link
{
public:
    Link() : address(0), id(0) { }
    Link(std::size_t address, std::size_t id) : address(address), id(id) { }

    std::size_t address;
    std::size_t id;
};

class Entity
{
public:
    Entity(char type, std::size_t id, std::size_t offset);

    char type;
    std::size_t id;
    std::size_t offset;
    std::vector<Link> links;
};

class Unit
{
public:
    Unit();

    std::vector<std::string> strings;
    std::vector<Entity> entities;
};

std::vector<std::string> readStringTable(InputStream &is);
Entity readEntity(InputStream &is, std::size_t offset, pcx::callback<void, std::size_t, InputStream&> read);

}

#endif // OBJECT_H
