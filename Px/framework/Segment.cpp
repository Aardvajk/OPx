#include "Segment.h"

#include "framework/InputStream.h"

#include <iostream>

Segment::Segment()
{
}

void Segment::read(std::size_t bytes, InputStream &is)
{
    v.resize(bytes);
    is.read(v.data(), bytes);
}
