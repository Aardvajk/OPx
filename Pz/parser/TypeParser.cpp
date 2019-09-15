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
    auto tok = c.scanner.next(get);

    bool constant = false;
    if(tok.type() == Token::Type::RwConst)
    {
        constant = true;
        tok = c.scanner.next(true);
    }

    bool ref = false;
    if(tok.type() == Token::Type::RwRef)
    {
        ref = true;
        tok = c.scanner.next(true);
    }

    std::size_t ptr = 0;
    while(tok.type() == Token::Type::RwPtr)
    {
        ++ptr;
        tok = c.scanner.next(true);
    }

    auto n = outer(c, false);

    n->constant = constant;
    n->ref = ref;
    n->ptr = ptr;

    return NodePtr(n.release());
}
