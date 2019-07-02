#include "ByteLister.h"

#include "nodes/CharLiteralNode.h"
#include "nodes/IntLiteralNode.h"

#include <cstring>

namespace
{

template<typename T> void writeValue(T value, std::vector<char> &v)
{
    v.resize(sizeof(T));
    std::memcpy(v.data(), &value, sizeof(T));
}

}

ByteLister::ByteLister()
{
}

void ByteLister::visit(CharLiteralNode &node)
{
    writeValue(node.value, v);
}

void ByteLister::visit(IntLiteralNode &node)
{
    writeValue(node.value, v);
}
