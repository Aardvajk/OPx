#ifndef LOWERTYPES_H
#define LOWERTYPES_H

class Context;

namespace LowerTypes
{

void convertRefsToPtrs(Context &c);
void convertPtrsToRefs(Context &c);

}

#endif // LOWERTYPES_H
