#include "ArgDecorator.h"

#include "framework/Error.h"

#include "nodes/VarNode.h"

#include "decorator/ExprDecorator.h"
#include "decorator/TypeDecorator.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

ArgDecorator::ArgDecorator(Context &c, std::size_t &no) : c(c), no(no)
{
}

void ArgDecorator::visit(VarNode &node)
{
    Type *vt = nullptr;
    if(node.value)
    {
        node.value = ExprDecorator::decorate(c, node.value);
        vt = TypeVisitor::assertType(c, node.value.get());

        ++no;
    }

    Type *at = nullptr;
    if(node.type)
    {
        Visitor::visit<TypeDecorator>(node.type.get(), c);
        at = TypeVisitor::assertType(c, node.type.get());
    }
    else if(!node.value)
    {
        throw Error(node.location(), "argument missing type - ", node.description());
    }

    if(vt && at && !TypeCompare(c).convertible(vt, at))
    {
        throw Error(node.value->location(), at->text(), " expected - ", node.value->description());
    }
}
