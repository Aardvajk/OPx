#ifndef COMPILER_H
#define COMPILER_H

#include "nodes/Node.h"

class Context;
class BlockNode;

namespace Compiler
{

void construct(Context &c, BlockNode *block, bool get);

NodePtr compile(Context &c);

}

#endif // COMPILER_H
