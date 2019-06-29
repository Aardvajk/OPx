#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <pcx/str.h>

std::string exe(const std::string &name)
{
    return pcx::str("C:/Projects/Px/Px/build-", name, "/release/", name, ".exe");
}

std::string wkspc(const std::string &name)
{
    return pcx::str("C:/Projects/Px/Px/workspace/", name);
}

template<typename... Args> void cmd(Args&&... args)
{
    if(std::system(pcx::str(std::forward<Args>(args)...).c_str()))
    {
        throw std::runtime_error("cmd failed");
    }
}

int main(int argc, char *argv[])
{
    try
    {
        cmd(exe("Pc"), " ", wkspc("script.pc"), " ", wkspc("script.pi"));
        cmd(exe("Pi"), " " , wkspc("script.pi"), " " , wkspc("script.po"));
        cmd(exe("Pd"), " ", wkspc("script.po"), " ", wkspc("script.po.pmap"));
        cmd(exe("Pl"), " ", wkspc("script.px"), " ", wkspc("script.po"));
        cmd(exe("Pv"), " ", wkspc("script.px"));
    }

    catch(...)
    {
        return -1;
    }
}
