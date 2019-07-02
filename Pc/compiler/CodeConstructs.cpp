#include "CodeConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "compiler/CommonConstructs.h"
#include "compiler/Expr.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/NullLiteralNode.h"

#include "visitors/TypeVisitor.h"

#include "types/TypeCompare.h"

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

void returnConstruct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::RwReturn, get);

    auto n = new ReturnNode(tok.location());
    block->nodes.push_back(n);

    tok = c.scanner.next(true);
    if(tok.type() != Token::Type::Semicolon)
    {
        n->expr = Expr::get(c, false);
    }
    else
    {
        n->expr = new NullLiteralNode(tok.location());
    }

    TypeVisitor tv(c);
    n->expr->accept(tv);

    auto rt = c.tree.current()->container()->property("type").to<const Type*>()->returnType.get();

    if(!TypeCompare::exact(tv.result(), rt))
    {
        throw Error(n->expr->location(), "mismatched return type - ", tv.result()->text());
    }

    c.scanner.consume(Token::Type::Semicolon, false);
}

}

void CodeConstructs::entity(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::LeftBrace: scopeConstruct(c, block, false); break;

        case Token::Type::RwReturn: returnConstruct(c, block, false); break;

        default: exprConstruct(c, block, false);
    }
}
