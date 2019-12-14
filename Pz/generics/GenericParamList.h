#ifndef GENERICPARAMLIST_H
#define GENERICPARAMLIST_H

#include "generics/GenericParam.h"

#include <vector>

class GenericParamList
{
public:
    using iterator = std::vector<GenericParam>::iterator;
    using const_iterator = std::vector<GenericParam>::const_iterator;
    using value_type = GenericParam;

    GenericParamList();

    void push_back(const GenericParam &g){ v.push_back(g); }

    iterator begin(){ return v.begin(); }
    iterator end(){ return v.end(); }

    const_iterator begin() const { return v.begin(); }
    const_iterator end() const { return v.end(); }

    std::size_t size() const { return v.size(); }
    bool empty() const { return v.empty(); }

private:
    std::vector<GenericParam> v;
};

#endif // GENERICPARAMLIST_H
