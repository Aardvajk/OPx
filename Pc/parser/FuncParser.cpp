#include "FuncParser.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/WhileNode.h"

#include "parser/CommonParser.h"
#include "parser/ExprParser.h"

namespace
{

void buildReturn(Context &c, BlockNode *block, bool get)
{
    auto n = new ReturnNode(c.scanner.token().location());
    block->push_back(n);

    auto tok = c.scanner.next(get);
    if(tok.type() != Token::Type::Semicolon)
    {
        n->expr = ExprParser::build(c, false);
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

void buildScope(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::LeftBrace, get);

    auto n = new ScopeNode(tok.location());
    block->push_back(n);

    n->body = CommonParser::blockContents(c, tok.location(), false);
}

void buildWhile(Context &c, BlockNode *block, bool get)
{
    auto n = new WhileNode(c.scanner.token().location());
    block->push_back(n);

    c.scanner.match(Token::Type::LeftParen, get);
    n->expr = ExprParser::build(c, true);
    c.scanner.match(Token::Type::RightParen, false);

    n->body = CommonParser::scopeContents(c, n->location(), true);
}

void buildExpr(Context &c, BlockNode *block, bool get)
{
    auto n = new ExprNode(c.scanner.token().location());
    block->push_back(n);

    n->expr = ExprParser::build(c, get);

    c.scanner.consume(Token::Type::Semicolon, false);
}

}

void FuncParser::build(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);

    if(c.containers.back() != Sym::Type::Func)
    {
        throw Error(tok.location(), "declaration expected - ", tok.text());
    }

    switch(tok.type())
    {
        case Token::Type::RwReturn: buildReturn(c, block, true); break;
        case Token::Type::LeftBrace: buildScope(c, block, false); break;

        case Token::Type::RwWhile: buildWhile(c, block, true); break;

        default: buildExpr(c, block, false);
    }
}
