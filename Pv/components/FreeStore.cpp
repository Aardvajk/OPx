#include "FreeStore.h"

#include <iostream>

FreeStore::FreeStore(Memory &mm, std::size_t start) : mm(mm), sp(start)
{
}

std::size_t FreeStore::allocate(std::size_t size)
{
    auto r = sp;
    sp += size;

    return r;
}

void FreeStore::release(std::size_t pointer)
{
}
