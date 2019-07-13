#include "TypeConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/TypeNode.h"

#include "compiler/CommonConstructs.h"

namespace
{

using TypeNodePtr = pcx::scoped_ptr<TypeNode>;

TypeNodePtr outer(Context &c, bool get);

void arg(Context &c, NodeList &container, bool get)
{
    container.push_back(outer(c, get).release());

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        arg(c, container, true);
    }
}

TypeNodePtr primary(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    TypeNodePtr n(new TypeNode(tok.location()));

    if(tok.type() == Token::Type::RwFunc)
    {
        n->function = true;

        c.scanner.consume(Token::Type::LeftParen, true);
        if(c.scanner.token().type() != Token::Type::RightParen)
        {
            arg(c, n->args, false);
        }

        c.scanner.consume(Token::Type::RightParen, false);

        if(c.scanner.token().type() == Token::Type::Colon)
        {
            n->returnType = NodePtr(outer(c, true).release());
        }
    }
    else
    {
        n->name = CommonConstructs::name(c, false);
    }

    return n;
}

TypeNodePtr outer(Context &c, bool get)
{
    unsigned ptr = 0;

    auto tok = c.scanner.next(get);
    while(tok.type() == Token::Type::RwPtr)
    {
        ++ptr;
        tok = c.scanner.next(true);
    }

    auto n = primary(c, false);
    n->ptr = ptr;

    return n;
}

}

NodePtr TypeConstructs::type(Context &c, bool get)
{
    return NodePtr(outer(c, get).release());
}
