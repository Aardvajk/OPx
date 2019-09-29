#ifndef SYMPRINTER_H
#define SYMPRINTER_H

#include <iostream>

class Context;
class Sym;

namespace SymPrinter
{

void print(Context &c, const Sym *root, std::ostream &os);

}

#endif // SYMPRINTER_H
