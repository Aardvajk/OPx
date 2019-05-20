#include "Machine.h"

#include "application/Error.h"

#include "components/Accessor.h"
#include "components/Vars.h"

#include <iostream>

Machine::Machine(const std::vector<char> &pr) : mm(10240)
{
    std::memcpy(mm.data(), pr.data(), pr.size());

    rg.pc() = mm.data();
    rg.sp() = mm.data() + mm.size();

    std::cout << "start: " << static_cast<void*>(rg.pc()) << "\n";
}

void Machine::execute()
{
    Accessor::Reader rp(rg.pc());
    Vars v;

    while(true)
    {
        using namespace OpCode;

        rp(v.op);

        switch(v.op)
        {
            case Type::End: std::cout << "end\n"; return;

            case Type::SubRI: rp(v.r0, v.s0); rg[v.r0] -= v.s0; break;
            case Type::AddRI: rp(v.r0, v.s0); rg[v.r0] += v.s0; break;

            case Type::CopyRR: rp(v.r0, v.r1); rg[v.r1] = rg[v.r0]; break;
            case Type::CopyRA: rp(v.r0, v.r1); rg.ra(v.r0, v.r1); break;
            case Type::CopyAR: rp(v.r0, v.r1); rg.ar(v.r0, v.r1); break;
            case Type::CopyAA: rp(v.r0, v.r1, v.s0); rg.aa(v.r1, v.r0, v.s0); break;

            case Type::PrintR: rp(v.r0); std::cout << toString(v.r0) << ": " << static_cast<void*>(rg[v.r0]) << "\n"; break;

            default: throw Error("invalid opcode - ", static_cast<int>(v.op));
        }
    }
}
