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
            throw Error("no output specified");
        }

        std::string output = argv[1];

        if(argc < 3)
        {
            throw Error("no sources specified");
        }

        std::vector<std::string> paths;
        for(int i = 2; i < argc; ++i)
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

        auto me = c.find("main():std.null");
        if(!me)
        {
            throw Error("main():std.null not found");
        }

        std::cout << "linking main():std.null to " << c.ds.position() + me->offset << "\n";
        mp.patch(c.ds, c.ds.position() + me->offset);

        if(true)
        {
            std::ofstream os(output, std::ios::binary);
            if(!os.is_open())
            {
                throw Error("unable to create - ", output);
            }

            auto db = c.ds.data();
            auto pb = c.ps.data();

            os.write(db.data(), std::streamoff(db.size()));
            os.write(pb.data(), std::streamoff(pb.size()));
        }

        if(true)
        {
            auto s = pcx::str(output, ".pmap");

            std::ofstream os(s);
            if(!os.is_open())
            {
                throw Error("unable to create - ", s);
            }

            c.vd.write(os);
            c.pd.write(os);
        }
    }

    catch(const Error &error)
    {
        std::cerr << "pl error: " << error.what() << "\n";
        return -1;
    }
}
