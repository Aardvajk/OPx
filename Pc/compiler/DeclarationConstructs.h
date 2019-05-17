#ifndef DECLARATIONCONSTRUCTS_H
#define DECLARATIONCONSTRUCTS_H

#include "symbols/Sym.h"

class Context;
class BlockNode;

namespace DeclarationConstructs
{

void declaration(Context &c, BlockNode *block, Sym::Attrs attrs, bool get);

}

#endif // DECLARATIONCONSTRUCTS_H
