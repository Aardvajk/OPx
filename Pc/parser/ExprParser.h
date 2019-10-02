#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include "nodes/Node.h"

class Context;

namespace ExprParser
{

NodePtr buildExpression(Context &c, bool get);
NodePtr buildExpressionList(Context &c, bool get);

}

#endif // EXPRPARSER_H
