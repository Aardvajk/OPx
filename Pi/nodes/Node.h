#ifndef NODE_H
#define NODE_H

#include "scanner/Location.h"

#include <pcx/scoped_ptr.h>
#include <pcx/ptr_vector.h>

class Visitor;

class Node
{
public:
    Node(Location location) : loc(location) { }
    virtual ~Node();

    virtual void accept(Visitor &visitor) = 0;

    Location location() const { return loc; }

private:
    Location loc;
};

using NodePtr = pcx::scoped_ptr<Node>;
using NodeList = pcx::ptr_vector<Node>;

#endif // NODE_H
