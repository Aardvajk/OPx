#include "framework/Error.h"
#include "framework/Console.h"
#include "framework/ByteReader.h"
#include "framework/LoadBinaryFile.h"

#include "application/Machine.h"

#include <pcx/args.h>

#include <iostream>
#include <fstream>

void intProc(int code, Memory &mm, Registers &rg)
{
    auto sp = rg.sp();
    ByteReader sr(mm(0), sp);

    if(code == 1)
    {
        auto i = sr.get<int>();
        std::cout << "integer " << i << "\n";
    }
    else if(code == 2)
    {
        auto i = sr.get<char>();
        std::cout << "char " << i << "\n";
    }
    else if(code == 3)
    {
        auto i = sr.get<std::size_t>();
        std::cout << "size " << i << "\n";
    }
    else if(code == 4)
    {
        auto i = sr.get<char>();
        std::cout << "bool " << (i ? "true" : "false") << "\n";
    }
    else if(code == 5)
    {
        auto i = sr.get<std::size_t>();
        auto p = mm(i);

        std::cout << p;
    }
    else if(code == 10)
    {
        auto i = sr.get<std::size_t>();
        auto p = mm(i);

        std::cout << "ptr to int " << i << " value = " << *(reinterpret_cast<const int*>(p)) << "\n";
    }
    else if(code == 11)
    {
        auto i = sr.get<std::size_t>();

        auto p = mm(i);

        std::size_t bp;
        std::memcpy(&bp, p, sizeof(std::size_t));

        auto p2 = mm(bp);

        std::cout << "ptr to ptr to int " << i << " value = " << *(reinterpret_cast<const int*>(p2)) << "\n";
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
        std::vector<std::string> files;
        pcx::args args(argc, argv, files);

        if(files.size() < 1)
        {
            throw Error("no program specified");
        }

        bool quiet = args.contains("q");

        if(!quiet)
        {
            std::cout << banner("executing");
        }

        Machine m(loadBinaryFile(files[0]), intProc);

        m.execute();

        if(!quiet)
        {
            std::cout << banner("shutdown");
        }

        m.shutdown(quiet);

        if(!quiet)
        {
            std::cout << banner();
        }
    }

    catch(const Error &error)
    {
        std::cerr << "pv error: " << error.what() << "\n";
        return -1;
    }
}
