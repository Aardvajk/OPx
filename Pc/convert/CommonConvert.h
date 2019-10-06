#ifndef COMMONCONVERT_H
#define COMMONCONVERT_H

#include "nodes/Node.h"

class Context;
class Type;

namespace CommonConvert
{

NodePtr convert(Context &c, NodePtr node, Type *type);

}

#endif // COMMONCONVERT_H
