/*
 * parser.c -- C11 recursive descent parser with Pratt precedence climbing
 *
 * Consumes a TokenList produced by the lexer/preprocessor and builds an AST
 * rooted at an ND_PROGRAM node.  All nodes are allocated from cc->arena.
 *
 * Entry point: cc_parse(CompilerCtx *cc, TokenList *tokens)
 */

#include "cc.h"

/* ------------------------------------------------------------------ */
/*  Parser state                                                       */
/* ------------------------------------------------------------------ */

typedef struct Scope Scope;

/* A typedef / tag / ordinary-identifier entry in the scope chain. */
typedef struct ScopeEntry {
    const char *name;          /* interned */
    Obj *obj;                  /* variable / enum constant */
    Type *tag;                 /* struct/union/enum tag type */
    Type *typedef_ty;          /* typedef target type */
} ScopeEntry;

struct Scope {
    Scope *parent;
    HashTab vars;              /* name -> Obj* (variables, enum constants) */
    HashTab tags;              /* name -> Type* (struct/union/enum tags) */
    HashTab typedefs;          /* name -> Type* (typedef names) */
};

typedef struct {
    CompilerCtx *cc;
    Arena *arena;
    Token *tok;                /* token array */
    int ntok;                  /* number of tokens */
    int pos;                   /* current position */

    Scope *scope;              /* current scope */

    /* Linked list of all local variables (for current function) */
    Obj *locals;

    /* Linked list of all global objects */
    Obj *globals;

    /* Labels for goto resolution within the current function */
    Node *labels;              /* linked via label_next */
    Node *gotos;               /* linked via label_next */

    /* Current switch statement node (for case/default) */
    Node *cur_switch;

    /* Current function name (for __func__) */
    const char *cur_func_name;

    /* String literal counter for unique names */
    int string_cnt;

    /* When true, suppress undeclared identifier errors (for VLA param parsing) */
    bool suppress_undecl;
} Parser;

/* ------------------------------------------------------------------ */
/*  Forward declarations                                               */
/* ------------------------------------------------------------------ */

/* Declarations */
static Node *declaration(Parser *p);
static Type *declspec(Parser *p, int *storage_class);
static Type *declarator(Parser *p, Type *base, const char **name_out);
static Type *abstract_declarator(Parser *p, Type *base);
static Type *type_suffix(Parser *p, Type *base);
static Param *func_params(Parser *p);
static Node *initializer(Parser *p, Obj *var);

/* Statements */
static Node *compound_stmt(Parser *p);
static Node *stmt(Parser *p);

/* Expressions */
static Node *expr(Parser *p);
static Node *assign_expr(Parser *p);
static Node *ternary_expr(Parser *p);
static Node *logor_expr(Parser *p);
static Node *logand_expr(Parser *p);
static Node *bitor_expr(Parser *p);
static Node *bitxor_expr(Parser *p);
static Node *bitand_expr(Parser *p);
static Node *equality_expr(Parser *p);
static Node *relational_expr(Parser *p);
static Node *shift_expr(Parser *p);
static Node *add_expr(Parser *p);
static Node *mul_expr(Parser *p);
static Node *cast_expr(Parser *p);
static Node *unary_expr(Parser *p);
static Node *postfix_expr(Parser *p);
static Node *postfix_tail(Parser *p, Node *nd);
static Node *primary_expr(Parser *p);
static Type *parse_type_name(Parser *p);
static int64_t const_expr_val(Node *n);

/* ------------------------------------------------------------------ */
/*  Token helpers                                                      */
/* ------------------------------------------------------------------ */

static Token *peek(Parser *p) {
    return &p->tok[p->pos];
}

static Token *peek_ahead(Parser *p, int n) {
    int idx = p->pos + n;
    if (idx >= p->ntok) idx = p->ntok - 1; /* clamp to EOF */
    return &p->tok[idx];
}

static SrcLoc cur_loc(Parser *p) {
    return peek(p)->loc;
}

static bool at_eof(Parser *p) {
    return peek(p)->kind == TOK_EOF;
}

static bool check(Parser *p, TokenKind k) {
    return peek(p)->kind == k;
}

static Token *advance(Parser *p) {
    Token *t = &p->tok[p->pos];
    if (t->kind != TOK_EOF)
        p->pos++;
    return t;
}

static bool match(Parser *p, TokenKind k) {
    if (peek(p)->kind == k) {
        advance(p);
        return true;
    }
    return false;
}

static Token *expect(Parser *p, TokenKind k) {
    Token *t = peek(p);
    if (t->kind != k) {
        diag_error(&p->cc->diag, t->loc,
                   "expected '%s', got '%s'",
                   token_kind_str(k), token_kind_str(t->kind));
    }
    return advance(p);
}

/* ------------------------------------------------------------------ */
/*  Scope management                                                   */
/* ------------------------------------------------------------------ */

static Scope *scope_new(Parser *p) {
    Scope *s = arena_new(p->arena, Scope);
    s->parent = p->scope;
    hashtab_init(&s->vars, 16);
    hashtab_init(&s->tags, 16);
    hashtab_init(&s->typedefs, 16);
    p->scope = s;
    return s;
}

static void scope_pop(Parser *p) {
    Scope *s = p->scope;
    p->scope = s->parent;
    hashtab_free(&s->vars);
    hashtab_free(&s->tags);
    hashtab_free(&s->typedefs);
}

static void scope_push_var(Parser *p, const char *name, Obj *obj) {
    hashtab_put(&p->scope->vars, name, obj);
}

static void scope_push_tag(Parser *p, const char *name, Type *ty) {
    hashtab_put(&p->scope->tags, name, ty);
}

static void scope_push_typedef(Parser *p, const char *name, Type *ty) {
    hashtab_put(&p->scope->typedefs, name, ty);
}

static Obj *scope_find_var(Parser *p, const char *name) {
    for (Scope *s = p->scope; s; s = s->parent) {
        Obj *o = hashtab_get(&s->vars, name);
        if (o) return o;
    }
    return NULL;
}

static Type *scope_find_tag(Parser *p, const char *name) {
    for (Scope *s = p->scope; s; s = s->parent) {
        Type *t = hashtab_get(&s->tags, name);
        if (t) return t;
    }
    return NULL;
}

static Type *scope_find_typedef(Parser *p, const char *name) {
    for (Scope *s = p->scope; s; s = s->parent) {
        Type *t = hashtab_get(&s->typedefs, name);
        if (t) return t;
    }
    return NULL;
}

/* ------------------------------------------------------------------ */
/*  Object (variable / function) creation                              */
/* ------------------------------------------------------------------ */

static Obj *new_obj(Parser *p, const char *name, Type *ty, SrcLoc loc) {
    Obj *obj = arena_new(p->arena, Obj);
    obj->name = name;
    obj->ty = ty;
    obj->loc = loc;
    return obj;
}

static Obj *new_local(Parser *p, const char *name, Type *ty, SrcLoc loc) {
    Obj *obj = new_obj(p, name, ty, loc);
    obj->next = p->locals;
    p->locals = obj;
    if (name)
        scope_push_var(p, name, obj);
    return obj;
}

static Obj *new_global(Parser *p, const char *name, Type *ty, SrcLoc loc) {
    Obj *obj = new_obj(p, name, ty, loc);
    obj->is_global = true;
    obj->next = p->globals;
    p->globals = obj;
    if (name)
        scope_push_var(p, name, obj);
    return obj;
}

/* Create a unique name for anonymous string literals */
static const char *new_string_label(Parser *p) {
    char buf[64];
    snprintf(buf, sizeof(buf), ".Lstr.%d", p->string_cnt++);
    return intern(&p->cc->strings, buf);
}

static Obj *new_string_literal(Parser *p, const char *val, int len, SrcLoc loc) {
    Type *ty = type_array(p->arena, ty_char, len);
    const char *name = new_string_label(p);
    Obj *obj = new_global(p, name, ty, loc);
    obj->string_val = val;
    obj->string_len = len;
    obj->is_definition = true;
    return obj;
}

/* ------------------------------------------------------------------ */
/*  Type helpers                                                       */
/* ------------------------------------------------------------------ */

/* Check whether the current token starts a type name (for casts, sizeof, etc.) */
static bool is_typename(Parser *p) {
    Token *t = peek(p);
    switch (t->kind) {
    case TOK_KW_VOID: case TOK_KW_CHAR: case TOK_KW_SHORT:
    case TOK_KW_INT: case TOK_KW_LONG: case TOK_KW_FLOAT:
    case TOK_KW_DOUBLE: case TOK_KW_SIGNED: case TOK_KW_UNSIGNED:
    case TOK_KW__BOOL: case TOK_KW__COMPLEX:
    case TOK_KW_STRUCT: case TOK_KW_UNION: case TOK_KW_ENUM:
    case TOK_KW_CONST: case TOK_KW_VOLATILE: case TOK_KW_RESTRICT:
    case TOK_KW__ATOMIC:
    case TOK_KW_STATIC: case TOK_KW_EXTERN: case TOK_KW_REGISTER:
    case TOK_KW_TYPEDEF: case TOK_KW_INLINE: case TOK_KW__NORETURN:
    case TOK_KW_AUTO: case TOK_KW__THREAD_LOCAL:
    case TOK_KW__ALIGNAS:
        return true;
    case TOK_IDENT:
        return scope_find_typedef(p, t->text) != NULL;
    default:
        return false;
    }
}

/* ------------------------------------------------------------------ */
/*  Declaration specifier parsing                                      */
/* ------------------------------------------------------------------ */

/* Storage class flags returned via *sc */
enum {
    SC_NONE     = 0,
    SC_TYPEDEF  = 1 << 0,
    SC_EXTERN   = 1 << 1,
    SC_STATIC   = 1 << 2,
    SC_AUTO     = 1 << 3,
    SC_REGISTER = 1 << 4,
};

