#ifndef EXPR_H
#define EXPR_H

#include "nodes/Node.h"

class Context;

namespace Expr
{

NodePtr get(Context &c, bool get);

}

#endif // EXPR_H
