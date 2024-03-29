#ifndef GENERICSTACK_H
#define GENERICSTACK_H

#include "generics/GenericParams.h"
#include "generics/GenericRef.h"

#include <pcx/optional.h>

class Node;

class GenericStack
{
public:
    using value_type = GenericParams;

    GenericStack();

    void push_back(const GenericParams &g){ v.push_back(g); }
    void pop_back(){ v.pop_back(); }

    pcx::optional<GenericRef> typeRef(Node *name) const;
    Type *type(const GenericRef &ref) const;

    Type *convert(Context &c, const Type *t) const;

    std::vector<Type*> currentTypes() const;

    void dump(std::ostream &os) const;

private:
    std::vector<GenericParams> v;
};

#endif // GENERICSTACK_H
