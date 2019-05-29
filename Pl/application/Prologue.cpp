#include "Prologue.h"

#include "common/OpCode.h"

#include "framework/ByteStream.h"

ByteStreamPatch Prologue::generate(ByteStream &bs)
{
    using namespace OpCode;

    ByteStreamPatch p;

    bs << Op::SetRI << Reg::Dx << p;
    bs << Op::Call << Reg::Dx;

    bs << Op::End;

    return p;
}
