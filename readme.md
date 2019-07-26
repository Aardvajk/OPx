Toy compiler and VM project

Queries to aardvajk@live.co.uk. Can't be bothered to keep updating this.

Currently supported example (26th July 2019)

include "stdlib.ph";

class Foo;

func f(a:ptr Foo);

class Foo
{
    func m()
    {
        std.print((*this).x);
        f(this);
    }

    var y:std.int;
    var x:std.int;
}

func f(a:ptr Foo)
{
    std.print((*a).x);
}

func main()
{
    var a:Foo, b:Foo;

    a.x = 123;
    b.x = 321;

    a.m();
    b.m();
}


