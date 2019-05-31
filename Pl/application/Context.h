#ifndef CONTEXT_H
#define CONTEXT_H

#include "framework/ByteStream.h"
#include "framework/Comments.h"

#include "components/Unit.h"

#include <pcx/optional.h>

class Context
{
public:
    Context();

    std::size_t getDataPosition() const;

    pcx::optional<Entity> lookup(const std::string &name);

    std::vector<Unit> units;

    ByteStream ds;
    ByteStream ps;

    Comments dataComments;
};

#endif // CONTEXT_H
