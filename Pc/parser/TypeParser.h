#ifndef TYPEPARSER_H
#define TYPEPARSER_H

#include "nodes/Node.h"

class Context;

namespace TypeParser
{

NodePtr build(Context &c, bool get);

}

#endif // TYPEPARSER_H
