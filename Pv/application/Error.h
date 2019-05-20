#include <pcx/str.h>

#include <stdexcept>

class Error : public std::runtime_error
{
public:
    template<typename... Args> Error(Args&&... args) : std::runtime_error(pcx::str(std::forward<Args>(args)...)) { }
};
