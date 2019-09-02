#ifndef PRAGMAS_H
#define PRAGMAS_H

#include <string>

class Context;
class PragmaNode;

namespace Pragmas
{

enum class Type
{
    Push,
    Pop,
    Set,

    Invalid
};

const char *toString(Type v);
Type fromString(const std::string &v);

bool takesArgument(Type v);

void execute(Context &c, PragmaNode &node);

}

#endif // PRAGMAS_H
