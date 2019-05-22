#ifndef MEMORY_H
#define MEMORY_H

#include <vector>

class Memory
{
public:
    explicit Memory(std::size_t size);

    char *operator()(std::size_t addr){ return v.data() + addr; }

    std::size_t size() const { return v.size(); }

private:
    std::vector<char> v;
};

#endif // MEMORY_H
