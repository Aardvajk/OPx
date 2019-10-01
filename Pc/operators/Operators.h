#ifndef OPERATORS_H
#define OPERATORS_H

#include "nodes/Node.h"

#include <string>

class Context;

namespace Operators
{

NodePtr scan(Context &c, bool get);

}

#endif // OPERATORS_H
