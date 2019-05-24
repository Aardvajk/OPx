#include "Prologue.h"

#include "common/Px.h"
#include "common/OpCode.h"

#include "framework/ByteStream.h"

ByteStreamPatch Prologue::generate(ByteStream &bs)
{
    using namespace OpCode;

    ByteStreamPatch p;

    bs << Op::SubRI << Reg::Sp << std::size_t(8);
    bs << Op::CopyAI << Reg::Sp << std::size_t(8) << p;
    bs << Op::PopR << Reg::Dx;

    bs << Op::Call << Reg::Dx;

    bs << Op::End;

    return p;
}
