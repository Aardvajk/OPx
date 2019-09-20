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
#include "parser/ExprParser.h"

#include "visitors/NameVisitors.h"

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

    auto special = Visitor::query<NameVisitors::SpecialName, Token::Type>(name.get());

    auto n = new FuncNode(name->location(), name);
    block->push_back(n);

    auto tok = c.scanner.consume(Token::Type::LeftParen, false);
    if(tok.type() != Token::Type::RightParen)
    {
        buildArgs(c, n->args, false);
    }

    if(special == Token::Type::RwDelete && !n->args.empty())
    {
        throw Error(name->location(), "delete method cannot have parameters - ", name->description());
    }

    c.scanner.consume(Token::Type::RightParen, false);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        if(special != Token::Type::Invalid)
        {
            throw Error(name->location(), "cannot return a value - ", name->description());
        }

        n->type = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::RwConst)
    {
        n->constMethod = true;
        c.scanner.next(true);
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

void buildVarImp(Context &c, BlockNode *block, bool get)
{
    auto name = CommonParser::extendedName(c, get);

    auto n = new VarNode(name->location(), name);
    block->push_back(n);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        n->type = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::Assign)
    {
        n->value = ExprParser::build(c, true);
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
        case Token::Type::RwFunc: buildFunc(c, block, true); break;
        case Token::Type::RwClass: buildClass(c, block, true); break;
        case Token::Type::RwVar: buildVar(c, block, true); break;

        default: throw Error(tok.location(), "declaration expected - ", tok.text());
    }
}
