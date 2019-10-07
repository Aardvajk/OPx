#ifndef OPERATORS_H
#define OPERATORS_H

#include "scanner/Token.h"

#include "nodes/Node.h"

#include <string>

class Context;

namespace Operators
{

NodePtr scan(Context &c, bool get);
Token opEqToOp(const Token &token);

}

#endif // OPERATORS_H
