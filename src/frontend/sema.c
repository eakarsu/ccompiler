#include "cc.h"

/* Scope for symbol table */
typedef struct Scope {
    struct Scope *parent;
    HashTab vars;       /* name -> Obj* */
    HashTab tags;       /* struct/union/enum tags -> Type* */
    HashTab typedefs;   /* typedef names -> Type* */
} Scope;

typedef struct SemaCtx {
    CompilerCtx *cc;
    Arena *arena;
    Scope *scope;
    Type *current_func_ret;  /* return type of current function */
    Node *current_switch;    /* innermost switch for case/default */
    int loop_depth;          /* for break/continue validation */
    Obj *locals;             /* linked list of local variables */
    Obj *globals;            /* linked list of global variables */
} SemaCtx;

static Scope *scope_new(SemaCtx *ctx) {
    Scope *s = arena_new(ctx->arena, Scope);
    s->parent = ctx->scope;
    hashtab_init(&s->vars, 16);
    hashtab_init(&s->tags, 16);
    hashtab_init(&s->typedefs, 16);
    ctx->scope = s;
    return s;
}

static void scope_pop(SemaCtx *ctx) {
    Scope *s = ctx->scope;
    ctx->scope = s->parent;
    hashtab_free(&s->vars);
    hashtab_free(&s->tags);
    hashtab_free(&s->typedefs);
}

static Obj *scope_lookup(SemaCtx *ctx, const char *name) {
    for (Scope *s = ctx->scope; s; s = s->parent) {
        Obj *obj = hashtab_get(&s->vars, name);
        if (obj) return obj;
    }
    return NULL;
}

static Type *scope_lookup_tag(SemaCtx *ctx, const char *tag) {
    for (Scope *s = ctx->scope; s; s = s->parent) {
        Type *ty = hashtab_get(&s->tags, tag);
        if (ty) return ty;
    }
    return NULL;
}

static Type *scope_lookup_typedef(SemaCtx *ctx, const char *name) {
    for (Scope *s = ctx->scope; s; s = s->parent) {
        Type *ty = hashtab_get(&s->typedefs, name);
        if (ty) return ty;
    }
    return NULL;
}

static void scope_define(SemaCtx *ctx, const char *name, Obj *obj) {
    hashtab_put(&ctx->scope->vars, name, obj);
}

static Obj *new_obj(SemaCtx *ctx, const char *name, Type *ty, SrcLoc loc) {
    Obj *obj = arena_new(ctx->arena, Obj);
    obj->name = name;
    obj->ty = ty;
    obj->loc = loc;
    return obj;
}

static void add_type(SemaCtx *ctx, Node *node);
static void check_stmt(SemaCtx *ctx, Node *node);
static void check_expr(SemaCtx *ctx, Node *node);

/* Integer promotion */
static Type *promote_int(Type *ty) {
    if (ty->kind == TY_BOOL || ty->kind == TY_CHAR || ty->kind == TY_SHORT) {
        return ty_int;
    }
    return ty;
}

/* Usual arithmetic conversion */
static Type *usual_arith_conv(Arena *a, Type *lhs, Type *rhs) {
    (void)a;
    lhs = promote_int(lhs);
    rhs = promote_int(rhs);

    if (lhs->kind == TY_LDOUBLE || rhs->kind == TY_LDOUBLE) return ty_ldouble;
    if (lhs->kind == TY_DOUBLE || rhs->kind == TY_DOUBLE) return ty_double;
    if (lhs->kind == TY_FLOAT || rhs->kind == TY_FLOAT) return ty_float;

    /* Both are integers - find common type */
    if (lhs->size != rhs->size) {
        return lhs->size > rhs->size ? lhs : rhs;
    }
    if (lhs->is_unsigned) return lhs;
    if (rhs->is_unsigned) return rhs;
    return lhs;
}

static Node *maybe_cast(SemaCtx *ctx, Node *node, Type *to) {
    if (!node->ty || !to) return node;
    if (node->ty == to) return node;
    if (node->ty->kind == to->kind && node->ty->is_unsigned == to->is_unsigned) return node;
    return new_cast(ctx->arena, node, to);
}