static Type *parse_struct_or_union(Parser *p, bool is_struct);
static Type *parse_enum(Parser *p);

/*
 * declspec -- parse declaration-specifiers (type specifiers, qualifiers,
 * storage class).  Returns the combined base Type*.  Storage-class bits
 * are written to *sc if non-NULL.
 *
 * This uses a counter approach for the type-specifier:
 *   void=1, char=2, short=4, int=8, long=16(+16), float=32, double=64,
 *   signed=128, unsigned=256, _Bool=512
 * which lets us detect "long long", "unsigned long long", etc.
 */
static Type *declspec(Parser *p, int *sc) {
    enum {
        TS_VOID    = 1 << 0,
        TS_CHAR    = 1 << 2,
        TS_SHORT   = 1 << 4,
        TS_INT     = 1 << 6,
        TS_LONG    = 1 << 8,
        TS_LONG2   = 1 << 10,   /* second 'long' */
        TS_FLOAT   = 1 << 12,
        TS_DOUBLE  = 1 << 14,
        TS_SIGNED  = 1 << 16,
        TS_UNSIGNED= 1 << 18,
        TS_BOOL    = 1 << 20,
        TS_OTHER   = 1 << 22,   /* struct/union/enum/typedef name */
    };

    int ts = 0;
    Type *user_ty = NULL;
    bool is_const = false;
    bool is_volatile = false;
    bool is_restrict = false;
    bool is_atomic = false;
    int requested_align = 0;
    if (sc) *sc = SC_NONE;

    bool seen_any = false;

    for (;;) {
        Token *t = peek(p);
        bool consumed = true;

        switch (t->kind) {
        /* Storage class */
        case TOK_KW_TYPEDEF:  if (sc) *sc |= SC_TYPEDEF;  advance(p); break;
        case TOK_KW_EXTERN:   if (sc) *sc |= SC_EXTERN;   advance(p); break;
        case TOK_KW_STATIC:   if (sc) *sc |= SC_STATIC;   advance(p); break;
        case TOK_KW_AUTO:     if (sc) *sc |= SC_AUTO;      advance(p); break;
        case TOK_KW_REGISTER: if (sc) *sc |= SC_REGISTER; advance(p); break;

        /* Function specifiers (just consume, semantics later) */
        case TOK_KW_INLINE:
        case TOK_KW__NORETURN:
        case TOK_KW__THREAD_LOCAL:
            advance(p);
            break;

        /* Type qualifiers */
        case TOK_KW_CONST:    is_const = true;    advance(p); break;
        case TOK_KW_VOLATILE: is_volatile = true;  advance(p); break;
        case TOK_KW_RESTRICT: is_restrict = true;  advance(p); break;
        case TOK_KW__ATOMIC:  is_atomic = true;    advance(p); break;

        /* _Alignas alignment-specifier */
        case TOK_KW__ALIGNAS:
            advance(p);
            expect(p, TOK_LPAREN);
            if (is_typename(p)) {
                Type *align_ty = parse_type_name(p);
                requested_align = align_ty->align;
            } else {
                Node *align_expr = assign_expr(p);
                requested_align = (int)const_expr_val(align_expr);
            }
            expect(p, TOK_RPAREN);
            break;

        /* Type specifiers */
        case TOK_KW_VOID:     ts |= TS_VOID;     advance(p); break;
        case TOK_KW_CHAR:     ts |= TS_CHAR;     advance(p); break;
        case TOK_KW_SHORT:    ts |= TS_SHORT;    advance(p); break;
        case TOK_KW_INT:      ts |= TS_INT;      advance(p); break;
        case TOK_KW_LONG:
            if (ts & TS_LONG) ts |= TS_LONG2;
            else              ts |= TS_LONG;
            advance(p);
            break;
        case TOK_KW_FLOAT:    ts |= TS_FLOAT;    advance(p); break;
        case TOK_KW_DOUBLE:   ts |= TS_DOUBLE;   advance(p); break;
        case TOK_KW_SIGNED:   ts |= TS_SIGNED;   advance(p); break;
        case TOK_KW_UNSIGNED: ts |= TS_UNSIGNED;  advance(p); break;
        case TOK_KW__BOOL:    ts |= TS_BOOL;     advance(p); break;
        case TOK_KW__COMPLEX: advance(p); break; /* TODO: _Complex */

        /* struct / union / enum */
        case TOK_KW_STRUCT:
            if (ts & TS_OTHER) { consumed = false; break; }
            user_ty = parse_struct_or_union(p, true);
            ts |= TS_OTHER;
            break;
        case TOK_KW_UNION:
            if (ts & TS_OTHER) { consumed = false; break; }
            user_ty = parse_struct_or_union(p, false);
            ts |= TS_OTHER;
            break;
        case TOK_KW_ENUM:
            if (ts & TS_OTHER) { consumed = false; break; }
            user_ty = parse_enum(p);
            ts |= TS_OTHER;
            break;

        /* Typedef name */
        case TOK_IDENT: {
            /* Only treat as typedef if no other type-specifier seen yet */
            if (ts) { consumed = false; break; }
            Type *td = scope_find_typedef(p, t->text);
            if (!td) { consumed = false; break; }
            user_ty = td;
            ts |= TS_OTHER;
            advance(p);
            break;
        }

        default:
            consumed = false;
            break;
        }

        if (!consumed) break;
        seen_any = true;
    }

    if (!seen_any) {
        diag_error(&p->cc->diag, cur_loc(p),
                   "expected type specifier");
        return ty_int; /* error recovery */
    }

    /* Resolve the type-specifier counters to a concrete type */
    Type *ty;
    if (ts & TS_OTHER) {
        ty = user_ty;
    } else if (ts == TS_VOID) {
        ty = ty_void;
    } else if (ts == TS_BOOL) {
        ty = ty_bool;
    } else if (ts == TS_CHAR || ts == (TS_SIGNED | TS_CHAR)) {
        ty = ty_schar;
    } else if (ts == (TS_UNSIGNED | TS_CHAR)) {
        ty = ty_uchar;
    } else if (ts == TS_SHORT || ts == (TS_SHORT | TS_INT) ||
               ts == (TS_SIGNED | TS_SHORT) ||
               ts == (TS_SIGNED | TS_SHORT | TS_INT)) {
        ty = ty_short;
    } else if (ts == (TS_UNSIGNED | TS_SHORT) ||
               ts == (TS_UNSIGNED | TS_SHORT | TS_INT)) {
        ty = ty_ushort;
    } else if (ts == TS_INT || ts == TS_SIGNED ||
               ts == (TS_SIGNED | TS_INT) || ts == 0) {
        ty = ty_int;
    } else if (ts == TS_UNSIGNED || ts == (TS_UNSIGNED | TS_INT)) {
        ty = ty_uint;
    } else if (ts == TS_LONG || ts == (TS_LONG | TS_INT) ||
               ts == (TS_SIGNED | TS_LONG) ||
               ts == (TS_SIGNED | TS_LONG | TS_INT)) {
        ty = ty_long;
    } else if (ts == (TS_UNSIGNED | TS_LONG) ||
               ts == (TS_UNSIGNED | TS_LONG | TS_INT)) {
        ty = ty_ulong;
    } else if (ts == (TS_LONG | TS_LONG2) ||
               ts == (TS_LONG | TS_LONG2 | TS_INT) ||
               ts == (TS_SIGNED | TS_LONG | TS_LONG2) ||
               ts == (TS_SIGNED | TS_LONG | TS_LONG2 | TS_INT)) {
        ty = ty_llong;
    } else if (ts == (TS_UNSIGNED | TS_LONG | TS_LONG2) ||
               ts == (TS_UNSIGNED | TS_LONG | TS_LONG2 | TS_INT)) {
        ty = ty_ullong;
    } else if (ts == TS_FLOAT) {
        ty = ty_float;
    } else if (ts == TS_DOUBLE || ts == (TS_LONG | TS_DOUBLE)) {
        ty = ty_double;  /* long double -> use double for now */
    } else {
        diag_error(&p->cc->diag, cur_loc(p),
                   "invalid type specifier combination");
        ty = ty_int;
    }

    /* Apply qualifiers -- create a copy if needed */
    if (is_const || is_volatile || is_restrict || is_atomic || requested_align > 0) {
        ty = type_copy(p->arena, ty);
        ty->is_const = is_const;
        ty->is_volatile = is_volatile;
        ty->is_restrict = is_restrict;
        ty->is_atomic = is_atomic;
        if (requested_align > ty->align)
            ty->align = requested_align;
    }

    return ty;
}

/* ------------------------------------------------------------------ */
/*  struct / union parsing                                             */
/* ------------------------------------------------------------------ */

