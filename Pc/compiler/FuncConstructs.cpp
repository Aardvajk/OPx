#include "FuncConstructs.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/WhileNode.h"
#include "nodes/IfNode.h"
#include "nodes/ForNode.h"
#include "nodes/BreakNode.h"

#include "compiler/CommonConstructs.h"
#include "compiler/ExprConstructs.h"
#include "compiler/DeclarationConstructs.h"

namespace
{

void returnConstruct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwReturn, get);

    auto n = new ReturnNode(tok.location());
    block->push_back(n);

    c.scanner.next(true);
    if(c.scanner.token().type() != Token::Type::Semicolon)
    {
        n->expr = ExprConstructs::expr(c, false);
    }
    else
    {
        n->expr = new NullLiteralNode(tok.location());
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void whileConstruct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwWhile, get);

    auto n = new WhileNode(tok.location());
    block->push_back(n);

    c.scanner.match(Token::Type::LeftParen, true);
    n->expr = ExprConstructs::expr(c, true);
    c.scanner.match(Token::Type::RightParen, false);

    n->body = CommonConstructs::loopScopeContents(c, tok.location(), true);
}

void ifConstruct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwIf, get);

    auto n = new IfNode(tok.location());
    block->push_back(n);

    c.scanner.match(Token::Type::LeftParen, true);
    n->expr = ExprConstructs::expr(c, true);
    c.scanner.match(Token::Type::RightParen, false);

    n->body = CommonConstructs::scopeContents(c, tok.location(), true);

    if(c.scanner.token().type() == Token::Type::RwElse)
    {
        n->elseBody = CommonConstructs::scopeContents(c, tok.location(), true);
    }
}

void forConstruct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwFor, get);

    auto sc = new ScopeNode(tok.location());
    block->push_back(sc);

    auto bn = new BlockNode(tok.location());
    sc->body = bn;

    auto n = new ForNode(tok.location());
    bn->push_back(n);

    c.scanner.consume(Token::Type::LeftParen, true);

    if(c.scanner.token().type() != Token::Type::Semicolon)
    {
        if(c.scanner.token().type() == Token::Type::RwVar)
        {
            auto b = new BlockNode(tok.location());
            n->init = b;

            DeclarationConstructs::var(c, b, true);
        }
        else
        {
            n->init = ExprConstructs::expr(c, false);
        }
    }

    c.scanner.next(true);

    if(c.scanner.token().type() != Token::Type::Semicolon)
    {
        n->cond = ExprConstructs::expr(c, false);
    }

    c.scanner.next(true);

    if(c.scanner.token().type() != Token::Type::RightParen)
    {
        n->post = ExprConstructs::expr(c, false);
    }

    c.scanner.match(Token::Type::RightParen, false);

    n->body = CommonConstructs::loopScopeContents(c, tok.location(), true);
}

void breakConstruct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwBreak, get);

    block->push_back(new BreakNode(tok.location()));

    c.scanner.consume(Token::Type::Semicolon, true);
}

void scopeConstruct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::LeftBrace, get);

    auto n = new ScopeNode(tok.location());
    block->push_back(n);

    n->body = CommonConstructs::blockContents(c, tok.location(), false);
}

void exprConstruct(Context &c, BlockNode *block, bool get)
{
    auto ex = ExprConstructs::expr(c, get);

    auto n = new ExprNode(ex->location(), ex);
    block->push_back(n);

    c.scanner.consume(Token::Type::Semicolon, false);
}

}

void FuncConstructs::entity(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    if(c.containers.back() != Sym::Type::Func)
    {
        throw Error(tok.location(), "declaration expected - ", tok.text());
    }

    switch(tok.type())
    {
        case Token::Type::RwReturn: returnConstruct(c, block, false); break;
        case Token::Type::RwWhile: whileConstruct(c, block, false); break;
        case Token::Type::RwIf: ifConstruct(c, block, false); break;
        case Token::Type::RwFor: forConstruct(c, block, false); break;
        case Token::Type::RwBreak: breakConstruct(c, block, false); break;

        case Token::Type::LeftBrace: scopeConstruct(c, block, false); break;

        default: exprConstruct(c, block, false);
    }
}
