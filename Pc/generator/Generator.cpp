#include "Generator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"

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
        n.accept(*this);
    }
}

void Generator::visit(NamespaceNode &node)
{
    node.body->accept(*this);
}

void Generator::visit(ClassNode &node)
{
}

void Generator::visit(VarNode &node)
{
    auto sym = node.property("sym").to<const Sym*>();
    os << "var \"" << sym->fullname() << "\":" << c.assertSize(node.location(), sym->property("type").to<const Type*>()) << ";\n";
}

void Generator::visit(FuncNode &node)
{
    auto sym = node.property("sym").to<const Sym*>();
    auto type = sym->property("type").to<const Type*>();

    os << "func \"" << sym->fullname() << type->text() << "\":" << c.assertSize(node.location(), type->returnType);

    if(node.body)
    {
        os << "\n{\n";

        for(auto &a: node.args)
        {
            auto sym = a.property("sym").to<const Sym*>();
            os << "    arg \"" << sym->fullname() << "\":" << c.assertSize(a.location(), sym->property("type").to<const Type*>()) << ";\n";
        }

        LocalsGenerator lg(c, os);
        node.body->accept(lg);

        os << "}\n";
    }
    else
    {
        os << ";\n";
    }
}
