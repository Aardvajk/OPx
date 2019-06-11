#ifndef VARCONSTRUCTS_H
#define VARCONSTRUCTS_H

#include "symbols/Sym.h"

class Context;
class BlockNode;

namespace VarConstructs
{

void var(Context &c, BlockNode *block, Sym::Attrs attrs, bool get);

}

#endif // VARCONSTRUCTS_H
