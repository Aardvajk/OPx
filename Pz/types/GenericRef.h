#ifndef GENERICREF_H
#define GENERICREF_H

#include <cstdlib>

class GenericRef
{
public:
    GenericRef();
    GenericRef(std::size_t index, std::size_t depth);

    bool operator==(const GenericRef &g) const;
    bool operator!=(const GenericRef &g) const;

    struct Hash
    {
        std::size_t operator()(const GenericRef &g) const;
    };

    std::size_t index;
    std::size_t depth;
};

#endif // GENERICREF_H
