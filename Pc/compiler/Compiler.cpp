#include "Compiler.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"

#include "compiler/CommonConstructs.h"
#include "compiler/DeclarationConstructs.h"
#include "compiler/CodeConstructs.h"
#include "compiler/TestConstructs.h"

namespace
{

void code(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    if(c.tree.current()->container()->type() != Sym::Type::Func)
    {
        throw Error(tok.location(), "declaration expected - ", tok.text());
    }

    CodeConstructs::entity(c, block, false);
}

}

void Compiler::construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace:
        case Token::Type::RwClass:
        case Token::Type::RwUsing:
        case Token::Type::RwVar:
        case Token::Type::RwFunc: DeclarationConstructs::entity(c, block, Sym::defaultAttrs(c.tree.current()->type()), false); break;

        case Token::Type::RwPublic: DeclarationConstructs::entity(c, block, Sym::Attr::Public, true); break;
        case Token::Type::RwPrivate: DeclarationConstructs::entity(c, block, Sym::Attr::Private, true); break;

        case Token::Type::RwLookup: TestConstructs::lookup(c, true); break;
        case Token::Type::RwTriggerError: TestConstructs::triggerError(c, block, true); break;

        default: code(c, block, false);
    }
}

NodePtr Compiler::compile(Context &c)
{
    auto block = new BlockNode(Location());
    NodePtr n(block);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, block, false);
    }

    return n;
}
