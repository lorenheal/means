#ifndef SYMLIST_H
#define SYMLIST_H

#include "symtab.h"   /* struct sym */

struct sym *symlist_new(const char *name);
struct sym *symlist_append(struct sym *list, struct sym *node);
struct sym *symlist_find(struct sym *list, const char *name);
void        symlist_free(struct sym *list);

#endif