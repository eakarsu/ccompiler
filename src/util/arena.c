#include "arena.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static ArenaChunk *chunk_new(size_t data_size) {
    ArenaChunk *c = malloc(sizeof(ArenaChunk) + data_size);
    assert(c && "out of memory");
    c->next = NULL;
    c->size = data_size;
    c->used = 0;
    return c;
}

void arena_init(Arena *a, size_t chunk_size) {
    if (chunk_size == 0) chunk_size = ARENA_DEFAULT_CHUNK_SIZE;
    a->default_chunk_size = chunk_size;
    a->current = chunk_new(chunk_size);
    a->chunks = a->current;
}

void arena_destroy(Arena *a) {
    ArenaChunk *c = a->chunks;
    while (c) {
        ArenaChunk *next = c->next;
        free(c);
        c = next;
    }
    a->current = NULL;
    a->chunks = NULL;
}

void *arena_alloc(Arena *a, size_t size) {
    /* Align to 8 bytes */
    size = (size + 7) & ~(size_t)7;

    if (a->current->used + size > a->current->size) {
        /* Need a new chunk */
        size_t cs = a->default_chunk_size;
        if (size > cs) cs = size;  /* oversized allocation */
        ArenaChunk *c = chunk_new(cs);
        c->next = a->chunks;
        a->chunks = c;
        a->current = c;
    }

    void *ptr = a->current->data + a->current->used;
    a->current->used += size;
    return ptr;
}

void *arena_calloc(Arena *a, size_t size) {
    void *p = arena_alloc(a, size);
    memset(p, 0, size);
    return p;
}

char *arena_strdup(Arena *a, const char *s) {
    size_t len = strlen(s);
    char *p = arena_alloc(a, len + 1);
    memcpy(p, s, len + 1);
    return p;
}

char *arena_strndup(Arena *a, const char *s, size_t n) {
    char *p = arena_alloc(a, n + 1);
    memcpy(p, s, n);
    p[n] = '\0';
    return p;
}

void arena_reset(Arena *a) {
    /* Reset all chunks but keep them allocated for reuse */
    for (ArenaChunk *c = a->chunks; c; c = c->next) {
        c->used = 0;
    }
    a->current = a->chunks;
}
