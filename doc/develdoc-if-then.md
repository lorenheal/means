The logic flow for a top-level if:

IF expr THEN → save outer context (NULL/NULL at top level), set collecting=1, reset body_head/cur_body
if_body parses → each assignment DOT / print_stmt DOT sees collecting==1, appends to the fresh body_head list
Action after if_body → assembles ST_IF node, restores outer context, sets collecting=0
Since saved_tail is NULL (top level), calls exec_if(s) immediately
For an if inside a function body:

collecting is already 1, body_head/cur_body point to the function's statement list
IF expr THEN saves that context, starts a fresh collection
Branch statements collect into the fresh list
Action restores the function's context, appends the ST_IF node to the function's list via stmt_append
One gotcha: the if_body: if_stmt alternative (nested if) works because the inner if_stmt's final action either appends to cur_body (if we're collecting) or executes (if top-level). Since we're always collecting when inside an if_body, it appends. The nested ST_IF node becomes part of the outer branch's statement list.
