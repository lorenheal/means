Here are the cases the plan covers, and how each is handled. In every case the call path first evaluates the actual arguments and builds the single list bound as `arguments`. Typed binding (if any) then happens automatically before the body or builtin runs.

**1. No typed parameters, no rest**  
Example: `print` (purely variadic) or a nullary function.  
- Parameter list is empty, `is_rest` is false.  
- Automatic binding does nothing.  
- Body/builtin sees only the list `arguments` and walks it itself.  
- Too many or too few arguments are not an error unless the builtin decides they are.

**2. One or more typed parameters, no rest**  
Example: `find(needle, haystack)` or `double(n)`.  
- `param_count` = N, `is_rest` = false.  
- Automatic binding walks the first N elements of `arguments`, type-checks each against the declared parameter, and binds the names in the callee scope.  
- If `arguments` is shorter than N → arity error.  
- If `arguments` is longer than N → arity error.  
- Body/builtin sees the named locals already set; it never touches `arguments` unless it wants to.

**3. No typed parameters, with rest**  
Example: a function declared to accept “zero or more of anything”.  
- `param_count` = 0, `is_rest` = true.  
- Automatic binding does nothing.  
- The entire `arguments` list is available (or a copy/slice is bound to a conventional rest name if you want one).  
- Body walks `arguments`.

**4. Some typed parameters + rest**  
Example: `format(template, …)` or `add(first, rest…)`.  
- `param_count` = N, `is_rest` = true.  
- Automatic binding takes the first N elements, type-checks and binds those names.  
- The remaining tail of `arguments` (possibly empty) is left in `arguments` or bound to a rest name.  
- Fewer than N arguments → arity error.  
- Extra arguments are legal and become the rest.

**5. Type mismatch on a typed parameter**  
- Automatic binding performs the type check while walking.  
- On failure it reports the error and does not enter the body.  
- Rest elements are not examined for type unless the language later adds a rest type.

**6. Overload resolution (if still present)**  
- Happens before list construction or binding, using the same arity/type information that the automatic binder will later enforce.  
- Once an overload is chosen, the cases above apply to that specific `func`.

In all cases the function or builtin itself never walks `arguments` merely to set up its own locals; that step is finished before it starts. Only code that explicitly wants the variadic tail looks at the list.

What's the difference between class 1 and class 3? Isn't print just no arguments and all rest? How is that determination to be made?

Class 1 and class 3 are distinguished by the `is_rest` flag on the function, not by the number of typed parameters.

- **Class 1** (`param_count == 0` && `is_rest == false`): the function accepts exactly zero arguments. Passing any is an arity error. Automatic binding does nothing because there are no names to bind and no rest to capture.
- **Class 3** (`param_count == 0` && `is_rest == true`): the function accepts zero or more arguments of any type. The entire `arguments` list (possibly empty) is the rest. Automatic binding again does nothing; the body simply walks `arguments`.

`print` is class 3: it is declared with an empty parameter list and `is_rest = true`. That is how the determination is made—by the flags stored on the `struct func` when the builtin (or Means function) is registered, not by inspecting the call site.

A function that truly wants “exactly nothing” would be registered with `is_rest = false`. `print` is not that function.

OK, that makes more sense. I wondered about sending formatting commands to print, but I think it's better to have the arguments preformatted and give print one job. At some point I will remove the spaces between printed arguments.

Yes. `print` receives already-evaluated values in `arguments`, prints them (spaces for now), and returns the count. Any formatting is done by the caller before the call. Removing the spaces later is then a one-line change inside the builtin and does not affect the calling convention.
