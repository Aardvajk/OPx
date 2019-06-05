#include "Generator.h"

#include "application/Context.h"

#include "framework/Error.h"
#include "framework/InputStream.h"
#include "framework/Console.h"

#include <pcx/indexed_range.h>

#include <fstream>

void Generator::generate(Context &c, const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if(!file.is_open())
    {
        throw Error("unable to open - ", path);
    }

    InputStream is(file);

    c.units.emplace_back();
    auto &unit = c.units.back();

    unit.strings = Object::readStringTable(is);

    auto count = is.get<std::size_t>();
    for(std::size_t i = 0; i < count; ++i)
    {
        unit.entities.push_back(Object::readEntity(is, pcx::make_callback(&c, &Context::offset), pcx::make_callback(&c, &Context::readData)));
    }
}
