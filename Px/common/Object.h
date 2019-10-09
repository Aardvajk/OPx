#ifndef OBJECT_H
#define OBJECT_H

#include "framework/InputStream.h"

#include <pcx/flags.h>

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
    enum class Flag : std::uint32_t
    {
        AutoGen = 1,
        GlobalInit = 2
    };

    using Flags = pcx::flags<Flag>;

    Entity(char type, Flags flags, std::size_t id, std::size_t offset, std::size_t unit);

    char type;
    Flags flags;
    std::size_t id;
    std::size_t offset;
    std::size_t unit;
    std::vector<Link> links;
    std::vector<char> data;
};

class Unit
{
public:
    Unit(std::string path = { });

    std::string path;
    std::vector<std::string> strings;
    std::vector<Entity> entities;
};

std::vector<std::string> readStringTable(InputStream &is);
Entity readEntity(InputStream &is, std::size_t unit);

}

template<> struct pcx_is_flag_enum<Object::Entity::Flag> : std::true_type { };

#endif // OBJECT_H
