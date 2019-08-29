#include "framework/Error.h"
#include "framework/Console.h"
#include "framework/ByteReader.h"
#include "framework/LoadBinaryFile.h"

#include "application/Machine.h"

#include <pcx/args.h>

#include <iostream>
#include <fstream>

struct SilentAbort { };

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
        std::cout << "bool " << (i ? "true" : "false") << "\n";
    }
    else if(code == 3)
    {
        auto i = sr.get<std::size_t>();
        std::cout << mm(i);
    }
    else if(code == 999)
    {
        throw SilentAbort();
    }
    else if(code >= 1000)
    {
        auto i = sr.get<std::size_t>();
        std::cout << code << ": " << i << "\n";
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

    catch(const SilentAbort&)
    {
        return -1;
    }
}
