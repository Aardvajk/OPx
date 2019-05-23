#include "Generator.h"

#include "application/Context.h"
#include "application/Error.h"

#include "components/InputStream.h"

#include <iostream>
#include <fstream>

namespace
{

void processUnit(Context &c, Unit &unit, InputStream &is)
{
    auto n = is.get<std::size_t>();
    for(std::size_t i = 0; i < n; ++i)
    {
        unit.strings.push_back(is.get<std::string>());
    }

    n = is.get<std::size_t>();
    for(std::size_t i = 0; i < n; ++i)
    {
        auto type = is.get<char>();
        auto id = is.get<std::size_t>();

        unit.entities.push_back(Entity(type, id, c.units.size(), c.ps.position()));

        auto n = is.get<std::size_t>();

        std::vector<char> v(n);
        is.read(v.data(), n);
        c.ps.write(v.data(), n);
    }
}

}

void Generator::generate(Context &c, const std::vector<std::string> &paths)
{
    for(auto &path: paths)
    {
        std::cout << "generating " << path << "\n";

        std::ifstream fs(path, std::ios::binary);
        if(!fs.is_open())
        {
            throw Error("unable to open - ", path);
        }

        c.units.push_back({ });

        InputStream is(fs);
        processUnit(c, c.units.back(), is);
    }
}
