#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

/* Chunk-based arena allocator. All allocations are bump-pointer within chunks.
 * O(1) teardown by freeing the chunk list. */

typedef struct ArenaChunk {
    struct ArenaChunk *next;
    size_t size;
    size_t used;
    char data[];      /* flexible array member */
} ArenaChunk;

typedef struct Arena {
    ArenaChunk *current;
    ArenaChunk *chunks;    /* linked list of all chunks for freeing */
    size_t default_chunk_size;
} Arena;

#define ARENA_DEFAULT_CHUNK_SIZE (64 * 1024) /* 64 KiB */

void  arena_init(Arena *a, size_t chunk_size);
void  arena_destroy(Arena *a);
void *arena_alloc(Arena *a, size_t size);
void *arena_calloc(Arena *a, size_t size);
char *arena_strdup(Arena *a, const char *s);
char *arena_strndup(Arena *a, const char *s, size_t n);
void  arena_reset(Arena *a);    /* reset for reuse without freeing chunks */

/* Convenience: alloc a typed object */
#define arena_new(a, T) ((T *)arena_calloc(a, sizeof(T)))

#endif /* ARENA_H */
