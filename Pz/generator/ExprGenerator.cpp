#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"

#include "generator/AddrGenerator.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"

#include <pcx/indexed_range.h>

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ExprGenerator::visit(IdNode &node)
{
    auto sym = node.property<Sym*>("sym");

    if(sym->type() == Sym::Type::Func)
    {
        os << "    push &\"" << sym->funcname() << "\";\n";
        sz = sizeof(std::size_t);
    }
    else
    {
        os << "    push \"" << sym->fullname() << "\";\n";
        sz = Type::assertSize(node.location(), sym->property<Type*>("type"));
    }
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    os << "    push int(" << node.value << ");\n";
    sz = c.tree.root()->child("std")->child("int")->property<std::size_t>("size");
}

void ExprGenerator::visit(CallNode &node)
{
    auto type = TypeVisitor::assertType(c, node.target.get());
    auto size = Type::assertSize(node.location(), type->returnType);

    os << "    allocs " << size << ";\n";

    for(auto &p: node.params)
    {
        ExprGenerator::generate(c, os, p.get());
    }

    ExprGenerator::generate(c, os, node.target.get());
    os << "    call;\n";

    sz = size;
}

void ExprGenerator::visit(AddrOfNode &node)
{
    AddrGenerator::generate(c, os, node.expr.get());
    sz = sizeof(std::size_t);
}

std::size_t ExprGenerator::generate(Context &c, std::ostream &os, Node *node)
{
    auto r = Visitor::query<ExprGenerator, pcx::optional<std::size_t> >(node, c, os);
    if(!r)
    {
        throw Error(node->location(), "ExprGenerator failed for ", node->classname(), "node ", node->description());
    }

    return *r;
}
