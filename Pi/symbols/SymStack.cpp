#include "SymStack.h"

SymStack::SymStack()
{
    sc.push_back(0);
}

void SymStack::push()
{
    sc.push_back(v.size());
}

void SymStack::pop()
{
    while(v.size() > sc.back())
    {
        v.pop_back();
    }

    sc.pop_back();
}

Sym *SymStack::add(Sym *sym)
{
    v.push_back(sym);
    return sym;
}

Sym *SymStack::find(const std::string &name)
{
    return find(name, 0);
}

Sym *SymStack::findLocal(const std::string &name)
{
    return find(name, sc.back());
}

void SymStack::print(std::ostream &os)
{
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        os << Sym::toString(v[i].type) << " " << v[i].name;
        if(auto pr = v[i].properties["size"])
        {
            os << ":" << pr.to<std::size_t>();
        }

        if(auto pr = v[i].properties["offset"])
        {
            os << " [" << pr.to<std::size_t>() << "]";
        }

        if(auto pr = v[i].properties["position"])
        {
            os << " @" << pr.to<std::size_t>();
        }

        os << "\n";
    }
}

Sym *SymStack::find(const std::string &name, std::size_t limit)
{
    if(!v.empty())
    {
        std::size_t i = v.size() - 1;
        if(i < limit) return nullptr;

        while(true)
        {
            if(v[i].name == name)
            {
                return v.ptr(i);
            }

            if(i-- == limit) break;
        }
    }

    return nullptr;
}
