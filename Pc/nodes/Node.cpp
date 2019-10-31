#include "Node.h"

#include "visitors/DescVisitor.h"

Node::Node(Location location) : n(location), bn(nullptr)
{
}

Node::~Node()
{
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
    return Visitor::query<DescVisitor, std::string>(const_cast<Node*>(this));
}

Node *Node::safeClone(const NodePtr &n)
{
    if(n)
    {
        auto r = n->clone();
        r->pm = n->pm;

        return r;
    }

    return nullptr;
}

NodeList Node::listClone(const NodeList &n)
{
    NodeList r;
    for(auto &i: n)
    {
        r.push_back(safeClone(i));
    }

    return r;
}
