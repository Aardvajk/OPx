#include "BlockNode.h"

#include "visitors/Visitor.h"

void BlockNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string BlockNode::classname() const
{
    return "block";
}

NodePtr BlockNode::cloneDetail() const
{
    auto b = copyDetails(this, new BlockNode(location()));
    NodePtr n(b);

    for(auto n: nodes)
    {
        b->push_back(clonePtr(n));
    }

    return n;
}

void BlockNode::push_back(Node *node)
{
    nodes.push_back(node);
    node->bn = this;
}

void BlockNode::push_back(NodePtr node)
{
    nodes.push_back(node);
    node->bn = this;
}

void BlockNode::insert(std::size_t index, Node *node)
{
    nodes.insert(nodes.begin() + static_cast<long long>(index), node);
    node->bn = this;
}

void BlockNode::insert(std::size_t index, NodePtr node)
{
    nodes.insert(nodes.begin() + static_cast<long long>(index), node);
    node->bn = this;
}

