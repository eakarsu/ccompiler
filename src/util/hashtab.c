#include "hashtab.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TOMBSTONE ((const char *)(uintptr_t)1)
#define LOAD_FACTOR_NUM 7
#define LOAD_FACTOR_DEN 10

uint32_t hash_string(const char *s, size_t len) {
    /* FNV-1a */
    uint32_t h = 2166136261u;
    for (size_t i = 0; i < len; i++) {
        h ^= (uint8_t)s[i];
        h *= 16777619u;
    }
    return h;
}

static uint32_t hash_ptr(const char *key) {
    /* For interned strings, hash the pointer itself */
    uintptr_t p = (uintptr_t)key;
    p = ((p >> 16) ^ p) * 0x45d9f3b;
    p = ((p >> 16) ^ p) * 0x45d9f3b;
    p = (p >> 16) ^ p;
    return (uint32_t)p;
}

void hashtab_init(HashTab *t, int initial_size) {
    if (initial_size < 16) initial_size = 16;
    /* Round up to power of 2 */
    int sz = 16;
    while (sz < initial_size) sz *= 2;
    t->entries = calloc(sz, sizeof(HashEntry));
    assert(t->entries);
    t->size = sz;
    t->count = 0;
    t->tombs = 0;
}

void hashtab_free(HashTab *t) {
    free(t->entries);
    t->entries = NULL;
    t->size = t->count = t->tombs = 0;
}

static void hashtab_resize(HashTab *t, int new_size) {
    HashEntry *old = t->entries;
    int old_size = t->size;

    t->entries = calloc(new_size, sizeof(HashEntry));
    assert(t->entries);
    t->size = new_size;
    t->count = 0;
    t->tombs = 0;

    for (int i = 0; i < old_size; i++) {
        if (old[i].key && old[i].key != TOMBSTONE) {
            hashtab_put(t, old[i].key, old[i].value);
        }
    }
    free(old);
}

void *hashtab_get(HashTab *t, const char *key) {
    if (!t->entries || t->count == 0) return NULL;

    uint32_t mask = t->size - 1;
    uint32_t idx = hash_ptr(key) & mask;

    for (;;) {
        HashEntry *e = &t->entries[idx];
        if (!e->key) return NULL;
        if (e->key == key) return e->value;  /* pointer equality (interned) */
        idx = (idx + 1) & mask;
    }
}

bool hashtab_put(HashTab *t, const char *key, void *value) {
    if ((t->count + t->tombs + 1) * LOAD_FACTOR_DEN > t->size * LOAD_FACTOR_NUM) {
        hashtab_resize(t, t->size * 2);
    }

    uint32_t mask = t->size - 1;
    uint32_t idx = hash_ptr(key) & mask;
    int tomb_idx = -1;

    for (;;) {
        HashEntry *e = &t->entries[idx];
        if (!e->key) {
            if (tomb_idx >= 0) {
                /* Reuse tombstone slot */
                t->entries[tomb_idx].key = key;
                t->entries[tomb_idx].value = value;
                t->tombs--;
            } else {
                e->key = key;
                e->value = value;
            }
            t->count++;
            return true; /* new entry */
        }
        if (e->key == TOMBSTONE) {
            if (tomb_idx < 0) tomb_idx = idx;
        } else if (e->key == key) {
            e->value = value;
            return false; /* updated existing */
        }
        idx = (idx + 1) & mask;
    }
}

bool hashtab_delete(HashTab *t, const char *key) {
    if (!t->entries || t->count == 0) return false;

    uint32_t mask = t->size - 1;
    uint32_t idx = hash_ptr(key) & mask;

    for (;;) {
        HashEntry *e = &t->entries[idx];
        if (!e->key) return false;
        if (e->key == key) {
            e->key = TOMBSTONE;
            e->value = NULL;
            t->count--;
            t->tombs++;
            return true;
        }
        idx = (idx + 1) & mask;
    }
}

void hashtab_clear(HashTab *t) {
    memset(t->entries, 0, sizeof(HashEntry) * t->size);
    t->count = 0;
    t->tombs = 0;
}

HashIter hashtab_iter(HashTab *t) {
    return (HashIter){.tab = t, .index = 0};
}

bool hashtab_next(HashIter *it, const char **key, void **value) {
    while (it->index < it->tab->size) {
        HashEntry *e = &it->tab->entries[it->index++];
        if (e->key && e->key != TOMBSTONE) {
            *key = e->key;
            *value = e->value;
            return true;
        }
    }
    return false;
}
