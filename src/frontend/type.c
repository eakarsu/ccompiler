#include "cc.h"

/* Canonical type instances */
Type *ty_void;
Type *ty_bool;
Type *ty_char;
Type *ty_schar;
Type *ty_uchar;
Type *ty_short;
Type *ty_ushort;
Type *ty_int;
Type *ty_uint;
Type *ty_long;
Type *ty_ulong;
Type *ty_llong;
Type *ty_ullong;
Type *ty_float;
Type *ty_double;
Type *ty_ldouble;

static Type *make_type(Arena *a, TypeKind kind, int size, int align, bool is_unsigned) {
    Type *t = arena_new(a, Type);
    t->kind = kind;
    t->size = size;
    t->align = align;
    t->is_unsigned = is_unsigned;
    return t;
}

void type_init(Arena *arena) {
    ty_void    = make_type(arena, TY_VOID, 0, 1, false);
    ty_bool    = make_type(arena, TY_BOOL, 1, 1, true);
    ty_char    = make_type(arena, TY_CHAR, 1, 1, false);
    ty_schar   = make_type(arena, TY_CHAR, 1, 1, false);
    ty_uchar   = make_type(arena, TY_CHAR, 1, 1, true);
    ty_short   = make_type(arena, TY_SHORT, 2, 2, false);
    ty_ushort  = make_type(arena, TY_SHORT, 2, 2, true);
    ty_int     = make_type(arena, TY_INT, 4, 4, false);
    ty_uint    = make_type(arena, TY_INT, 4, 4, true);
    ty_long    = make_type(arena, TY_LONG, 8, 8, false);
    ty_ulong   = make_type(arena, TY_LONG, 8, 8, true);
    ty_llong   = make_type(arena, TY_LLONG, 8, 8, false);
    ty_ullong  = make_type(arena, TY_LLONG, 8, 8, true);
    ty_float   = make_type(arena, TY_FLOAT, 4, 4, false);
    ty_double  = make_type(arena, TY_DOUBLE, 8, 8, false);
    ty_ldouble = make_type(arena, TY_LDOUBLE, 16, 16, false);
}

Type *type_ptr(Arena *a, Type *base) {
    Type *t = arena_new(a, Type);
    t->kind = TY_PTR;
    t->size = 8;
    t->align = 8;
    t->base = base;
    return t;
}

Type *type_array(Arena *a, Type *base, int len) {
    Type *t = arena_new(a, Type);
    t->kind = TY_ARRAY;
    t->base = base;
    t->array_len = len;
    t->size = base->size * len;
    t->align = base->align;
    if (len < 0) {
        t->is_incomplete = true;
        t->size = 0;
    }
    return t;
}

Type *type_func(Arena *a, Type *ret) {
    Type *t = arena_new(a, Type);
    t->kind = TY_FUNC;
    t->return_ty = ret;
    t->size = 0;
    t->align = 1;
    return t;
}

Type *type_struct(Arena *a, const char *tag) {
    Type *t = arena_new(a, Type);
    t->kind = TY_STRUCT;
    t->tag = tag;
    t->is_incomplete = true;
    t->size = 0;
    t->align = 1;
    return t;
}

Type *type_union(Arena *a, const char *tag) {
    Type *t = arena_new(a, Type);
    t->kind = TY_UNION;
    t->tag = tag;
    t->is_incomplete = true;
    t->size = 0;
    t->align = 1;
    return t;
}

Type *type_enum(Arena *a, const char *tag) {
    Type *t = arena_new(a, Type);
    t->kind = TY_ENUM;
    t->tag = tag;
    t->size = 4;
    t->align = 4;
    return t;
}

Type *type_copy(Arena *a, Type *t) {
    Type *c = arena_alloc(a, sizeof(Type));
    *c = *t;
    return c;
}

bool type_is_integer(Type *t) {
    switch (t->kind) {
    case TY_BOOL: case TY_CHAR: case TY_SHORT:
    case TY_INT: case TY_LONG: case TY_LLONG:
    case TY_ENUM:
        return true;
    default:
        return false;
    }
}

bool type_is_floating(Type *t) {
    return t->kind == TY_FLOAT || t->kind == TY_DOUBLE || t->kind == TY_LDOUBLE;
}

bool type_is_numeric(Type *t) {
    return type_is_integer(t) || type_is_floating(t);
}

bool type_is_scalar(Type *t) {
    return type_is_numeric(t) || t->kind == TY_PTR;
}

bool type_is_compatible(Type *a, Type *b) {
    if (a->kind != b->kind) return false;
    switch (a->kind) {
    case TY_PTR:
        return type_is_compatible(a->base, b->base);
    case TY_ARRAY:
        if (a->array_len >= 0 && b->array_len >= 0 && a->array_len != b->array_len)
            return false;
        return type_is_compatible(a->base, b->base);
    case TY_FUNC: {
        if (!type_is_compatible(a->return_ty, b->return_ty)) return false;
        Param *pa = a->params, *pb = b->params;
        while (pa && pb) {
            if (!type_is_compatible(pa->ty, pb->ty)) return false;
            pa = pa->next;
            pb = pb->next;
        }
        return pa == NULL && pb == NULL;
    }
    case TY_STRUCT:
    case TY_UNION:
        return a->tag == b->tag;  /* interned pointer comparison */
    default:
        return a->is_unsigned == b->is_unsigned;
    }
}

Type *type_common(Arena *arena, Type *a, Type *b) {
    if (a->kind == TY_LDOUBLE || b->kind == TY_LDOUBLE) return ty_ldouble;
    if (a->kind == TY_DOUBLE || b->kind == TY_DOUBLE) return ty_double;
    if (a->kind == TY_FLOAT || b->kind == TY_FLOAT) return ty_float;

    /* Integer promotion */
    if (a->size < 4) a = ty_int;
    if (b->size < 4) b = ty_int;
    (void)arena;

    if (a->size != b->size) return a->size > b->size ? a : b;
    if (a->is_unsigned) return a;
    if (b->is_unsigned) return b;
    return a;
}
