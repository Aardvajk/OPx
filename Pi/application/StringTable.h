#ifndef STRINGTABLE_H
#define STRINGTABLE_H

#include <string>
#include <vector>

class StringTable
{
public:
    StringTable();

    std::size_t insert(const std::string &s);

    std::string &operator[](std::size_t i){ return v[i]; }

    std::size_t size() const { return v.size(); }

    std::vector<std::string>::iterator begin(){ return v.begin(); }
    std::vector<std::string>::iterator end(){ return v.end(); }

private:
    std::vector<std::string> v;
};

#endif // STRINGTABLE_H
