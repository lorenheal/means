#include "symlist.h"
#include "help.h"

#include <stdlib.h>
#include <string.h>

struct sym *
symlist_new(const char *name)
	{
	struct sym *s = calloc(1, sizeof *s);
	if (!s)
		{
		diagnose(DIAG_ERROR, DBG_SYMTAB,"sln", 0, 0, "out of memory");
		exit(1);
		}
	s->name = name ? strdup(name) : NULL;
	return s;
	}

struct sym *
symlist_append(struct sym *list, struct sym *node)
	{
	if (!node)
		return list;
	if (!list)
		return node;
	struct sym *p = list;
	while (p->next)
		p = p->next;
	p->next = node;
	return list;
	}

struct sym *
symlist_find(struct sym *list, const char *name)
	{
	for (struct sym *p = list; p; p = p->next)
		if (p->name && strcmp(p->name, name) == 0)
			return p;
	return NULL;
	}

void
symlist_free(struct sym *list)
	{
	while (list)
		{
		struct sym *next = list->next;
		free(list->name);
		/* free value later if needed */
		free(list);
		list = next;
		}
	}
