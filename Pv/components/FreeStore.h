#ifndef FREESTORE_H
#define FREESTORE_H

#include <vector>

class Memory;

class FreeStore
{
public:
    FreeStore(Memory &mm, std::size_t start);

    std::size_t allocate(std::size_t size);
    void release(std::size_t pointer);

private:
    Memory &mm;
    std::size_t sp;
};

#endif // FREESTORE_H
