#include "Sym.h"

Sym::Sym() : type(Type::Invalid)
{
}

Sym::Sym(Type type, std::string name) : type(type), name(std::move(name))
{
}

const char *Sym::toString(Type type)
{
    static const char *s[] = { "global", "func", "arg", "local", "label", "(invalid)" };
    return s[static_cast<int>(type)];
}
