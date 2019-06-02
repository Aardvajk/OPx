#include "Prologue.h"

#include "application/Context.h"

#include "common/OpCode.h"

#include "framework/ByteStreamPatch.h"

ByteStreamPatch Prologue::generate(Context &c)
{
    ByteStreamPatch p;

    c.ds << OpCode::Op::SetRI << OpCode::Reg::Dx << p;
    c.ds << OpCode::Op::Call << OpCode::Reg::Dx;

    c.ds << OpCode::Op::End;

    return p;
}