static Type *parse_struct_or_union(Parser *p, bool is_struct) {
    SrcLoc loc = cur_loc(p);
    advance(p); /* consume 'struct' or 'union' */

    const char *tag = NULL;
    if (check(p, TOK_IDENT)) {
        tag = peek(p)->text;
        advance(p);
    }

    /* Forward reference or existing type if no body */
    if (!check(p, TOK_LBRACE)) {
        if (!tag) {
            diag_error(&p->cc->diag, loc,
                       "expected tag name or '{'");
            return ty_int;
        }
        Type *existing = scope_find_tag(p, tag);
        if (existing) return existing;
        /* Forward declaration */
        Type *ty = is_struct ? type_struct(p->arena, tag)
                             : type_union(p->arena, tag);
        ty->is_incomplete = true;
        scope_push_tag(p, tag, ty);
        return ty;
    }

    /* Body definition */
    Type *ty = NULL;
    if (tag) {
        ty = scope_find_tag(p, tag);
        if (ty && !ty->is_incomplete) {
            /* Redefining -- create new */
            ty = NULL;
        }
    }
    if (!ty) {
        ty = is_struct ? type_struct(p->arena, tag)
                       : type_union(p->arena, tag);
        if (tag) scope_push_tag(p, tag, ty);
    }

    expect(p, TOK_LBRACE);

    Member head = {0};
    Member *cur = &head;
    int offset = 0;
    int max_align = 1;
    int max_size = 0;
    int bitfield_bits_used = 0;
    int bitfield_unit_offset = 0;

    while (!check(p, TOK_RBRACE) && !at_eof(p)) {
        /* _Static_assert inside struct */
        if (check(p, TOK_KW__STATIC_ASSERT)) {
            advance(p);
            expect(p, TOK_LPAREN);
            assign_expr(p);  /* constant expr */
            expect(p, TOK_COMMA);
            expect(p, TOK_STRING_LIT);
            expect(p, TOK_RPAREN);
            expect(p, TOK_SEMI);
            continue;
        }

        Type *base = declspec(p, NULL);

        /* Handle empty declaration (e.g. anonymous struct member) */
        if (check(p, TOK_SEMI)) {
            advance(p);
            /* Anonymous struct/union member */
            if ((base->kind == TY_STRUCT || base->kind == TY_UNION) && !base->tag) {
                Member *m = arena_new(p->arena, Member);
                m->name = NULL;
                m->ty = base;
                m->bit_width = -1;
                if (is_struct) {
                    /* Align */
                    if (base->align > 0) {
                        offset = (offset + base->align - 1) & ~(base->align - 1);
                    }
                    m->offset = offset;
                    offset += base->size;
                } else {
                    m->offset = 0;
                    if (base->size > max_size) max_size = base->size;
                }
                if (base->align > max_align) max_align = base->align;
                cur->next = m;
                cur = m;
            }
            continue;
        }

        bool first = true;
        while (!check(p, TOK_SEMI) && !at_eof(p)) {
            if (!first) expect(p, TOK_COMMA);
            first = false;

            const char *mname = NULL;
            Type *mty = base;

            /* Check for bitfield without declarator */
            if (check(p, TOK_COLON)) {
                /* Anonymous bitfield */
            } else {
                mty = declarator(p, base, &mname);
            }

            int bit_width = -1;
            if (match(p, TOK_COLON)) {
                /* Bitfield */
                Node *bw = assign_expr(p);
                bit_width = (int)const_expr_val(bw);
            }

            Member *m = arena_new(p->arena, Member);
            m->name = mname;
            m->ty = mty;
            m->bit_width = bit_width;
            m->bit_offset = 0;

            if (is_struct) {
                if (bit_width >= 0) {
                    /* Bitfield member: pack consecutive bitfields into storage units */
                    int unit_size = mty->size > 0 ? mty->size : 4; /* storage unit size in bytes */
                    int unit_bits = unit_size * 8;
                    int align = mty->align > 0 ? mty->align : 4;

                    if (bit_width == 0) {
                        /* Zero-width bitfield: force alignment to next storage unit */
                        offset = (offset + unit_size - 1) & ~(unit_size - 1);
                        bitfield_bits_used = 0;
                        bitfield_unit_offset = offset;
                        /* Don't add this member to the list */
                        if (align > max_align) max_align = align;
                        continue;
                    }

                    /* Check if this bitfield fits in the current storage unit */
                    if (bitfield_bits_used == 0 || bitfield_bits_used + bit_width > unit_bits) {
                        /* Start a new storage unit */
                        offset = (offset + align - 1) & ~(align - 1);
                        bitfield_unit_offset = offset;
                        bitfield_bits_used = 0;
                    }

                    m->offset = bitfield_unit_offset;
                    m->bit_offset = bitfield_bits_used;
                    bitfield_bits_used += bit_width;

                    /* Only advance offset when we finish or overflow a unit */
                    int new_end = bitfield_unit_offset + unit_size;
                    if (new_end > offset) offset = new_end;
                    if (align > max_align) max_align = align;
                } else {
                    /* Normal (non-bitfield) member: reset bitfield state */
                    bitfield_bits_used = 0;
                    int align = mty->align > 0 ? mty->align : 1;
                    offset = (offset + align - 1) & ~(align - 1);
                    m->offset = offset;
                    offset += mty->size;
                    if (align > max_align) max_align = align;
                }
            } else {
                m->offset = 0;
                if (mty->size > max_size) max_size = mty->size;
                if (mty->align > max_align) max_align = mty->align;
            }

            cur->next = m;
            cur = m;
        }
        expect(p, TOK_SEMI);
    }
    expect(p, TOK_RBRACE);

    ty->members = head.next;
    ty->is_incomplete = false;

    if (is_struct) {
        ty->size = (offset + max_align - 1) & ~(max_align - 1);
    } else {
        ty->size = (max_size + max_align - 1) & ~(max_align - 1);
    }
    ty->align = max_align;

    return ty;
}

/* ------------------------------------------------------------------ */
/*  enum parsing                                                       */
/* ------------------------------------------------------------------ */

static Type *parse_enum(Parser *p) {
    SrcLoc loc = cur_loc(p);
    advance(p); /* consume 'enum' */

    const char *tag = NULL;
    if (check(p, TOK_IDENT)) {
        tag = peek(p)->text;
        advance(p);
    }

    if (!check(p, TOK_LBRACE)) {
        if (!tag) {
            diag_error(&p->cc->diag, loc, "expected tag name or '{'");
            return ty_int;
        }
        Type *existing = scope_find_tag(p, tag);
        if (existing) return existing;
        Type *ty = type_enum(p->arena, tag);
        ty->is_incomplete = true;
        scope_push_tag(p, tag, ty);
        return ty;
    }

    Type *ty = type_enum(p->arena, tag);
    if (tag) scope_push_tag(p, tag, ty);

    expect(p, TOK_LBRACE);
    long long val = 0;

    while (!check(p, TOK_RBRACE) && !at_eof(p)) {
        Token *name_tok = expect(p, TOK_IDENT);
        const char *name = name_tok->text;

        if (match(p, TOK_ASSIGN)) {
            Node *e = assign_expr(p);
            if (e->kind == ND_NUM) {
                val = e->ival;
            } else {
                /* Try to evaluate constant expression (e.g., -1, ~0, 1+2) */
                /* Inline simple negation since try_const_eval isn't available yet */
                if (e->kind == ND_NEG && e->lhs && e->lhs->kind == ND_NUM) {
                    val = -(e->lhs->ival);
                } else if (e->kind == ND_BITNOT && e->lhs && e->lhs->kind == ND_NUM) {
                    val = ~(e->lhs->ival);
                } else if (e->kind == ND_SUB && e->lhs && e->lhs->kind == ND_NUM &&
                           e->rhs && e->rhs->kind == ND_NUM) {
                    val = e->lhs->ival - e->rhs->ival;
                } else if (e->kind == ND_ADD && e->lhs && e->lhs->kind == ND_NUM &&
                           e->rhs && e->rhs->kind == ND_NUM) {
                    val = e->lhs->ival + e->rhs->ival;
                } else {
                    val = e->ival;
                }
            }
        }

        /* Create an Obj to represent the enum constant */
        Obj *obj = new_obj(p, name, ty_int, name_tok->loc);
        obj->is_enum_const = true;
        obj->enum_val = val;
        scope_push_var(p, name, obj);

        val++;

        if (!check(p, TOK_RBRACE))
            expect(p, TOK_COMMA);
    }
    expect(p, TOK_RBRACE);

    ty->is_incomplete = false;
    /* Enum underlying type is int */
    ty->size = ty_int->size;
    ty->align = ty_int->align;

    return ty;
}

/* ------------------------------------------------------------------ */
/*  Declarator parsing                                                 */
/* ------------------------------------------------------------------ */

/*
 * declarator = pointer? direct-declarator
 * direct-declarator = ident type-suffix
 *                   | '(' declarator ')' type-suffix
 *
 * type-suffix = '[' expr? ']' type-suffix
 *             | '(' param-list ')' type-suffix
 *             | (nothing)
 */

/* Parse pointer prefixes: *const *volatile ... */
static Type *parse_pointers(Parser *p, Type *base) {
    while (match(p, TOK_STAR)) {
        base = type_ptr(p->arena, base);
        for (;;) {
            if (match(p, TOK_KW_CONST))    base->is_const = true;
            else if (match(p, TOK_KW_VOLATILE)) base->is_volatile = true;
            else if (match(p, TOK_KW_RESTRICT)) base->is_restrict = true;
            else if (match(p, TOK_KW__ATOMIC))  base->is_atomic = true;
            else break;
        }
    }
    return base;
}

/* Try to evaluate an AST node as a compile-time integer constant.
 * Returns true and sets *out if the expression is a constant, false otherwise. */
static bool try_const_eval(Node *n, int64_t *out) {
    if (!n) return false;
    if (n->kind == ND_NUM) {
        *out = n->ival;
        return true;
    }
    int64_t l, r;
    if (n->lhs && n->rhs && try_const_eval(n->lhs, &l) && try_const_eval(n->rhs, &r)) {
        switch (n->kind) {
        case ND_ADD: *out = l + r; return true;
        case ND_SUB: *out = l - r; return true;
        case ND_MUL: *out = l * r; return true;
        case ND_DIV: if (r != 0) { *out = l / r; return true; } return false;
        case ND_MOD: if (r != 0) { *out = l % r; return true; } return false;
        case ND_SHL: *out = l << r; return true;
        case ND_SHR: *out = l >> r; return true;
        case ND_BITAND: *out = l & r; return true;
        case ND_BITOR:  *out = l | r; return true;
        case ND_BITXOR: *out = l ^ r; return true;
        default: return false;
        }
    }
    /* Unary negation */
    if (n->kind == ND_NEG && n->lhs && try_const_eval(n->lhs, &l)) {
        *out = -l;
        return true;
    }
    /* Unary bitwise NOT */
    if (n->kind == ND_BITNOT && n->lhs && try_const_eval(n->lhs, &l)) {
        *out = ~l;
        return true;
    }
    /* sizeof produces a constant */
    if (n->kind == ND_SIZEOF) {
        *out = n->ival;
        return true;
    }
    return false;
}

