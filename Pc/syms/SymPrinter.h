#ifndef SYMPRINTER_H
#define SYMPRINTER_H

#include <iostream>

class Sym;

namespace SymPrinter
{

void print(const Sym *root, std::ostream &os);

}

#endif // SYMPRINTER_H
