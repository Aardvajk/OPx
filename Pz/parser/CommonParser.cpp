#include "CommonParser.h"

#include "application/Context.h"

#include "operators/Operators.h"

#include "nodes/IdNode.h"

namespace
{

NodePtr nameImp(Context &c, NodePtr parent, bool extensions, bool get)
{
    std::string name;

    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::RwOperator && extensions)
    {
        tok = Operators::scan(c, true);
        name = tok.text();
    }
    else if((tok.type() == Token::Type::RwNew || tok.type() == Token::Type::RwDelete) && extensions)
    {
        name = tok.text();
        c.scanner.next(true);
    }
    else
    {
        tok = c.scanner.match(Token::Type::Id, false);

        name = tok.text();
        c.scanner.next(true);
    }

    auto n = new IdNode(tok.location(), parent, name);
    NodePtr nn(n);

    while(c.scanner.token().type() == Token::Type::Dot)
    {
        nn = nameImp(c, nn, extensions, true);
    }

    return nn;
}

}

NodePtr CommonParser::name(Context &c, bool get)
{
    return nameImp(c, { }, false, get);
}

NodePtr CommonParser::extendedName(Context &c, bool get)
{
    return nameImp(c, { }, true, get);
}
