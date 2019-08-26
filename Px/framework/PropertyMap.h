#ifndef PROPERTYMAP_H
#define PROPERTYMAP_H

#include "framework/Error.h"

#include <pcx/any.h>

#include <string>
#include <unordered_map>
#include <type_traits>

class PropertyMap
{
public:
    PropertyMap(){ }

    void set(const std::string &name, pcx::any value){ v[name] = value; }
    template<typename T> T get(const std::string &name) const ;

    pcx::any operator[](const std::string &name) const { auto i = v.find(name); return i == v.end() ? pcx::any() : i->second; }

private:
    std::unordered_map<std::string, pcx::any> v;
};

template<typename T> T PropertyMap::get(const std::string &name) const
{
    auto i = v.find(name);
    if(i == v.end())
    {
        throw Error("bad property read - ", name);
    }

    return i->second.to<T>();
}

#endif // PROPERTYMAP_H
