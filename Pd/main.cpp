#include "common/OpCode.h"

#include "framework/Error.h"
#include "framework/InputStream.h"
#include "framework/ByteReader.h"
#include "framework/Console.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

#include <fstream>
#include <iostream>
#include <vector>

std::vector<char> readFile(const std::string &path)
{
    std::ifstream is(path, std::ios::binary);
    if(!is.is_open())
    {
        throw Error("unable to open - ", path);
    }

    return { std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>() };
}

std::string byteString(ByteReader &rm, std::size_t n)
{
    std::vector<std::string> v;
    for(std::size_t i = 0; i < n; ++i)
    {
        v.push_back(pcx::str(int(rm.get<char>())));
    }

    return pcx::join_str(v, ",");
}

int main(int argc, char *argv[])
{
    try
    {
        if(argc < 2)
        {
            throw Error("no input specified");
        }

        auto v = readFile(argv[1]);

        std::cout << banner("disassembly");

        std::size_t pc = 0;
        ByteReader rm(v.data(), pc);

        using namespace OpCode;

        Op op;
        Reg r0, r1;
        std::size_t s0;
        int i0;

        while(pc < v.size())
        {
            std::cout << pad(pcx::str(pc), 4) << ": ";

            rm(op);
            std::cout << pad(toString(op), 8) << " ";

            switch(op)
            {
                case Op::AddRI:
                case Op::SubRI: rm(r0, s0); std::cout << toString(r0) << " " << s0; break;

                case Op::CopyRR:
                case Op::CopyRA:
                case Op::CopyAR: rm(r0, r1); std::cout << toString(r0) << " " << toString(r1); break;

                case Op::CopyAI: rm(r0, s0); std::cout << toString(r0) << " " << s0 << " " << byteString(rm, s0); break;

                case Op::PushR:
                case Op::PopR:
                case Op::Call: rm(r0); std::cout << toString(r0); break;

                case Op::Ret: rm(s0); std::cout << s0; break;

                case Op::Int: rm(i0); std::cout << i0; break;

                case Op::End: break;

                default: throw Error("invalid opcode");
            }

            std::cout << "\n";
        }
    }

    catch(const Error &e)
    {
        std::cerr << "error: " << e.what() << "\n";
    }
}
