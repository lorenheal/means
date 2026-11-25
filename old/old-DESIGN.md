Means is a natural language programming language that uses a tiny subset of English for its grammar and keywords. Means statements are generally well-formed English sentences. 

### Means Programs

## Start

Means programs are either simple lists of statements at the global level, which are run sequentially, or can contain a verb named "start", which is taken as the entry point. 

Case is not significant except in the contents of words.

## Means by example

```means
#These are equivalent:
print "Hello, world.".
print "Hello," and "world.".

# Here is a Means function, called a verb, named "empty"
# Empty ignores its arguments without altering them and returns an empty list.
To empty means to return.

# Verbs are called with a list of arguments.
# The variable left gets the first item in the arguments.  
# The variable right gets the second item in the arguments.  
# The variable words gets a list.  
To wrap the word left and the word right and the list words means to do
	the result is empty.
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
print wrap "[" and "]" and roll "first" and "second" and "third".
#[first] and [second] and [third].
```

See also the examples in the test suite for Means programs.

### Statements
Means statements are ordinary English sentences. The allowed statement
forms are:
- Assignment
- Definition
- `if` / `then` / `else`
- `while *condition* *block*`
- Verb call *argument list*
- `return *list*`
- block: `DO` [*statements*] `DONE`

### Types

- `number` (integer)
- `decimal` (floating-point)
- `word` (character string)
- `list` (the only aggregate for now)

Variables are inferred from first assignment and type locked thereafter.  Assignments to a known variable must be of the same type.  Numbers and decimals convert automatically.  Constants have the word `constant` before the right-hand-side value when first assigned. Functions can have local constants.  Explicit types appear only in function parameter signatures. 

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
Strings, called `word` in Means, are represented internally as a C struct:
```C
struct mstring {
	int length; /* the length of the current C string in buf including '\0' */
	int size;   /* the allocated size of buf */
	char *buf;  /* the nul-terminated C string plus unused space */
	int ref;    /* reference count */
}
```

Operations that mutate words always update their C string length. If the
operation results in a string longer than the buffer, the buffer is
automatically reallocated to twice its size until the C string fits.

### Comparison 

`greater than`, `less than`, `equal to`, `unequal to`, `at least`, `at most`.

The keyword `stays` exists only to keep from saying `is greater than` or `is at most`. It is not required, and is not used internally. These are equivalent:
```means
	while length the words stays greater than 0 do ...

	while length the words greater than 0 do ...
```


### Logical (planned)

`logical and`, `logical or`, `exclusive or`.

The word `and` is reserved for list construction only.

## Lists

A list is internally a header `{ head, tail, length }` plus singly-linked listnodes. A listnode can be of any type; heterogenous lists are allowed. Just as with fundamental types, attempting to assign the value of a list element to a variable of a different type generates a compile-time error if the types are known lexically, or a runtime error otherwise.

### List literals. 

- "AND" between expressions makes a list of them
- AND between an expression and a list makes a two-item list, one of which is a sublist
- To add an item to a list, use append or insert
- To name a list, use the roll() builtin, which simply returns its arguments
- To create an empty list, create a function that returns no value.

Inside a function the name `arguments` is a constant local variable of list type; it is created by the same path that creates any other local. As the argument list is evaluated, named parameters are checked and assigned values from the arguments. The named parameters become ordinary mutable local variables.

List verbs that also apply to words are separate overloads, not a shared representation.

## Builtins

Builtins are ordinary functions whose implementation pointer is non-NULL.  They are registered into the global scope from a static table before parsing begins. They obey the same list-in / list-out convention as Means functions.

| verb | argument | Bounds| Description|
|------|----------|-------|-------------------------------------|
|print|*list*|O(n)|outputs its arguments separated by a space
|read|(no args yet)||A list of words input; Planned: read from a filename
|first|*list*|O(1)|a reference to the first element
|rest|*list*|O(1)|a reference to the list starting at the second element
|split|index *list*|O(index)|tail starting at *index*, removing it
|last|*list*|O(1)| a reference to the last element
|length|*list*|O(1)|number of elements in *list*
|append|*expr* *list*|O(1)|*list* with *expr* as new last element
|insert|index *expr* *list*|O(index)|list with *expr* as new element at *index*
|push|*expr* *list*|O(1)|list with *expr* as new first element 
|pop|*list*|O(1)|first element of *list*, removes it from *list*
|find|*expr* *list*|O(n)|first position of *expr* in *list* 
|join|*word* *list*|O(n)|value of each element in *list*, separated by *word*.
|words|*list*|O(n)|list of rest(*list*) between first(*list*) 
|exit|[number [message list]]||end the program with error code or 0, print error messages.
|empty|*list*|O(1)| (planned) an empty list, when one is needed.
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
defines the struct. All other code uses the functions declared in the
header. Side effects are documented before a function is admitted.
Formatting follows the project style.

### Scoping

Each function has its own local symbol table. Lookup starts in the
current function and proceeds outward to the global scope. Locals
override enclosing names. There is no nesting of functions in this
revision; the scope chain is therefore only global → function.

### Memory

Explicit ownership via `value_copy` / `value_free` (and the corresponding
list and word operations). No garbage collector. Constructors return
owned objects; destructors take ownership. `value_copy` on a list
deep-copies.

## Deferred

- Nested functions
- User-defined Things (objects) and methods
- Default methods
- Code generation (begins after the interpreter subset is solid)

## Reserved words

`the`, `is`, `to`, `means`, `do`, `done`, `return`,
`a`, `an`, `and`, `plus`, `minus`, `times`, `over`, `constant`,
`number`, `decimal`, `word`, `if`, `then`, `else`, `while`, `stays`,
`greater than`, `less than`, `equal to`, `unequal to`, `at least`, `at most`
