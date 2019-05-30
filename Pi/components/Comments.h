#ifndef COMMENTS_H
#define COMMENTS_H

#include "components/Function.h"

#include <vector>
#include <string>
#include <utility>

#include <pcx/str.h>

class Comments
{
public:
    Comments(const std::vector<Function> &fv) : fv(fv) { }

    template<typename... Args> Comments &operator()(Args&&... args);
    template<typename T> Comments &operator<<(const T &t){ v.back().second += pcx::str(t); return *this; }

    std::pair<std::size_t, std::string> operator[](std::size_t index) const { return v[index]; }
    std::size_t size() const { return v.size(); }

private:
    const std::vector<Function> &fv;
    std::vector<std::pair<std::size_t, std::string> > v;
};

template<typename... Args> Comments &Comments::operator()(Args&&... args)
{
    v.push_back(std::make_pair(fv.back().bytes.position(), pcx::str(std::forward<Args>(args)...)));
    return *this;
}

#endif // COMMENTS_H
