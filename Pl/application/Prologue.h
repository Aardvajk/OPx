#ifndef PROLOGUE_H
#define PROLOGUE_H

class Context;
class ByteStream;
class ByteStreamPatch;

namespace Prologue
{

ByteStreamPatch generate(Context &c, ByteStream &bs);

}

#endif // PROLOGUE_H
