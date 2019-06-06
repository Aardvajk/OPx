#include "Prologue.h"

#include "application/Context.h"

#include "common/OpCode.h"

#include "framework/ByteStreamPatch.h"

namespace
{

class AddressProvider
{
public:
    AddressProvider(Context &c) : c(c) { }

    std::size_t position() const { return c.ds.position(); }

private:
    Context &c;
};

}

ByteStreamPatch Prologue::generate(Context &c)
{
    AddressProvider ap(c);
    ByteStreamPatch p;

    c.vd.begin('F', "", 0, pcx::make_callback(&ap, &AddressProvider::position));

    c.vd("func system_prologue()");
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