static void check_expr(SemaCtx *ctx, Node *node) {
    if (!node) return;

    switch (node->kind) {
    case ND_NUM:
        if (!node->ty) node->ty = ty_int;
        return;

    case ND_FNUM:
        if (!node->ty) node->ty = ty_double;
        return;

    case ND_VAR:
        if (!node->var) {
            /* Need to look up variable */
            diag_error(&ctx->cc->diag, node->loc, "undefined variable");
            node->ty = ty_int;
            return;
        }
        node->ty = node->var->ty;
        return;

    case ND_STRING:
        node->ty = type_array(ctx->arena, ty_char, node->str_len);
        return;

    case ND_ADDR:
        check_expr(ctx, node->lhs);
        node->ty = type_ptr(ctx->arena, node->lhs->ty);
        return;

    case ND_DEREF:
        check_expr(ctx, node->lhs);
        if (node->lhs->ty->kind != TY_PTR && node->lhs->ty->kind != TY_ARRAY) {
            diag_error(&ctx->cc->diag, node->loc, "cannot dereference non-pointer type");
            node->ty = ty_int;
            return;
        }
        node->ty = node->lhs->ty->base;
        return;

    case ND_NEG:
        check_expr(ctx, node->lhs);
        node->ty = promote_int(node->lhs->ty);
        return;

    case ND_NOT:
        check_expr(ctx, node->lhs);
        node->ty = ty_int;
        return;

    case ND_BITNOT:
        check_expr(ctx, node->lhs);
        node->ty = promote_int(node->lhs->ty);
        return;

    case ND_ADD: case ND_SUB:
    case ND_MUL: case ND_DIV: case ND_MOD:
    case ND_BITAND: case ND_BITOR: case ND_BITXOR:
    case ND_SHL: case ND_SHR:
        check_expr(ctx, node->lhs);
        check_expr(ctx, node->rhs);

        /* Pointer arithmetic */
        if (node->kind == ND_ADD || node->kind == ND_SUB) {
            if (node->lhs->ty->kind == TY_PTR || node->lhs->ty->kind == TY_ARRAY) {
                node->ty = node->lhs->ty->kind == TY_ARRAY
                    ? type_ptr(ctx->arena, node->lhs->ty->base) : node->lhs->ty;
                return;
            }
            if (node->kind == ND_ADD && (node->rhs->ty->kind == TY_PTR || node->rhs->ty->kind == TY_ARRAY)) {
                /* Swap so pointer is on the left */
                Node *tmp = node->lhs;
                node->lhs = node->rhs;
                node->rhs = tmp;
                node->ty = node->lhs->ty->kind == TY_ARRAY
                    ? type_ptr(ctx->arena, node->lhs->ty->base) : node->lhs->ty;
                return;
            }
        }

        node->ty = usual_arith_conv(ctx->arena, node->lhs->ty, node->rhs->ty);
        return;

    case ND_EQ: case ND_NE:
    case ND_LT: case ND_LE:
        check_expr(ctx, node->lhs);
        check_expr(ctx, node->rhs);
        node->ty = ty_int;
        return;

    case ND_LOGAND: case ND_LOGOR:
        check_expr(ctx, node->lhs);
        check_expr(ctx, node->rhs);
        node->ty = ty_int;
        return;

    case ND_ASSIGN:
        check_expr(ctx, node->lhs);
        check_expr(ctx, node->rhs);
        node->rhs = maybe_cast(ctx, node->rhs, node->lhs->ty);
        node->ty = node->lhs->ty;
        return;

    case ND_COMPOUND_ASSIGN:
        check_expr(ctx, node->lhs);
        check_expr(ctx, node->rhs);
        node->ty = node->lhs->ty;
        return;

    case ND_TERNARY:
        check_expr(ctx, node->cond);
        check_expr(ctx, node->then);
        check_expr(ctx, node->els);
        /* Compute the common type using usual arithmetic conversions */
        if (node->then->ty && node->els->ty &&
            (type_is_integer(node->then->ty) || type_is_floating(node->then->ty)) &&
            (type_is_integer(node->els->ty) || type_is_floating(node->els->ty))) {
            node->ty = usual_arith_conv(ctx->arena, node->then->ty, node->els->ty);
        } else if (node->then->ty) {
            node->ty = node->then->ty;
        } else {
            node->ty = node->els->ty;
        }
        return;

    case ND_COMMA:
        check_expr(ctx, node->lhs);
        check_expr(ctx, node->rhs);
        node->ty = node->rhs->ty;
        return;

    case ND_CAST:
        check_expr(ctx, node->lhs);
        /* ty is already set by parser */
        return;

    case ND_GENERIC: {
        /* Resolve _Generic: match controlling expression type against associations */
        check_expr(ctx, node->lhs);
        Type *ctrl_ty = node->lhs->ty;
        /* Array and function types decay to pointers for matching */
        if (ctrl_ty && ctrl_ty->kind == TY_ARRAY) ctrl_ty = type_ptr(ctx->arena, ctrl_ty->base);
        if (ctrl_ty && ctrl_ty->kind == TY_FUNC) ctrl_ty = type_ptr(ctx->arena, ctrl_ty);

        Node *result = NULL;
        /* Check each association (body is linked list of association exprs with ty set to assoc type) */
        for (Node *assoc = node->body; assoc; assoc = assoc->next) {
            Type *assoc_ty = assoc->ty;
            if (ctrl_ty && assoc_ty && type_is_compatible(ctrl_ty, assoc_ty)) {
                result = assoc;
                break;
            }
        }
        if (!result && node->rhs) result = node->rhs; /* default */
        if (result) {
            check_expr(ctx, result);
            /* Replace ND_GENERIC with the matched result inline */
            Node saved_next = {0};
            saved_next.next = node->next;
            *node = *result;
            node->next = saved_next.next;
        } else {
            node->ty = ty_int; /* no match, error recovery */
        }
        return;
    }

    case ND_SIZEOF:
    case ND_ALIGNOF:
        if (node->lhs) check_expr(ctx, node->lhs);
        node->ty = ty_ulong;  /* size_t */
        return;

    case ND_CALL: {
        /* Check function */
        if (node->lhs) check_expr(ctx, node->lhs);
        /* Check arguments */
        for (Node *arg = node->args; arg; arg = arg->next) {
            check_expr(ctx, arg);
        }
        /* Determine return type */
        if (node->lhs && node->lhs->ty) {
            Type *fty = node->lhs->ty;
            if (fty->kind == TY_PTR) fty = fty->base;
            if (fty->kind == TY_FUNC) {
                node->ty = fty->return_ty;
            } else {
                node->ty = ty_int;
            }
        } else {
            /* Implicit function declaration */
            if (node->lhs && node->lhs->kind == ND_VAR && node->lhs->var)
                diag_warn(&ctx->cc->diag, node->loc,
                          "implicit declaration of function '%s'",
                          node->lhs->var->name);
            node->ty = ty_int;
        }
        return;
    }

    case ND_PRE_INC: case ND_PRE_DEC:
    case ND_POST_INC: case ND_POST_DEC:
        check_expr(ctx, node->lhs);
        node->ty = node->lhs->ty;
        return;

    case ND_MEMBER:
        check_expr(ctx, node->lhs);
        /* Resolve struct/union member by name */
        if (node->member && node->member->name && node->lhs->ty) {
            Type *sty = node->lhs->ty;
            if (sty->kind == TY_PTR && sty->base)
                sty = sty->base;
            if ((sty->kind == TY_STRUCT || sty->kind == TY_UNION) && sty->members) {
                for (Member *m = sty->members; m; m = m->next) {
                    if (m->name && strcmp(m->name, node->member->name) == 0) {
                        node->member = m;
                        break;
                    }
                }
            }
            node->ty = node->member->ty ? node->member->ty : ty_int;
        } else {
            node->ty = ty_int;
        }
        return;

    case ND_STMT_EXPR:
        check_stmt(ctx, node->body);
        /* Type is the type of the last expression */
        node->ty = ty_void;
        for (Node *s = node->body; s; s = s->next) {
            if (!s->next && s->kind == ND_EXPR_STMT && s->lhs) {
                check_expr(ctx, s->lhs);
                node->ty = s->lhs->ty;
            }
        }
        return;

    case ND_INIT_LIST:
        for (Node *e = node->body; e; e = e->next) {
            check_expr(ctx, e);
        }
        if (!node->ty) node->ty = ty_int;
        return;

    default:
        if (!node->ty) node->ty = ty_int;
        return;
    }
}

