#include "DeclarationParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"

#include "parser/CommonParser.h"

void DeclarationParser::buildNamespace(Context &c, BlockNode *block, bool get)
{
    auto nm = CommonParser::name(c, get);

    auto n = new NamespaceNode(nm->location(), nm);
    block->push_back(n);

    n->body = CommonParser::blockContents(c, n->location(), false);
}
