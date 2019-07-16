#include "Node.h"

Node::Node(Location location) : n(location)
{
}

Node::~Node()
{
}

void Node::setProperty(const std::string &name, pcx::any value)
{
    pm.set(name, value);
}

pcx::any Node::getProperty(const std::string &name)
{
    return pm[name];
}
