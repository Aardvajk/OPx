#ifndef CONTEXT_H
#define CONTEXT_H

#include "common/Object.h"
#include "common/DebugMap.h"

#include <vector>

class Context
{
public:
    Context();

    Object::Unit unit;

    DebugMap dm;
};

#endif // CONTEXT_H
