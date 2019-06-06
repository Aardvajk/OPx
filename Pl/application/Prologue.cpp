#include "Prologue.h"

#include "application/Context.h"

#include "common/OpCode.h"

#include "framework/ByteStreamPatch.h"

ByteStreamPatch Prologue::generate(Context &c)
{
    ByteStreamPatch p;

    c.vd.begin('F', "", 0, pcx::make_callback(&c.ds, &ByteStream::position));

    c.vd("prologue");
    c.vd("{");

    c.vd("-program prologue");
    c.ds << OpCode::Op::SetRI << OpCode::Reg::Dx << p;
    c.ds << OpCode::Op::Call << OpCode::Reg::Dx;

    c.vd("-terminate");
    c.ds << OpCode::Op::End;
    c.vd("-end");

    c.vd("}");

    c.vd.back().size = c.ds.position();

    return p;
}
