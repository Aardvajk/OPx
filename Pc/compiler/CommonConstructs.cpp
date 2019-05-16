#include "CommonConstructs.h"

#include "application/Context.h"

#include "nodes/GlobalNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

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

