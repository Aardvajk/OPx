#include "ClassDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/VarNode.h"

#include "types/Type.h"
#include "types/TypeBuilder.h"

#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"

ClassDecorator::ClassDecorator(Context &c) : c(c)
{
}

void ClassDecorator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void ClassDecorator::visit(VarNode &node)
{
    if(!node.type)
    {
        throw Error(node.location(), "type missing - ", NameVisitors::prettyName(node.name.get()));
    }

    auto type = TypeBuilder::type(c, node.type.get());

    auto name = c.assertSimpleNameUnique(node.name.get());

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Var, node.name->location(), name));
    sym->setProperty("type", type);
    sym->setProperty("member", true);

    node.setProperty("sym", sym);

    auto size = c.tree.current()->container()->getProperty("size");

    sym->setProperty("offset", size.value<std::size_t>());

    c.tree.current()->container()->setProperty("size", size.value<std::size_t>() + c.assertSize(node.location(), type));
}
