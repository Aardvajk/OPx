#include "GenericParams.h"

GenericParams::GenericParams()
{
}

bool GenericParams::namesEquivalent(const GenericParams &g) const
{
    return params.size() == g.params.size();
}
