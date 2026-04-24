#ifndef TYPE_H
#define TYPE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Arena Arena;
typedef struct Type Type;

typedef enum {
    TY_VOID,
    TY_BOOL,
    TY_CHAR,
    TY_SHORT,
    TY_INT,
    TY_LONG,
    TY_LLONG,
    TY_FLOAT,
    TY_DOUBLE,
    TY_LDOUBLE,
    TY_PTR,
    TY_ARRAY,
    TY_STRUCT,
    TY_UNION,
    TY_ENUM,
    TY_FUNC,
} TypeKind;

typedef struct Member {
    struct Member *next;
    const char *name;
    Type *ty;
    int offset;
    int bit_width;    /* -1 if not bitfield */
    int bit_offset;
} Member;

typedef struct Param {
    struct Param *next;
    const char *name;
    Type *ty;
} Param;

struct Type {
    TypeKind kind;
    int size;         /* sizeof */
    int align;        /* alignof */
    bool is_unsigned;
    bool is_const;
    bool is_volatile;
    bool is_restrict;
    bool is_atomic;
    bool is_incomplete;

    /* Pointer / Array */
    Type *base;
    int array_len;    /* -1 for VLA / incomplete */
    bool is_vla;
    void *vla_size;   /* Node* for VLA size expression (set in parser) */

    /* Struct / Union */
    const char *tag;
    Member *members;

    /* Enum */
    /* (members reused for enum constants in sema) */

    /* Function */
    Type *return_ty;
    Param *params;
    bool is_variadic;

    /* Typedef name (for diagnostics) */
    const char *typedef_name;
};

/* Canonical types (created once) */
extern Type *ty_void;
extern Type *ty_bool;
extern Type *ty_char;
extern Type *ty_schar;
extern Type *ty_uchar;
extern Type *ty_short;
extern Type *ty_ushort;
extern Type *ty_int;
extern Type *ty_uint;
extern Type *ty_long;
extern Type *ty_ulong;
extern Type *ty_llong;
extern Type *ty_ullong;
extern Type *ty_float;
extern Type *ty_double;
extern Type *ty_ldouble;

void  type_init(Arena *arena);
Type *type_ptr(Arena *a, Type *base);
Type *type_array(Arena *a, Type *base, int len);
Type *type_func(Arena *a, Type *ret);
Type *type_struct(Arena *a, const char *tag);
Type *type_union(Arena *a, const char *tag);
Type *type_enum(Arena *a, const char *tag);
Type *type_copy(Arena *a, Type *t);
bool  type_is_integer(Type *t);
bool  type_is_floating(Type *t);
bool  type_is_numeric(Type *t);
bool  type_is_scalar(Type *t);
bool  type_is_compatible(Type *a, Type *b);
Type *type_common(Arena *arena, Type *a, Type *b);

#endif /* TYPE_H */
