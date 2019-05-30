#include "Header.h"

std::string header_imp(const std::string &title)
{
    return pcx::str("--- ", title, " ", std::string(119 - (title.length() + 5), '-'));
}
