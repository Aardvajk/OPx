#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"
#include "application/Process.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main(int argc, char *argv[])
{
    Context c;

    try
    {
        if(argc < 2)
        {
            throw Error("no source specified");
        }

        if(argc > 2)
        {
            std::ifstream is(argv[2]);
            if(!is.is_open())
            {
                throw Error("unable to open - ", argv[2]);
            }

            c.dm.read(is);
        }

        std::string path = argv[1];
        if(path.length() > 4 && path.substr(path.length() - 3) == ".po")
        {
            Process::processUnit(c, path);
        }
        else
        {
            if(c.dm.empty())
            {
                Process::processExe(c, path);
            }
            else
            {
                Process::processMappedExe(c, path);
            }
        }
    }

    catch(const Error &error)
    {
        std::cerr << "pd error: " << error.what() << "\n";
        return -1;
    }
}