static Type *type_suffix(Parser *p, Type *base) {
    /* Array suffix: [expr] or [] */
    if (match(p, TOK_LBRACKET)) {
        /* Optional 'static', type qualifiers inside [] -- skip for now */
        while (check(p, TOK_KW_STATIC) || check(p, TOK_KW_CONST) ||
               check(p, TOK_KW_VOLATILE) || check(p, TOK_KW_RESTRICT)) {
            advance(p);
        }

        int len = -1;
        Node *vla_size_expr = NULL;
        if (!check(p, TOK_RBRACKET)) {
            Node *size_expr = assign_expr(p);
            int64_t val;
            if (try_const_eval(size_expr, &val)) {
                len = (int)val;
            } else {
                /* Variable-length array */
                vla_size_expr = size_expr;
            }
        }
        expect(p, TOK_RBRACKET);

        Type *elem = type_suffix(p, base);
        Type *arr_ty = type_array(p->arena, elem, len);
        if (vla_size_expr) {
            arr_ty->is_vla = true;
            arr_ty->vla_size = vla_size_expr;
        }
        return arr_ty;
    }

    /* Function suffix: (param-list) */
    if (match(p, TOK_LPAREN)) {
        Type *fn_ty = type_func(p->arena, base);
        if (!check(p, TOK_RPAREN)) {
            /* Check for (void) */
            if (check(p, TOK_KW_VOID) && peek_ahead(p, 1)->kind == TOK_RPAREN) {
                advance(p); /* consume 'void' */
                fn_ty->params = NULL;
            } else {
                fn_ty->params = func_params(p);
                fn_ty->is_variadic = false;
                /* Check for variadic sentinel (NULL ty param) and set flag */
                Param *last = NULL;
                for (Param *pm = fn_ty->params; pm; pm = pm->next) {
                    if (pm->ty == NULL) {
                        fn_ty->is_variadic = true;
                        if (last) last->next = NULL;
                        else fn_ty->params = NULL;
                        break;
                    }
                    last = pm;
                }
            }
        }
        expect(p, TOK_RPAREN);
        return fn_ty;
    }

    return base;
}

static Param *func_params(Parser *p) {
    Param head = {0};
    Param *cur = &head;

    /* Suppress undeclared-identifier errors for VLA parameter sizes
     * like int arr[n] where n is a preceding parameter name */
    bool saved_suppress = p->suppress_undecl;
    p->suppress_undecl = true;

    bool first = true;
    while (!check(p, TOK_RPAREN) && !at_eof(p)) {
        if (!first) expect(p, TOK_COMMA);
        first = false;

        /* Variadic */
        if (match(p, TOK_ELLIPSIS)) {
            /* Mark the enclosing function type as variadic.
             * We need to do this from the caller -- for now set a flag.
             * Actually we walk back up in the caller. */
            /* HACK: we create a sentinel param with NULL type */
            Param *pm = arena_new(p->arena, Param);
            pm->name = NULL;
            pm->ty = NULL; /* sentinel for variadic */
            cur->next = pm;
            cur = pm;
            break;
        }

        Type *base = declspec(p, NULL);
        const char *pname = NULL;

        /* The parameter might have an abstract declarator (no name) */
        if (check(p, TOK_RPAREN) || check(p, TOK_COMMA)) {
            /* abstract: just base type */
        } else {
            base = declarator(p, base, &pname);
        }

        /* Array parameters decay to pointers */
        if (base->kind == TY_ARRAY) {
            base = type_ptr(p->arena, base->base);
        }
        /* Function parameters decay to pointers */
        if (base->kind == TY_FUNC) {
            base = type_ptr(p->arena, base);
        }

        Param *pm = arena_new(p->arena, Param);
        pm->name = pname;
        pm->ty = base;
        cur->next = pm;
        cur = pm;
    }

    p->suppress_undecl = saved_suppress;

    /* Check if last param was variadic sentinel */
    Param *result = head.next;
    /* Walk to find variadic sentinel and remove it, set flag in caller */
    /* We handle this by returning the params and letting the caller
       check the last param's ty == NULL */

    return result;
}

static Type *declarator(Parser *p, Type *base, const char **name_out) {
    base = parse_pointers(p, base);

    /* Parenthesized declarator */
    if (match(p, TOK_LPAREN)) {
        /* We need to handle "int (*fp)(int)" style.
         * Save position, skip inner part, parse suffix, then re-parse inner
         * with the suffix type as the base.
         */
        int saved_pos = p->pos;
        /* Skip the inner declarator tokens to find the matching ')' */
        int depth = 1;
        while (depth > 0 && !at_eof(p)) {
            if (check(p, TOK_LPAREN)) depth++;
            if (check(p, TOK_RPAREN)) depth--;
            if (depth > 0) advance(p);
        }
        expect(p, TOK_RPAREN); /* consume the matching ')' */

        /* Now parse type suffixes (array/function) that come after the ')' */
        Type *suffix = type_suffix(p, base);

        /* Save position after suffix */
        int after_suffix_pos = p->pos;

        /* Go back and re-parse the inner declarator with the suffix as base */
        p->pos = saved_pos;
        Type *result = declarator(p, suffix, name_out);

        expect(p, TOK_RPAREN);
        p->pos = after_suffix_pos;
        return result;
    }

    /* Identifier */
    if (check(p, TOK_IDENT)) {
        Token *t = advance(p);
        if (name_out) *name_out = t->text;
    }

    return type_suffix(p, base);
}

static Type *abstract_declarator(Parser *p, Type *base) {
    base = parse_pointers(p, base);

    /* Parenthesized abstract declarator */
    if (check(p, TOK_LPAREN)) {
        /* Disambiguate: is this a grouped abstract declarator, or a function suffix?
         * If next token after '(' is '*' or '(' or '[', it's a grouped declarator.
         * Otherwise it's a function parameter list. */
        Token *next = peek_ahead(p, 1);
        if (next->kind == TOK_STAR || next->kind == TOK_LPAREN ||
            next->kind == TOK_LBRACKET) {
            advance(p); /* consume '(' */
            int saved_pos = p->pos;
            int depth = 1;
            while (depth > 0 && !at_eof(p)) {
                if (check(p, TOK_LPAREN)) depth++;
                if (check(p, TOK_RPAREN)) depth--;
                if (depth > 0) advance(p);
            }
            expect(p, TOK_RPAREN);
            Type *suffix = type_suffix(p, base);
            int after_suffix_pos = p->pos;
            p->pos = saved_pos;
            Type *result = abstract_declarator(p, suffix);
            expect(p, TOK_RPAREN);
            p->pos = after_suffix_pos;
            return result;
        }
    }

    return type_suffix(p, base);
}

/* ------------------------------------------------------------------ */
/*  type-name: used in casts, sizeof, _Alignof                        */
/* ------------------------------------------------------------------ */

static Type *parse_type_name(Parser *p) {
    Type *base = declspec(p, NULL);
    return abstract_declarator(p, base);
}

/* ------------------------------------------------------------------ */
/*  Initializer parsing                                                */
/* ------------------------------------------------------------------ */

static Node *init_list(Parser *p);

static Node *initializer(Parser *p, Obj *var) {
    (void)var; /* used for designated initializer resolution in sema */
    if (check(p, TOK_LBRACE)) {
        return init_list(p);
    }
    return assign_expr(p);
}

static Node *init_list(Parser *p) {
    SrcLoc loc = cur_loc(p);
    expect(p, TOK_LBRACE);

    Node *nd = new_node(p->arena, ND_INIT_LIST, loc);
    Node head = {0};
    Node *cur = &head;

    while (!check(p, TOK_RBRACE) && !at_eof(p)) {
        if (cur != &head) {
            if (!match(p, TOK_COMMA)) break;
            if (check(p, TOK_RBRACE)) break; /* trailing comma */
        }

        /* Designated initializers: .field = expr  or  [index] = expr */
        const char *desig_name = NULL;
        int desig_index = -1;
        Designator *desig_chain = NULL;
        if (check(p, TOK_DOT) || check(p, TOK_LBRACKET)) {
            Designator *chain_head = NULL, *chain_tail = NULL;
            while (check(p, TOK_DOT) || check(p, TOK_LBRACKET)) {
                Designator *d = arena_new(p->arena, Designator);
                d->index = -1;
                d->next = NULL;
                if (match(p, TOK_DOT)) {
                    Token *id_tok = peek(p);
                    expect(p, TOK_IDENT);
                    d->name = id_tok->text;
                } else {
                    advance(p); /* '[' */
                    Node *idx_expr = assign_expr(p);
                    expect(p, TOK_RBRACKET);
                    d->index = (int)const_expr_val(idx_expr);
                }
                if (!chain_head) chain_head = d;
                if (chain_tail) chain_tail->next = d;
                chain_tail = d;
            }
            expect(p, TOK_ASSIGN);
            /* Set backwards-compat single designator from last in chain */
            if (chain_tail) {
                desig_name = chain_tail->name;
                desig_index = chain_tail->index;
            }
            /* Only store chain if more than one level */
            if (chain_head && chain_head->next)
                desig_chain = chain_head;
        }

        Node *elem;
        if (check(p, TOK_LBRACE)) {
            elem = init_list(p);
        } else {
            elem = assign_expr(p);
        }
        elem->designator_name = desig_name;
        elem->designator_index = desig_index;
        elem->desig_chain = desig_chain;
        cur->next = elem;
        cur = elem;
    }
    expect(p, TOK_RBRACE);

    nd->body = head.next;
    return nd;
}

