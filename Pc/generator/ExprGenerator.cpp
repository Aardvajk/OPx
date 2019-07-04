#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/GlobalNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"
#include "nodes/CharLiteralNode.h"
#include "nodes/IntLiteralNode.h"
#include "nodes/CallNode.h"

#include "visitors/NameVisitors.h"

namespace
{

std::size_t generateId(Context &c, std::ostream &os, Node &node)
{
    auto syms = node.property("syms").to<std::vector<SymResult> >();
    if(syms.size() > 1)
    {
        throw Error(node.location(), "ambigious - ", NameVisitors::prettyName(&node));
    }

    auto sym = syms.front().sym;
    if(sym->attrs() & Sym::Attr::Member)
    {
    }

    os << "    push \"" << sym->fullname() << "\";\n";
    return c.assertSize(node.location(), sym->property("type").to<const Type*>());
}

}

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ExprGenerator::visit(GlobalNode &node)
{
    sz = generateId(c, os, node);
}

void ExprGenerator::visit(IdNode &node)
{
    sz = generateId(c, os, node);
}

void ExprGenerator::visit(DotNode &node)
{
    sz = generateId(c, os, node);
}

void ExprGenerator::visit(NullLiteralNode &node)
{
    os << "    allocs 0;\n";
    sz = 0;
}

void ExprGenerator::visit(CharLiteralNode &node)
{
    os << "    push char(" << static_cast<unsigned int>(node.value) << ");\n";
    sz = sizeof(char);
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    os << "    push int(" << node.value << ");\n";
    sz = sizeof(int);
}

void ExprGenerator::visit(CallNode &node)
{
    auto sym = node.property("sym").to<const Sym*>();
    auto type = sym->property("type").to<const Type*>();

    sz = c.assertSize(node.target->location(), type->returnType.get());

    os << "    allocs " << *sz << ";\n";

    for(auto &a: node.args)
    {
        ExprGenerator::generate(c, os, a);
    }

    os << "    push &\"" << sym->fullname() << type->text() << "\";\n";
    os << "    call;\n";
}

std::size_t ExprGenerator::generate(Context &c, std::ostream &os, Node &node)
{
    ExprGenerator eg(c, os);
    node.accept(eg);

    if(!eg.size())
    {
        throw Error(node.location(), "cannot generate expr - ", NameVisitors::prettyName(&node));
    }

    return *eg.size();
}

