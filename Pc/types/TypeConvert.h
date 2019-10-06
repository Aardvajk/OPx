#ifndef TYPECONVERT_H
#define TYPECONVERT_H

#include <vector>

class Context;
class Type;
class Sym;

namespace TypeConvert
{

std::vector<Sym*> find(Context &c, Type *from, Type *to);

}

#endif // TYPECONVERT_H
