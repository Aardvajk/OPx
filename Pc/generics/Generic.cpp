#include "Generic.h"

Generic::Generic(std::string name, Type *type) : name(std::move(name)), type(type)
{
}
