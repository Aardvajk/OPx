#ifndef COMMONCONSTRUCTS_H
#define COMMONCONSTRUCTS_H

#include "nodes/Node.h"

class Context;

namespace CommonConstructs
{

NodePtr name(Context &c, bool get);
NodePtr extendedName(Context &c, bool get);

NodePtr blockContents(Context &c, Location location, bool get);
NodePtr funcContents(Context &c, Location location, bool get);
NodePtr scopeContents(Context &c, Location location, bool get);

}

#endif // COMMONCONSTRUCTS_H
