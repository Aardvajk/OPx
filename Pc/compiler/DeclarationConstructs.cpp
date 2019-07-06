#include "DeclarationConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"

#include "compiler/CommonConstructs.h"
#include "compiler/TypeConstructs.h"

namespace
{

void namespaceConstruct(Context &c, BlockNode *block, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    auto n = new NamespaceNode(nn->location(), nn);
    block->nodes.push_back(n);

    n->block = CommonConstructs::scopeContents(c, n->location(), false);
}

void classConstruct(Context &c, BlockNode *block, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    auto n = new ClassNode(nn->location(), nn);
    block->nodes.push_back(n);

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        n->block = CommonConstructs::scopeContents(c, n->location(), false);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void varConstruct(Context &c, BlockNode *block, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    auto n = new VarNode(nn->location(), nn);
    block->nodes.push_back(n);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        n->type = TypeConstructs::type(c, true);
    }

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        varConstruct(c, block, true);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void args(Context &c, NodeList &container, bool get)
{
    std::string name;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::Id)
    {
        name = tok.text();
        c.scanner.next(true);
    }

    auto n = new VarNode(tok.location(), new IdNode(tok.location(), name));
    container.push_back(n);

    c.scanner.match(Token::Type::Colon, false);
    n->type = TypeConstructs::type(c, true);

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        args(c, container, true);
    }
}

void funcConstruct(Context &c, BlockNode *block, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    auto n = new FuncNode(nn->location(), nn);
    block->nodes.push_back(n);

    c.scanner.consume(Token::Type::LeftParen, false);
    if(c.scanner.token().type() != Token::Type::RightParen)
    {
        args(c, n->args, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        n->type = TypeConstructs::type(c, true);
    }

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        n->block = CommonConstructs::scopeContents(c, n->location(), false);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

}

void DeclarationConstructs::entity(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: namespaceConstruct(c, block, true); break;
        case Token::Type::RwClass: classConstruct(c, block, true); break;
        case Token::Type::RwVar: varConstruct(c, block, true); break;
        case Token::Type::RwFunc: funcConstruct(c, block, true); break;

        default: throw Error(tok.location(), "declaration expected - ", tok.text());
    }
}
