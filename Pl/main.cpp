#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"
#include "application/Prologue.h"
#include "application/Generator.h"
#include "application/Linker.h"

#include <pcx/args.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main(int argc, char *argv[])
{
    try
    {
        std::vector<std::string> files;
        pcx::args args(argc, argv, files);

        if(files.size() < 1)
        {
            throw Error("no output specified");
        }

        std::string output = files[0];

        if(files.size() < 2)
        {
            throw Error("no sources specified");
        }

        std::vector<std::string> paths;
        for(std::size_t i = 1; i < files.size(); ++i)
        {
            paths.push_back(files[i]);
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
