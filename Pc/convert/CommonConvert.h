#ifndef COMMONCONVERT_H
#define COMMONCONVERT_H

#include "nodes/Node.h"

#include "types/TypeConvert.h"

class Context;
class Type;

namespace CommonConvert
{

NodePtr convert(Context &c, NodePtr node, Type *type, TypeConvert::Permission permission);
NodePtr convertImplicitBoolean(Context &c, NodePtr node);

}

#endif // COMMONCONVERT_H
