#include "Generator.h"

#include "application/Context.h"

#include "framework/Error.h"
#include "framework/InputStream.h"
#include "framework/Comments.h"

#include <pcx/str.h>
#include <pcx/indexed_range.h>
#include <pcx/optional.h>

#include <iostream>
#include <fstream>
#include <cstring>

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
        for(std::size_t j = 0; j < n; ++j)
        {
            auto a = is.get<std::size_t>();
            auto b = is.get<std::size_t>();

            unit.entities.back().links.push_back(std::make_pair(a, b));
        }

        n = is.get<std::size_t>();

        std::vector<char> v(n);
        is.read(v.data(), n);
        c.ps.write(v.data(), n);
    }
}

class PositionProvider
{
public:
    PositionProvider(Context &c) : c(c), u(0), e(0), b(0) { }

    void setBase(std::size_t unit, std::size_t entity, std::size_t base){ u = unit; e = entity; b = base; }

    std::size_t position() const { return c.ds.position() + c.units[u].entities[e].offset + b; }

private:
    Context &c;
    std::size_t u, e, b;
};

}

void Generator::generate(Context &c, const std::vector<std::string> &paths)
{
    for(auto &path: paths)
    {
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

Comments Generator::comments(Context &c, const std::vector<std::string> &paths)
{
    PositionProvider pp(c);
    Comments out(pcx::make_callback(&pp, &PositionProvider::position));

    for(auto path: pcx::indexed_range(paths))
    {
        Comments cm(pcx::str(path.value, ".pmap"));
        if(!cm.failed())
        {
            pcx::optional<std::size_t> last;

            std::size_t entity = 0;
            for(std::size_t i = 0; i < cm.size(); ++i)
            {
                if(last && cm[i].position < *last)
                {
                    ++entity;
                }

                last = cm[i].position;

                pp.setBase(path.index, entity, cm[i].position);
                out(Comments::Bare, cm[i].text);
            }
        }
    }

    return out;
}
