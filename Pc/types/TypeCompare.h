#ifndef TYPECOMPARE_H
#define TYPECOMPARE_H

class Type;

namespace TypeCompare
{

bool compatible(const Type *a, const Type *b);
bool compatibleArgs(const Type *a, const Type *b);

bool exact(const Type *a, const Type *b);
bool exactArgs(const Type *a, const Type *b);

}

#endif // TYPECOMPARE_H
