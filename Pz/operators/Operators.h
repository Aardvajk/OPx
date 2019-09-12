#ifndef OPERATORS_H
#define OPERATORS_H

#include "scanner/Token.h"

class Context;

namespace Operators
{

Token scan(Context &c, bool get);

}

#endif // OPERATORS_H
