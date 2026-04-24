#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>

/*
 * Type-safe dynamic array macros.
 *
 * Usage:
 *   Vec(int) v = {0};
 *   vec_push(&v, 42);
 *   for (int i = 0; i < v.len; i++) printf("%d\n", v.data[i]);
 *   vec_free(&v);
 *
 * Or with arena allocation:
 *   Vec(int) v = {0};
 *   vec_push_arena(&v, arena, 42);
 *   // no free needed, arena owns the memory
 */

#define Vec(T) struct { T *data; int len, cap; }

#define vec_init(v) do { (v)->data = NULL; (v)->len = 0; (v)->cap = 0; } while(0)

#define vec_free(v) do { free((v)->data); (v)->data = NULL; (v)->len = (v)->cap = 0; } while(0)

#define vec_grow(v, newcap) do { \
    (v)->cap = (newcap); \
    (v)->data = realloc((v)->data, sizeof(*(v)->data) * (v)->cap); \
} while(0)

#define vec_ensure(v, needed) do { \
    if ((v)->len + (needed) > (v)->cap) { \
        int _nc = (v)->cap ? (v)->cap * 2 : 8; \
        while (_nc < (v)->len + (needed)) _nc *= 2; \
        vec_grow(v, _nc); \
    } \
} while(0)

#define vec_push(v, item) do { \
    vec_ensure(v, 1); \
    (v)->data[(v)->len++] = (item); \
} while(0)

#define vec_pop(v) ((v)->data[--(v)->len])

#define vec_last(v) ((v)->data[(v)->len - 1])

#define vec_clear(v) ((v)->len = 0)

#define vec_reserve(v, n) do { \
    if ((n) > (v)->cap) vec_grow(v, n); \
} while(0)

/* Arena-backed versions: allocate from arena instead of realloc */
#define vec_grow_arena(v, arena, newcap) do { \
    void *_old = (v)->data; \
    int _oldsz = sizeof(*(v)->data) * (v)->cap; \
    (v)->cap = (newcap); \
    (v)->data = arena_alloc(arena, sizeof(*(v)->data) * (v)->cap); \
    if (_old) memcpy((v)->data, _old, _oldsz); \
} while(0)

#define vec_ensure_arena(v, arena, needed) do { \
    if ((v)->len + (needed) > (v)->cap) { \
        int _nc = (v)->cap ? (v)->cap * 2 : 8; \
        while (_nc < (v)->len + (needed)) _nc *= 2; \
        vec_grow_arena(v, arena, _nc); \
    } \
} while(0)

#define vec_push_arena(v, arena, item) do { \
    vec_ensure_arena(v, arena, 1); \
    (v)->data[(v)->len++] = (item); \
} while(0)

#endif /* VEC_H */
