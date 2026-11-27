#include "means.h"
#include "meanslist.h"
#include "help.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list *list_new(void) /* def */
	{
	struct list *lst = calloc(1, sizeof *lst);
	lst->refcount = 1;
	return lst;   /* head = tail = NULL, length = 0 */
	}

void list_free(struct list *lst) /* def */
	{
	if (!lst) return;
	struct listnode *n = lst->head;
	while (n)
		{
		struct listnode *next = n->next;
		Value_Free(n->val);
		free(n);
		n = next;
		}
	free(lst);
	}

struct listnode *listnode_new(struct value v) /* def */
	{
	struct listnode *ret = calloc(1, sizeof(*ret));
	ret->val = Value_Copy(v);
	ret->next = NULL;
	return (ret);
	}

int list_isempty(const struct list *lst) /* def */
	{
	return !lst || lst->length == 0;
	}

int list_length(const struct list *lst) /* def */
	{
	return lst ? lst->length : 0;
	}

struct value list_first(const struct list *lst) /* def */
	{
	struct value ret = Value_Empty();
	if (! list_isempty(lst))
		ret = Value_Copy(lst->head->val);
	return(ret);
	}

struct value list_last(const struct list *lst) /* def */
	{
	struct value z = Value_Empty();
	if (list_isempty(lst)) return z;
	return Value_Copy(lst->tail->val);
	}

struct list *list_append(struct value v, struct list *lst) /* def */
	{
	struct list *ret = lst;
	if (lst)
		{
		struct listnode *n = listnode_new(v);
		n->next = NULL;
		if (lst->tail)
			lst->tail->next = n;
		else
			{
			lst->head = n;
			}
		lst->tail = n;
		lst->length++;
		}
	else
		diagnose(DIAG_ERROR, DBG_LIST,"la", 0,0, "null list in append");
	return(ret);
	}

struct list *list_insert(struct value v, struct list *lst) /* def */
	{
	/* just returns the lst pointer */
	if(lst)
		{
		struct listnode *n = listnode_new(v);
		n->next = lst->head;
		lst->head = n;
		if (!(lst->tail))
			lst->tail = n;
		lst->length++;
		}
	else
		diagnose(DIAG_ERROR, DBG_LIST,"li", 0,0, "null list in insert");
	return(lst);
	}

struct list *list_push(struct value v, struct list *lst) /* def */
	{
	return(list_insert(v, lst));
	}

/* returns a new copy of the rest of the list */
struct list *list_rest(const struct list *lst) /* def */
	{
	struct list *r = list_new();
	if (!lst || !lst->head || !lst->head->next)
		return r;
	for (struct listnode *n = lst->head->next; n; n = n->next)
		list_append(n->val, r);    /* copies each element */
	return r;
	}

struct value list_pop(struct list *list) /* def */
	{
	struct value ret = Value_Empty();
	if(! list_isempty(list))
		{
		struct listnode *old = list->head;
		ret = Value_Copy(old->val);
		Value_Free(old->val);
		list->head = old->next;
		if(NULL == list->head)
			list->tail = NULL;
		list->length--;
		free(old);
		}
	return(ret);
	}

struct list *list_split(struct list *lst) /* def */
	{
	struct list *rest = list_new();
	if (!lst || !lst->head || !lst->head->next)
		return rest;                 /* DEBUG: nothing to split */

	/* WARN: mutates original; callers must own lst */

	rest->head   = lst->head->next;
	rest->tail   = lst->tail;
	rest->length = lst->length - 1;

	lst->head->next = NULL;
	lst->tail       = lst->head;
	lst->length     = 1;
	return rest;
	}


int
list_find(struct value v, const struct list *lst) /* def */
	{
	int ret = MEANS_NOT_FOUND;
	int i = 0;
	if (lst)
		{
		for (const struct listnode *n = list_head(lst); n; n = listnode_next(n), i++)
			if (Value_Equal(listnode_val(n), v))
				{
				ret = i;
				break;
				}
		}
	return ret;
	}

struct list *list_copy(const struct list *lst) /* def */
	{
	struct list *c = list_new();
	if (!lst) return c;
	for (struct listnode *n = lst->head; n; n = n->next)
		list_append(n->val, c);
	return c;
	}
struct listnode *list_head(const struct list *lst) /* def */
	{
	return lst ? lst->head : NULL;
	}
struct listnode *list_tail(const struct list *lst) /* def */
	{
	return lst ? lst->tail : NULL;
	}
int list_get_refcount(const struct list *lst) /* def */
	{
	return lst ? lst->refcount : 0;
	}

int list_incref(struct list *lst) /* def */
	{
	int ret = 0;
	if (lst)
		{
		lst->refcount++;
		ret = lst->refcount;  /* value after increment */
		}
	return(ret);
	}

int list_decref(struct list *lst) /* def */
	{
	int ret = 0;
	if (lst)
		{
		if(--lst->refcount <= 0)
			list_free(lst);
		}
	return(ret);
	}

struct value listnode_val(const struct listnode *n) /* def */
	{
	return n ? n->val : Value_Empty();
	}

struct listnode *list_next(const struct listnode *n) /* def */
	{
	return n ? n->next : NULL;
	}

struct listnode *listnode_next(const struct listnode *n) /* def */
	{
	return n ? n->next : NULL;
	}

struct listnode *list_begin(const struct list *lst) /* def */
	{
	return lst ? lst->head : NULL;
	}

const struct value *
listnode_value(const struct listnode *n) /* def */
	{
	if(n)
		return(&n->val);
	else
		return(NULL);
	}

int list_equal(const struct list *a, const struct list *b) {
    if (a == b) return 1;
    if (!a || !b || list_length(a) != list_length(b)) return 0;
    struct listnode *na = list_head(a);
    struct listnode *nb = list_head(b);
    while (na && nb) {
        if (!Value_Equal(na->val, nb->val)) return 0;  /* recurses for sublists */
        na = list_next(na);
        nb = list_next(nb);
    }
    return 1;
}

int print_list(const struct list *lst) /* def */
	{
	struct listnode *n;
	int printed = 0;
	for (n = lst ? list_head(lst) : NULL; n; n = list_next(n))
		{
		printed = Value_Print(&n->val);
		if ((0 < printed) && list_next(n))
			printf(" ");
		}
	return(printed);
	}

unsigned int list_OK(const struct list *l)
	{
	unsigned int bad = 0;
	if (l == NULL)
		bad |= LIST_BADNULL;
	else
		{
		if (l->refcount <= 0)
			bad |= LIST_BADREF;
		if (l->length < 0)
			bad |= LIST_BADUNK;
		else if (l->length == 0)
			{
			if (l->head != NULL || l->tail != NULL)
				bad |= LIST_BADSTALE;
			}
		else
			{
			if (l->head == NULL)
				bad |= LIST_BADHEAD;
			if (l->tail == NULL)
				bad |= LIST_BADTAIL;
			if (l->head != NULL && l->tail != NULL)
				{
				int count = 0;
				struct listnode *p = l->head;
				while (p != NULL)
					{
					count++;
					if (count > l->length)
						{
						bad |= LIST_BADLONG;
						break;
						}
					if (count == l->length && p != l->tail)
						bad |= LIST_BADTAIL;
					p = p->next;
					}
				if (!(bad & LIST_BADLONG) && count < l->length)
					bad |= LIST_BADSHORT;
				if (!(bad & (LIST_BADLONG | LIST_BADSHORT)) && l->tail && l->tail->next != NULL)
					bad |= LIST_BADTAIL;  
				}
			}
		}
	return bad;
	}
