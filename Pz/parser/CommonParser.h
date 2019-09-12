#ifndef COMMONPARSER_H
#define COMMONPARSER_H

#include "nodes/Node.h"

class Context;

namespace CommonParser
{

NodePtr name(Context &c, bool get);
NodePtr extendedName(Context &c, bool get);

}

#endif // COMMONPARSER_H
