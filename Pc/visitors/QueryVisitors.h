#ifndef QUERYVISITORS_H
#define QUERYVISITORS_H

#include "visitors/Visitor.h"

#include "nodes/Node.h"

#include <unordered_map>

class Type;

namespace QueryVisitors
{

class DirectType : public Visitor
{
public:
    DirectType();

    Type *result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    Type *r;
};

class GetParent : public Visitor
{
public:
    GetParent();

    NodePtr result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    NodePtr r;
};

class GetBlockNode : public Visitor
{
public:
    GetBlockNode();

    BlockNode *result() const { return r; }

    virtual void visit(BlockNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(ClassNode &node) override;

private:
    BlockNode *r;
};

class GetConstructNode : public Visitor
{
public:
    GetConstructNode();

    ConstructNode *result() const { return r; }

    virtual void visit(ConstructNode &node) override;

private:
    ConstructNode *r;
};

class InitNodeMap : public Visitor
{
public:
    InitNodeMap(std::unordered_map<std::string, NodePtr> &m, NodePtr &n);

    virtual void visit(InitNode &node) override;

private:
    std::unordered_map<std::string, NodePtr> &m;
    NodePtr &n;
};

}

#endif // QUERYVISITORS_H
