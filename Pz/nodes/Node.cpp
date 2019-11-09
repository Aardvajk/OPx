#include "Node.h"

Node::Node(Location location) : n(location), bn(nullptr)
{
}

Node::~Node()
{
}

NodePtr Node::clone() const
{
    auto n = cloneDetail();

    n->pm = pm;
    return n;
}

void Node::setProperty(const std::string &key, const pcx::any &value)
{
    pm[key] = value;
}

pcx::any Node::findProperty(const std::string &key) const
{
    auto i = pm.find(key);
    return i == pm.end() ? pcx::any() : i->second;
}

std::string Node::description() const
{
    return { };
}

Location Node::location() const
{
    return n;
}

NodePtr Node::clonePtr(const NodePtr &n)
{
    if(n)
    {
        return n->clone();
    }

    return { };
}

NodeList Node::cloneList(const NodeList &n)
{
    NodeList r;
    for(auto &i: n)
    {
        r.push_back(clonePtr(i));
    }

    return r;
}
