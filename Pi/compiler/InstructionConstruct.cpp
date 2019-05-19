#include "InstructionConstruct.h"

#include "error/Error.h"

#include "application/Context.h"

void InstructionConstruct::entity(Context &c, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        default: throw Error(tok.location(), "instruction expected - ", tok.text());
    }
}
