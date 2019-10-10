#include "Parser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"

#include "parser/CommonParser.h"
#include "parser/PragmaParser.h"
#include "parser/IncludeParser.h"
#include "parser/DeclarationParser.h"
#include "parser/FuncParser.h"

#include <pcx/scoped_push.h>
#include <pcx/str.h>
#include <pcx/base64.h>

namespace
{

NodePtr createFunction(const std::string &name, bool init)
{
    NodePtr id(new IdNode({ }, { }, name));

    auto fn = new FuncNode({ }, id);
    NodePtr n(fn);

    fn->setProperty("access", Access::Public);

    fn->setProperty("autogen", true);
    fn->setProperty("globalinit", init);
    fn->setProperty("globaldestroy", !init);

    auto sc = new ScopeNode({ });
    fn->body = sc;

    auto body = new BlockNode({ });
    sc->body = body;

    return n;
}

}

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

    auto i = createFunction(pcx::str("#global_init_", pcx::base64::encode(c.sources.path(c.scanner.sourceId()))), true);
    block->push_back(i);

    c.globalInit = i.get();

    auto d = createFunction(pcx::str("#global_destroy_", pcx::base64::encode(c.sources.path(c.scanner.sourceId()))), false);
    block->push_back(d);

    c.globalDestroy = d.get();

    return n;
}
