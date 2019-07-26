#ifndef STACK_H
#define STACK_H

#include "components/Memory.h"
#include "components/Registers.h"

#include <string>

#include <iostream>

class Stack
{
public:
    Stack(Memory &mm, Registers &rg) : mm(mm), rg(rg) { }

    template<typename T> void push(T v){ rg.sp() -= sizeof(T); std::memcpy(mm(rg.sp()), &v, sizeof(T)); }
    template<typename T> void pop(T &v){ std::memcpy(&v, mm(rg.sp()), sizeof(T)); rg.sp() += sizeof(T); }

private:
    Memory &mm;
    Registers &rg;
};

#endif // STACK_H
