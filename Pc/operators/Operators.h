#ifndef OPERATORS_H
#define OPERATORS_H

#include "scanner/Token.h"

#include <string>

class Context;

namespace Operators
{

std::string scan(Context &c, bool get);
Token opEqToOp(const Token &token);

}

#endif // OPERATORS_H
