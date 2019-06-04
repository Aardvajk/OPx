#include "framework/Error.h"
#include "framework/Console.h"
#include "framework/LoadBinaryFile.h"

#include "application/Context.h"
#include "application/Loader.h"
#include "application/Process.h"
#include "application/Disassmble.h"

#include <iostream>
#include <vector>
#include <string>

int main(int argc, char *argv[])
{
    Context c;

    try
    {
        if(argc < 2)
        {
            throw Error("no source specified");
        }

        std::string path = argv[1];

        auto ext = path.length() > 3 ? path.substr(path.length() - 3) : std::string();

        if(ext == ".po")
        {
            c.unit = Loader::loadObjectUnit(path, c.segments);

            Process::process(c);
        }
        else
        {
            auto file = loadBinaryFile(path);

            std::cout << banner("disassemble ", path);
            Disassemble::disassemble(c, std::cout, file.data(), file.size());
        }
    }

    catch(const Error &error)
    {
        std::cerr << "pd error: " << error.what() << "\n";
        return -1;
    }
}
