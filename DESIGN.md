Means is a natural language programming language that uses a tiny subset of English for its grammar and keywords. Means statements are generally well-formed English sentences. 

### Means Programs

## Start

Means programs are either simple lists of statements at the global level, which
are run sequentially, or can contain a verb named "start", which is taken as the
entry point. 

## Means by example

```means
#These are equivalent:
print "Hello, world." and newline.
print "Hello," and space and "world." and newline.

# Here is a Means function, called a verb.
# Verbs are called with a list of arguments.
# The variable left gets the first item in the arguments.  
# The variable right gets the second item in the arguments.  
# The variable words gets a list.  
To wrap the left is "" and the right is "" and the rest means to do
	the result is empty.
	The left is pop the words.
	The right is pop the words.
	while length the words stays greater than 0 
	do
		the item is pop the words.
		the output is the left.
		append the item and the output.
		append the right and the output.
		append the output and the result.
	done.
	return the result.
done.
print wrap "[" and "]" and "first" and "second" and "third".
#[first] and [second] and [third].
```

See also the examples in the test suite for Means programs.

### Statements
Means statements are ordinary English sentences. The allowed statement
forms are:
- Assignment
- Definition
- `if` / `then` / `else`
- While condition block
- Verb call *argument list*
- `return *list*`
- block: `DO` [*statements*] `DONE`

### Types

- `number` (integer)
- `decimal` (floating-point)
- `word` (character string)
- `list` (the only aggregate for now)

Variables are inferred from first assignment and type locked thereafter.
Assignments to a known variable must be of the same type.
Constants have the word `constant` before the right-hand-side value when first
assigned. Functions can have local constants.
Explicit types appear only in function parameter signatures. 

## Operators

### Arithmetic

| Word     | Operation |
|----------|-----------|
| `plus`   | +         |
| `minus`  | −         |
| `times`  | ×         |
| `over`   | ÷         |

`times`/`over` bind tighter than `plus`/`minus`. Left-associative.
Integer division truncates; use a decimal literal for a decimal result.

To parenthize, surround higher precedence expressions with the keywords QUANTITY
and END QUANTITY.

### Strings
Strings, called `word` in Means, are represented internally as a 
```C
struct mstring {
	int length; /* the length of the current string in buf + '\0' */
	int size;   /* the allocated size of buf */
	char *buf;  /* the nul-terminated C string */
	int ref;    /* reference count */
}
```

Operations that mutate words always update their string length. If the
operation results in a string longer than the buffer, the buffer is
automatically reallocated to twice its size until the string fits.

### Comparison 

`greater than`, `less than`, `equal to`, `unequal to`, `at least`, `at most`.

### Logical (planned)

`logical and`, `logical or`, `exclusive or`.

The word `and` is reserved for list construction only.

## Lists

A list is internally a header `{ head, tail, length }` plus singly-linked
listnodes. A listnode can be of any type; heterogenous lists are
allowed. Just as with fundamental types, attempting to assign the value of
a list element to a variable of a different type generates a compile-time
error if the types are known lexically, or a runtime error otherwise.

### List literals. 

- "AND" between expressions makes a list of them
- AND between an expression and a list makes a two-item list, one of which is a sublist
- To add an item to a list, use append or insert
- To name a list, use the list() builtin, which simply returns its arguments.

Inside a function the name `arguments` is an constant local variable of
list type; it is created by the same path that creates any other local.

List verbs that also apply to strings are separate overloads, not a
shared representation.

## Builtins

Builtins are ordinary functions whose implementation pointer is non-NULL.
They are registered into the global scope from a static table before
parsing begins. They obey the same list-in / list-out convention as
Means functions. The implementation is slightly different, in that type checking for a builtin is handled by an internal table. A list of types is declared. The builtin function itself is required to read its list of arguments and to know what to do with them.

| verb | argument | Bounds| Description|
|------|----------|-------|-------------------------------------|
|print|*list*|O(n)|outputs its arguments separated by a space
|read|||A list of words input
|first|*list*|O(1)|a reference to the first element
|rest|*list*|O(1)|a reference to the list staring at the second element
|split|index *list*|O(index)|tail starting at index, removing it
|last|*list*|O(1)| a reference to the last element
|length|*list*|O(1)|number of elements in list
|append|*expr* *list*|O(1)|list with *expr* as new last element
|insert|index *expr* *list*|O(index)|list with *expr* as new element at index
|push|*expr* *list*|O(1)|list with *expr* as new first element 
|pop|*list*|O(1)|first element of *list*, removes it from list
|find|*expr* *list*|O(n)|first position of *expr* in list 
|join|*word* *list*|O(n)|single item of value of all elements in list, separated by *word*.
|words|*list*|O(n)|list of rest(*list*) between first(list) 
|exit|*list*||end the program, setting error code, with error messages.
|flatten|*list*|O(n)| (planned) Turn complex list structure into a flat list

List builtin operations are O(1) at the ends.In the table above, O(index) means
O(1) if index is 1, O(n) otherwise.

## Diagnose: Help, Warnings, Debugging, and Errors
- Program command line execution failure calls a help function 
- Errors generate a diagnose() call 
- If the error is unrecoverable, it causes an exit
- There are compile time exits and runtime exits
- Diagnose is also used for warnings and debugging statements
- Planned: Warnings add up and depending on severity may trigger an exit.
- Multiple return statements in a function generate a warning

## Diagnose action table
|severity|always print?|increments error_count?|exits?|
|--------|-------------|-----------------------|------|
|DIAG_DEBUG|only if area bit set|no|no|
|DIAG_WARNING|always|no|no|
|DIAG_ERROR|always|yes|no|
|DIAG_FATAL|always|(yes, if you want)|yes, with the given code|
## Architecture

- Parser: Bison (LALR(1))
- Lexer: Flex
- Front end produces an AST and fills scopes.
- Back ends (interpreter, C, LLVM, …) are selected by a `-e` flag and
  walk the same AST.
- Supporting modules: `symtab`, `params`, `builtins`, `meanslist`,
  `mstring`, `help`.

### Module boundaries

Struct members may be accessed only inside the compilation unit that
defines the struct. All other code is to use the functions declared in the
header. Side effects are documented before a function is admitted.
Formatting follows the project style:
- Whitessmith's C style
- No blank lines inside functions
- If blank lines are needed, indent a comment past the current indent

### Scoping

Each function has its own local symbol table. Lookup starts in the current function and proceeds outward to the global scope. Locals override enclosing names. There is no nesting of functions in this revision; the scope chain is therefore only global → function. The overall plan includes nested functions and objects with their own scopes, nested objects, and methods which can include nested functions.

### Memory

Explicit ownership via `value_copy` / `value_free` (and the corresponding
list and string operations). No garbage collector. Constructors return
owned objects; destructors take ownership. `value_copy` on a list
deep-copies.

Structure access functions should be named to hint at ownership.

## Deferred

- Nested functions
- User-defined Things (objects) and methods
- Default methods
- Code generation (begins after the interpreter subset is solid)
- STREAM I/O 

## Reserved words

`the`, `is`, `are`, `to`, `means`, `like`, `do`, `done`, `return`,
`and`, `empty`, `plus`, `minus`, `times`, `over`, `constant`,
`number`, `decimal`, `word`, `if`, `then`, `else`, `while`, `stays`,
`greater than`, `less than`, `equal to`, `unequal to`, `at least`, `at most`
