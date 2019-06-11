#ifndef FUNCCONSTRUCTS_H
#define FUNCCONSTRUCTS_H

#include "symbols/Sym.h"

class Context;
class BlockNode;

namespace FuncConstructs
{

void func(Context &c, BlockNode *block, Sym::Attrs attrs, bool get);

}

#endif // FUNCCONSTRUCTS_H
