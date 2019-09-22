#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include "nodes/Node.h"

class Context;

namespace ExprParser
{

NodePtr build(Context &c, bool get);

}

#endif // EXPRPARSER_H
