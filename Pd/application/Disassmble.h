#ifndef DISASSMBLE_H
#define DISASSMBLE_H

#include <iostream>

namespace Disassemble
{

void disassemble(std::ostream &os, const char *data, std::size_t size);

}

#endif // DISASSMBLE_H
