#include "ByteWriter.h"

#include "framework/Error.h"

#include "nodes/LiteralNode.h"
#include "nodes/StringNode.h"
#include "nodes/IdNode.h"
#include "nodes/AddrOfNode.h"

namespace
{

template<typename T> void writeBytes(std::vector<char> &v, const T &value)
{
    const char *s = reinterpret_cast<const char*>(&value);

    for(std::size_t i = 0; i < sizeof(T); ++i)
    {
        v.push_back(s[i]);
    }
}

}

ByteWriter::ByteWriter(std::vector<char> &v) : v(v)
{
}

void ByteWriter::visit(LiteralNode<char> &node)
{
    writeBytes(v, node.value);
    ok = true;
}

void ByteWriter::visit(LiteralNode<int> &node)
{
    writeBytes(v, node.value);
    ok = true;
}

void ByteWriter::visit(LiteralNode<std::size_t> &node)
{
    writeBytes(v, node.value);
    ok = true;
}

void ByteWriter::visit(StringNode &node)
{
    for(auto ch: node.value)
    {
        v.push_back(ch);
    }

    ok = true;
}
