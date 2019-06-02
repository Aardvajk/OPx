#include "framework/Error.h"
#include "framework/Console.h"
#include "framework/LoadBinaryFile.h"

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

        for(int i = 1; i < 2; ++i)
        {
            std::string path = argv[i];

            auto ext = path.length() > 3 ? path.substr(path.length() - 3) : std::string();

            if(ext == ".po")
            {
                std::vector<std::vector<char> > sgs;
                auto u = Loader::loadObjectUnit(path, sgs);

                for(std::size_t i = 0; i < u.entities.size(); ++i)
                {
                    auto &e = u.entities[i];

                    std::cout << banner("disassemble ", u.strings[e.id]);

                    switch(e.type)
                    {
                        case 'V': std::cout << pcx::join_str(sgs[i], ",", [](char c){ return pcx::str(int(c)); }) << "\n"; break;
                        case 'F': Disassemble::disassemble(std::cout, sgs[i].data(), sgs[i].size()); break;
                    }
                }
            }
            else
            {
                auto file = loadBinaryFile(path);

                std::cout << banner("disassemble ", path);
                Disassemble::disassemble(std::cout, file.data(), file.size());
            }
        }
    }

    catch(const Error &e)
    {
        std::cerr << "pd error: " << e.what() << "\n";
        return -1;
    }
}
