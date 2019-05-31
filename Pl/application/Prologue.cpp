#include "Prologue.h"

#include "common/OpCode.h"

#include "framework/ByteStream.h"

#include "application/Context.h"

ByteStreamPatch Prologue::generate(Context &c, ByteStream &bs)
{
    using namespace OpCode;

    ByteStreamPatch p;

    c.dataComments("program prologue");

    bs << Op::SetRI << Reg::Dx << p;
    bs << Op::Call << Reg::Dx;

    c.dataComments("terminate");

    bs << Op::End;

    c.dataComments("end of program prologue");

    return p;
}
