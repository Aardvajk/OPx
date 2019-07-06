#include "Compiler.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"

#include "compiler/DeclarationConstructs.h"
#include "compiler/CodeConstructs.h"

namespace
{

void codeConstruct(Context &c, BlockNode *block, bool get)
{
    CodeConstructs::entity(c, block, get);
}

}

void Compiler::construct(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwNamespace:
        case Token::Type::RwClass:
        case Token::Type::RwVar:
        case Token::Type::RwFunc: DeclarationConstructs::entity(c, block, false); break;

        default: codeConstruct(c, block, false);
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
