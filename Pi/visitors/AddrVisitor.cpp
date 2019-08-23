#include "AddrVisitor.h"

#include "common/OpCode.h"

#include "application/Context.h"

#include "nodes/IdNode.h"

AddrVisitor::AddrVisitor(Context &c) : c(c)
{
}

void AddrVisitor::visit(IdNode &node)
{
    auto sym = c.syms.find(node.value);
    if(!sym || sym->type == Sym::Type::Global || sym->type == Sym::Type::Func)
    {
        ByteStreamPatch p;

        c.func().bytes << OpCode::Op::SetRI << OpCode::Reg::Dx << p;
        c.func().bytes << OpCode::Op::PushR << OpCode::Reg::Dx;

        c.func().links.emplace_back(p.position(), c.strings.insert(node.value));

        ok = true;
    }
    else if(sym->type == Sym::Type::Arg || sym->type == Sym::Type::Local)
    {
        c.func().bytes << OpCode::Op::CopyRR << OpCode::Reg::Bp << OpCode::Reg::Dx;
        c.func().bytes << (sym->type == Sym::Type::Arg ? OpCode::Op::AddRI : OpCode::Op::SubRI) << OpCode::Reg::Dx << sym->properties["offset"].to<std::size_t>();

        c.func().bytes << OpCode::Op::PushR << OpCode::Reg::Dx;

        ok = true;
    }
}
