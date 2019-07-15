#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"

#include "types/Type.h"

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ExprGenerator::visit(IdNode &node)
{
    auto s = node.property("sym").to<const Sym*>();
    auto t = s->property("type").to<const Type*>();

    if(s->type() == Sym::Type::Func)
    {
        os << "    push &\"" << s->fullname() << t->text() << "\";\n";
    }
    else
    {
        os << "    push \"" << s->fullname() << "\";\n";
    }

    sz = c.assertSize(node.location(), t);
}

void ExprGenerator::visit(CharLiteralNode &node)
{
    os << "    push char(" << static_cast<unsigned int>(node.value) << ");\n";
    sz = c.tree.root()->child("std")->child("char")->property("size").to<std::size_t>();
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    os << "    push int(" << node.value << ");\n";
    sz = c.tree.root()->child("std")->child("int")->property("size").to<std::size_t>();
}

void ExprGenerator::visit(CallNode &node)
{
    auto s = node.target->property("sym").to<const Sym*>();
    auto t = s->property("type").to<const Type*>();

    auto rs = c.assertSize(node.location(), t->returnType);
    os << "    allocs " << rs << ";\n";

    for(auto &p: node.params)
    {
        ExprGenerator::generate(c, os, p);
    }

    ExprGenerator::generate(c, os, *node.target);

    os << "    call;\n";

    sz = rs;
}

std::size_t ExprGenerator::generate(Context &c, std::ostream &os, Node &node)
{
    ExprGenerator eg(c, os);
    node.accept(eg);

    if(!eg.size())
    {
        throw Error(node.location(), "expr failed");
    }

    return *(eg.size());
}
