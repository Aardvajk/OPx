#include <iostream>
#include <fstream>
#include <vector>

#include "application/Machine.h"
#include "application/Error.h"

#include "framework/Console.h"

#include "components/Accessor.h"

#include <cstring>
#include <sstream>

std::vector<char> readFile(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    return std::vector<char>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

void intFunc(int code, Memory &mm, Registers &rg)
{
    auto i = rg.sp();
    Accessor::Reader sp(mm(0), i);

    if(code == 0)
    {
        std::cout << "mm: 0\n";
    }
    else if(code == 1)
    {
        OpCode::Reg v;
        sp(v);

        std::cout << toString(v) << ": " << rg[v] << "\n";
    }
    else if(code == 2)
    {
        std::size_t x;
        sp(x);

        std::cout << "size_t: " << x << "\n";
    }
    else if(code == 3)
    {
        int x;
        sp(x);

        std::cout << "int: " << x << "\n";
    }
    else if(code >= 1000)
    {
        std::cout << "hit " << code << "\n";
    }
}

int main(int argc, char *argv[])
{
    try
    {
        if(argc < 2)
        {
            throw Error("no file specified");
        }

        Machine m(readFile(argv[1]), intFunc);
        m.execute();

        std::cout << banner("");
    }

    catch(const Error &e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return -1;
    }
}
