#include "Generator.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "generator/LocalsGenerator.h"
#include "generator/FuncGenerator.h"

Generator::Generator(Context &c, std::ostream &os) : c(c), os(os)
{
}

void Generator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Generator::visit(NamespaceNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void Generator::visit(FuncNode &node)
{
    if(node.body)
    {
        auto sym = node.property<Sym*>("sym");
        auto type = sym->property<Type*>("type");

        os << "func \"" << sym->fullname() << type->text() << "\":" << Type::assertSize(node.location(), type->returnType) << "\n";
        os << "{\n";

        for(auto &a: node.args)
        {
            auto s = a->property<Sym*>("sym");
            os << "    arg \"" << s->fullname() << "\":" << Type::assertSize(a->location(), s->property<Type*>("type")) << ";\n";
        }

        Visitor::visit<LocalsGenerator>(node.body.get(), c, os);
        Visitor::visit<FuncGenerator>(node.body.get(), c, os);

        os << "}\n";
    }
}

void Generator::visit(ClassNode &node)
{
    if(node.body)
    {
        auto sg = c.tree.open(node.property<Sym*>("sym"));
        node.body->accept(*this);
    }
}

void Generator::visit(PragmaNode &node)
{
    Pragmas::execute(c, node);
}
