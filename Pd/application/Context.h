#ifndef CONTEXT_H
#define CONTEXT_H

#include "common/Object.h"
#include "common/DebugMap.h"

#include <vector>

class Context
{
public:
    Context();

    std::vector<std::vector<char> > segments;
    Object::Unit unit;

    DebugMap dm;
};

#endif // CONTEXT_H
