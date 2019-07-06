#include "TypeConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/TypeNode.h"

#include "compiler/CommonConstructs.h"

namespace
{

NodePtr primary(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    auto n = new TypeNode(tok.location());
    n->name = CommonConstructs::name(c, false);

    return n;
}

}

NodePtr TypeConstructs::type(Context &c, bool get)
{
    return primary(c, get);
}