static void check_stmt(SemaCtx *ctx, Node *node) {
    if (!node) return;

    switch (node->kind) {
    case ND_BLOCK:
        scope_new(ctx);
        for (Node *s = node->body; s; s = s->next) {
            check_stmt(ctx, s);
        }
        scope_pop(ctx);
        return;

    case ND_IF:
        check_expr(ctx, node->cond);
        check_stmt(ctx, node->then);
        if (node->els) check_stmt(ctx, node->els);
        return;

    case ND_FOR:
        scope_new(ctx);
        /* Do-while uses init=(Node*)1 as a flag, skip it */
        if (node->init && node->init != (Node *)(intptr_t)1)
            check_stmt(ctx, node->init);
        if (node->cond) check_expr(ctx, node->cond);
        if (node->inc) check_expr(ctx, node->inc);
        ctx->loop_depth++;
        check_stmt(ctx, node->then);
        ctx->loop_depth--;
        scope_pop(ctx);
        return;

    case ND_SWITCH:
        check_expr(ctx, node->cond);
        {
            Node *saved_switch = ctx->current_switch;
            ctx->current_switch = node;
            ctx->loop_depth++;
            check_stmt(ctx, node->then);
            ctx->loop_depth--;
            ctx->current_switch = saved_switch;
        }
        return;

    case ND_CASE:
    case ND_DEFAULT:
        if (!ctx->current_switch) {
            diag_error(&ctx->cc->diag, node->loc, "case/default outside switch");
        }
        check_stmt(ctx, node->lhs);
        return;

    case ND_RETURN:
        if (node->lhs) {
            check_expr(ctx, node->lhs);
            if (ctx->current_func_ret && ctx->current_func_ret->kind != TY_VOID) {
                node->lhs = maybe_cast(ctx, node->lhs, ctx->current_func_ret);
            }
        }
        return;

    case ND_BREAK:
    case ND_CONTINUE:
        if (ctx->loop_depth == 0) {
            diag_error(&ctx->cc->diag, node->loc,
                       "%s outside loop/switch",
                       node->kind == ND_BREAK ? "break" : "continue");
        }
        return;

    case ND_GOTO:
        /* Label validation done later */
        return;

    case ND_LABEL:
        check_stmt(ctx, node->lhs);
        return;

    case ND_EXPR_STMT:
        if (node->lhs) check_expr(ctx, node->lhs);
        return;

    case ND_NULL_STMT:
        return;

    case ND_DECLARATION: {
        Obj *var = node->var;
        if (var) {
            scope_define(ctx, var->name, var);
            if (var->init_expr) {
                check_expr(ctx, var->init_expr);
                var->init_expr = maybe_cast(ctx, var->init_expr, var->ty);
            }
        }
        return;
    }

    default:
        /* Expression statement or unknown */
        if (node->lhs) check_expr(ctx, node->lhs);
        if (node->rhs) check_expr(ctx, node->rhs);
        return;
    }
}

