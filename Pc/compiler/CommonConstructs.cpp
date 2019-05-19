#include "CommonConstructs.h"

#include "error/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/GlobalNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

#include "compiler/Compiler.h"

namespace
{

NodePtr nameTrail(Context &c, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    auto dot = c.scanner.next(true);
    if(dot.type() == Token::Type::Dot)
    {
        return new DotNode(dot.location(), tok.text(), nameTrail(c, true));
    }

    return new IdNode(tok.location(), tok.text());
}

}

NodePtr CommonConstructs::name(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::Dot)
    {
        return new GlobalNode(tok.location(), nameTrail(c, true));
    }

    return nameTrail(c, false);
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


