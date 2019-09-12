#include "DeclarationParser.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/VarNode.h"

#include "parser/CommonParser.h"
#include "parser/TypeParser.h"

namespace
{

void buildNamespace(Context &c, BlockNode *block, bool get)
{
    auto n = new NamespaceNode(c.scanner.token().location());
    block->push_back(n);

    n->name = CommonParser::name(c, get);
    n->body = CommonParser::blockContents(c, n->location(), false);
}

void buildVarImp(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    auto n = new VarNode(name->location(), name);
    block->push_back(n);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        n->type = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildVarImp(c, block, true);
    }
}

void buildVar(Context &c, BlockNode *block, bool get)
{
    buildVarImp(c, block, get);
    c.scanner.consume(Token::Type::Semicolon, false);
}

}

void DeclarationParser::build(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: buildNamespace(c, block, true); break;
        case Token::Type::RwVar: buildVar(c, block, true); break;

        default: throw Error(tok.location(), "declaration expected - ", tok.text());
    }
}
