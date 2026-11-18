

With a header that stores length, head pointer, and tail pointer, the complete set is:
push, and append are pass-by-value.

**Construction / destruction**
- `list_new()` — empty list (length 0, head = tail = NULL)
- `list_free(list)` — free the header and all nodes (does not free the values themselves unless you decide otherwise)

**O(1) access**
- `first(list)` — value of the front node, or a defined empty value if empty
- `rest(list)` — new list header that shares the remaining nodes (non-destructive); or a destructive version if you prefer
- `last(list)` — value of the tail node
- `length(list)` — the stored count

**O(1) mutation**
- `push(list, value)` — insert at front, update head and length
- `pop(list)` — remove front node, return its value, update head and length
- `append(list, value)` — insert at back, update tail and length

**Optional but useful**
- `list_empty(list)` — length == 0
- `list_copy(list)` — deep copy of the spine (values shared or copied, your choice)
- `list_concat(a, b)` — append all of b onto a (O(length of b) unless you also keep a tail; still fine)

`push`, `pop`, and `append` are the three that must mutate the header in place. Everything else can be built on top of them. Once those exist, the `and` operator and the call-argument path can be written against this API without further changes to the list representation.

Empty list
==========
header
  length: 0
  head:   NULL
  tail:   NULL

After insert(list, A)          (push front)
==============================
header
  length: 1
  head:   -----> [A | next: NULL]
  tail:   -----> [A | next: NULL]

After insert(list, B)          (push front again)
==============================
header
  length: 2
  head:   -----> [B | next] -----> [A | next: NULL]
  tail:   -------------------------------^

After append(list, C)          (push back)
==============================
header
  length: 3
  head:   -----> [B | next] -----> [A | next] -----> [C | next: NULL]
  tail:   ----------------------------------------->^

After append(list, D)
==============================
header
  length: 4
  head:   -----> [B | next] -----> [A | next] -----> [C | next] -----> [D | next: NULL]
  tail:   --------------------------------------------------------->^

insert always updates head (and length).
append always updates tail (and length).
When the list was empty, both also set the other pointer.

Before split()
==============
header
  length: 4
  head:   -----> [B | next] -----> [A | next] -----> [C | next] -----> [D | next: NULL]
  tail:   --------------------------------------------------------->^

split(list) returns B
and leaves
==============
header
  length: 3
  head:   -----> [A | next] -----> [C | next] -----> [D | next: NULL]
  tail:   --------------------------------------->^

(The node that held B is freed; its value is returned to the caller.)

split on a one-element list
===========================
header
  length: 1
  head:   -----> [X | next: NULL]
  tail:   -----> [X | next: NULL]

split(list) returns X
and leaves
==============
header
  length: 0
  head:   NULL
  tail:   NULL

split on an empty list
======================
header
  length: 0
  head:   NULL
  tail:   NULL

split(list) returns a defined empty value
and leaves the header unchanged.
