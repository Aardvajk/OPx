#ifndef GENERICUSAGESET_H
#define GENERICUSAGESET_H

#include "generics/GenericUsage.h"

class Context;

class GenericUsageSet
{
public:
    GenericUsageSet();

    void insert(Context &c, const GenericUsage &u);

    std::vector<GenericUsage>::iterator begin(){ return v.begin(); }
    std::vector<GenericUsage>::iterator end(){ return v.end(); }

private:
    std::vector<GenericUsage> v;
};

#endif // GENERICUSAGESET_H
