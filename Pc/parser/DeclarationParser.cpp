#include "DeclarationParser.h"

#include "application/Context.h"

#include "info/Qual.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/InitNode.h"

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

    n->setProperty("access", c.access.back());

    auto cg = pcx::scoped_push(c.containers, Sym::Type::Namespace);
    auto ag = pcx::scoped_push(c.access, Access::Public);

    n->body = CommonParser::blockContents(c, n->location(), false);
}

void buildGenerics(Context &c, GenericParams &params, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);
    params.params.emplace_back(Generic(tok.text()));

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Comma)
    {
        buildGenerics(c, params, true);
    }
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

    n->setProperty("access", Access::Private);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        n->type = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::Assign)
    {
        n->value = ExprParser::buildExpression(c, true);
    }
    else if(!n->type)
    {
        throw Error(n->location(), "type expected - ", n->description());
    }

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildArgs(c, container, true);
    }
}

void buildParams(Context &c, NodeList &container, bool get)
{
    container.push_back(ExprParser::buildExpression(c, get));

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildParams(c, container, true);
    }
}

void buildInits(Context &c, NodeList &container, bool get)
{
    auto tok = c.scanner.match(Token::Type::Id, get);

    auto n = new InitNode(tok.location(), tok.text());
    container.push_back(n);

    c.scanner.consume(Token::Type::LeftParen, true);
    if(c.scanner.token().type() != Token::Type::RightParen)
    {
        buildParams(c, n->params, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);
    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildInits(c, container, true);
    }
}

void buildFunc(Context &c, Qual::Flags quals, BlockNode *block, bool get)
{
    GenericParams generics;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::Lt)
    {
        buildGenerics(c, generics, true);
        c.scanner.consume(Token::Type::Gt, false);
    }

    auto name = CommonParser::extendedName(c, false);

    if(c.containers.back() == Sym::Type::Func)
    {
        throw Error(name->location(), "invalid function - ", name->description());
    }

    auto special = Visitor::query<NameVisitors::SpecialName, Token::Type>(name.get());

    auto n = new FuncNode(name->location(), name);
    block->push_back(n);

    n->setProperty("access", c.access.back());
    n->setProperty("free", quals[Qual::Flag::Free]);
    n->setProperty("explicit", quals[Qual::Flag::Explicit]);
    n->generics = generics;

    tok = c.scanner.consume(Token::Type::LeftParen, false);
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
        if(special != Token::Type::Invalid)
        {
            throw Error(name->location(), "cannot be const - ", name->description());
        }

        n->constMethod = true;
        c.scanner.next(true);
    }

    if(c.scanner.token().type() == Token::Type::Ellipsis && special == Token::Type::RwNew)
    {
        buildInits(c, n->inits, true);
    }

    if(c.scanner.token().type() == Token::Type::LeftBrace && !quals[Qual::Flag::External])
    {
        auto cg = pcx::scoped_push(c.containers, Sym::Type::Func);
        auto ag = pcx::scoped_push(c.access, Access::Private);

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

    n->setProperty("access", c.access.back());

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        auto cg = pcx::scoped_push(c.containers, Sym::Type::Class);
        auto ag = pcx::scoped_push(c.access, Access::Private);

        n->body = CommonParser::blockContents(c, n->location(), false);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }
}

void buildVarImp(Context &c, Qual::Flags quals, BlockNode *block, bool get)
{
    auto name = CommonParser::extendedName(c, get);

    if(c.containers.back() == Sym::Type::Class)
    {
        if(quals[Qual::Flag::External] && !quals[Qual::Flag::Free])
        {
            throw Error(name->location(), "instance member cannot be external - ", name->description());
        }
    }

    if(quals[Qual::Flag::Explicit])
    {
        throw Error(name->location(), "variables cannot be explicit - ", name->description());
    }

    if(quals[Qual::Flag::Free])
    {
        quals |= Qual::Flag::External;
    }

    auto n = new VarNode(name->location(), name);
    block->push_back(n);

    n->setProperty("access", c.access.back());
    n->setProperty("free", quals[Qual::Flag::Free]);
    n->setProperty("external", quals[Qual::Flag::External]);

    if(c.scanner.token().type() == Token::Type::Colon)
    {
        n->type = TypeParser::build(c, true);
    }

    if(c.scanner.token().type() == Token::Type::Assign && c.containers.back() != Sym::Type::Class && !quals[Qual::Flag::External])
    {
        n->value = ExprParser::buildExpression(c, true);
    }

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        buildVarImp(c, quals, block, true);
    }
}

void buildVar(Context &c, Qual::Flags quals, BlockNode *block, bool get)
{
    buildVarImp(c, quals, block, get);
    c.scanner.consume(Token::Type::Semicolon, false);
}

void buildAccess(Context &c, Access type, bool get)
{
    if(c.containers.back() == Sym::Type::Func)
    {
        throw Error(c.scanner.token().location(), "invalid access - ", c.scanner.token().text());
    }

    c.access.back() = type;
    c.scanner.consume(Token::Type::Colon, get);
}

}

void DeclarationParser::build(Context &c, BlockNode *block, bool get)
{
    Qual::Flags quals;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::RwExternal)
    {
        quals |= Qual::Flag::External;
        tok = c.scanner.next(true);
    }

    if(tok.type() == Token::Type::RwFree)
    {
        quals |= Qual::Flag::Free;
        tok = c.scanner.next(true);
    }

    if(tok.type() == Token::Type::RwExplicit)
    {
        quals |= Qual::Flag::Explicit;
        tok = c.scanner.next(true);
    }

    tok = c.scanner.next(false);
    switch(tok.type())
    {
        case Token::Type::RwNamespace: buildNamespace(c, block, true); break;
        case Token::Type::RwFunc: buildFunc(c, quals, block, true); break;
        case Token::Type::RwClass: buildClass(c, block, true); break;
        case Token::Type::RwVar: buildVar(c, quals, block, true); break;

        case Token::Type::RwPublic: buildAccess(c, Access::Public, true); break;
        case Token::Type::RwPrivate: buildAccess(c, Access::Private, true); break;

        default: throw Error(tok.location(), "declaration expected - ", tok.text());
    }
}

void DeclarationParser::buildVarDecl(Context &c, BlockNode *block, bool get)
{
    buildVarImp(c, { }, block, get);
}
