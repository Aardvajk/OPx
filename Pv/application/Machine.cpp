#include "Machine.h"

#include "framework/Error.h"
#include "framework/ByteReader.h"

#include "components/Stack.h"
#include "components/Vars.h"

Machine::Machine(const std::vector<char> &v, ServiceProc sp) : mm(1024 * 5), sp(sp)
{
    std::memcpy(mm(0), v.data(), v.size());

    rg.pc() = 0;
    rg.sp() = mm.size();
}

#include <iostream>

void Machine::execute()
{
    ByteReader rm(mm(0), rg.pc());
    Stack s(mm, rg);
    Vars v;

    while(true)
    {
        using namespace OpCode;

        rm(v.op);

        switch(v.op)
        {
            case Op::SetRI: rm(v.r0); std::memcpy(&(rg[v.r0]), mm(rg.pc()), sizeof(std::size_t)); rg.pc() += sizeof(std::size_t); break;

            case Op::AddRI: rm(v.r0, v.s0); rg[v.r0] += v.s0; break;
            case Op::SubRI: rm(v.r0, v.s0); rg[v.r0] -= v.s0; break;

            case Op::CopyRR: rm(v.r0, v.r1); rg[v.r1] = rg[v.r0]; break;
            case Op::CopyRA: rm(v.r0, v.r1); std::memcpy(mm(rg[v.r1]), &(rg[v.r0]), sizeof(std::size_t)); break;
            case Op::CopyAR: rm(v.r0, v.r1); std::memcpy(&(rg[v.r1]), mm(rg[v.r0]), sizeof(std::size_t)); break;
            case Op::CopyAI: rm(v.r0, v.s0); std::memcpy(mm(rg[v.r0]), mm(rg.pc()), v.s0); rg.pc() += v.s0; break;
            case Op::CopyAA: rm(v.r0, v.r1, v.s0); std::memcpy(mm(rg[v.r1]), mm(rg[v.r0]), v.s0); break;

            case Op::PushR: rm(v.r0); s.push(rg[v.r0]); break;
            case Op::PopR: rm(v.r0); s.pop(rg[v.r0]); break;

            case Op::Call: rm(v.r0); rg.sp() -= sizeof(std::size_t); std::memcpy(mm(rg.sp()), &(rg.pc()), sizeof(std::size_t)); rg.pc() = rg[v.r0]; break;
            case Op::Ret: rm(v.s0); std::memcpy(&(rg.pc()), mm(rg.sp()), sizeof(std::size_t)); rg.sp() += sizeof(std::size_t) + v.s0; break;

            case Op::Service: rm(v.i0); sp(v.i0, mm, rg); break;

            case Op::End: return;

            default: throw Error("invalid opcode");
        }
    }
}

void Machine::shutdown()
{
    bool ok = true;

    if(rg.sp() != mm.size())
    {
        std::cout << "stack pointer invalid\n";
        ok = false;
    }

    if(ok)
    {
        std::cout << "okay\n";
    }
}
