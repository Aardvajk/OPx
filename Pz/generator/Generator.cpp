#include "Generator.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

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
}

void Generator::visit(FuncNode &node)
{
    if(node.body)
    {
        auto sym = node.property<Sym*>("sym");
        auto type = sym->property<Type*>("type");

        os << "func \"" << sym->fullname() << type->text() << "\":" << Type::assertSize(node.location(), type->returnType) << "\n";
        os << "{\n";
        os << "}\n";
    }
}

void Generator::visit(ClassNode &node)
{
}
