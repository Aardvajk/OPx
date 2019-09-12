#ifndef PARSER_H
#define PARSER_H

#include "nodes/Node.h"

class Context;

namespace Parser
{

void construct(Context &c, BlockNode *block, bool get);

NodePtr build(Context &c);

}

#endif // PARSER_H
