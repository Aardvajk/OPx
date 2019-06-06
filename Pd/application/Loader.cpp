#include "Loader.h"

#include "framework/Error.h"

#include <fstream>

namespace
{

class SegmentWrapper
{
public:
    SegmentWrapper(std::vector<char> &sg) : sg(sg) { }

    void read(char type, InputStream &is);
    std::size_t offset(char){ return 0; }

private:
    std::vector<char> &sg;
};

void SegmentWrapper::read(char type, InputStream &is)
{
    auto bytes = is.get<std::size_t>();

    sg.resize(bytes);
    is.read(sg.data(), bytes);
}

}

Object::Unit Loader::loadObjectUnit(const std::string &path, std::vector<std::vector<char> > &segments)
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
        segments.emplace_back();

        SegmentWrapper sw(segments.back());
        unit.entities.push_back(Object::readEntity(is, pcx::make_callback(&sw, &SegmentWrapper::offset), pcx::make_callback(&sw, &SegmentWrapper::read)));
    }

    return unit;
}