static void check_function(SemaCtx *ctx, Node *func_node) {
    if (!func_node->var) return;
    Obj *fn = func_node->var;

    scope_new(ctx);

    /* Add parameters to scope */
    if (fn->ty && fn->ty->kind == TY_FUNC) {
        ctx->current_func_ret = fn->ty->return_ty;
        for (Param *p = fn->ty->params; p; p = p->next) {
            Obj *param = new_obj(ctx, p->name, p->ty, func_node->loc);
            scope_define(ctx, p->name, param);
        }
    }

    /* Check function body */
    check_stmt(ctx, func_node->body);

    ctx->current_func_ret = NULL;
    scope_pop(ctx);
}

void cc_sema(CompilerCtx *cc, Node *program) {
    if (!program) return;

    SemaCtx ctx = {0};
    ctx.cc = cc;
    ctx.arena = &cc->arena;
    ctx.scope = NULL;
    ctx.current_func_ret = NULL;
    ctx.current_switch = NULL;
    ctx.loop_depth = 0;
    ctx.locals = NULL;
    ctx.globals = NULL;

    /* Create global scope */
    scope_new(&ctx);

    /* Process top-level declarations */
    for (Node *decl = program->body; decl; decl = decl->next) {
        switch (decl->kind) {
        case ND_FUNCDEF:
            if (decl->var) {
                decl->var->is_global = true;
                scope_define(&ctx, decl->var->name, decl->var);
            }
            check_function(&ctx, decl);
            break;

        case ND_DECLARATION:
            if (decl->var) {
                decl->var->is_global = true;
                scope_define(&ctx, decl->var->name, decl->var);
                if (decl->var->init_expr) {
                    check_expr(&ctx, decl->var->init_expr);
                }
            }
            break;

        case ND_TYPEDEF:
            if (decl->var && decl->var->ty) {
                hashtab_put(&ctx.scope->typedefs, decl->var->name, decl->var->ty);
            }
            break;

        default:
            break;
        }
    }

    scope_pop(&ctx);
}
