#ifndef GENERICREF_H
#define GENERICREF_H

#include <cstdlib>
#include <unordered_set>

class GenericRef
{
public:
    GenericRef(){ }
    GenericRef(std::size_t index, std::size_t depth) : index(index), depth(depth) { }

    bool operator==(const GenericRef &r) const { return index == r.index && depth == r.depth; }
    bool operator!=(const GenericRef &r) const { return index != r.index || depth != r.depth; }

    std::size_t index;
    std::size_t depth;
};

class GenericRefHash
{
public:
    GenericRefHash(){ }

    std::size_t operator()(const GenericRef &r) const { return r.index ^ r.depth; }
};

#endif // GENERICREF_H
