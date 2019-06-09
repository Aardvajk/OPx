#include "Generator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"

#include "symbols/Sym.h"

#include "types/Type.h"

#include "visitors/NameVisitors.h"

#include "generator/ByteLister.h"

#include <pcx/join_str.h>

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
    node.block->accept(*this);
}

void Generator::visit(ClassNode &node)
{
    if(node.block)
    {
        node.block->accept(*this);
    }
}

void Generator::visit(VarNode &node)
{
    if(node.sym->container()->type() == Sym::Type::Namespace)
    {
        auto type = node.sym->property("type").to<const Type*>();
        auto size = c.assertSize(node.location(), type);

        os << "var \"" << node.sym->fullname() << "\":" << size;

        if(node.value)
        {
            ByteLister b;
            node.value->accept(b);

            os << " = " << pcx::join_str(b.result(), ",", [](char c){ return pcx::str(int(static_cast<unsigned char>(c))); });
        }

        os << ";\n";
    }
}

void Generator::visit(FuncNode &node)
{
    auto type = node.sym->property("type").to<const Type*>();
    auto size = c.assertSize(node.location(), type->returnType.get());

    os << "func \"" << node.sym->fullname() << type->text() << "\":" << size << "\n";
    os << "{\n";

    if(node.block)
    {
        node.block->accept(*this);
    }

    os << "    svc 1234;\n";

    os << "}\n";
}
