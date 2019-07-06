#include "CodeConstructs.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ExprNode.h"

#include "compiler/ExprConstructs.h"

namespace
{

void exprConstruct(Context &c, BlockNode *block, bool get)
{
    auto ex = ExprConstructs::expr(c, get);

    auto n = new ExprNode(ex->location(), ex);
    block->nodes.push_back(n);

    c.scanner.consume(Token::Type::Semicolon, false);
}

}

void CodeConstructs::entity(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        default: exprConstruct(c, block, false);
    }
}
