namespace std
{

class ostream
{
}

func operator<<(:ref ostream, :std.int):ref ostream;
func operator<<(:ref ostream, :std.bool):ref ostream;
func operator<<(:ref ostream, :ptr std.char):ref ostream;

func out():ref ostream;

}
