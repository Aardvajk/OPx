#include "Compiler.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"

#include "compiler/IncludeConstructs.h"
#include "compiler/PragmaConstructs.h"
#include "compiler/DeclarationConstructs.h"
#include "compiler/FuncConstructs.h"

#include <pcx/scoped_push.h>

void Compiler::construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwInclude: IncludeConstructs::entity(c, block, false); break;
        case Token::Type::RwPragma: PragmaConstructs::entity(c, block, false); break;

        case Token::Type::RwNamespace:
        case Token::Type::RwClass:
        case Token::Type::RwVar:
        case Token::Type::RwFunc: DeclarationConstructs::entity(c, block, false); break;

        default: FuncConstructs::entity(c, block, false);
    }
}

NodePtr Compiler::compile(Context &c)
{
    auto block = new BlockNode(Location());
    NodePtr n(block);

    auto cg = pcx::scoped_push(c.containers, Sym::Type::Namespace);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, block, false);
    }

    return n;
}
