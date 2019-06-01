#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Loader.h"
#include "application/Disassmble.h"

#include <pcx/join_str.h>
#include <pcx/str.h>

#include <iostream>
#include <vector>
#include <string>

int main(int argc, char *argv[])
{
    try
    {
        if(argc < 2)
        {
            throw Error("no source specified");
        }

        for(int i = 1; i < argc; ++i)
        {
            auto path = argv[i];

            std::vector<Segment> sgs;
            auto u = Loader::loadObjectUnit(path, sgs);

            for(std::size_t i = 0; i < u.entities.size(); ++i)
            {
                auto &e = u.entities[i];

                std::cout << banner(u.strings[e.id]);

                if(e.type == 'V')
                {
                    std::cout << pcx::join_str(sgs[i], ",", [](char c){ return pcx::str(int(c)); }) << "\n";
                }
                else if(e.type == 'F')
                {
                    Disassemble::disassemble(std::cout, sgs[i].data(), sgs[i].size());
                }
            }
        }

        std::cout << banner("");
    }

    catch(const Error &e)
    {
        std::cerr << "pd error: " << e.what() << "\n";
        return -1;
    }
}
