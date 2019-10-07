#ifndef TYPECONVERT_H
#define TYPECONVERT_H

#include <vector>

class Context;
class Type;
class Sym;

namespace TypeConvert
{

enum class Permission
{
    Implicit,
    Explicit
};

std::vector<Sym*> find(Context &c, Type *from, Type *to, Permission permission);

}

#endif // TYPECONVERT_H
