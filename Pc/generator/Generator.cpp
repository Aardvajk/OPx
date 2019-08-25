#include "Generator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "visitors/NameVisitors.h"

#include "generator/LocalsGenerator.h"
#include "generator/FuncGenerator.h"
#include "generator/ByteListGenerator.h"

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
    auto g = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void Generator::visit(ClassNode &node)
{
    if(node.body)
    {
        auto g = c.tree.open(node.property<Sym*>("sym"));
        node.body->accept(*this);
    }
}

void Generator::visit(VarNode &node)
{
    auto sym = node.property<const Sym*>("sym");

    if(!sym->getProperty("member").value<bool>())
    {
        os << "var \"" << sym->fullname() << "\":" << c.assertInitSize(node.location(), sym->property<const Type*>("type"));

        if(node.value)
        {
            os << " = ";

            ByteListGenerator bg(c, os);
            node.value->accept(bg);

            if(!bg.result())
            {
                throw Error(node.value->location(), "invalid static initialiser - ", NameVisitors::prettyName(node.value.get()));
            }
        }

        os << ";\n";
    }
}

void Generator::visit(FuncNode &node)
{
    if(node.body)
    {
        auto sym = node.property<const Sym*>("sym");
        auto type = sym->property<const Type*>("type");

        os << "func \"" << sym->fullname() << type->text() << "\":" << c.assertSize(node.location(), type->returnType);

        os << "\n{\n";

        for(auto &a: node.args)
        {
            auto sym = a->property<const Sym*>("sym");
            os << "    arg \"" << sym->fullname() << "\":" << c.assertSize(a->location(), sym->property<const Type*>("type")) << ";\n";
        }

        auto g = c.tree.open(node.property<Sym*>("sym"));

        c.labels = 0;

        os << "    var \"@rf\":1;\n";

        LocalsGenerator lg(c, os);
        node.body->accept(lg);

        os << "    clrf \"@rf\";\n";

        FuncGenerator fg(c, os);

        if(node.initialisers)
        {
            node.initialisers->accept(fg);
        }

        node.body->accept(fg);

        os << "\"#end_function\":\n";
        os << "}\n";
    }
}
