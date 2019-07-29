#include "ExprGenerator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/AssignNode.h"
#include "nodes/ThisNode.h"
#include "nodes/DerefNode.h"
#include "nodes/BinaryNode.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "generator/AddrGenerator.h"

#include "operators/AddOperators.h"

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
        if(s->getProperty("member").value<bool>() && node.parent)
        {
            if(TypeVisitor::type(c, node.parent.get())->ptr)
            {
                throw Error(node.parent->location(), "cannot access member via pointer - ", NameVisitors::prettyName(node.parent.get()));
            }

            AddrGenerator::generate(c, os, *node.parent);

            os << "    push size(" << s->property<std::size_t>("offset") << ");\n";
            os << "    add size;\n";
            os << "    load " << c.assertSize(node.location(), t) << ";\n";
        }
        else
        {
            if(t->sub)
            {
                os << "    push &\"" << s->fullname() << "\";\n";
            }
            else
            {
                os << "    push \"" << s->fullname() << "\";\n";
            }
        }
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

void ExprGenerator::visit(SizeLiteralNode &node)
{
    os << "    push size(" << node.value << ");\n";
    sz = c.tree.root()->child("std")->child("size")->property<std::size_t>("size");
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
    AddrGenerator::generate(c, os, *node.expr);
    sz = sizeof(std::size_t);
}

void ExprGenerator::visit(AssignNode &node)
{
    auto s = ExprGenerator::generate(c, os, *node.expr);

    AddrGenerator::generate(c, os, *node.target);

    os << "    store " << s << ";\n";

    sz = s;
}

void ExprGenerator::visit(ThisNode &node)
{
    os << "    push \"" << c.tree.current()->container()->fullname() << ".this\";\n";
    sz = sizeof(std::size_t);
}

void ExprGenerator::visit(DerefNode &node)
{
    auto s = c.assertSize(node.expr->location(), TypeVisitor::type(c, &node));

    ExprGenerator::generate(c, os, *node.expr);

    os << "    load " << s << ";\n";
    sz = s;
}

void ExprGenerator::visit(BinaryNode &node)
{
    switch(node.op)
    {
        case Operators::Type::Add: sz = AddOperators::generate(c, os, node); break;

        default: break;
    }
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
