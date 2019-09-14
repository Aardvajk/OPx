#ifndef TYPECOMPARE_H
#define TYPECOMPARE_H

class Context;
class Type;

class TypeCompare
{
public:
    TypeCompare(Context &c);

    bool compatible(const Type *a, const Type *b) const;
    bool compatibleArgs(const Type *a, const Type *b) const;

    bool exact(const Type *a, const Type *b) const;
    bool exactArgs(const Type *a, const Type *b) const;

    bool convertsTo(const Type *from, const Type *to) const;

private:
    Context &c;
};

#endif // TYPECOMPARE_H
