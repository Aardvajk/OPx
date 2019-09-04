#include "Context.h"

#include "framework/InputStream.h"

Context::Context() : trim(false)
{
}

Object::Entity *Context::find(const std::string &text)
{
    for(auto &u: units)
    {
        for(auto &e: u.entities)
        {
            if(u.strings[e.id] == text)
            {
                return &e;
            }
        }
    }

    return nullptr;
}

ByteStream &Context::stream(char type)
{
    return type == 'V' ? ds : ps;
}
