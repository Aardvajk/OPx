#ifndef NODE_H
#define NODE_H

#include "framework/Error.h"
#include "framework/PropertyMap.h"

#include "scanner/Location.h"

#include <pcx/shared_ptr.h>
#include <pcx/any.h>

#include <vector>
#include <unordered_map>

class Visitor;
class BlockNode;

class Node
{
public:
    Node(Location location);
    virtual ~Node();

    virtual void accept(Visitor &v) = 0;
    virtual const char *classname() const = 0;

    void setProperty(const std::string &key, const pcx::any &value);
    pcx::any findProperty(const std::string &key) const;

    std::string description() const;

    Location location() const { return n; }
    BlockNode *block() const { return bn; }

    template<typename T> T property(const std::string &key) const;

private:
    friend class BlockNode;

    Location n;
    BlockNode *bn;

    std::unordered_map<std::string, pcx::any> pm;
};

template<typename T> T Node::property(const std::string &key) const
{
    auto i = pm.find(key);
    if(i == pm.end())
    {
        throw Error("bad property read on ", classname(), " node ", description(), " - ", key);
    }

    return i->second.to<T>();
}

using NodePtr = pcx::shared_ptr<Node>;
using NodeList = std::vector<pcx::shared_ptr<Node> >;

#endif // NODE_H
