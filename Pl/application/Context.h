#ifndef CONTEXT_H
#define CONTEXT_H

#include "framework/ByteStream.h"

#include "components/Unit.h"

#include <pcx/optional.h>

class Context
{
public:
    Context();

    pcx::optional<Entity> lookup(const std::string &name);

    std::vector<Unit> units;

    ByteStream ds;
    ByteStream ps;
};

#endif // CONTEXT_H
