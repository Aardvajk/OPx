#include "CodeConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "compiler/CommonConstructs.h"
#include "compiler/Expr.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"

namespace
{

void exprConstruct(Context &c, BlockNode *block, bool get)
{
    auto n = Expr::get(c, get);
    block->nodes.push_back(new ExprNode(n->location(), n));

    c.scanner.consume(Token::Type::Semicolon, false);
}

void scopeConstruct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    auto sym = c.tree.current()->add(new Sym(Sym::Type::Scope, { }, tok.location(), ""));

    auto g = c.tree.open(sym);

    auto scope = new ScopeNode(tok.location(), sym);
    NodePtr sn(scope);

    scope->block = CommonConstructs::scopeContents(c, tok.location(), false);

    block->nodes.push_back(sn.release());
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
