#include "Sym.h"

Sym::Sym() : type(Type::Invalid)
{
}

Sym::Sym(Type type, std::string name) : type(type), name(std::move(name))
{
}
