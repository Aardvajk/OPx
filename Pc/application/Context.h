#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

class Context
{
public:
    Context();

    void open(const std::string &path);

    SourceList sources;
    Scanner scanner;
};

#endif // CONTEXT_H
