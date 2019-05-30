#include "common/OpCode.h"

#include "framework/Error.h"
#include "framework/InputStream.h"
#include "framework/ByteReader.h"
#include "framework/Console.h"

#include <pcx/str.h>
#include <pcx/join_str.h>
#include <pcx/lexical_cast.h>

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

std::vector<std::pair<std::size_t, std::string> > loadComments(const std::string &path)
{
    std::vector<std::pair<std::size_t, std::string> > comments;

    std::ifstream file(path);
    if(!file.is_open())
    {
        throw Error("unable to open - ", path);
    }

    std::string line;
    while(std::getline(file, line))
    {
        std::string no, tx;
        bool getting = true;
        for(std::size_t i = 0; i < line.length(); ++i)
        {
            if(std::isdigit(line[i]) && getting)
            {
                no += line[i];
            }
            else if(getting)
            {
                getting = false;
            }
            else
            {
                tx += line[i];
            }
        }

        if(!no.empty())
        {
            comments.push_back(std::make_pair(pcx::lexical_cast<std::size_t>(no), tx));
        }
    }

    return comments;
}

void dumpComment(const std::vector<std::pair<std::size_t, std::string> > &cv, std::size_t &ci, std::size_t pc)
{
    if(ci < cv.size())
    {
        while(cv[ci].first == pc)
        {
            std::cout << cv[ci].second << "\n";
            ++ci;
        }
    }
}

void disassemble(const std::vector<char> &data, const std::vector<std::pair<std::size_t, std::string> > &cv, std::size_t &ci)
{
    std::size_t pc = 0;
    ByteReader rm(data.data(), pc);

    using namespace OpCode;

    Op op;
    Reg r0, r1;
    std::size_t s0;
    int i0;

    while(pc < data.size())
    {
        dumpComment(cv, ci, pc);

        std::cout << pad(pcx::str(pc), 4) << ": ";

        rm(op);
        std::cout << pad(toString(op), 8) << " ";

        switch(op)
        {
            case Op::SetRI: rm(r0); std::cout << toString(r0) << " " << byteString(rm, sizeof(std::size_t)); break;

            case Op::AddRI:
            case Op::SubRI: rm(r0, s0); std::cout << toString(r0) << " " << s0; break;

            case Op::CopyRR:
            case Op::CopyRA:
            case Op::CopyAR: rm(r0, r1); std::cout << toString(r0) << " " << toString(r1); break;

            case Op::CopyAI: rm(r0, s0); std::cout << toString(r0) << " " << s0 << " " << byteString(rm, s0); break;

            case Op::CopyAA: rm(r0, r1, s0); std::cout << toString(r0) << " " << toString(r1) << " " << s0; break;

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

    dumpComment(cv, ci, pc);
}

int main(int argc, char *argv[])
{
    try
    {
        if(argc < 2)
        {
            throw Error("no input specified");
        }

        std::string path = argv[1];

        std::vector<std::pair<std::size_t, std::string> > comments;
        if(argc > 2)
        {
            comments = loadComments(argv[2]);
        }

        if(path.length() > 3 && path.substr(path.length() - 3) == ".po")
        {
            std::ifstream file(path, std::ios::binary);
            if(!file.is_open())
            {
                throw Error("unable to open - ", path);
            }

            InputStream is(file);
            std::vector<std::string> strings;

            auto n = is.get<std::size_t>();
            for(std::size_t i = 0; i < n; ++i)
            {
                strings.push_back(is.get<std::string>());
            }

            std::size_t ci = 0;

            n = is.get<std::size_t>();
            for(std::size_t i = 0; i < n; ++i)
            {
                auto type = is.get<char>();
                auto id = is.get<std::size_t>();

                std::vector<std::pair<std::size_t, std::size_t> > links;

                auto n = is.get<std::size_t>();
                for(std::size_t j = 0; j < n; ++j)
                {
                    auto a = is.get<std::size_t>();
                    auto b = is.get<std::size_t>();

                    links.push_back(std::make_pair(a, b));
                }

                n = is.get<std::size_t>();

                std::vector<char> data(n);
                is.read(data.data(), n);

                if(type == 'F')
                {
                    std::cout << banner("disassembly ", path, " ", strings[id]);
                    disassemble(data, comments, ci);
                }
            }
        }
        else
        {
            std::cout << banner("disassembly ", path);

            std::size_t ci = 0;
            disassemble(readFile(path), comments, ci);
        }
    }

    catch(const Error &e)
    {
        std::cerr << "error: " << e.what() << "\n";
    }
}
