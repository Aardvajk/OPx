#include "framework/Error.h"

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

        std::vector<std::string> paths;
        for(int i = 1; i < argc; ++i)
        {
            paths.push_back(argv[i]);
        }
    }

    catch(const Error &e)
    {
        std::cerr << "pl error: " << e.what() << "\n";
        return -1;
    }
}
