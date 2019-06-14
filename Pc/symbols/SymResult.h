#ifndef SYMRESULT_H
#define SYMRESULT_H

class Sym;

struct SymResult
{
    Sym *sym;
    bool accessible;
};

#endif // SYMRESULT_H
