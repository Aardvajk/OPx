#include "Object.h"

#include "framework/InputStream.h"

Object::Entity::Entity(char type, std::size_t id, std::size_t offset) : type(type), id(id), offset(offset)
{
}

Object::Unit::Unit()
{
}

std::vector<std::string> Object::readStringTable(InputStream &is)
{
    std::vector<std::string> v;

    auto n = is.get<std::size_t>();
    for(std::size_t i = 0; i < n; ++i)
    {
        v.push_back(is.get<std::string>());
    }

    return v;
}

Object::Entity Object::readEntity(InputStream &is, std::size_t offset, pcx::callback<void, std::size_t, InputStream&> read)
{
    auto type = is.get<char>();
    auto id = is.get<std::size_t>();

    Entity e(type, id, offset);

    auto n = is.get<std::size_t>();
    for(std::size_t i = 0; i < n; ++i)
    {
        auto address = is.get<std::size_t>();
        auto id = is.get<std::size_t>();

        e.links.emplace_back(address, id);
    }

    auto bytes = is.get<std::size_t>();
    read(bytes, is);

    return e;
}
