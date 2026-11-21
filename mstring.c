#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mstring.h"
#include "help.h"

/*
* Means uses a struct mstring to store C string data. "size" is the size of the
* allocated buffer, including a '\0' at the end. Functions writing to data should
* never exceed size bytes.

struct mstring {
    char *data;      * C string
    int   len;       * strlen(data); O(1) length
    int   size;      * allocated bytes, including the NUL
    int   refcount;
	};

struct mstring *mstring_new(const char *s);
struct mstring *mstring_incref(struct mstring *s);
void            mstring_decref(struct mstring *s);
const char     *mstring_cstr(const struct mstring *s);
int             mstring_len(const struct mstring *s);
struct mstring *mstring_split(struct mstring *s, int n);   / * split * /
void            mstring_append(const char *extra, struct mstring *s );
*/


int mstring_refcount(const struct mstring *m)
	{
	return m ? m->refcount : 0;
	}

char *mstring_data(struct mstring *m)
	{
	return m ? m->data : NULL;
	}

struct mstring *mstring_new(const char *s)
	{
	if(!s)
		{
		s="";
		}
	/* s is OK for strdup() */
	struct mstring *m = calloc(1, sizeof *m);
	if(m)
		{
		m->data = strdup(s);
		m->len  = strlen(m->data);
		m->size = m->len + 1;
		m->refcount = 1;
		}
	else
		{
		diagnose(DIAG_FATAL,DBG_MSTRING,0,0,"mem error in mstring_new");
		exit(1); /* unreachable */
		}
	return m;
	}

void mstring_free(struct mstring *m)
	{
	if(m && (--m->refcount < 1))
		{
		if(m->data)
			free(m->data);
		free(m);
		}
	}

void mstring_decref(struct mstring *s)
	{
	mstring_free(s);
	}

struct mstring * mstring_incref(struct mstring *m)
	{
	if(m) m->refcount++;
	return(m);
	}


const char     *mstring_cstr(const struct mstring *m)
	{
	char *ret = NULL;
	if(m) ret = m->data;
	return(ret);
	}

int mstring_len(const struct mstring *m)
	{
	int ret = 0;
	if(m) ret = m->len;
	return(ret);
	}

struct mstring  *mstring_rest(struct mstring *m, int n)
	{
	struct mstring *ret = NULL;
	if (!m || !m->data)
		ret = mstring_new("");
	else
		{
		if (n < 0) n = 0;
		if (n > m->len) n = m->len;  /* stay inside len, hence inside size */
		ret = mstring_new(m->data + n);   /* copies [n, len) */
		}
	return ret;
	}

struct mstring  *mstring_split(struct mstring *m, int n)
	{
	struct mstring *ret = NULL;
	if (!m || !m->data)
		ret = mstring_new("");
	else
		{
		if (n < 0) n = 0;
		if (n > m->len) n = m->len;          /* stay inside len, hence inside size */
		ret = mstring_new(m->data + n);   /* copies [n, len) */
		m->data[n] = '\0';                   /* truncate original in place */
		m->len = n;
		/* size unchanged; spare capacity is fine */
		}
	return ret;
	}

void mstring_append( const char *extra, struct mstring *m)
	{
	if (!m || !extra) return; /* ERROR: null */
	int extralen = strlen(extra);
	if (extralen == 0) return;
	int need = m->len + extralen + 1;          /* including NUL */
	if (need > m->size)
		{
		int newsize = m->size > 0 ? m->size : 1;
		while (newsize < need) newsize *= 2;
		char *p = realloc(m->data, newsize);
		if (!p) return;                        /* FATAL - alloc failed; diagnose but leave m unchanged */
		m->data = p;
		m->size = newsize;
		}
	memcpy(m->data + m->len, extra, extralen + 1);
	m->len += extralen;
	}

void mstring_insert(const char *extra, struct mstring *m )
	{
	if (!m || !extra) return;     /* ERROR: null */
	int extralen = strlen(extra);
	if (extralen == 0) return;
	int need = m->len + extralen + 1;          /* including NUL */
	if (need > m->size)
		{
		int newsize = m->size > 0 ? m->size : 1;
		while (newsize < need) newsize *= 2;
		char *p = realloc(m->data, newsize);
		if (!p) return;                        /* FATAL: realloc failed — diagnose and exit or abort */
		m->data = p;
		m->size = newsize;
		}
	memmove(m->data + extralen, m->data, m->len + 1);  /* shift right */
	memcpy(m->data, extra, extralen);                    /* write at front */
	m->len += extralen;
	}

int mstring_find(const struct mstring *needle, const struct mstring *hay)
	{
	if (!hay || !needle || mstring_len(needle) == 0) return -1;
	const char *h = mstring_cstr(hay);
	const char *p = strstr(h, mstring_cstr(needle));
	return p ? (int)(p - h) : -1;
	}
