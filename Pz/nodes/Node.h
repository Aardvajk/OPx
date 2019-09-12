#ifndef NODE_H
#define NODE_H

#include "framework/PropertyMap.h"

#include "scanner/Location.h"

#include <pcx/shared_ptr.h>

#include <vector>

class Visitor;

class Node
{
public:
    explicit Node(Location location);
    virtual ~Node();

    virtual void accept(Visitor &v) = 0;

    Location location() const { return n; }

private:
    Location n;
};

using NodePtr = pcx::shared_ptr<Node>;
using NodeList = std::vector<pcx::shared_ptr<Node> >;

#endif // NODE_H
