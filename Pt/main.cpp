#include <iostream>

#include "application/TextFile.h"
#include "application/Test.h"

int main(int argc, char *argv[])
{
    TextFile config("pt.txt");
    if(config.failed())
    {
        std::cerr << "pt error: unable to find pt.txt\n";
        return -1;
    }

    for(std::size_t i = 2; i < config.size(); ++i)
    {
        TextFile file(config[i]);
        if(file.failed())
        {
            std::cerr << "pt error: unable to open - " << config[i] << "\n";
            return -1;
        }

        if(!runTest(config[0], config[1], file))
        {
            return -1;
        }
    }
}
