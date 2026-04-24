#ifndef HASHTAB_H
#define HASHTAB_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Open-addressing hash table with interned string keys (pointer equality).
 * Values are void pointers. Supports deletion via tombstones. */

typedef struct {
    const char *key;   /* interned string (NULL = empty, TOMBSTONE = deleted) */
    void *value;
} HashEntry;

typedef struct {
    HashEntry *entries;
    int size;       /* allocated slots */
    int count;      /* live entries */
    int tombs;      /* tombstone count */
} HashTab;

void  hashtab_init(HashTab *t, int initial_size);
void  hashtab_free(HashTab *t);
void *hashtab_get(HashTab *t, const char *key);
bool  hashtab_put(HashTab *t, const char *key, void *value);
bool  hashtab_delete(HashTab *t, const char *key);
void  hashtab_clear(HashTab *t);

/* Iteration */
typedef struct {
    HashTab *tab;
    int index;
} HashIter;

HashIter hashtab_iter(HashTab *t);
bool     hashtab_next(HashIter *it, const char **key, void **value);

/* Hash function (FNV-1a) for use by other modules */
uint32_t hash_string(const char *s, size_t len);

#endif /* HASHTAB_H */
