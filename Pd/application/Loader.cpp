#include "Loader.h"

#include "framework/Error.h"

#include <fstream>

Object::Unit Loader::loadObjectUnit(const std::string &path)
{
    Object::Unit unit(path);

    std::ifstream file(path, std::ios::binary);
    if(!file.is_open())
    {
        throw Error("unable to open - ", path);
    }

    InputStream is(file);

    unit.strings = Object::readStringTable(is);

    auto count = is.get<std::size_t>();
    for(std::size_t i = 0; i < count; ++i)
    {
        unit.entities.push_back(Object::readEntity(is, 0));
    }

    return unit;
}
