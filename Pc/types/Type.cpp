#include "Type.h"

#include "framework/Error.h"

#include "syms/Sym.h"

#include <pcx/join_str.h>

namespace
{

std::string toString(const Type *type)
{
    std::string s;

    if(type->constant)
    {
        s += "const ";
    }

    if(type->ref)
    {
        s += "ref ";
    }

    for(std::size_t i = 0; i < type->ptr; ++i)
    {
        s += "ptr ";
    }

    if(type->sym)
    {
        s += type->sym->fullname();
    }
    else
    {
        s += "(" + pcx::join_str(type->args, ",", toString) + ")";
    }

    if(type->returnType)
    {
        s += ":" + toString(type->returnType);
    }

    if(type->constMethod)
    {
        s += " const";
    }

    if(type->method)
    {
        s += " [method]";
    }

    return s;
}

}

Type::Type() : constant(false), ref(false), ptr(0), sym(nullptr), returnType(nullptr), method(false), constMethod(false)
{
}

Type Type::addPointer() const
{
    auto t = *this;
    ++t.ptr;

    return t;
}

Type Type::removePointer() const
{
    auto t = *this;
    --t.ptr;

    return t;
}

Type Type::addReference() const
{
    auto t = *this;
    t.ref = true;

    return t;
}

Type Type::removeReference() const
{
    auto t = *this;
    t.ref = false;

    return t;
}

std::string Type::text() const
{
    return toString(this);
}

bool Type::function() const
{
    return returnType;
}

bool Type::primitive() const
{
    return ptr || function() || (sym && sym->findProperty("primitive").value<Primitive::Type>() != Primitive::Type::Invalid);
}

bool Type::primitiveOrRef() const
{
    return ref || primitive();
}

Primitive::Type Type::primitiveType() const
{
    if(ptr || returnType)
    {
        return Primitive::Type::Size;
    }

    if(auto t = sym->findProperty("primitive"))
    {
        return t.to<Primitive::Type>();
    }

    return Primitive::Type::Invalid;
}

pcx::optional<std::size_t> Type::size() const
{
    if(ptr || returnType)
    {
        return sizeof(std::size_t);
    }

    if(sym)
    {
        if(auto sz = sym->findProperty("size"))
        {
            return sz.to<std::size_t>();
        }
    }

    return { };
}

Type Type::makePrimary(Sym *sym)
{
    Type t;
    t.sym = sym;

    return t;
}

Type Type::makePrimary(bool constant, bool ref, Sym *sym)
{
    Type t;
    t.constant = constant;
    t.ref = ref;
    t.sym = sym;

    return t;
}

Type Type::makeFunction(Type *returnType, const std::vector<Type*> &args)
{
    Type t;
    t.returnType = returnType;
    t.args = args;

    return t;
}

std::size_t Type::assertSize(Location location, const Type *type)
{
    if(auto s = type->size())
    {
        return *s;
    }

    throw Error(location, "use of forward-declared type - ", type->text());
}

