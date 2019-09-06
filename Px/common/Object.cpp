#include "Object.h"

#include "framework/InputStream.h"

Object::Entity::Entity(char type, Flags flags, std::size_t id, std::size_t offset, std::size_t unit) : type(type), flags(flags), id(id), offset(offset), unit(unit)
{
}

Object::Unit::Unit(std::string path) : path(std::move(path))
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

Object::Entity Object::readEntity(InputStream &is, std::size_t unit)
{
    auto type = is.get<char>();
    auto flags = is.get<std::uint32_t>();
    auto id = is.get<std::size_t>();

    Entity e(type, static_cast<Entity::Flag>(flags), id, 0, unit);

    auto n = is.get<std::size_t>();
    for(std::size_t i = 0; i < n; ++i)
    {
        auto address = is.get<std::size_t>();
        auto id = is.get<std::size_t>();

        e.links.emplace_back(address, id);
    }

    n = is.get<std::size_t>();
    e.data.resize(n);

    is.read(e.data.data(), n);

    return e;
}
