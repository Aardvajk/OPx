#include "FuncInfo.h"

#include <pcx/str.h>

FuncInfo::FuncInfo() : scopes(0), labels(0)
{
}

std::string FuncInfo::nextLabel()
{
    return pcx::str("#label_", labels++);
}

std::string FuncInfo::nextLabelQuoted()
{
    return pcx::str("\"", nextLabel(), "\"");
}

