namespace std
{

func print(:std.int);
func print(:std.char);
func print(:std.bool);
func print(:std.size);
func print(:ptr std.char);
func print(:ptr std.null);

func abort();

class prefix { }
class postfix { }

func allocate(:std.size):ptr std.null;
func release(:ptr std.null);

}