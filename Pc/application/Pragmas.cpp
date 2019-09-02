#include "Pragmas.h"

#include "application/Context.h"

#include "nodes/PragmaNode.h"

namespace
{

static const char *strings[] = { "push", "pop", "set", "" };

}

const char *Pragmas::toString(Type v)
{
    return strings[static_cast<int>(v)];
}

Pragmas::Type Pragmas::fromString(const std::string &v)
{
    for(int i = 0; strings[i][0]; ++i)
    {
        if(strings[i] == v)
        {
            return static_cast<Type>(i);
        }
    }

    return Type::Invalid;
}

bool Pragmas::takesArgument(Type v)
{
    return v == Type::Set;
}

void Pragmas::execute(Context &c, PragmaNode &node)
{
    switch(node.cmd)
    {
        case Type::Push: c.args.push_back(c.args.back()); break;
        case Type::Pop: c.args.pop_back(); break;
        case Type::Set: c.args.back().process(node.arg); break;

        default: break;
    }
}
