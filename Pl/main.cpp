#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"
#include "application/Prologue.h"
#include "application/Generator.h"
#include "application/Linker.h"

#include <iostream>

#include <vector>
#include <string>
#include <fstream>

int main(int argc, char *argv[])
{
    try
    {
        if(argc < 2)
        {
            throw Error("no source specified");
        }

        std::vector<std::string> paths;
        for(int i = 1; i < argc; ++i)
        {
            paths.push_back(argv[i]);
        }

        Context c;

        auto mp = Prologue::generate(c);

        for(auto &p: paths)
        {
            Generator::generate(c, p);
        }

        Linker::link(c);

        auto me = c.find("main()");
        if(!me)
        {
            throw Error("main() not found");
        }

        std::cout << "linking main() to " << c.ds.position() + me->offset << "\n";
        mp.patch(c.ds, c.ds.position() + me->offset);

        if(true)
        {
            std::ofstream os("C:/Projects/Px/Px/out.px", std::ios::binary);
            if(!os.is_open())
            {
                throw Error("unable to create - out.px");
            }

            auto db = c.ds.data();
            auto pb = c.ps.data();

            os.write(db.data(), std::streamoff(db.size()));
            os.write(pb.data(), std::streamoff(pb.size()));
        }

        checked_system("C:/Projects/Px/Px/build-Pv/release/pv C:/Projects/Px/Px/out.px");
    }

    catch(const Error &error)
    {
        std::cerr << "pl error: " << error.what() << "\n";
        return -1;
    }
}
