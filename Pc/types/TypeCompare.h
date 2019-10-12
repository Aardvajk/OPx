#ifndef TYPECOMPARE_H
#define TYPECOMPARE_H

#include <vector>

class Context;
class Type;

class TypeCompare
{
public:
    TypeCompare(Context &c);

    bool compatible(const Type *a, const Type *b) const;
    bool compatibleArgs(const std::vector<Type*> &a, const std::vector<Type*> &b) const;

    bool exact(const Type *a, const Type *b) const;
    bool exactArgs(const Type *a, const Type *b) const;

    bool convertible(Type *from, Type *to) const;

private:
    Context &c;
};

#endif // TYPECOMPARE_H
