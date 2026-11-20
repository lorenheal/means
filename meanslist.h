#ifndef MEANSLIST_H
#define MEANSLIST_H


#include "means.h"
#include "value.h"   /* for struct value */

struct listnode {
    struct value     val;
    struct listnode *next;
};

struct list {
    struct listnode *head;
    struct listnode *tail;
    int     length;
	int 	refcount;
};

#define LIST_FIND_NOT_FOUND (0)
#define LIST_FIRST_POSITION (1)

/* lifecycle */
struct list *list_new(void);
void         list_free(struct list *lst);

struct listnode *listnode_new(struct value v);
const struct value *listnode_value(const struct listnode *);

/* O(1) operations */
struct value list_first(const struct list *lst); /* head value; Empty if empty */
struct list *list_split(struct list *lst); /* returns rest; leaves first node */
int          list_length(const struct list *lst);
struct value list_last(const struct list *lst);  /* tail value; Empty if empty */
struct value list_pop(struct list *lst);  /* tail value; Empty if empty */
void         list_append(struct value v, struct list *lst); /* copy v, O(1) */
void         list_insert(struct value v, struct list *lst); /* copy v, O(1) */
void         list_push(struct value v, struct list *lst); /* copy v, O(1) */

/* convenience */
struct list *list_rest(const struct list *lst);  /* new header, copied tail, length-1 */
struct list *list_copy(const struct list *lst);  /* deep copy of values */
int          list_isempty(const struct list *lst);
int 		list_find(struct value v, const struct list *lst);
void print_list(const struct list *lst);
struct listnode *list_head(const struct list *lst);
struct listnode *list_next(const struct listnode *lst);
struct listnode *list_tail(const struct list *lst);
int list_get_length(const struct list *lst);   /* already have list_length */
int list_get_refcount(const struct list *lst);
void list_incref(struct list *lst);
void list_decref(struct list *lst);

struct value listnode_val(const struct listnode *n);
struct listnode *listnode_next(const struct listnode *n);

#endif
