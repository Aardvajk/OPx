#ifndef SOURCE_H
#define SOURCE_H

#include "scanner/Location.h"

#include <pcx/scoped_ptr.h>

#include <iostream>

class Source
{
public:
    Source(Location::Id id, std::istream *is);

    using Char = std::istream::int_type;

    Char get();
    void unget(Char ch);

    Location location() const;

private:
    Location::Id i;
    Location::Column c, pc;
    Location::Line n;

    pcx::scoped_ptr<std::istream> is;
};

#endif // SOURCE_H
