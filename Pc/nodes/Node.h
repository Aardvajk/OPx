#ifndef NODE_H
#define NODE_H

#include "scanner/Location.h"

#include <pcx/scoped_ptr.h>
#include <pcx/ptr_vector.h>
#include <pcx/any.h>

#include <string>
#include <unordered_map>

class Visitor;

class Node
{
public:
    explicit Node(Location location);
    virtual ~Node();

    virtual void accept(Visitor &v) = 0;

    void setProperty(const std::string &name, pcx::any value);

    Location location() const { return n; }
    pcx::any property(const std::string &name) const;

private:
    Location n;
    std::unordered_map<std::string, pcx::any> pm;
};

using NodePtr = pcx::scoped_ptr<Node>;
using NodeList = pcx::ptr_vector<Node>;

#endif // NODE_H
