#include "Node.h"

Node::Node(Location location) : n(location)
{
}

Node::~Node()
{
}

void Node::setProperty(const std::string &name, pcx::any value)
{
    pm[name] = value;
}

pcx::any Node::property(const std::string &name) const
{
    auto i = pm.find(name);
    return i == pm.end() ? pcx::any() : i->second;
}