/* ------------------------------------------------------------------ */
/*  Declaration                                                        */
/* ------------------------------------------------------------------ */

/* Check if the next tokens look like a function definition:
 * declspec declarator '{' -- i.e. no ';' or ',' before '{' */
static bool is_function_def(Parser *p) {
    int saved = p->pos;

    int sc;
    declspec(p, &sc);

    const char *name = NULL;
    Type *ty = declarator(p, ty_int, &name); /* dummy base */
    (void)ty;

    bool result = (name != NULL && check(p, TOK_LBRACE));
    p->pos = saved;
    return result;
}

/* Parse a function definition */
static Node *funcdef(Parser *p) {
    SrcLoc loc = cur_loc(p);

    int sc = SC_NONE;
    Type *base = declspec(p, &sc);
    const char *name = NULL;
    Type *ty = declarator(p, base, &name);

    if (!name) {
        diag_error(&p->cc->diag, loc, "expected function name");
        name = intern(&p->cc->strings, "<unnamed>");
    }

    /* Create function object */
    Obj *fn = new_global(p, name, ty, loc);
    fn->is_definition = true;
    fn->is_static = (sc & SC_STATIC) != 0;

    /* Enter function scope */
    p->locals = NULL;
    p->labels = NULL;
    p->gotos = NULL;
    p->cur_func_name = name;

    scope_new(p);

    /* Add parameters as local variables */
    if (ty->kind == TY_FUNC) {
        /* Walk the variadic sentinel to set the flag */
        Param *last = NULL;
        for (Param *pm = ty->params; pm; pm = pm->next) {
            if (pm->ty == NULL) {
                /* variadic sentinel -- remove and set flag */
                ty->is_variadic = true;
                if (last) last->next = NULL;
                break;
            }
            last = pm;
        }

        for (Param *pm = ty->params; pm; pm = pm->next) {
            if (pm->ty && pm->name) {
                new_local(p, pm->name, pm->ty, loc);
            }
        }
    }

    /* Parse function body */
    Node *body = compound_stmt(p);

    scope_pop(p);

    /* Resolve gotos to labels */
    for (Node *g = p->gotos; g; g = g->label_next) {
        bool found = false;
        for (Node *l = p->labels; l; l = l->label_next) {
            if (strcmp(g->label, l->label) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            diag_error(&p->cc->diag, g->loc,
                       "undefined label '%s'", g->label);
        }
    }

    fn->body = body;
    fn->locals = p->locals;

    Node *nd = new_node(p->arena, ND_FUNCDEF, loc);
    nd->var = fn;
    nd->body = body;
    return nd;
}

/*
 * Parse a top-level or block-scope declaration (variables, typedefs).
 * Returns a linked list of ND_DECLARATION nodes (or NULL for typedefs/tags).
 */
static Node *declaration(Parser *p) {
    SrcLoc loc = cur_loc(p);

    int sc = SC_NONE;
    Type *base = declspec(p, &sc);

    /* Bare struct/union/enum declaration: "struct Foo { ... };" */
    if (check(p, TOK_SEMI)) {
        advance(p);
        return new_node(p->arena, ND_NULL_STMT, loc);
    }

    Node head = {0};
    Node *cur = &head;
    bool first = true;

    while (!check(p, TOK_SEMI) && !at_eof(p)) {
        if (!first) expect(p, TOK_COMMA);
        first = false;

        const char *name = NULL;
        Type *ty = declarator(p, base, &name);

        if (!name) {
            diag_error(&p->cc->diag, loc, "expected identifier in declaration");
            break;
        }

        /* typedef */
        if (sc & SC_TYPEDEF) {
            ty->typedef_name = name;
            scope_push_typedef(p, name, ty);
            continue;
        }

        /* Variable declaration */
        bool is_global_scope = (p->scope->parent == NULL);
        Obj *obj;
        if (is_global_scope) {
            obj = new_global(p, name, ty, loc);
            obj->is_static = (sc & SC_STATIC) != 0;
            obj->is_extern = (sc & SC_EXTERN) != 0;
        } else {
            obj = new_local(p, name, ty, loc);
            obj->is_static = (sc & SC_STATIC) != 0;
            obj->is_extern = (sc & SC_EXTERN) != 0;
        }

        /* Initializer */
        Node *init_nd = NULL;
        if (match(p, TOK_ASSIGN)) {
            init_nd = initializer(p, obj);
            obj->init_expr = init_nd;

            /* Complete incomplete array types from initializer list.
             * For declarations like "int arr[] = {1, 2, 3}", the array
             * length is inferred from the number of initializer elements. */
            if (ty && ty->kind == TY_ARRAY && ty->is_incomplete &&
                init_nd && init_nd->kind == ND_INIT_LIST) {
                int count = 0;
                for (Node *e = init_nd->body; e; e = e->next)
                    count++;
                ty->array_len = count;
                ty->size = ty->base->size * count;
                ty->is_incomplete = false;
            }
            /* Complete incomplete char array from string literal initializer.
             * For "char s[] = "hello"", infer array length from the string. */
            if (ty && ty->kind == TY_ARRAY && ty->is_incomplete &&
                init_nd && init_nd->str_val) {
                int len = init_nd->ty->array_len;
                ty->array_len = len;
                ty->size = ty->base->size * len;
                ty->is_incomplete = false;
            }
        }

        Node *decl = new_node(p->arena, ND_DECLARATION, loc);
        decl->var = obj;
        decl->lhs = init_nd;
        cur->next = decl;
        cur = decl;
    }
    expect(p, TOK_SEMI);

    if (!head.next) {
        /* Typedef or bare tag -- return a null stmt */
        return new_node(p->arena, ND_NULL_STMT, loc);
    }

    /* If only one declaration, return it directly.
     * If multiple (e.g. "int a, b, c;"), chain via next. */
    return head.next;
}

/* ------------------------------------------------------------------ */
/*  _Static_assert                                                     */
/* ------------------------------------------------------------------ */

static Node *parse_static_assert(Parser *p) {
    SrcLoc loc = cur_loc(p);
    advance(p); /* consume _Static_assert */
    expect(p, TOK_LPAREN);
    Node *cond_expr = assign_expr(p);
    expect(p, TOK_COMMA);
    Token *msg_tok = peek(p);
    const char *msg = msg_tok->kind == TOK_STRING_LIT ? msg_tok->text : "static assertion failed";
    expect(p, TOK_STRING_LIT); /* message */
    expect(p, TOK_RPAREN);
    expect(p, TOK_SEMI);

    /* Evaluate the constant expression and report error if it's zero */
    int64_t val = const_expr_val(cond_expr);
    if (val == 0) {
        diag_error(&p->cc->diag, loc, "_Static_assert failed: %s", msg);
    }

    return new_node(p->arena, ND_NULL_STMT, loc);
}

/* ------------------------------------------------------------------ */
/*  Statement parsing                                                  */
/* ------------------------------------------------------------------ */

static Node *compound_stmt(Parser *p) {
    SrcLoc loc = cur_loc(p);
    expect(p, TOK_LBRACE);
    scope_new(p);

    Node *nd = new_node(p->arena, ND_BLOCK, loc);
    Node head = {0};
    Node *cur = &head;

    while (!check(p, TOK_RBRACE) && !at_eof(p)) {
        Node *s;
        if (check(p, TOK_KW__STATIC_ASSERT)) {
            s = parse_static_assert(p);
        } else if (is_typename(p)) {
            s = declaration(p);
        } else {
            s = stmt(p);
        }
        /* declaration() may return a chain -- walk to end */
        if (s) {
            cur->next = s;
            while (cur->next) cur = cur->next;
        }
    }
    expect(p, TOK_RBRACE);
    scope_pop(p);

    nd->body = head.next;
    return nd;
}

static Node *expr_stmt(Parser *p) {
    SrcLoc loc = cur_loc(p);
    if (match(p, TOK_SEMI)) {
        return new_node(p->arena, ND_NULL_STMT, loc);
    }
    Node *nd = new_unary(p->arena, ND_EXPR_STMT, expr(p), loc);
    expect(p, TOK_SEMI);
    return nd;
}

/* Evaluate a compile-time constant expression (for case labels, array sizes, etc.) */
static int64_t const_expr_val(Node *n) {
    if (!n) return 0;
    if (n->kind == ND_NUM) return n->ival;
    int64_t l = n->lhs ? const_expr_val(n->lhs) : 0;
    int64_t r = n->rhs ? const_expr_val(n->rhs) : 0;
    switch (n->kind) {
    case ND_ADD:    return l + r;
    case ND_SUB:    return l - r;
    case ND_MUL:    return l * r;
    case ND_DIV:    return r ? l / r : 0;
    case ND_MOD:    return r ? l % r : 0;
    case ND_SHL:    return l << r;
    case ND_SHR:    return l >> r;
    case ND_BITAND: return l & r;
    case ND_BITOR:  return l | r;
    case ND_BITXOR: return l ^ r;
    case ND_EQ:     return l == r;
    case ND_NE:     return l != r;
    case ND_LT:     return l < r;
    case ND_LE:     return l <= r;
    case ND_LOGAND: return l && r;
    case ND_LOGOR:  return l || r;
    case ND_NOT:    return !l;
    case ND_NEG:    return -l;
    case ND_BITNOT: return ~l;
    case ND_CAST:   return l;
    case ND_SIZEOF: return n->ival;
    case ND_ALIGNOF: return n->ival;
    case ND_TERNARY: return const_expr_val(n->cond) ? const_expr_val(n->then) : const_expr_val(n->els);
    case ND_COMMA:  return r;
    default:        return n->ival;
    }
}

static Node *stmt(Parser *p) {
    Token *t = peek(p);
    SrcLoc loc = t->loc;

    switch (t->kind) {
    case TOK_KW_RETURN: {
        advance(p);
        Node *nd = new_node(p->arena, ND_RETURN, loc);
        if (!check(p, TOK_SEMI)) {
            nd->lhs = expr(p);
        }
        expect(p, TOK_SEMI);
        return nd;
    }

    case TOK_KW_IF: {
        advance(p);
        Node *nd = new_node(p->arena, ND_IF, loc);
        expect(p, TOK_LPAREN);
        nd->cond = expr(p);
        expect(p, TOK_RPAREN);
        nd->then = stmt(p);
        if (match(p, TOK_KW_ELSE)) {
            nd->els = stmt(p);
        }
        return nd;
    }

    case TOK_KW_WHILE: {
        advance(p);
        Node *nd = new_node(p->arena, ND_FOR, loc);
        expect(p, TOK_LPAREN);
        nd->cond = expr(p);
        expect(p, TOK_RPAREN);
        nd->then = stmt(p);
        return nd;
    }

    case TOK_KW_DO: {
        advance(p);
        Node *nd = new_node(p->arena, ND_FOR, loc);
        nd->then = stmt(p);
        expect(p, TOK_KW_WHILE);
        expect(p, TOK_LPAREN);
        nd->cond = expr(p);
        expect(p, TOK_RPAREN);
        expect(p, TOK_SEMI);
        nd->is_dowhile = true;
        return nd;
    }

    case TOK_KW_FOR: {
        advance(p);
        Node *nd = new_node(p->arena, ND_FOR, loc);
        expect(p, TOK_LPAREN);

        scope_new(p);

        /* init */
        if (match(p, TOK_SEMI)) {
            /* no init */
        } else if (is_typename(p)) {
            nd->init = declaration(p);
        } else {
            nd->init = new_unary(p->arena, ND_EXPR_STMT, expr(p), loc);
            expect(p, TOK_SEMI);
        }

        /* cond */
        if (!check(p, TOK_SEMI)) {
            nd->cond = expr(p);
        }
        expect(p, TOK_SEMI);

        /* inc */
        if (!check(p, TOK_RPAREN)) {
            nd->inc = expr(p);
        }
        expect(p, TOK_RPAREN);

        nd->then = stmt(p);

        scope_pop(p);
        return nd;
    }

    case TOK_KW_SWITCH: {
        advance(p);
        Node *nd = new_node(p->arena, ND_SWITCH, loc);
        expect(p, TOK_LPAREN);
        nd->cond = expr(p);
        expect(p, TOK_RPAREN);

        Node *saved_switch = p->cur_switch;
        p->cur_switch = nd;
        nd->then = stmt(p);
        p->cur_switch = saved_switch;
        return nd;
    }

    case TOK_KW_CASE: {
        advance(p);
        Node *nd = new_node(p->arena, ND_CASE, loc);
        Node *val = assign_expr(p);
        /* Evaluate constant expression for case value */
        nd->case_val = const_expr_val(val);
        expect(p, TOK_COLON);
        nd->lhs = stmt(p);

        /* Link into switch's case list (append to maintain source order) */
        if (p->cur_switch) {
            nd->case_next = NULL;
            Node **tail = &p->cur_switch->case_next;
            while (*tail) tail = &(*tail)->case_next;
            *tail = nd;
        } else {
            diag_error(&p->cc->diag, loc, "'case' outside of switch");
        }
        return nd;
    }

    case TOK_KW_DEFAULT: {
        advance(p);
        expect(p, TOK_COLON);
        Node *nd = new_node(p->arena, ND_DEFAULT, loc);
        nd->lhs = stmt(p);
        if (p->cur_switch) {
            p->cur_switch->default_case = nd;
        } else {
            diag_error(&p->cc->diag, loc, "'default' outside of switch");
        }
        return nd;
    }

    case TOK_KW_BREAK: {
        advance(p);
        expect(p, TOK_SEMI);
        return new_node(p->arena, ND_BREAK, loc);
    }

    case TOK_KW_CONTINUE: {
        advance(p);
        expect(p, TOK_SEMI);
        return new_node(p->arena, ND_CONTINUE, loc);
    }

    case TOK_KW_GOTO: {
        advance(p);
        Node *nd = new_node(p->arena, ND_GOTO, loc);
        Token *label_tok = expect(p, TOK_IDENT);
        nd->label = label_tok->text;
        expect(p, TOK_SEMI);

        /* Track for resolution */
        nd->label_next = p->gotos;
        p->gotos = nd;
        return nd;
    }

    case TOK_LBRACE:
        return compound_stmt(p);

    case TOK_IDENT: {
        /* Check if this is a label: IDENT ':' */
        if (peek_ahead(p, 1)->kind == TOK_COLON) {
            Token *label_tok = advance(p);
            advance(p); /* consume ':' */
            Node *nd = new_node(p->arena, ND_LABEL, loc);
            nd->label = label_tok->text;
            nd->lhs = stmt(p);

            /* Track for goto resolution */
            nd->label_next = p->labels;
            p->labels = nd;
            return nd;
        }
        return expr_stmt(p);
    }

    case TOK_KW__STATIC_ASSERT:
        return parse_static_assert(p);

    default:
        return expr_stmt(p);
    }
}

/* ------------------------------------------------------------------ */
/*  Expression parsing (recursive descent with precedence climbing)    */
/* ------------------------------------------------------------------ */

/*
 * Precedence levels (C11 6.5):
 *   1  ,        (comma)
 *   2  = += ... (assignment)
 *   3  ?:       (ternary)
 *   4  ||       (logical or)
 *   5  &&       (logical and)
 *   6  |        (bitwise or)
 *   7  ^        (bitwise xor)
 *   8  &        (bitwise and)
 *   9  == !=    (equality)
 *  10  < > <= >=(relational)
 *  11  << >>    (shift)
 *  12  + -      (additive)
 *  13  * / %    (multiplicative)
 *  14  cast     (unary / cast)
 *  15  unary    (prefix ++, --, sizeof, &, *, +, -, ~, !)
 *  16  postfix  (call, subscript, member, postfix ++/--)
 *
 * We implement each level as a function, chaining downward.
 */

/* expr = assign_expr (',' assign_expr)* */
static Node *expr(Parser *p) {
    Node *nd = assign_expr(p);

    while (check(p, TOK_COMMA)) {
        SrcLoc loc = cur_loc(p);
        advance(p);
        Node *rhs = assign_expr(p);
        nd = new_binary(p->arena, ND_COMMA, nd, rhs, loc);
    }

    return nd;
}

/* assign_expr = ternary_expr (assign-op assign_expr)? */
static Node *assign_expr(Parser *p) {
    Node *nd = ternary_expr(p);

    Token *t = peek(p);
    SrcLoc loc = t->loc;

    if (t->kind == TOK_ASSIGN) {
        advance(p);
        Node *rhs = assign_expr(p);
        return new_binary(p->arena, ND_ASSIGN, nd, rhs, loc);
    }

    /* Compound assignment operators */
    NodeKind compound_op = 0;
    switch (t->kind) {
    case TOK_PLUS_ASSIGN:    compound_op = ND_ADD; break;
    case TOK_MINUS_ASSIGN:   compound_op = ND_SUB; break;
    case TOK_STAR_ASSIGN:    compound_op = ND_MUL; break;
    case TOK_SLASH_ASSIGN:   compound_op = ND_DIV; break;
    case TOK_PERCENT_ASSIGN: compound_op = ND_MOD; break;
    case TOK_AMP_ASSIGN:     compound_op = ND_BITAND; break;
    case TOK_PIPE_ASSIGN:    compound_op = ND_BITOR;  break;
    case TOK_CARET_ASSIGN:   compound_op = ND_BITXOR; break;
    case TOK_LSHIFT_ASSIGN:  compound_op = ND_SHL; break;
    case TOK_RSHIFT_ASSIGN:  compound_op = ND_SHR; break;
    default: return nd;
    }

    advance(p);
    Node *rhs = assign_expr(p);
    Node *node = new_binary(p->arena, ND_COMPOUND_ASSIGN, nd, rhs, loc);
    node->compound_op = compound_op;
    return node;
}

/* ternary_expr = logor_expr ('?' expr ':' ternary_expr)? */
static Node *ternary_expr(Parser *p) {
    Node *cond = logor_expr(p);

    if (!match(p, TOK_QUESTION))
        return cond;

    SrcLoc loc = cur_loc(p);
    Node *nd = new_node(p->arena, ND_TERNARY, loc);
    nd->cond = cond;
    nd->then = expr(p);
    expect(p, TOK_COLON);
    nd->els = ternary_expr(p);
    return nd;
}

/* logor_expr = logand_expr ('||' logand_expr)* */
static Node *logor_expr(Parser *p) {
    Node *nd = logand_expr(p);
    while (check(p, TOK_OR)) {
        SrcLoc loc = cur_loc(p);
        advance(p);
        Node *rhs = logand_expr(p);
        nd = new_binary(p->arena, ND_LOGOR, nd, rhs, loc);
    }
    return nd;
}

/* logand_expr = bitor_expr ('&&' bitor_expr)* */
static Node *logand_expr(Parser *p) {
    Node *nd = bitor_expr(p);
    while (check(p, TOK_AND)) {
        SrcLoc loc = cur_loc(p);
        advance(p);
        Node *rhs = bitor_expr(p);
        nd = new_binary(p->arena, ND_LOGAND, nd, rhs, loc);
    }
    return nd;
}

/* bitor_expr = bitxor_expr ('|' bitxor_expr)* */
static Node *bitor_expr(Parser *p) {
    Node *nd = bitxor_expr(p);
    while (check(p, TOK_PIPE)) {
        SrcLoc loc = cur_loc(p);
        advance(p);
        Node *rhs = bitxor_expr(p);
        nd = new_binary(p->arena, ND_BITOR, nd, rhs, loc);
    }
    return nd;
}

/* bitxor_expr = bitand_expr ('^' bitand_expr)* */
static Node *bitxor_expr(Parser *p) {
    Node *nd = bitand_expr(p);
    while (check(p, TOK_CARET)) {
        SrcLoc loc = cur_loc(p);
        advance(p);
        Node *rhs = bitand_expr(p);
        nd = new_binary(p->arena, ND_BITXOR, nd, rhs, loc);
    }
    return nd;
}

/* bitand_expr = equality_expr ('&' equality_expr)* */
static Node *bitand_expr(Parser *p) {
    Node *nd = equality_expr(p);
    while (check(p, TOK_AMP)) {
        SrcLoc loc = cur_loc(p);
        advance(p);
        Node *rhs = equality_expr(p);
        nd = new_binary(p->arena, ND_BITAND, nd, rhs, loc);
    }
    return nd;
}

/* equality_expr = relational_expr (('==' | '!=') relational_expr)* */
static Node *equality_expr(Parser *p) {
    Node *nd = relational_expr(p);
    for (;;) {
        SrcLoc loc = cur_loc(p);
        if (match(p, TOK_EQ)) {
            nd = new_binary(p->arena, ND_EQ, nd, relational_expr(p), loc);
        } else if (match(p, TOK_NE)) {
            nd = new_binary(p->arena, ND_NE, nd, relational_expr(p), loc);
        } else {
            break;
        }
    }
    return nd;
}

/* relational_expr = shift_expr (('<' | '>' | '<=' | '>=') shift_expr)* */
static Node *relational_expr(Parser *p) {
    Node *nd = shift_expr(p);
    for (;;) {
        SrcLoc loc = cur_loc(p);
        if (match(p, TOK_LT)) {
            nd = new_binary(p->arena, ND_LT, nd, shift_expr(p), loc);
        } else if (match(p, TOK_LE)) {
            nd = new_binary(p->arena, ND_LE, nd, shift_expr(p), loc);
        } else if (match(p, TOK_GT)) {
            /* a > b  =>  b < a */
            nd = new_binary(p->arena, ND_LT, shift_expr(p), nd, loc);
        } else if (match(p, TOK_GE)) {
            /* a >= b  =>  b <= a */
            nd = new_binary(p->arena, ND_LE, shift_expr(p), nd, loc);
        } else {
            break;
        }
    }
    return nd;
}

/* shift_expr = add_expr (('<<' | '>>') add_expr)* */
static Node *shift_expr(Parser *p) {
    Node *nd = add_expr(p);
    for (;;) {
        SrcLoc loc = cur_loc(p);
        if (match(p, TOK_LSHIFT)) {
            nd = new_binary(p->arena, ND_SHL, nd, add_expr(p), loc);
        } else if (match(p, TOK_RSHIFT)) {
            nd = new_binary(p->arena, ND_SHR, nd, add_expr(p), loc);
        } else {
            break;
        }
    }
    return nd;
}

/* add_expr = mul_expr (('+' | '-') mul_expr)* */
static Node *add_expr(Parser *p) {
    Node *nd = mul_expr(p);
    for (;;) {
        SrcLoc loc = cur_loc(p);
        if (match(p, TOK_PLUS)) {
            nd = new_binary(p->arena, ND_ADD, nd, mul_expr(p), loc);
        } else if (match(p, TOK_MINUS)) {
            nd = new_binary(p->arena, ND_SUB, nd, mul_expr(p), loc);
        } else {
            break;
        }
    }
    return nd;
}

/* mul_expr = cast_expr (('*' | '/' | '%') cast_expr)* */
static Node *mul_expr(Parser *p) {
    Node *nd = cast_expr(p);
    for (;;) {
        SrcLoc loc = cur_loc(p);
        if (match(p, TOK_STAR)) {
            nd = new_binary(p->arena, ND_MUL, nd, cast_expr(p), loc);
        } else if (match(p, TOK_SLASH)) {
            nd = new_binary(p->arena, ND_DIV, nd, cast_expr(p), loc);
        } else if (match(p, TOK_PERCENT)) {
            nd = new_binary(p->arena, ND_MOD, nd, cast_expr(p), loc);
        } else {
            break;
        }
    }
    return nd;
}

/* cast_expr = '(' type-name ')' cast_expr | unary_expr */
static Node *cast_expr(Parser *p) {
    /* Disambiguate: '(' followed by type-name means cast */
    if (check(p, TOK_LPAREN)) {
        /* Save position to backtrack if it's not a cast */
        int saved = p->pos;
        advance(p); /* consume '(' */

        if (is_typename(p)) {
            Type *ty = parse_type_name(p);
            if (match(p, TOK_RPAREN)) {
                /* Check for compound literal: (type){...} */
                if (check(p, TOK_LBRACE)) {
                    /* Compound literal */
                    Node *init = init_list(p);
                    init->ty = ty;
                    /* Infer incomplete array size from init list element count */
                    if (ty->kind == TY_ARRAY && ty->array_len <= 0 && ty->base) {
                        int count = 0;
                        for (Node *e = init->body; e; e = e->next) count++;
                        ty->array_len = count;
                        ty->size = count * ty->base->size;
                    }
                    /* Allow postfix ops: (Type){...}.field, [idx], etc. */
                    return postfix_tail(p, init);
                }
                /* Regular cast */
                Node *operand = cast_expr(p);
                Node *nd = new_cast(p->arena, operand, ty);
                nd->loc = peek(p)->loc;
                return nd;
            }
        }

        /* Not a cast -- restore and fall through to unary */
        p->pos = saved;
    }

    return unary_expr(p);
}

/* unary_expr = postfix_expr
 *            | '++' unary_expr
 *            | '--' unary_expr
 *            | '&' cast_expr
 *            | '*' cast_expr
 *            | '+' cast_expr
 *            | '-' cast_expr
 *            | '~' cast_expr
 *            | '!' cast_expr
 *            | sizeof unary_expr
 *            | sizeof '(' type-name ')'
 *            | _Alignof '(' type-name ')'
 */
static Node *unary_expr(Parser *p) {
    SrcLoc loc = cur_loc(p);

    if (match(p, TOK_INC)) {
        Node *operand = unary_expr(p);
        return new_unary(p->arena, ND_PRE_INC, operand, loc);
    }
    if (match(p, TOK_DEC)) {
        Node *operand = unary_expr(p);
        return new_unary(p->arena, ND_PRE_DEC, operand, loc);
    }
    if (match(p, TOK_AMP)) {
        Node *operand = cast_expr(p);
        return new_unary(p->arena, ND_ADDR, operand, loc);
    }
    if (match(p, TOK_STAR)) {
        Node *operand = cast_expr(p);
        return new_unary(p->arena, ND_DEREF, operand, loc);
    }
    if (match(p, TOK_PLUS)) {
        return cast_expr(p); /* unary + is a no-op */
    }
    if (match(p, TOK_MINUS)) {
        Node *operand = cast_expr(p);
        return new_unary(p->arena, ND_NEG, operand, loc);
    }
    if (match(p, TOK_TILDE)) {
        Node *operand = cast_expr(p);
        return new_unary(p->arena, ND_BITNOT, operand, loc);
    }
    if (match(p, TOK_BANG)) {
        Node *operand = cast_expr(p);
        return new_unary(p->arena, ND_NOT, operand, loc);
    }

    /* sizeof */
    if (match(p, TOK_KW_SIZEOF)) {
        /* sizeof(type-name) */
        if (check(p, TOK_LPAREN)) {
            int saved = p->pos;
            advance(p); /* '(' */
            if (is_typename(p)) {
                Type *ty = parse_type_name(p);
                expect(p, TOK_RPAREN);
                Node *nd = new_node(p->arena, ND_SIZEOF, loc);
                nd->ty = ty;
                nd->ival = ty->size;
                return nd;
            }
            /* Not a type -- restore and parse as sizeof unary-expr */
            p->pos = saved;
        }
        Node *operand = unary_expr(p);
        Node *nd = new_node(p->arena, ND_SIZEOF, loc);
        nd->lhs = operand;
        return nd;
    }

    /* _Alignof */
    if (match(p, TOK_KW__ALIGNOF)) {
        expect(p, TOK_LPAREN);
        Type *ty = parse_type_name(p);
        expect(p, TOK_RPAREN);
        Node *nd = new_node(p->arena, ND_ALIGNOF, loc);
        nd->ty = ty;
        nd->ival = ty->align;
        return nd;
    }

    return postfix_expr(p);
}

/* postfix_tail: apply postfix suffixes to an already-parsed base node */
static Node *postfix_tail(Parser *p, Node *nd) {
    for (;;) {
        SrcLoc loc = cur_loc(p);

        /* Array subscript: a[i] desugars to *(a + i) */
        if (match(p, TOK_LBRACKET)) {
            Node *idx = expr(p);
            expect(p, TOK_RBRACKET);
            Node *add = new_binary(p->arena, ND_ADD, nd, idx, loc);
            nd = new_unary(p->arena, ND_DEREF, add, loc);
            continue;
        }

        /* Function call */
        if (match(p, TOK_LPAREN)) {
            Node *call = new_node(p->arena, ND_CALL, loc);
            call->lhs = nd;

            /* Extract function name only for direct function references,
             * not for function pointer variables/parameters. */
            if (nd->kind == ND_VAR && nd->var && nd->var->ty &&
                nd->var->ty->kind == TY_FUNC) {
                call->func_name = nd->var->name;
            }

            /* Parse argument list */
            Node arg_head = {0};
            Node *arg_cur = &arg_head;
            bool first = true;
            while (!check(p, TOK_RPAREN) && !at_eof(p)) {
                if (!first) expect(p, TOK_COMMA);
                first = false;
                Node *arg = assign_expr(p);
                arg_cur->next = arg;
                arg_cur = arg;
            }
            expect(p, TOK_RPAREN);
            call->args = arg_head.next;
            nd = call;
            continue;
        }

        /* Member access: expr.member */
        if (match(p, TOK_DOT)) {
            Token *name_tok = expect(p, TOK_IDENT);
            Node *member = new_node(p->arena, ND_MEMBER, loc);
            member->lhs = nd;

            /* Create a placeholder member -- sema resolves it */
            Member *m = arena_new(p->arena, Member);
            m->name = name_tok->text;
            member->member = m;
            nd = member;
            continue;
        }

        /* Arrow member access: expr->member  desugars to (*expr).member */
        if (match(p, TOK_ARROW)) {
            Token *name_tok = expect(p, TOK_IDENT);
            Node *deref = new_unary(p->arena, ND_DEREF, nd, loc);
            Node *member = new_node(p->arena, ND_MEMBER, loc);
            member->lhs = deref;

            Member *m = arena_new(p->arena, Member);
            m->name = name_tok->text;
            member->member = m;
            nd = member;
            continue;
        }

        /* Postfix increment */
        if (match(p, TOK_INC)) {
            nd = new_unary(p->arena, ND_POST_INC, nd, loc);
            continue;
        }

        /* Postfix decrement */
        if (match(p, TOK_DEC)) {
            nd = new_unary(p->arena, ND_POST_DEC, nd, loc);
            continue;
        }

        break;
    }

    return nd;
}

/* postfix_expr = primary_expr ( '[' expr ']'
 *                              | '(' arg-list? ')'
 *                              | '.' ident
 *                              | '->' ident
 *                              | '++'
 *                              | '--' )* */
static Node *postfix_expr(Parser *p) {
    Node *nd = primary_expr(p);
    return postfix_tail(p, nd);
}

/* primary_expr = number | float | char | string | ident | '(' expr ')'
 *             | '(' '{' compound_stmt '}' ')'  (GNU statement expression)
 */
static Node *primary_expr(Parser *p) {
    Token *t = peek(p);
    SrcLoc loc = t->loc;

    /* Integer literal */
    if (t->kind == TOK_INT_LIT) {
        advance(p);
        Node *nd = new_num(p->arena, (long long)t->int_val, loc);
        /* Set type based on suffix */
        switch (t->int_suffix) {
        case INT_SUFFIX_NONE:  nd->ty = ty_int;   break;
        case INT_SUFFIX_U:     nd->ty = ty_uint;  break;
        case INT_SUFFIX_L:     nd->ty = ty_long;  break;
        case INT_SUFFIX_UL:    nd->ty = ty_ulong; break;
        case INT_SUFFIX_LL:    nd->ty = ty_llong; break;
        case INT_SUFFIX_ULL:   nd->ty = ty_ullong; break;
        }
        return nd;
    }

    /* Float literal */
    if (t->kind == TOK_FLOAT_LIT) {
        advance(p);
        Node *nd = new_node(p->arena, ND_FNUM, loc);
        nd->fval = t->float_val;
        nd->ty = ty_double;
        return nd;
    }

    /* Character literal */
    if (t->kind == TOK_CHAR_LIT) {
        advance(p);
        Node *nd = new_num(p->arena, (long long)t->char_val, loc);
        nd->ty = ty_int; /* char literals have type int in C */
        return nd;
    }

    /* String literal */
    if (t->kind == TOK_STRING_LIT) {
        advance(p);
        /* Concatenate adjacent string literals */
        const char *str = t->text;
        int len = t->text_len;

        /* For simplicity, handle single strings; adjacent concatenation
         * is typically done by the preprocessor. If we see more strings,
         * concatenate them. */
        while (check(p, TOK_STRING_LIT)) {
            Token *next = advance(p);
            /* Create concatenated string in arena */
            int new_len = len + next->text_len;
            char *buf = arena_alloc(p->arena, new_len + 1);
            memcpy(buf, str, len);
            memcpy(buf + len, next->text, next->text_len);
            buf[new_len] = '\0';
            str = buf;
            len = new_len;
        }

        /* Create a string literal global variable */
        Obj *obj = new_string_literal(p, str, len + 1, loc); /* +1 for NUL */
        Node *nd = new_var(p->arena, obj, loc);
        nd->ty = type_array(p->arena, ty_char, len + 1);
        nd->str_val = str;
        nd->str_len = len;
        return nd;
    }

    /* Identifier */
    if (t->kind == TOK_IDENT) {
        /* __func__ predefined identifier (C99/C11) */
        if (strcmp(t->text, "__func__") == 0 || strcmp(t->text, "__FUNCTION__") == 0) {
            advance(p);
            const char *fname = p->cur_func_name ? p->cur_func_name : "";
            int len = (int)strlen(fname);
            Obj *obj = new_string_literal(p, fname, len + 1, loc);
            Node *nd = new_var(p->arena, obj, loc);
            nd->ty = type_array(p->arena, ty_char, len + 1);
            nd->str_val = fname;
            nd->str_len = len;
            return nd;
        }

        advance(p);

        Obj *obj = scope_find_var(p, t->text);
        if (!obj) {
            /* Implicit function declaration (C89 compat) -- create it */
            if (check(p, TOK_LPAREN)) {
                Type *fn_ty = type_func(p->arena, ty_int);
                fn_ty->is_variadic = true;
                obj = new_global(p, t->text, fn_ty, loc);
                obj->is_definition = false;
            } else {
                if (!p->suppress_undecl) {
                    diag_error(&p->cc->diag, loc,
                               "undeclared identifier '%s'", t->text);
                }
                /* Error recovery: create a dummy int variable */
                obj = new_obj(p, t->text, ty_int, loc);
            }
        }

        /* Enum constant: return as integer literal */
        if (obj->is_enum_const) {
            return new_num(p->arena, obj->enum_val, loc);
        }

        return new_var(p->arena, obj, loc);
    }

    /* Parenthesized expression or GNU statement expression */
    if (t->kind == TOK_LPAREN) {
        advance(p);

        /* GNU statement expression: ({ ... }) */
        if (check(p, TOK_LBRACE)) {
            Node *body = compound_stmt(p);
            expect(p, TOK_RPAREN);
            Node *nd = new_node(p->arena, ND_STMT_EXPR, loc);
            nd->body = body->body; /* unwrap the block */
            return nd;
        }

        Node *nd = expr(p);
        expect(p, TOK_RPAREN);
        return nd;
    }

    /* _Generic selection expression — deferred to sema for type matching */
    if (t->kind == TOK_KW__GENERIC) {
        advance(p);
        expect(p, TOK_LPAREN);
        Node *controlling = assign_expr(p);
        expect(p, TOK_COMMA);

        Node *nd = new_node(p->arena, ND_GENERIC, loc);
        nd->lhs = controlling;

        /* Parse association list: store as linked list in body */
        Node head = {0};
        Node *cur = &head;
        Node *default_expr = NULL;

        while (!check(p, TOK_RPAREN) && !at_eof(p)) {
            if (check(p, TOK_KW_DEFAULT)) {
                advance(p);
                expect(p, TOK_COLON);
                default_expr = assign_expr(p);
            } else {
                Type *assoc_ty = parse_type_name(p);
                expect(p, TOK_COLON);
                Node *assoc_expr = assign_expr(p);
                assoc_expr->ty = assoc_ty; /* Store associated type */
                cur->next = assoc_expr;
                cur = assoc_expr;
            }
            if (!check(p, TOK_RPAREN))
                expect(p, TOK_COMMA);
        }
        expect(p, TOK_RPAREN);
        nd->body = head.next;
        nd->rhs = default_expr;
        return nd;
    }

    diag_error(&p->cc->diag, loc,
               "expected expression, got '%s'", token_kind_str(t->kind));
    advance(p); /* skip the offending token to avoid infinite loops */
    return new_num(p->arena, 0, loc); /* error recovery */
}

/* ------------------------------------------------------------------ */
/*  Top-level: translation unit                                        */
/* ------------------------------------------------------------------ */

Node *cc_parse(CompilerCtx *cc, TokenList *tokens) {
    Parser parser = {0};
    Parser *p = &parser;

    p->cc = cc;
    p->arena = &cc->arena;
    p->tok = tokens->tokens;
    p->ntok = tokens->count;
    p->pos = 0;
    p->scope = NULL;
    p->locals = NULL;
    p->globals = NULL;
    p->labels = NULL;
    p->gotos = NULL;
    p->cur_switch = NULL;
    p->string_cnt = 0;

    /* Create the global (file) scope */
    scope_new(p);

    Node *program = new_node(p->arena, ND_PROGRAM, SRCLOC_NONE);
    Node head = {0};
    Node *cur = &head;

    while (!at_eof(p)) {
        if (diag_too_many_errors(&cc->diag))
            break;

        Node *nd;
        if (check(p, TOK_KW__STATIC_ASSERT)) {
            nd = parse_static_assert(p);
        } else if (check(p, TOK_SEMI)) {
            /* Empty declaration at top level */
            advance(p);
            continue;
        } else if (is_function_def(p)) {
            nd = funcdef(p);
        } else {
            nd = declaration(p);
        }

        if (nd) {
            cur->next = nd;
            while (cur->next) cur = cur->next;
        }
    }

    scope_pop(p);

    program->body = head.next;
    return program;
}
