#include "Parser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"

#include "parser/DeclarationParser.h"

void Parser::construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: DeclarationParser::buildNamespace(c, block, true); break;
        case Token::Type::RwFunc: DeclarationParser::buildFunction(c, block, true); break;

        default: throw Error(tok.location(), "declaration expected - ", tok.text());
    }
}

NodePtr Parser::build(Context &c)
{
    auto b = new BlockNode({ });
    NodePtr n(b);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, b, false);
    }

    return n;
}
