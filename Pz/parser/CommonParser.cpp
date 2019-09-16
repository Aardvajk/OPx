#include "CommonParser.h"

#include "application/Context.h"

#include "operators/Operators.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"

#include "parser/Parser.h"

namespace
{

NodePtr nameImp(Context &c, NodePtr parent, bool extensions, bool get)
{
    std::string name;

    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::RwOperator && extensions)
    {
        tok = Operators::scan(c, true);
        name = pcx::str("operator", tok.text());

        c.scanner.next(true);
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

NodePtr CommonParser::blockContents(Context &c, Location location, bool get)
{
    auto block = new BlockNode(location);
    NodePtr n(block);

    c.scanner.match(Token::Type::LeftBrace, get);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::RightBrace)
    {
        Parser::construct(c, block, false);
    }

    c.scanner.next(true);

    return n;
}
