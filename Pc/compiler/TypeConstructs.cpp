#include "TypeConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/TypeNode.h"

#include "compiler/CommonConstructs.h"
#include "compiler/ExprConstructs.h"

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

    if(tok.type() == Token::Type::LeftParen)
    {
        n->function = true;

        c.scanner.next(true);
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

    unsigned ptr = 0;
    while(tok.type() == Token::Type::RwPtr)
    {
        ++ptr;
        tok = c.scanner.next(true);
    }

    auto n = primary(c, false);

    n->constant = constant;
    n->ref = ref;
    n->ptr = ptr;

    if(c.scanner.token().type() == Token::Type::LeftSub)
    {
        n->sub = ExprConstructs::expr(c, true);
        c.scanner.consume(Token::Type::RightSub, false);
    }

    return n;
}

}

NodePtr TypeConstructs::type(Context &c, bool get)
{
    return NodePtr(outer(c, get).release());
}
