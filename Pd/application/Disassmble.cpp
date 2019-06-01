#include "Disassmble.h"

#include <common/OpCode.h>

#include "framework/ByteReader.h"
#include "framework/Error.h"
#include "framework/Console.h"

void Disassemble::disassemble(std::ostream &os, const char *data, std::size_t size)
{
    std::size_t pc = 0;
    ByteReader rm(data, pc);
    auto pcw = padw(size);

    while(pc < size)
    {
        using namespace OpCode;

        Op op;
        Reg r0, r1;
        std::size_t s0;
        int i0;

        os << pad(pc, pcw) << ": ";

        rm(op);
        os << pad(toString(op), 8) << " ";

        switch(op)
        {
            case Op::SetRI: rm(r0); os << toString(r0); break;

            case Op::AddRI: rm(r0, s0); os << toString(r0) << " " << s0; break;
            case Op::SubRI: rm(r0, s0); os << toString(r0) << " " << s0;  break;

            case Op::CopyRR: rm(r0, r1); os << toString(r0) << " " << toString(r1); break;
            case Op::CopyRA: rm(r0, r1); os << toString(r0) << " " << toString(r1); break;
            case Op::CopyAR: rm(r0, r1); os << toString(r0) << " " << toString(r1); break;
            case Op::CopyAI: rm(r0, s0); os << toString(r0) << " " << s0; break;
            case Op::CopyAA: rm(r0, r1, s0); os << toString(r0) << " " << toString(r1) << " " << s0; break;

            case Op::PushR: rm(r0); os << toString(r0); break;
            case Op::PopR: rm(r0); os << toString(r0); break;

            case Op::Call: rm(r0); os << toString(r0); break;
            case Op::Ret: rm(s0); os << s0; break;

            case Op::Int: rm(i0); os << i0;break;

            case Op::End: break;

            default: throw Error("invalid opcode");
        }

        std::cout << "\n";
    }
}
