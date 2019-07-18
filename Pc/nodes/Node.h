#ifndef NODE_H
#define NODE_H

#include "framework/PropertyMap.h"

#include "scanner/Location.h"

#include <pcx/shared_ptr.h>
#include <pcx/any.h>

#include <vector>
#include <string>
#include <unordered_map>

class Visitor;
class BlockNode;

class Node
{
public:
    explicit Node(Location location);
    virtual ~Node();

    virtual void accept(Visitor &v) = 0;

    void setProperty(const std::string &name, pcx::any value){ pm.set(name, value); }
    pcx::any getProperty(const std::string &name){ return pm[name]; }

    Location location() const { return n; }
    BlockNode *block() const { return bn; }

    template<typename T> T property(const std::string &name) const { return pm.get<T>(name); }

private:
    friend class BlockNode;

    Location n;
    BlockNode *bn;
    PropertyMap pm;
};

using NodePtr = pcx::shared_ptr<Node>;
using NodeList = std::vector<pcx::shared_ptr<Node> >;

#endif // NODE_H
