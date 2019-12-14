#include "GenericRef.h"

GenericRef::GenericRef() : index(0), depth(0)
{
}

GenericRef::GenericRef(std::size_t index, std::size_t depth) : index(index), depth(depth)
{
}

bool GenericRef::operator==(const GenericRef &g) const
{
    return index == g.index && depth == g.depth;
}

bool GenericRef::operator!=(const GenericRef &g) const
{
    return index != g.index || depth != g.depth;
}

std::size_t GenericRef::Hash::operator()(const GenericRef &g) const
{
    return g.index ^ g.depth;
}

