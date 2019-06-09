#ifndef MEMORY_H
#define MEMORY_H

#include <vector>

class Memory
{
public:
    Memory(std::size_t size);

    char *operator()(std::size_t address){ return v.data() + address; }

    const char *operator[](std::size_t address) const { return v.data() + address; }

    std::size_t size() const { return v.size(); }

private:
    std::vector<char> v;
};

#endif // MEMORY_H
