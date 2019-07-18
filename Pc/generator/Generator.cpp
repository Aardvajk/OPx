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
    node.body->accept(*this);
}

void Generator::visit(ClassNode &node)
{
}

void Generator::visit(VarNode &node)
{
    auto sym = node.property<const Sym*>("sym");
    os << "var \"" << sym->fullname() << "\":" << c.assertSize(node.location(), sym->property<const Type*>("type")) << ";\n";
}

void Generator::visit(FuncNode &node)
{
    auto sym = node.property<const Sym*>("sym");
    auto type = sym->property<const Type*>("type");

    os << "func \"" << sym->fullname() << type->text() << "\":" << c.assertSize(node.location(), type->returnType);

    if(node.body)
    {
        os << "\n{\n";

        for(auto &a: node.args)
        {
            auto sym = a->property<const Sym*>("sym");
            os << "    arg \"" << sym->fullname() << "\":" << c.assertSize(a->location(), sym->property<const Type*>("type")) << ";\n";
        }

        LocalsGenerator lg(c, os);
        node.body->accept(lg);

        FuncGenerator fg(c, os);
        node.body->accept(fg);

        os << "\"#end_function\":\n";
        os << "}\n";
    }
    else
    {
        os << ";\n";
    }
}
