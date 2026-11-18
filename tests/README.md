Coverage summary:

File	Feature(s)
literals.m	int, float, string, bool
arith.m	all 4 ops, precedence, grouping
arith_prec.m	associativity, float precision (5 dp, no trailing zeros)
compare.m	all 6 comparison phrases
logical.m	and/or/xor, combined with comparisons
group.m	QUANTITY … END QUANTITY
vars.m	assignment, reassignment, type inference
if.m	single-stmt, DO/DONE, nested, else
while.m	counter loop, list-drain loop
func.m	0-arg, 1-arg, void
func_multi.m	2-arg, 3-arg, typed-args form
func_nested.m	closure capturing enclosing locals
recur.m	factorial, fibonacci
lists.m	roll, first, last, rest, append, insert, split, find, join, length
strings.m	length, first, last, rest, split, find on strings
thing.m	basic thing + field
tell.m	method call, state mutation
thing_inherit.m	derived type, inherited field
error_syntax.m	missing period
error_undef.m	undefined variable
error_badcall.m	undefined function
error_dotfloat.m	10. ambiguity

The four error tests will produce non-zero exit codes and stderr output from your yyerror/diagnose — after the first run you'll see the exact format and can add # expected-output lines or adjust the runner to check exit codes instead.

