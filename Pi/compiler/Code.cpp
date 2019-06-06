#include "Code.h"

#include "common/OpCode.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "compiler/Instruction.h"

#include <pcx/lexical_cast.h>

namespace
{

void jmpConstruct(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::Id, get);

    if(auto s = c.syms.findLocal(id.text()))
    {
        if(s->type != Sym::Type::Label)
        {
            throw Error(id.location(), "label expected - ", id.text());
        }

        ByteStreamPatch p;
        c.func().bytes << OpCode::Op::SubRI << OpCode::Reg::Pc << p;

        auto pos = s->properties["position"].to<std::size_t>();
        p.patch(c.func().bytes, c.func().bytes.position() - pos);
    }
    else
    {
        ByteStreamPatch p;
        c.func().bytes << OpCode::Op::AddRI << OpCode::Reg::Pc << p;

        c.func().jumps.emplace_back(id, p);
    }

    c.scanner.consume(Token::Type::Semicolon, true);
}

void svcConstruct(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::IntLiteral, get);

    c.func().bytes << OpCode::Op::Service << pcx::lexical_cast<int>(id.text());

    c.scanner.consume(Token::Type::Semicolon, true);
}

void labelConstruct(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::Id, get);
    c.assertUnique(id.location(), id.text());

    auto sym = c.syms.add(new Sym(Sym::Type::Label, id.text()));
    sym->properties["position"] = c.func().bytes.position();

    c.scanner.consume(Token::Type::Colon, true);
}

}

void Code::construct(Context &c, bool get)
{
    auto tok = c.scanner.next(get);
    auto ins = Instruction::fromString(tok.text());

    switch(ins)
    {
        case Instruction::Type::Jmp: jmpConstruct(c, true); break;

        case Instruction::Type::Svc: svcConstruct(c, true); break;

        default: labelConstruct(c, false);
    }
}
