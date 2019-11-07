#include "GenericStack.h"

#include "application/Context.h"

#include "nodes/Node.h"

#include "visitors/NameVisitors.h"

#include "types/Type.h"

#include <pcx/indexed_range.h>

GenericStack::GenericStack()
{
}

pcx::optional<GenericRef> GenericStack::typeRef(Node *name) const
{
    if(Visitor::query<NameVisitors::IsNameSimple, bool>(name) && !v.empty())
    {
        auto tag = Visitor::query<NameVisitors::LastIdOfName, std::string>(name);

        for(auto i: pcx::indexed_range(v.back().params))
        {
            if(i.value.name == tag)
            {
                return GenericRef(i.index, 0);
            }
        }
    }

    return { };
}

Type *GenericStack::type(const GenericRef &ref) const
{
    return v.back().params[ref.index].type;
}

Type *GenericStack::convert(Context &c, const Type *t) const
{
    auto r = *t;

    if(r.generic)
    {
        r = *c.generics.type(*r.generic);
    }

    for(std::size_t i = 0; i < r.args.size(); ++i)
    {
        if(r.args[i]->generic)
        {
            r.args[i] = c.generics.type(*r.args[i]->generic);
        }
    }

    if(r.returnType && r.returnType->generic)
    {
        r.returnType = c.generics.type(*r.returnType->generic);
    }

    Generic::combineTypes(r, *t);

    return c.types.insert(r);
}

std::vector<Type*> GenericStack::currentTypes() const
{
    std::vector<Type*> r;
    for(auto t: v.back().params)
    {
        r.push_back(t.type);
    }

    return r;
}

void GenericStack::dump(std::ostream &os) const
{
    for(auto &p: v)
    {
        os << "stack [" << p.params.size() << "]\n";
        for(auto t: p.params)
        {
            os << "    " << t.name << " " << t.type->text() << "\n";
        }
    }

    os << "====end\n";
}

