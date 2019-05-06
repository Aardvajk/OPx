#ifndef ERROR_H
#define ERROR_H

#include "scanner/Location.h"

#include <pcx/str.h>

#include <stdexcept>

class Error : public std::runtime_error
{
public:
    template<typename... Args> Error(Location location, Args&&... args) : std::runtime_error(pcx::str(std::forward<Args>(args)...)), n(location) { }

    Location location() const { return n; }

private:
    Location n;
};

#endif // ERROR_H
