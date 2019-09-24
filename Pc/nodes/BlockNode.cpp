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

void BlockNode::push_back(Node *node)
{
    nodes.push_back(node);
    node->bn = this;
}

void BlockNode::push_back(NodePtr &node)
{
    nodes.push_back(node);
    node->bn = this;
}

void BlockNode::insert(std::size_t index, Node *node)
{
    nodes.insert(nodes.begin() + static_cast<long long>(index), node);
    node->bn = this;
}

void BlockNode::insert(std::size_t index, NodePtr &node)
{
    nodes.insert(nodes.begin() + static_cast<long long>(index), node);
    node->bn = this;
}

std::size_t BlockNode::indexOf(Node *node) const
{
    for(std::size_t i = 0; i < nodes.size(); ++i)
    {
        if(nodes[i].get() == node)
        {
            return i;
        }
    }

    return { };
}

std::size_t BlockNode::size() const
{
    return nodes.size();
}
