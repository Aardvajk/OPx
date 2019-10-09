#include "GlobalFuncs.h"

#include "application/Context.h"

#include "common/OpCode.h"

#include "framework/ByteStreamPatch.h"

#include <pcx/range_reverse.h>

namespace
{

class Callback
{
public:
    Callback(ByteStream &b) : b(b), p(b.position()) { }

    std::size_t position() const { return b.position() - p; }

private:
    ByteStream &b;
    std::size_t p;
};

}

std::size_t GlobalFuncs::generateGlobalInit(Context &c, std::size_t entryPointOffset)
{
    auto offset = c.ps.position();

    c.refs.insert("#global_init");

    Callback cb(c.ps);
    c.pd.begin('F', "#global_init", 0, pcx::make_callback(&cb, &Callback::position));

    c.pd("prologue");
    c.pd("{");

    for(auto n: c.globalInits)
    {
        c.pd("-call global init ", n);

        auto ep = c.find(n);

        c.ps << OpCode::Op::SetRI << OpCode::Reg::Dx << c.ds.position() + ep->offset;
        c.ps << OpCode::Op::Call << OpCode::Reg::Dx;
    }

    c.pd("-call entrypoint");
    c.ps << OpCode::Op::SetRI << OpCode::Reg::Dx << c.ds.position() + entryPointOffset;
    c.ps << OpCode::Op::Call << OpCode::Reg::Dx;

    for(auto n: pcx::range_reverse(c.globalDestroys))
    {
        c.pd("-call global destroy ", n);

        auto ep = c.find(n);

        c.ps << OpCode::Op::SetRI << OpCode::Reg::Dx << c.ds.position() + ep->offset;
        c.ps << OpCode::Op::Call << OpCode::Reg::Dx;
    }

    c.pd("-return");
    c.ps << OpCode::Op::Ret << std::size_t(0);

    c.pd("}");

    c.pd.back().size = cb.position();

    return offset;
}
