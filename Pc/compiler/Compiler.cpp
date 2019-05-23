#include "Compiler.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"

#include "compiler/CommonConstructs.h"
#include "compiler/DeclarationConstructs.h"
#include "compiler/TestConstructs.h"

#include "compiler/TypeConstructs.h"

#include <pcx/scoped_push.h>

namespace
{

void typeTest(Context &c, bool get)
{
    auto tn = TypeConstructs::type(c, get);
    std::cout << tn->text() << "\n";

    c.scanner.consume(Token::Type::Semicolon, false);
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

        default: typeTest(c, false);
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
