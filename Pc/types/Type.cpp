#include "Type.h"

#include "framework/Error.h"

#include "syms/Sym.h"

#include <pcx/join_str.h>

#include <unordered_set>

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
    else if(type->generic)
    {
        s += pcx::str("<", type->generic->index, ",", type->generic->depth, ">");
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
        auto sz = sym->findProperty("size");
        auto cp = sym->findProperty("complete").value<bool>();

        if(sz && cp)
        {
            return sz.to<std::size_t>();
        }
    }

    return { };
}

std::size_t Type::genericTypeCount() const
{
    std::unordered_set<GenericRef, GenericRefHash> s;

    for(auto t: args)
    {
        if(t->generic)
        {
            s.insert(*t->generic);
        }
    }

    if(returnType && returnType->generic)
    {
        s.insert(*returnType->generic);
    }

    return s.size();
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

Type Type::makeGeneric(const GenericRef &ref)
{
    Type t;
    t.generic = ref;

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

std::vector<Type*> Type::nonDefaultArgs(Sym *sym)
{
    auto args = sym->property<Type*>("type")->args;

    auto no = sym->findProperty("defaults").value<std::size_t>();
    while(no)
    {
        args.pop_back();
        --no;
    }

    return args;
}
