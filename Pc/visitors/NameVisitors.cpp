#include "NameVisitors.h"

#include "nodes/Node.h"

IsNameSimple::IsNameSimple() : r(false)
{
}

void IsNameSimple::visit(IdNode &node)
{
    r = true;
}

bool isNameSimple(Node *node)
{
    IsNameSimple is;
    node->accept(is);

    return is.result();
}
