#ifndef NODE_H
#define NODE_H

#include "scanner/Location.h"

#include <pcx/scoped_ptr.h>
#include <pcx/ptr_vector.h>

#include <string>

class Visitor;

class Node
{
public:
    explicit Node(Location location);
    virtual ~Node();

    virtual void accept(Visitor &v) = 0;
    virtual std::string text() const;

    Location location() const { return n; }

private:
    Location n;
};

using NodePtr = pcx::scoped_ptr<Node>;
using NodeList = pcx::ptr_vector<Node>;

#endif // NODE_H
