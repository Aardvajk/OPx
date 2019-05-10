#ifndef COMPILER_H
#define COMPILER_H

#include "nodes/Node.h"

class Context;

NodePtr compile(Context &c);

#endif // COMPILER_H
