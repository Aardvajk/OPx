#include "DeclarationParser.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"

#include "parser/CommonParser.h"
#include "parser/TypeParser.h"

#include <pcx/scoped_push.h>

namespace
{

void buildNamespace(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::name(c, get);

    if(c.containers.back() != Sym::Type::Namespace)
    {
        throw Error(name->location(), "invalid namespace - ", name->description());
    }

    auto n = new NamespaceNode(c.scanner.token().location(), name);
    block->push_back(n);

    auto cg = pcx::scoped_push(c.containers, Sym::Type::Namespace);

    n->body = CommonParser::blockContents(c, n->location(), false);
}

void buildArgs(Context &c, NodeList &container, bool get)
{
    std::string name;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::Id)
    {
        name = tok.text();
        c.scanner.next(true);
    }

    NodePtr nn(new IdNode(tok.location(), { }, name));

    auto n = new VarNode(tok.location(), nn);
    container.push_back(n);

    c.scanner.match(Token::Type::Colon, false);
    n->type = TypeParser::build(c, true);

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildArgs(c, container, true);
    }
}

void buildFunc(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::extendedName(c, get);

    if(c.containers.back() == Sym::Type::Func)
    {
        throw Error(name->location(), "invalid function - ", name->description());
    }

    auto n = new FuncNode(name->location(), name);
    block->push_back(n);

    auto tok = c.scanner.consume(Token::Type::LeftParen, false);
    if(tok.type() != Token::Type::RightParen)
    {
        buildArgs(c, n->args, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        n->type = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        auto cg = pcx::scoped_push(c.containers, Sym::Type::Func);

        auto scope = new ScopeNode(n->location());
        n->body = scope;

        scope->body = CommonParser::blockContents(c, n->location(), false);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void buildClass(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::extendedName(c, get);

    if(c.containers.back() == Sym::Type::Func)
    {
        throw Error(name->location(), "invalid class - ", name->description());
    }

    auto n = new ClassNode(name->location(), name);
    block->push_back(n);

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        auto cg = pcx::scoped_push(c.containers, Sym::Type::Class);

        n->body = CommonParser::blockContents(c, n->location(), false);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

}

void DeclarationParser::build(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: buildNamespace(c, block, true); break;
        case Token::Type::RwFunc: buildFunc(c, block, true); break;
        case Token::Type::RwClass: buildClass(c, block, true); break;

        default: throw Error(tok.location(), "declaration expected - ", tok.text());
    }
}
