#include "CommonConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"

#include "compiler/Compiler.h"

#include <algorithm>

namespace
{

NodePtr nameImpl(Context &c, bool allowOperator, bool get)
{
    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::RwOperator && allowOperator)
    {
        static const std::vector<Token::Type> operators =
        {
            Token::Type::Assign
        };

        auto op = c.scanner.next(true);
        if(std::find(operators.begin(), operators.end(), op.type()) == operators.end())
        {
            throw Error(op.location(), "invalid operator - ", op.text());
        }

        c.scanner.next(true);
        return new IdNode(tok.location(), tok.text() + op.text());
    }

    c.scanner.match(Token::Type::Id, false);

    auto dot = c.scanner.next(true);
    if(dot.type() == Token::Type::Dot)
    {
        return new IdNode(dot.location(), tok.text(), nameImpl(c, allowOperator, true));
    }

    return new IdNode(tok.location(), tok.text());
}

}

NodePtr CommonConstructs::name(Context &c, bool get)
{
    return nameImpl(c, false, get);
}

NodePtr CommonConstructs::extendedName(Context &c, bool get)
{
    return nameImpl(c, true, get);
}

NodePtr CommonConstructs::scopeContents(Context &c, Location location, bool get)
{
    auto block = new BlockNode(location);
    NodePtr nn(block);

    c.scanner.match(Token::Type::LeftBrace, get);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::RightBrace)
    {
        Compiler::construct(c, block, false);
    }

    c.scanner.next(true);

    return nn;
}


