#include "strintern.h"
#include "arena.h"
#include "hashtab.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define INTERN_INITIAL_SIZE 256
#define INTERN_LOAD_FACTOR_NUM 7
#define INTERN_LOAD_FACTOR_DEN 10

void intern_init(InternTab *tab, Arena *arena) {
    tab->arena = arena;
    tab->size = INTERN_INITIAL_SIZE;
    tab->count = 0;
    tab->buckets = calloc(tab->size, sizeof(const char *));
    assert(tab->buckets);
}

void intern_free(InternTab *tab) {
    free(tab->buckets);
    tab->buckets = NULL;
    tab->size = tab->count = 0;
}

static void intern_resize(InternTab *tab) {
    int new_size = tab->size * 2;
    const char **old = tab->buckets;
    int old_size = tab->size;

    tab->buckets = calloc(new_size, sizeof(const char *));
    assert(tab->buckets);
    tab->size = new_size;
    tab->count = 0;

    uint32_t mask = new_size - 1;
    for (int i = 0; i < old_size; i++) {
        if (old[i]) {
            uint32_t h = hash_string(old[i], strlen(old[i])) & mask;
            while (tab->buckets[h]) h = (h + 1) & mask;
            tab->buckets[h] = old[i];
            tab->count++;
        }
    }
    free(old);
}

const char *intern_n(InternTab *tab, const char *s, size_t len) {
    if ((tab->count + 1) * INTERN_LOAD_FACTOR_DEN > tab->size * INTERN_LOAD_FACTOR_NUM) {
        intern_resize(tab);
    }

    uint32_t mask = tab->size - 1;
    uint32_t h = hash_string(s, len) & mask;

    for (;;) {
        const char *entry = tab->buckets[h];
        if (!entry) break;
        if (strlen(entry) == len && memcmp(entry, s, len) == 0) {
            return entry;
        }
        h = (h + 1) & mask;
    }

    /* Not found, insert new */
    char *dup = arena_strndup(tab->arena, s, len);
    tab->buckets[h] = dup;
    tab->count++;
    return dup;
}

const char *intern(InternTab *tab, const char *s) {
    return intern_n(tab, s, strlen(s));
}
