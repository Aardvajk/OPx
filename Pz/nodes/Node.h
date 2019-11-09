#ifndef NODE_H
#define NODE_H

#include "framework/Error.h"

#include "scanner/Location.h"

#include <pcx/shared_ptr.h>
#include <pcx/any.h>

#include <vector>
#include <string>
#include <unordered_map>

class Node;

using NodePtr = pcx::shared_ptr<Node>;
using NodeList = std::vector<pcx::shared_ptr<Node> >;

class Visitor;
class BlockNode;

class Node
{
public:
    explicit Node(Location location);
    virtual ~Node();

    NodePtr clone() const;

    virtual void accept(Visitor &v) = 0;
    virtual std::string classname() const = 0;
    virtual NodePtr cloneDetail() const = 0;

    void setProperty(const std::string &key, const pcx::any &value);
    pcx::any findProperty(const std::string &key) const;

    std::string description() const;
    Location location() const;

    template<typename T> T property(const std::string &key) const;

protected:
    template<typename T> static T copyDetails(const Node *from, T to){ to->pm = from->pm; return to; }

    static NodePtr clonePtr(const NodePtr &n);
    static NodeList cloneList(const NodeList &n);

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
        throw Error(location(), "bad property read on ", classname(), "node ", description(), " - ", key);
    }

    return i->second.to<T>();
}

#endif // NODE_H
