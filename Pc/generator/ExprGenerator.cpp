#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/AssignNode.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "generator/AddrGenerator.h"

#include "types/Type.h"

ExprGenerator::ExprGenerator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void ExprGenerator::visit(IdNode &node)
{
    auto s = node.property<const Sym*>("sym");
    auto t = s->property<const Type*>("type");

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

void ExprGenerator::visit(NullLiteralNode &node)
{
    os << "    allocs 0;\n";
    sz = 0;
}

void ExprGenerator::visit(CharLiteralNode &node)
{
    os << "    push char(" << static_cast<unsigned int>(node.value) << ");\n";
    sz = c.tree.root()->child("std")->child("char")->property<std::size_t>("size");
}

void ExprGenerator::visit(IntLiteralNode &node)
{
    os << "    push int(" << node.value << ");\n";
    sz = c.tree.root()->child("std")->child("int")->property<std::size_t>("size");
}

void ExprGenerator::visit(BoolLiteralNode &node)
{
    os << "    push char(" << (node.value ? 1 : 0) << ");\n";
    sz = c.tree.root()->child("std")->child("bool")->property<std::size_t>("size");
}

void ExprGenerator::visit(CallNode &node)
{
    auto t = TypeVisitor::type(c, node.target.get());

    auto rs = c.assertSize(node.location(), t->returnType);
    os << "    allocs " << rs << ";\n";

    for(auto &p: node.params)
    {
        ExprGenerator::generate(c, os, *p.get());
    }

    ExprGenerator::generate(c, os, *node.target);

    os << "    call;\n";

    sz = rs;
}

void ExprGenerator::visit(AddrOfNode &node)
{
    AddrGenerator ag(c, os);
    node.expr->accept(ag);

    if(!ag.result())
    {
        throw Error(node.expr->location(), "addressable expected - ", NameVisitors::prettyName(node.expr.get()));
    }

    sz = sizeof(std::size_t);
}

void ExprGenerator::visit(AssignNode &node)
{
    auto s = ExprGenerator::generate(c, os, *node.expr);

    AddrGenerator ag(c, os);
    node.target->accept(ag);

    os << "    store " << s << ";\n";

    sz = s;
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
