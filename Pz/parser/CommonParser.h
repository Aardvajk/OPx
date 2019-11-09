#ifndef COMMONPARSER_H
#define COMMONPARSER_H

#include "nodes/Node.h"

class Context;

namespace CommonParser
{

NodePtr name(Context &c, bool get);

NodePtr blockContents(Context &c, Location location, bool get);

}

#endif // COMMONPARSER_H
