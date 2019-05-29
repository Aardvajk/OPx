#include "framework/Error.h"
#include "framework/Console.h"
#include "framework/ByteReader.h"

#include "common/Interrupt.h"

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

void intProc(int code, Memory &mm, Registers &rg)
{
    auto sp = rg.sp();
    ByteReader sr(mm(0), sp);

    if(code == Interrupt::PrintInt)
    {
        auto i = sr.get<int>();
        std::cout << "integer " << i << "\n";
    }
    else if(code >= 1000)
    {
        std::cout << "int " << code << "\n";
    }
}

int main(int argc, char *argv[])
{
    try
    {
        if(argc < 2)
        {
            throw Error("no program specified");
        }

        std::cout << banner("executing");

        Machine m(readFile(argv[1]), intProc);

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
