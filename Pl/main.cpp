#include "application/Context.h"
#include "application/Prologue.h"
#include "application/Generator.h"
#include "application/Linker.h"

#include "framework/Error.h"
#include "framework/Console.h"

#include <pcx/indexed_range.h>

#include <iostream>
#include <fstream>
#include <cstring>

template<typename T> T readAt(const char *data, std::size_t offset)
{
    T t;
    std::memcpy(reinterpret_cast<char*>(&t), data + offset, sizeof(T));

    return t;
}

template<typename T> void writeAt(char *data, const T &t, std::size_t offset)
{
    std::memcpy(data + offset, reinterpret_cast<const char*>(&t), sizeof(T));
}

int main(int argc, char *argv[])
{
    Context c;

    try
    {
        if(argc < 2)
        {
            throw Error("no input specified");
        }

        std::vector<std::string> paths;
        for(int i = 1; i < argc; ++i)
        {
            paths.push_back(argv[i]);
        }

        auto mp = Prologue::generate(c.ds);

        Generator::generate(c, paths);

        std::cout << banner("linker");
        for(auto &u: c.units)
        {
            std::cout << "strings:\n";

            auto w = padw(u.strings.size());
            for(auto s: pcx::indexed_range(u.strings))
            {
                std::cout << "    " << pad(s.index, w) << ": " << s.value << "\n";
            }

            std::cout << "entities:\n";
            for(auto &e: u.entities)
            {
                std::cout << "    " << e.type << " " << u.strings[e.id] << " " << e.offset << "\n";
            }
        }

        std::cout << banner("linking");

        Linker::link(c);

        auto me = c.lookup("main()");
        if(!me)
        {
            throw Error("missing symbol - main()");
        }

        mp.patch(c.ds, c.ds.position() + me->offset);

        auto ds = c.ds.data();
        auto ps = c.ps.data();

        std::vector<char> db(ds.begin(), ds.end());
        std::vector<char> pb(ps.begin(), ps.end());

        if(true)
        {
            std::ofstream os("C:/Projects/Px/Px/out.px", std::ios::binary);
            if(!os.is_open())
            {
                throw Error("unable to create - out.px");
            }

            os.write(db.data(), db.size());
            os.write(pb.data(), pb.size());
        }

        std::system("C:/Projects/Px/Px/build-Pd/release/pd C:/Projects/Px/Px/out.px");
        std::system("C:/Projects/Px/Px/build-Pv/release/pv C:/Projects/Px/Px/out.px");
    }

    catch(const Error &e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return -1;
    }
}
