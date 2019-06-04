#ifndef DISASSMBLE_H
#define DISASSMBLE_H

#include <iostream>

class Context;

namespace Disassemble
{

void disassemble(Context &c, std::ostream &os, const char *data, std::size_t size);

}

#endif // DISASSMBLE_H
