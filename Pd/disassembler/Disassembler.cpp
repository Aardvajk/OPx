#include "Disassembler.h"

#include "common/OpCode.h"

#include "framework/Error.h"
#include "framework/ByteReader.h"
#include "framework/Console.h"

#include "application/Context.h"

#include <pcx/join_str.h>
#include <pcx/lexical_cast.h>

#include <vector>

namespace
{

std::string bytes(ByteReader &rm, std::size_t n)
{
    std::vector<char> v;
    for(std::size_t i = 0; i < n; ++i)
    {
        v.push_back(rm.get<char>());
    }

    return pcx::join_str(v, ",", [](char c){ return pcx::lexical_cast<std::string>(int(c)); });
}

}

Disassembler::Disassembler(std::size_t index, std::size_t offset) : index(index), offset(offset), i(0)
{
}

void Disassembler::map(Context &c, std::size_t &pc)
{
    if(index < c.dm.size())
    {
        auto &cm = c.dm[index].comments;
        while(i < cm.size() && cm[i].address == pc)
        {
            auto s = cm[i].text;

            std::cout << (s[0] == '-' ? custom_banner('-', s.substr(1)) : elided(s));
            ++i;
        }
    }
}

void Disassembler::disassemble(Context &c, std::ostream &os, const char *data, std::size_t size)
{
    std::size_t pc = 0;
    ByteReader rm(data, pc);
    auto pcw = padw(offset + size);

    while(pc < size)
    {
        using namespace OpCode;

        Op op;
        Reg r0, r1;
        std::size_t s0;
        int i0;

        map(c, pc);

        os << pad(pc + offset, pcw) << ": ";

        rm(op);
        os << pad(toString(op), 7) << " ";

        switch(op)
        {
            case Op::SetRI: rm(r0); os << toString(r0) << " " << bytes(rm, 8); break;

            case Op::AddRI: rm(r0, s0); os << toString(r0) << " " << s0; break;
            case Op::SubRI: rm(r0, s0); os << toString(r0) << " " << s0;  break;

            case Op::CopyRR: rm(r0, r1); os << toString(r0) << " " << toString(r1); break;
            case Op::CopyRA: rm(r0, r1); os << toString(r0) << " " << toString(r1); break;
            case Op::CopyAR: rm(r0, r1); os << toString(r0) << " " << toString(r1); break;
            case Op::CopyAI: rm(r0, s0); os << toString(r0) << " " << s0 << " " << bytes(rm, s0); break;
            case Op::CopyAA: rm(r0, r1, s0); os << toString(r0) << " " << toString(r1) << " " << s0; break;

            case Op::PushR: rm(r0); os << toString(r0); break;
            case Op::PopR: rm(r0); os << toString(r0); break;

            case Op::Call: rm(r0); os << toString(r0); break;
            case Op::Ret: rm(s0); os << s0; break;
            case Op::JmpZ: rm(s0); os << " " << s0; break;

            case Op::AddS: break;
            case Op::AddI: break;

            case Op::SubI: break;

            case Op::MulS: break;

            case Op::NotS: break;

            case Op::IToS: break;
            case Op::SToC: break;

            case Op::Alloc: break;
            case Op::Free: break;

            case Op::Service: rm(i0); os << i0; break;

            case Op::End: break;

            default: throw Error("invalid opcode");
        }

        std::cout << "\n";
    }

    map(c, pc);
}
