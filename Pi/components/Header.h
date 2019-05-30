#ifndef HEADER_H
#define HEADER_H

#include <pcx/str.h>

std::string header_imp(const std::string &title);

template<typename... Args> std::string header(Args&&... args)
{
    return header_imp(pcx::str(std::forward<Args>(args)...));
}

#endif // HEADER_H
