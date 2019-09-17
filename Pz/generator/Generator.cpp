#include "Generator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "generator/LocalsGenerator.h"

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
            os << "    arg \"" << a->property<Sym*>("sym")->fullname() << "\":" << Type::assertSize(a->location(), a->property<Type*>("type")) << ";\n";
        }

        Visitor::visit<LocalsGenerator>(node.body.get(), c, os);

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
