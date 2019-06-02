#include "framework/Error.h"
#include "framework/Console.h"
#include "framework/ByteReader.h"
#include "framework/LoadBinaryFile.h"

#include "common/Interrupt.h"

#include "application/Machine.h"

#include <iostream>
#include <fstream>

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

        Machine m(loadBinaryFile(argv[1]), intProc);

        m.execute();

        std::cout << banner("shutdown");
        m.shutdown();

        std::cout << banner("");
    }

    catch(const Error &e)
    {
        std::cerr << "pv error: " << e.what() << "\n";
        return -1;
    }
}
