#ifndef GENERICPARAMS_H
#define GENERICPARAMS_H

#include "generics/Generic.h"

#include <vector>

class GenericParams
{
public:
    GenericParams();

    operator bool() const { return !params.empty(); }

    bool namesEquivalent(const GenericParams &g) const;
    GenericParams combine(const std::vector<Type*> &types) const;

    std::vector<Generic> params;
};

#endif // GENERICPARAMS_H
