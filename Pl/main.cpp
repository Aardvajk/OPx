#include "application/Context.h"
#include "application/Error.h"
#include "application/Prologue.h"
#include "application/Generator.h"

#include "framework/Console.h"

#include <pcx/indexed_range.h>

#include <iostream>
#include <fstream>

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

            for(auto &e: u.entities)
            {
                std::cout << "entities:\n";
                std::cout << "    " << e.type << " " << u.strings[e.id] << " " << e.offset << "\n";
            }
        }

        auto me = c.lookup("main()");
        if(!me)
        {
            throw Error("missing symbol - main()");
        }

        mp.patch(c.ds, c.ds.position() + me->offset);

        if(true)
        {
            std::ofstream os("C:/Projects/Px/Px/out.px", std::ios::binary);
            if(!os.is_open())
            {
                throw Error("unable to create - out.px");
            }

            os.write(c.ds.data().c_str(), c.ds.position());
            os.write(c.ps.data().c_str(), c.ps.position());
        }

        std::cout << banner("launching pv");
        std::system("C:/Projects/Px/Px/build-Pv/release/pv C:/Projects/Px/Px/out.px");
    }

    catch(const Error &e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return -1;
    }
}
