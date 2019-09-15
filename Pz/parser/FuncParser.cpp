#include "FuncParser.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ExprNode.h"

#include "parser/ExprParser.h"

namespace
{

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
        default: buildExpr(c, block, false);
    }
}
