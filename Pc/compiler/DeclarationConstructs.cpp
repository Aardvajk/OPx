#include "DeclarationConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"

#include "visitors/NameVisitors.h"

#include "compiler/CommonConstructs.h"
#include "compiler/TypeConstructs.h"
#include "compiler/ExprConstructs.h"

#include <pcx/scoped_push.h>

#include <algorithm>

namespace
{

void namespaceConstruct(Context &c, BlockNode *block, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    if(c.containers.back() != Sym::Type::Namespace)
    {
        throw Error(nn->location(), "invalid namespace - ", NameVisitors::prettyName(nn.get()));
    }

    auto n = new NamespaceNode(nn->location(), nn);
    block->push_back(n);

    auto cg = pcx::scoped_push(c.containers, Sym::Type::Namespace);
    n->body = CommonConstructs::blockContents(c, n->location(), false);
}

void classConstruct(Context &c, BlockNode *block, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    if(c.containers.back() == Sym::Type::Func)
    {
        throw Error(nn->location(), "invalid class - ", NameVisitors::prettyName(nn.get()));
    }

    auto n = new ClassNode(nn->location(), nn);
    block->push_back(n);

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        auto cg = pcx::scoped_push(c.containers, Sym::Type::Class);
        n->body = CommonConstructs::blockContents(c, n->location(), false);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void params(Context &c, NodeList &container, bool get)
{
    container.push_back(ExprConstructs::expr(c, get));

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        params(c, container, true);
    }
}

void varConstruct(Context &c, BlockNode *block, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    auto n = new VarNode(nn->location(), nn);
    block->push_back(n);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        n->type = TypeConstructs::type(c, true);
    }

    if(c.scanner.token().type() == Token::Type::LeftParen)
    {
        auto tok = c.scanner.next(true);
        if(tok.type() != Token::Type::RightParen)
        {
            params(c, n->params, false);
        }

        c.scanner.consume(Token::Type::RightParen, false);
    }

    if(c.scanner.token().type() == Token::Type::Assign)
    {
        n->value = ExprConstructs::expr(c, true);
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
    else
    {
        name = pcx::str("#unnamed", c.labels++);
    }

    auto n = new VarNode(tok.location(), new IdNode(tok.location(), { }, name));
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
    NodePtr nn = CommonConstructs::extendedName(c, get);

    if(c.containers.back() == Sym::Type::Func)
    {
        throw Error(nn->location(), "invalid function - ", NameVisitors::prettyName(nn.get()));
    }

    auto n = new FuncNode(nn->location(), nn);
    block->push_back(n);

    auto special = NameVisitors::isNameSpecial(nn.get());

    c.scanner.consume(Token::Type::LeftParen, false);
    if(c.scanner.token().type() != Token::Type::RightParen)
    {
        args(c, n->args, false);
    }

    if(special == Token::Type::RwDelete && !n->args.empty())
    {
        throw Error(nn->location(), "delete function cannot take parameters");
    }

    c.scanner.consume(Token::Type::RightParen, false);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        if(special != Token::Type::Invalid)
        {
            throw Error(nn->location(), "cannot return a value - ", NameVisitors::prettyName(nn.get()));
        }

        n->type = TypeConstructs::type(c, true);
    }

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        auto cg = pcx::scoped_push(c.containers, Sym::Type::Func);
        n->body = CommonConstructs::funcContents(c, n->location(), false);
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
