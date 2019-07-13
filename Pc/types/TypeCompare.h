#ifndef TYPECOMPARE_H
#define TYPECOMPARE_H

class Type;

namespace TypeCompare
{

bool exact(const Type *a, const Type *b);
bool args(const Type *a, const Type *b);

}

#endif // TYPECOMPARE_H
