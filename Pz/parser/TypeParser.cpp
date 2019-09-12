#include "TypeParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/TypeNode.h"

#include "parser/CommonParser.h"

namespace
{

using TypeNodePtr = pcx::scoped_ptr<TypeNode>;

TypeNodePtr outer(Context &c, bool get);

TypeNodePtr primary(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    TypeNodePtr n(new TypeNode(tok.location()));

    if(tok.type() == Token::Type::LeftParen)
    {
        throw Error("internal - function types not implemented");
    }
    else
    {
        n->name = CommonParser::name(c, false);
    }

    return n;
}

TypeNodePtr outer(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    auto n = primary(c, false);

    return n;
}

}

NodePtr TypeParser::build(Context &c, bool get)
{
    return NodePtr(outer(c, get).release());
}
