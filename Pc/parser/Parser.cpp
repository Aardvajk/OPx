#include "Parser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"

#include "parser/CommonParser.h"
#include "parser/PragmaParser.h"
#include "parser/IncludeParser.h"
#include "parser/DeclarationParser.h"
#include "parser/FuncParser.h"

#include <pcx/scoped_push.h>

void Parser::construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwPragma: PragmaParser::build(c, block, true); break;
        case Token::Type::RwInclude: IncludeParser::build(c, block, true); break;

        case Token::Type::RwNamespace:
        case Token::Type::RwFunc:
        case Token::Type::RwClass:
        case Token::Type::RwVar:
        case Token::Type::RwPublic:
        case Token::Type::RwPrivate:
        case Token::Type::RwFree:
        case Token::Type::RwExternal:
        case Token::Type::RwExplicit: DeclarationParser::build(c, block, false); break;

        default: FuncParser::build(c, block, false);
    }
}

NodePtr Parser::build(Context &c)
{
    auto block = new BlockNode(Location());
    NodePtr n(block);

    auto cg = pcx::scoped_push(c.containers, Sym::Type::Namespace);
    auto ag = pcx::scoped_push(c.access, Access::Public);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, block, false);
    }

    return n;
}
