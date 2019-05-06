#ifndef LOCATION_H
#define LOCATION_H

#include <cstddef>

class Location
{
public:
    using Id = unsigned;
    using Column = unsigned;
    using Line = unsigned;

    Location() : i(0), c(0), n(0) { }
    Location(Id id, unsigned column, unsigned line) : i(id), c(column), n(line) { }

    Id id() const { return i; }
    Column column() const { return c; }
    Line line() const { return n; }

private:
    Id i;
    Column c;
    Line n;
};

#endif // LOCATION_H
