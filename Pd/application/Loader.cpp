#include "Loader.h"

#include "framework/Error.h"

#include <fstream>

Object::Unit Loader::loadObjectUnit(const std::string &path, std::vector<Segment> &segments)
{
    Object::Unit unit;

    std::ifstream file(path);
    if(!file.is_open())
    {
        throw Error("unable to open - ", path);
    }

    InputStream is(file);

    unit.strings = Object::readStringTable(is);

    auto count = is.get<std::size_t>();
    for(std::size_t i = 0; i < count; ++i)
    {
        segments.emplace_back();
        unit.entities.push_back(Object::readEntity(is, 0, pcx::make_callback(&segments.back(), &Segment::read)));
    }

    return unit;
}
