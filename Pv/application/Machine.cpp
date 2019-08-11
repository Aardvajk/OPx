#include "Machine.h"

#include "framework/Error.h"
#include "framework/ByteReader.h"

#include "components/Stack.h"
#include "components/Vars.h"

#include "operations/MathOps.h"
#include "operations/UnaryOps.h"
#include "operations/CompareOps.h"
#include "operations/ConvertOps.h"

Machine::Machine(const std::vector<char> &v, ServiceProc sp) : mm(1024 * 10), fs(mm, v.size()), sp(sp)
{
    std::memcpy(mm(0), v.data(), v.size());

    rg.pc() = 0;
    rg.sp() = mm.size();
}

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

            case Op::Call: rm(v.r0); s.push(rg.pc()); rg.pc() = rg[v.r0]; break;
            case Op::Ret: rm(v.s0); s.pop(rg.pc()); rg.sp() += v.s0; break;
            case Op::JmpZ: rm(v.s0); s.pop(v.c0); if(v.c0) rg.pc() += v.s0; break;

            case Op::Add: MathOps::mathOp<MathOps::Add>(rm, s); break;
            case Op::Sub: MathOps::mathOp<MathOps::Sub>(rm, s); break;
            case Op::Mul: MathOps::mathOp<MathOps::Mul>(rm, s); break;
            case Op::Div: MathOps::mathOp<MathOps::Div>(rm, s); break;
            case Op::Mod: MathOps::mathOp<MathOps::Mod>(rm, s); break;

            case Op::Not: UnaryOps::unaryOp<UnaryOps::Not>(rm, s); break;
            case Op::Neg: UnaryOps::unaryOp<UnaryOps::Neg>(rm, s); break;

            case Op::Lt: CompareOps::compareOp<CompareOps::Lt>(rm, s); break;
            case Op::LtEq: CompareOps::compareOp<CompareOps::LtEq>(rm, s); break;

            case Op::Conv: ConvertOps::convert(rm, s); break;

            case Op::Alloc: s.pop(v.s0); s.push(fs.allocate(v.s0)); break;
            case Op::Free: s.pop(v.s0); fs.release(v.s0); break;

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
