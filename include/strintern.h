#ifndef STRINTERN_H
#define STRINTERN_H

#include <stddef.h>

/* String interning: deduplicate strings so pointer comparison works.
 * All interned strings are stored in an arena and indexed in a hash set. */

typedef struct Arena Arena;

typedef struct {
    Arena *arena;
    /* Hash set of (char*) pointers, open addressing */
    const char **buckets;
    int size;
    int count;
} InternTab;

void        intern_init(InternTab *tab, Arena *arena);
void        intern_free(InternTab *tab);
const char *intern(InternTab *tab, const char *s);
const char *intern_n(InternTab *tab, const char *s, size_t len);

#endif /* STRINTERN_H */
