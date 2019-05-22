#include "Machine.h"

#include "application/Error.h"

#include "components/Accessor.h"
#include "components/Vars.h"

Machine::Machine(const std::vector<char> &pr, IntFunc intFunc) : mm(10240), ip(intFunc)
{
    std::memcpy(mm(0), pr.data(), pr.size());

    rg.pc() = 0;
    rg.sp() = mm.size();
}

void Machine::execute()
{
    Accessor::Reader rp(mm(0), rg.pc());
    Vars v;

    while(true)
    {
        using namespace OpCode;

        rp(v.op);

        switch(v.op)
        {
            case Type::End: return;

            case Type::SubRI: rp(v.r0, v.s0); rg[v.r0] -= v.s0; break;
            case Type::AddRI: rp(v.r0, v.s0); rg[v.r0] += v.s0; break;

            case Type::CopyRR: rp(v.r0, v.r1); rg[v.r1] = rg[v.r0]; break;
            case Type::CopyRA: rp(v.r0, v.r1); std::memcpy(mm(rg[v.r1]), &(rg[v.r0]), sizeof(std::size_t)); break;
            case Type::CopyAR: rp(v.r0, v.r1); std::memcpy(&(rg[v.r1]), mm(rg[v.r0]), sizeof(std::size_t)); break;
            case Type::CopyAA: rp(v.r0, v.r1, v.s0); std::memcpy(mm(rg[v.r1]), mm(rg[v.r0]), v.s0); break;
            case Type::CopyAI: rp(v.r0, v.s0); std::memcpy(mm(rg[v.r0]), mm(rg.pc()), v.s0); rg.pc() += v.s0; break;

            case Type::PushR: rp(v.r0); rg.sp() -= sizeof(std::size_t); std::memcpy(mm(rg.sp()), &(rg[v.r0]), sizeof(std::size_t)); break;
            case Type::PopR: rp(v.r0); std::memcpy(&(rg[v.r0]), mm(rg.sp()), sizeof(std::size_t)); rg.sp() += sizeof(std::size_t); break;

            case Type::Int: rp(v.i0); ip(v.i0, mm, rg); break;

            default: throw Error("invalid opcode - ", static_cast<int>(v.op));
        }
    }
}
