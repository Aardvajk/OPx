#include "Prologue.h"

#include "Px.h"

#include "opcode/OpCode.h"

#include "framework/ByteStream.h"

ByteStreamPatch Prologue::generate(ByteStream &bs)
{
    ByteStreamPatch p;

    bs << OpCode::Type::SubRI << OpCode::Reg::Sp << Px::ptr_size;
    bs << OpCode::Type::CopyAI << OpCode::Reg::Sp << Px::ptr_size << p;

    bs << OpCode::Type::Call;

    bs << OpCode::Type::End;

    return p;
}
