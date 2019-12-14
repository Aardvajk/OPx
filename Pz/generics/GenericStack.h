#ifndef GENERICSTACK_H
#define GENERICSTACK_H

#include "types/GenericRef.h"

#include "generics/GenericParamList.h"

#include <pcx/optional.h>

class Node;

class GenericStack
{
public:
    using value_type = GenericParamList;

    GenericStack();

    void push_back(const GenericParamList &g);
    void pop_back();

    pcx::optional<GenericRef> typeRef(Node *name) const;

private:
    std::vector<GenericParamList> v;
};

#endif // GENERICSTACK_H
