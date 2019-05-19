#ifndef SYM_H
#define SYM_H

#include "scanner/Location.h"

#include <pcx/any.h>

#include <string>
#include <unordered_map>

class Sym
{
public:
    enum class Type { Var, Func, Arg, Invalid };

    Sym();
    Sym(Type type, std::string name);

    Type type;
    std::string name;
    std::unordered_map<std::string, pcx::any> properties;
};

#endif // SYM_H
