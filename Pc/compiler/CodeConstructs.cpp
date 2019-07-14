#include "CodeConstructs.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"

#include "compiler/CommonConstructs.h"
#include "compiler/ExprConstructs.h"

namespace
{

void scopeConstruct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::LeftBrace, get);

    auto n = new ScopeNode(tok.location());
    block->nodes.push_back(n);

    n->body = CommonConstructs::scopeContents(c, tok.location(), false);
}

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
        case Token::Type::LeftBrace: scopeConstruct(c, block, false); break;

        default: exprConstruct(c, block, false);
    }
}
