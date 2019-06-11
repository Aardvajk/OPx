#include "StoreVisitor.h"

#include "common/OpCode.h"

#include "application/Context.h"

#include "nodes/IdNode.h"

StoreVisitor::StoreVisitor(Context &c) : c(c)
{
}

void StoreVisitor::visit(IdNode &node)
{
    auto sym = c.find(node.location(), node.value);
    if(sym->type == Sym::Type::Global)
    {
        ByteStreamPatch p;

        c.func().bytes << OpCode::Op::SetRI << OpCode::Reg::Dx << p;

        c.func().links.emplace_back(p.position(), c.strings.insert(node.value));

        ok = true;
    }
    else if(sym->type == Sym::Type::Arg || sym->type == Sym::Type::Local)
    {
        c.func().bytes << OpCode::Op::CopyRR << OpCode::Reg::Bp << OpCode::Reg::Dx;
        c.func().bytes << (sym->type == Sym::Type::Arg ? OpCode::Op::AddRI : OpCode::Op::SubRI) << OpCode::Reg::Dx << sym->properties["offset"].to<std::size_t>();

        ok = true;
    }

    if(ok)
    {
        auto size = sym->properties["size"].to<std::size_t>();
        c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Sp << OpCode::Reg::Dx << size;
    }
}
