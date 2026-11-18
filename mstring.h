#ifndef MSTRING_INC
#define MSTRING_INC
#include <stddef.h>

/*
* Means uses a struct mstring to store C string data. "size" is the size of the
* allocated buffer, including a '\0' at the end. Functions writing to data should
* never exceed size bytes.
*/

struct mstring {
    char *data;      /* C string */
    int   len;       /* strlen(data); O(1) length */
    int   size;      /* allocated bytes, including the NUL */
    int   refcount;
};


void mstring_free(struct mstring *m);
struct mstring *mstring_new(const char *s);
struct mstring *mstring_incref(struct mstring *s);
void            mstring_decref(struct mstring *s);
const char     *mstring_cstr(const struct mstring *s);
int             mstring_len(const struct mstring *s);
struct mstring  *mstring_split(struct mstring *m, int n);
struct mstring *mstring_rest(struct mstring *m, int n);
void            mstring_append(struct mstring *s, const char *extra);
void mstring_insert(struct mstring *m, const char *extra);
int mstring_find(const struct mstring *needle, const struct mstring *hay );
int mstring_refcount(const struct mstring *m);
char *mstring_data(struct mstring *m);   /* non-const for the rare mutator */


#endif /* MSTRING_INC */
