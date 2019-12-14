#ifndef TYPECOMPARE_H
#define TYPECOMPARE_H

#include <vector>

class Type;

namespace TypeCompare
{

bool compatible(const Type *a, const Type *b);
bool compatible(const std::vector<Type*> &a, const std::vector<Type*> &b);

bool exact(const Type *a, const Type *b);
bool exact(const std::vector<Type*> &a, const std::vector<Type*> &b);

}

#endif // TYPECOMPARE_H
