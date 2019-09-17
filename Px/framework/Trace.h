#ifndef TRACE_H
#define TRACE_H

#include <string>
#include <iostream>

class Trace
{
public:
    Trace(std::string name) : s(std::move(name)) { std::cout << "begin " << s << "\n"; }
    ~Trace(){ std::cout << "end " << s << "\n"; }

private:
    std::string s;
};

#define TRACE Trace _trace(__PRETTY_FUNCTION__)

#endif // TRACE_H
