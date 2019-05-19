#ifndef SYMSTACK_H
#define SYMSTACK_H

#include "symbols/Sym.h"

#include <pcx/ptr_vector.h>

#include <iostream>

class SymStack
{
public:
    SymStack();

    void push();
    void pop();

    Sym *add(Sym *sym);

    Sym *find(const std::string &name);
    Sym *findLocal(const std::string &name);

    void print(std::ostream &os);

private:
    Sym *find(const std::string &name, std::size_t limit);

    pcx::ptr_vector<Sym> v;
    std::vector<std::size_t> sc;
};

#endif // SYMSTACK_H
