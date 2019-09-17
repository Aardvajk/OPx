#include "Pragmas.h"

#include "application/Context.h"

#include "nodes/PragmaNode.h"

#include <vector>
#include <algorithm>

namespace
{

static const std::vector<std::string> strings = { "push", "pop", "set" };

}

std::string Pragmas::toString(Type v)
{
    return strings[static_cast<std::size_t>(v)];
}

Pragmas::Type Pragmas::fromString(const std::string &v)
{
    auto i = std::find(strings.begin(), strings.end(), v);
    return i == strings.end() ? Type::Invalid : static_cast<Type>(i - strings.begin());
}

void Pragmas::execute(Context &c, PragmaNode &node)
{
    switch(node.type)
    {
        case Type::Push: c.args.push_back(c.args.back()); break;
        case Type::Pop: c.args.pop_back(); break;
        case Type::Set: c.args.back().process(node.arg); break;

        default: break;
    }
}
