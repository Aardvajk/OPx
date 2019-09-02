#include "Generator.h"

#include "application/Context.h"

#include "framework/Error.h"
#include "framework/InputStream.h"
#include "framework/Console.h"

#include <pcx/indexed_range.h>

#include <fstream>

namespace
{

class Wrapper
{
public:
    Wrapper(Context &c, std::size_t unit, std::size_t entity) : c(c), unit(unit), entity(entity) { }

    void readData(char type, InputStream &is);

private:
    Context &c;
    std::size_t unit;
    std::size_t entity;
};

void Wrapper::readData(char type, InputStream &is)
{
    if(c.data.size() <= unit)
    {
        c.data.push_back({ });
    }

    if(c.data[unit].size() <= entity)
    {
        c.data[unit].push_back({ });
    }

    auto &sg = c.data[unit][entity];

    auto bytes = is.get<std::size_t>();

    sg.resize(bytes);
    is.read(sg.data(), bytes);
}

}

void Generator::generate(Context &c, const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if(!file.is_open())
    {
        throw Error("unable to open - ", path);
    }

    InputStream is(file);

    c.units.emplace_back(path);
    auto &unit = c.units.back();

    unit.strings = Object::readStringTable(is);

    auto count = is.get<std::size_t>();
    for(std::size_t i = 0; i < count; ++i)
    {
        Wrapper w(c, c.units.size() - 1, unit.entities.size());
        unit.entities.push_back(Object::readEntity(is, c.units.size() - 1, pcx::make_callback(&w, &Wrapper::readData)));
    }

    std::ifstream dmap(path + ".pmap");
    if(dmap.is_open())
    {
        DebugMap in;
        in.read(dmap);

        for(std::size_t i = 0; i < in.size(); ++i)
        {
            switch(in[i].type)
            {
                case 'V': c.vd.push_back(in[i]); break;
                case 'F': c.pd.push_back(in[i]); break;
            }
        }
    }
}
