#ifndef CONTEXT_H
#define CONTEXT_H

#include "common/Object.h"

#include <vector>

class Context
{
public:
    Context();

    std::vector<std::vector<char> > segments;
    Object::Unit unit;
};

#endif // CONTEXT_H
