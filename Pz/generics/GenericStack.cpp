#include "GenericStack.h"

#include "nodes/Node.h"

#include "visitors/NameVisitors.h"

#include <pcx/range_reverse.h>
#include <pcx/indexed_range.h>

GenericStack::GenericStack()
{
}

void GenericStack::push_back(const GenericParamList &g)
{
    v.push_back(g);
}

void GenericStack::pop_back()
{
    v.pop_back();
}

pcx::optional<GenericRef> GenericStack::typeRef(Node *name) const
{
    if(!v.empty() && Visitor::query<NameVisitors::IsNameSimple, bool>(name))
    {
        auto tag = Visitor::query<NameVisitors::LastIdOfName, std::string>(name);

        std::size_t i = v.size() - 1;
        while(true)
        {
            for(auto j: pcx::indexed_range(v[i]))
            {
                if(j.value.name == tag)
                {
                    return GenericRef(j.index, i);
                }
            }

            if(!i--) break;
        }
    }

    return { };
}
