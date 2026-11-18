Cleaned list. Done items are gone. Pipe dreams are marked.

### Do next

x **grammar** adjusting token precedence for AND, operators, THEN, and ELSE.
x **Things.** `a noun is a thing with \ldots`; instance `the ball is a noun`; field read/write (write sticks to the instance); a verb defined inside the `with`, lexical scope = template scope. `for a \ldots to \ldots` is later sugar, not required.  
Order that keeps each step testable in bulding Things:
x **Instances** — a runtime object that points at a struct thing and holds field values. Construction (a noun with \ldots or a simple maker) produces one. No methods yet.
x **Field access** — ITS color reads from that instance. You can test it by constructing an instance and printing a field from outside any method.
x **Method call** — TELL the ball TO bounce sets current_receiver, looks up the method in the type’s method scope, and runs it. The method can now see ITS.
.- **Formatter** — print looks for a method named format and, if found, calls it the same way. Falls back to a default dump otherwise.
- **`words`.** Split an mstring on whitespace into a list of mstrings. Inverse of `join`.
- **Nested-list test.** `roll roll 1 and 2 and roll 3 and 4`. Fix only if it fails.
- **Drop `paraminfo`.** `paramlist` calls `params_add` directly; `funcdef` stops copying.
x **Return-count warning.** `count_returns` (or the counter) when a verb is closed; warn if more than one. Type agreement can ride the same walk. -W Switch to disable.
- **Recursion safety.** Depth counter or `sigaction` on `SIGSEGV` → `diagnose` and exit. No tail-call work.
- **noun extension** 
-- a noun also has \ldots adds fields.
-- for a noun to bounce means \ldots adds a method.
-- lambdas and functions as parameters

### Soon, not now
- Testing harness rigor (after Things settle).
- `external` / foreign calls. Needs a clearer type story; you already deferred it to codegen.
- Field constraints (`color in roll "red" and "blue"`). Uses `find` / `member`; do it with Things, not before.
- Code generation: interpret (have it), then C, then LLVM/Gimple/ARM as you care. `-e` flag is the switch.

- I want to create a thorough help system that will teach the user how to write means programs from scratch, including the subtle gotchas. For example, part of it should parse the builtins description array and explain what builtins are available and what types they can handle.

### Set aside (next language, or explicit non-goals)
- User-defined argument delimiters.
- `just` as a closer.
- Comma-only or punctuation-required argument lists (audio constraint).
- Universal single-list calling convention (Lisp-y; fine for `join`/`print`, not for `find`).
- `the result is` as a magic noun. `return` stays; multiple returns warn.
- Guaranteed tail calls / trampolines. Stack watch only.
- Multiple inheritance, interfaces, visibility keywords, constructors.
- `MAKES` return-type clause. Optional later; not needed for objects.

### Already done (do not re-list)
`while`, lists and string overloads, `mstring` only, `of`, `find`, `join`, `roll`, `into`/`onto`, typed args, `diagnose`, debug mask, `-f` / inline program, `member` in Means, early-return propagation.
