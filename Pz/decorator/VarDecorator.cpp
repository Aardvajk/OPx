#include "VarDecorator.h"

#include "application/Context.h"

#include "nodes/VarNode.h"

#include "decorator/TypeDecorator.h"
#include "decorator/ExprDecorator.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "types/Type.h"

VarDecorator::VarDecorator(Context &c) : c(c)
{
}

void VarDecorator::visit(VarNode &node)
{
    Type *type = nullptr;

    if(node.type)
    {
        Visitor::visit<TypeDecorator>(node.type.get(), c);
        type = Visitor::query<TypeVisitor, Type*>(node.type.get());
    }

    if(node.value)
    {
        node.value = ExprDecorator::decorate(c, node.value);
        if(!type)
        {
            type = Visitor::query<TypeVisitor, Type*>(node.value.get());
        }
    }

    if(!type)
    {
        throw Error(node.location(), "no type specified - ", node.name->description());
    }

    auto name = NameVisitors::assertSimpleUniqueName(c, node.name.get());
    if(name.empty())
    {
        name = pcx::str("#unnamed", c.func().labels++);
    }

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Var, node.location(), name));
    sym->setProperty("type", type);

    node.setProperty("type", type);
    node.setProperty("sym", sym);
}
