#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Machine.h"

#include <iostream>
#include <fstream>

std::vector<char> readFile(const std::string &path)
{
    std::ifstream is(path, std::ios::binary);
    if(!is.is_open())
    {
        throw Error("unable to open - ", path);
    }

    return { std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>() };
}

int main(int argc, char *argv[])
{
    try
    {
//        if(argc < 2)
//        {
//            throw Error("no program specified");
//        }

        std::cout << banner("executing");

        Machine m(readFile("C:/Projects/Px/Px/out.px"));

        m.execute();

        std::cout << banner("shutdown");
        m.shutdown();

        std::cout << banner("");
    }

    catch(const Error &e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return -1;
    }
}
