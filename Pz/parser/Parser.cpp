#include "Parser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"

#include "parser/DeclarationParser.h"

#include <pcx/scoped_push.h>

void Parser::construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: DeclarationParser::buildNamespace(c, block, true); break;
        case Token::Type::RwClass: DeclarationParser::buildClass(c, block, true); break;
        case Token::Type::RwVar: DeclarationParser::buildTerminatedVar(c, block, true); break;
        case Token::Type::RwFunc: DeclarationParser::buildFunction(c, block, true); break;

        default: throw Error(tok.location(), "declaration expected - ", tok.text());
    }
}

NodePtr Parser::build(Context &c)
{
    auto b = new BlockNode({ });
    NodePtr n(b);

    auto cg = pcx::scoped_push(c.parseInfo.containers, Sym::Type::Namespace);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, b, false);
    }

    return n;
}
