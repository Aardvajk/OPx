#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"
#include "application/Process.h"

#include <pcx/args.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main(int argc, char *argv[])
{
    Context c;

    try
    {
        std::vector<std::string> files;
        pcx::args args(argc, argv, files);

        if(files.size() < 1)
        {
            throw Error("no source specified");
        }

        std::string path = files[0];

        if(true)
        {
            std::ifstream is(path + ".pmap");
            if(is.is_open())
            {
                c.dm.read(is);
            }
        }

        if(path.length() > 4 && path.substr(path.length() - 3) == ".po")
        {
            Process::processUnit(c, path);
        }
        else if(path.length() > 6 && path.substr(path.length() - 5) == ".praw")
        {
            Process::processExe(c, path);
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
