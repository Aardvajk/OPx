#ifndef NODE_H
#define NODE_H

#include "framework/PropertyMap.h"

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

    void setProperty(const std::string &name, pcx::any value){ pm.set(name, value); }
    pcx::any getProperty(const std::string &name){ return pm[name]; }

    Location location() const { return n; }
    template<typename T> T property(const std::string &name) const { return pm.get<T>(name); }

private:
    Location n;
    PropertyMap pm;
};

using NodePtr = pcx::scoped_ptr<Node>;
using NodeList = pcx::ptr_vector<Node>;

#endif // NODE_H
