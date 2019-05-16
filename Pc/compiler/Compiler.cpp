#include "Compiler.h"

#include "application/Error.h"
#include "application/Context.h"

#include "nodes/BlockNode.h"

#include "compiler/CommonConstructs.h"
#include "compiler/DeclarationConstructs.h"
#include "compiler/TestConstructs.h"

#include <pcx/scoped_push.h>

void Compiler::construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace:
        case Token::Type::RwClass:
        case Token::Type::RwUsing: DeclarationConstructs::declarationConstruct(c, block, Sym::defaultAttrs(c.tree.current()->type()), false); break;

        case Token::Type::RwPublic: DeclarationConstructs::declarationConstruct(c, block, Sym::Attr::Public, true); break;
        case Token::Type::RwPrivate: DeclarationConstructs::declarationConstruct(c, block, Sym::Attr::Private, true); break;

        case Token::Type::RwLookup: TestConstructs::lookup(c, true); break;
        case Token::Type::RwTriggerError: TestConstructs::triggerError(c, block, true); break;

        default: throw Error(tok.location(), "construct expected - ", tok.text());
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
