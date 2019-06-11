#include "PushVisitor.h"

#include "common/OpCode.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/LiteralNode.h"
#include "nodes/StringNode.h"
#include "nodes/IdNode.h"
#include "nodes/AddrOfNode.h"

#include "visitors/AddrVisitor.h"
#include "visitors/AstPrinter.h"

namespace
{

template<typename T> void pushConstant(Context &c, T value)
{
    c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << sizeof(T);
    c.func().bytes << OpCode::Op::CopyAI << OpCode::Reg::Sp << sizeof(T) << value;
}

}

PushVisitor::PushVisitor(Context &c) : c(c)
{
}

void PushVisitor::visit(LiteralNode<char> &node)
{
    pushConstant(c, node.value);
    ok = true;
}

void PushVisitor::visit(LiteralNode<int> &node)
{
    pushConstant(c, node.value);
    ok = true;
}

void PushVisitor::visit(LiteralNode<std::size_t> &node)
{
    pushConstant(c, node.value);
    ok = true;
}

void PushVisitor::visit(IdNode &node)
{
    auto sym = c.find(node.location(), node.value);
    if(sym->type == Sym::Type::Global)
    {
        ByteStreamPatch p;

        auto size = sym->properties["size"].to<std::size_t>();

        c.func().bytes << OpCode::Op::SetRI << OpCode::Reg::Dx << p;
        c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << size;
        c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Dx << OpCode::Reg::Sp << size;

        c.func().links.emplace_back(p.position(), c.strings.insert(node.value));

        ok = true;
    }
    else if(sym->type == Sym::Type::Arg || sym->type == Sym::Type::Local)
    {
        c.func().bytes << OpCode::Op::CopyRR << OpCode::Reg::Bp << OpCode::Reg::Dx;
        c.func().bytes << (sym->type == Sym::Type::Arg ? OpCode::Op::AddRI : OpCode::Op::SubRI) << OpCode::Reg::Dx << sym->properties["offset"].to<std::size_t>();

        auto size = sym->properties["size"].to<std::size_t>();;

        c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Sp << size;
        c.func().bytes << OpCode::Op::CopyAA << OpCode::Reg::Dx << OpCode::Reg::Sp << size;

        ok = true;
    }
}

void PushVisitor::visit(AddrOfNode &node)
{
    AddrVisitor av(c);
    node.node->accept(av);

    if(!av.okay())
    {
        throw Error(node.node->location(), "addressable expected - ", astReconstruct(node.node.get()));
    }

    ok = true;
}
