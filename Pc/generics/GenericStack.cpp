#include "GenericStack.h"

#include "nodes/Node.h"

#include "visitors/NameVisitors.h"

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
