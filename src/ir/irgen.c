/*
 * irgen.c - AST to IR lowering for mycc
 *
 * Generates unoptimized IR from the AST using an alloca-based approach.
 * All local variables are represented as alloca+load+store sequences.
 * The mem2reg pass later promotes them to SSA form.
 */

#include "cc.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* ================================================================
 * IRGen context
 * ================================================================ */

/* Case info for switch dispatch chain */
typedef struct CaseInfo {
    long long val;
    IRBlock *bb;
    struct CaseInfo *next;
} CaseInfo;

typedef struct {
    CompilerCtx *cc;
    Arena *a;
    IRModule *mod;
    IRFunc *cur_func;
    IRBlock *cur_block;

    /* Map from Obj* to its alloca IRInst* (for local variables) */
    HashTab locals;       /* key: Obj->name, value: IRInst* (alloca) */

    /* For break/continue in loops */
    IRBlock *break_target;
    IRBlock *continue_target;

    /* For switch */
    IRBlock *switch_end;
    IRInst  *switch_val;
    IRBlock *switch_default;

    /* Label map for goto */
    HashTab labels;       /* key: label name, value: IRBlock* */

    /* String literal counter */
    int str_count;

    /* For function return */
    IRInst  *ret_alloca;  /* alloca for return value */
    IRBlock *ret_block;   /* function exit block */
} IRGenCtx;

/* Forward declarations */
static IRInst *gen_expr(IRGenCtx *ctx, Node *node);
static void    gen_stmt(IRGenCtx *ctx, Node *node);
static IRInst *gen_lvalue(IRGenCtx *ctx, Node *node);
static void    gen_declaration(IRGenCtx *ctx, Node *node);
static void    gen_init_list(IRGenCtx *ctx, IRInst *base_addr, Node *init_list, Type *ty);
static void    gen_switch_body(IRGenCtx *ctx, Node *node,
                               CaseInfo **cur_case, IRBlock *default_bb,
                               IRBlock *end_bb);

static IRType ctype_to_ir(Type *ty);
static IRInst *to_bool(IRGenCtx *ctx, IRInst *val, Type *ty);

/* Evaluate a constant expression from the AST. Returns true if successful. */
static bool eval_const_expr(Node *n, int64_t *out)
{
    if (!n) return false;
    if (n->kind == ND_NUM) { *out = n->ival; return true; }
    int64_t l, r;
    if (n->lhs && n->rhs && eval_const_expr(n->lhs, &l) && eval_const_expr(n->rhs, &r)) {
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
    if (n->kind == ND_NEG && n->lhs && eval_const_expr(n->lhs, &l)) { *out = -l; return true; }
    if (n->kind == ND_BITNOT && n->lhs && eval_const_expr(n->lhs, &l)) { *out = ~l; return true; }
    if (n->kind == ND_SIZEOF) { *out = n->ival; return true; }
    /* Enum constants are ND_NUM nodes, so they're handled above */
    return false;
}

/* Ensure a global Obj (e.g. string literal) has an IRGlobal in the module. */
static void ensure_global_registered(IRGenCtx *ctx, Obj *var)
{
    /* Functions are emitted as code, not global variables */
    if (var->ty && var->ty->kind == TY_FUNC)
        return;

    /* Check if already registered */
    for (IRGlobal *g = ctx->mod->globals; g; g = g->next) {
        if (g->name == var->name || (g->name && var->name &&
            strcmp(g->name, var->name) == 0))
            return;
    }
    /* Register it */
    IRGlobal *g = arena_new(ctx->a, IRGlobal);
    g->name = var->name;
    g->ty = ctype_to_ir(var->ty);
    g->alloc_size = var->ty->size;
    g->alloc_align = var->ty->align;
    g->is_string = (var->string_val != NULL);
    g->string_val = var->string_val;
    g->string_len = var->string_len;
    g->is_extern = var->is_extern;
    g->is_static = var->is_static;
    g->has_init = false;
    g->init_val = 0;
    g->next = ctx->mod->globals;
    ctx->mod->globals = g;
}

/* ================================================================
 * Helpers
 * ================================================================ */

static IRType make_ir_type(IRTypeKind kind)
{
    IRType ty = {0};
    ty.kind = kind;
    switch (kind) {
    case IR_TYPE_I1:        ty.size = 1; ty.align = 1; break;
    case IR_TYPE_I8:        ty.size = 1; ty.align = 1; break;
    case IR_TYPE_I16:       ty.size = 2; ty.align = 2; break;
    case IR_TYPE_I32:       ty.size = 4; ty.align = 4; break;
    case IR_TYPE_I64:       ty.size = 8; ty.align = 8; break;
    case IR_TYPE_F32:       ty.size = 4; ty.align = 4; break;
    case IR_TYPE_F64:       ty.size = 8; ty.align = 8; break;
    case IR_TYPE_PTR:       ty.size = 8; ty.align = 8; break;
    case IR_TYPE_VOID:      ty.size = 0; ty.align = 0; break;
    case IR_TYPE_AGGREGATE: ty.size = 0; ty.align = 0; break;
    }
    return ty;
}

static IRType ptr_type(void)
{
    return make_ir_type(IR_TYPE_PTR);
}

static IRType i32_type(void)
{
    return make_ir_type(IR_TYPE_I32);
}

static IRType i64_type(void)
{
    return make_ir_type(IR_TYPE_I64);
}

static IRType void_type(void)
{
    return make_ir_type(IR_TYPE_VOID);
}

/* Create a new named basic block in the current function */
static IRBlock *new_block(IRGenCtx *ctx, const char *name)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%s.%d", name, ctx->cur_func->next_block_id);
    const char *block_name = arena_strdup(ctx->a, buf);
    return ir_block_new(ctx->a, ctx->cur_func, block_name);
}

/* Switch the "insertion point" to a different block */
static void set_block(IRGenCtx *ctx, IRBlock *bb)
{
    ctx->cur_block = bb;
}

/* Check if the current block already has a terminator */
static bool block_terminated(IRGenCtx *ctx)
{
    IRBlock *bb = ctx->cur_block;
    if (!bb || !bb->last) return false;
    IROpcode op = bb->last->op;
    return op == IR_BR || op == IR_CONDBR || op == IR_RET || op == IR_RETV;
}

/* Insert an alloca at the beginning of the entry block (among existing allocas),
 * rather than appending to the end (which could be after the terminator). */
static IRInst *entry_alloca(IRGenCtx *ctx, int size, int align)
{
    IRBlock *entry = ctx->cur_func->entry;
    IRType pty = ptr_type();
    IRInst *alloca_inst = ir_inst_new(ctx->a, entry, IR_ALLOCA, pty);
    alloca_inst->alloca_size = size;
    alloca_inst->alloca_align = align;

    /* Find insertion point: after existing allocas in entry block */
    IRInst *insert_after = NULL;
    for (IRInst *inst = entry->first; inst; inst = inst->next) {
        if (inst->op == IR_ALLOCA)
            insert_after = inst;
        else
            break;
    }

    if (insert_after) {
        alloca_inst->next = insert_after->next;
        alloca_inst->prev = insert_after;
        if (insert_after->next)
            insert_after->next->prev = alloca_inst;
        insert_after->next = alloca_inst;
        if (entry->last == insert_after)
            entry->last = alloca_inst;
    } else {
        alloca_inst->next = entry->first;
        alloca_inst->prev = NULL;
        if (entry->first)
            entry->first->prev = alloca_inst;
        entry->first = alloca_inst;
        if (!entry->last)
            entry->last = alloca_inst;
    }
    alloca_inst->parent = entry;
    entry->inst_count++;
    return alloca_inst;
}

/* Emit an unconditional branch if the current block hasn't been terminated yet */
static void emit_br_if_needed(IRGenCtx *ctx, IRBlock *target)
{
    if (!block_terminated(ctx)) {
        ir_build_br(ctx->a, ctx->cur_block, target);
    }
}

/* Get the IRType for a C type */
static IRType ctype_to_ir(Type *ty)
{
    return ir_type_from_ctype(ty);
}

/* Determine if a C type is a floating-point type */
static bool is_float_type(Type *ty)
{
    return ty && type_is_floating(ty);
}

/* Determine if a C type is an unsigned integer type */
static bool is_unsigned_type(Type *ty)
{
    return ty && ty->is_unsigned;
}

/* Determine if a C type is an integer type */
static bool is_int_type(Type *ty)
{
    return ty && type_is_integer(ty);
}

/* Determine if a C type is a pointer type */
static bool is_pointer_type(Type *ty)
{
    return ty && (ty->kind == TY_PTR || ty->kind == TY_ARRAY);
}

/* Get the size of a pointed-to element (for pointer arithmetic) */
static int pointee_size(Type *ty)
{
    if (!ty) return 1;
    if (ty->kind == TY_PTR && ty->base)
        return ty->base->size > 0 ? ty->base->size : 1;
    if (ty->kind == TY_ARRAY && ty->base)
        return ty->base->size > 0 ? ty->base->size : 1;
    return 1;
}

/* ================================================================
 * Integer/float conversion helpers
 * ================================================================ */

/* Generate a cast from one IR type to another */
static IRInst *gen_cast(IRGenCtx *ctx, IRInst *val, Type *from, Type *to)
{
    if (!from || !to) return val;

    IRType from_ir = ctype_to_ir(from);
    IRType to_ir = ctype_to_ir(to);

    /* Same type - no cast needed */
    if (ir_type_eq(from_ir, to_ir))
        return val;

    /* Pointer to integer */
    if (is_pointer_type(from) && is_int_type(to)) {
        return ir_build_cast(ctx->a, ctx->cur_block, IR_PTRTOINT, to_ir, val);
    }

    /* Integer to pointer */
    if (is_int_type(from) && is_pointer_type(to)) {
        return ir_build_cast(ctx->a, ctx->cur_block, IR_INTTOPTR, to_ir, val);
    }

    /* Pointer to pointer (bitcast) */
    if (is_pointer_type(from) && is_pointer_type(to)) {
        return ir_build_cast(ctx->a, ctx->cur_block, IR_BITCAST, to_ir, val);
    }

    /* Float to float */
    if (is_float_type(from) && is_float_type(to)) {
        if (ir_type_size(from_ir) < ir_type_size(to_ir))
            return ir_build_cast(ctx->a, ctx->cur_block, IR_FPEXT, to_ir, val);
        else
            return ir_build_cast(ctx->a, ctx->cur_block, IR_FPTRUNC, to_ir, val);
    }

    /* Integer to float */
    if (is_int_type(from) && is_float_type(to)) {
        if (is_unsigned_type(from))
            return ir_build_cast(ctx->a, ctx->cur_block, IR_UITOFP, to_ir, val);
        else
            return ir_build_cast(ctx->a, ctx->cur_block, IR_SITOFP, to_ir, val);
    }

    /* Float to integer */
    if (is_float_type(from) && is_int_type(to)) {
        if (is_unsigned_type(to))
            return ir_build_cast(ctx->a, ctx->cur_block, IR_FPTOUI, to_ir, val);
        else
            return ir_build_cast(ctx->a, ctx->cur_block, IR_FPTOSI, to_ir, val);
    }

    /* Anything to _Bool: compare != 0 (C11 6.3.1.2) */
    if (to->kind == TY_BOOL) {
        return to_bool(ctx, val, from);
    }

    /* Integer to integer */
    if (is_int_type(from) && is_int_type(to)) {
        int from_sz = ir_type_size(from_ir);
        int to_sz = ir_type_size(to_ir);
        if (from_sz < to_sz) {
            if (is_unsigned_type(from))
                return ir_build_cast(ctx->a, ctx->cur_block, IR_ZEXT, to_ir, val);
            else
                return ir_build_cast(ctx->a, ctx->cur_block, IR_SEXT, to_ir, val);
        } else if (from_sz > to_sz) {
            return ir_build_cast(ctx->a, ctx->cur_block, IR_TRUNC, to_ir, val);
        }
        /* Same size, different signedness: no-op */
        return val;
    }

    /* Bool to integer: zext */
    if (from->kind == TY_BOOL && is_int_type(to)) {
        return ir_build_cast(ctx->a, ctx->cur_block, IR_ZEXT, to_ir, val);
    }

    return val;
}

/* Convert a value to i1 (boolean) for use in conditional branches */
static IRInst *to_bool(IRGenCtx *ctx, IRInst *val, Type *ty)
{
    if (val->ty.kind == IR_TYPE_I1)
        return val;

    if (is_float_type(ty)) {
        IRInst *zero = ir_build_const_float(ctx->a, ctx->cur_block, val->ty, 0.0);
        return ir_build_binop(ctx->a, ctx->cur_block, IR_FCMP_ONE, val, zero);
    }

    /* Integer or pointer: compare against zero */
    IRInst *zero = ir_build_const_int(ctx->a, ctx->cur_block, val->ty, 0);
    return ir_build_binop(ctx->a, ctx->cur_block, IR_ICMP_NE, val, zero);
}

/* ================================================================
 * L-value generation (returns a pointer to the value)
 * ================================================================ */

static IRInst *gen_lvalue(IRGenCtx *ctx, Node *node)
{
    switch (node->kind) {
    case ND_VAR: {
        Obj *var = node->var;
        if (var->is_global) {
            /* Ensure the global is registered in the IR module (e.g. string
             * literals created by the parser that aren't top-level decls). */
            ensure_global_registered(ctx, var);

            /* Reference global as a symbol address. We create a const_int
             * with IR_TYPE_PTR that represents the global address symbolically.
             * The call_target field stores the symbol name. */
            IRType p = ptr_type();
            IRInst *inst = ir_inst_new(ctx->a, ctx->cur_block, IR_CONST_INT, p);
            inst->ival = 0;
            inst->call_target = var->name; /* reuse field for symbol name */
            ir_block_append(ctx->cur_block, inst);
            return inst;
        }
        /* Local variable: look up its alloca via Obj pointer */
        IRInst *alloca_inst = (IRInst *)var->ir_alloca;
        if (!alloca_inst) {
            /* Fallback to hash table for backward compatibility */
            alloca_inst = hashtab_get(&ctx->locals, var->name);
        }
        if (!alloca_inst) {
            fprintf(stderr, "irgen: undefined variable '%s'\n", var->name);
            return ir_build_const_int(ctx->a, ctx->cur_block, ptr_type(), 0);
        }
        /* VLA: the alloca stores a pointer to the VLA memory, load it */
        if (var->ty && var->ty->is_vla) {
            return ir_build_load(ctx->a, ctx->cur_block, ptr_type(), alloca_inst);
        }
        return alloca_inst;
    }

    case ND_DEREF:
        /* *ptr => the l-value is the result of evaluating ptr */
        return gen_expr(ctx, node->lhs);

    case ND_MEMBER: {
        /* struct.member or struct->member */
        IRInst *base;
        if (node->lhs->ty && node->lhs->ty->kind == TY_PTR) {
            /* Arrow operator: lhs is a pointer, evaluate it as rvalue */
            base = gen_expr(ctx, node->lhs);
        } else if (node->lhs->kind == ND_CALL) {
            /* Dot operator on function return: materialize rvalue into temp */
            IRInst *call_result = gen_expr(ctx, node->lhs);
            IRType agg_ty = ctype_to_ir(node->lhs->ty);
            int sz = agg_ty.size > 0 ? agg_ty.size : 8;
            int align = agg_ty.align > 0 ? agg_ty.align : 8;
            IRInst *tmp = entry_alloca(ctx, sz, align);
            /* Copy call result into the temporary */
            ir_build_memcpy(ctx->a, ctx->cur_block, tmp, call_result, sz);
            base = tmp;
        } else {
            /* Dot operator: lhs is a struct, get its address */
            base = gen_lvalue(ctx, node->lhs);
        }
        Member *mem = node->member;
        if (mem && mem->offset != 0) {
            return ir_build_gep(ctx->a, ctx->cur_block, base, mem->offset);
        }
        return base;
    }

    case ND_INIT_LIST: {
        /* Compound literal used as lvalue (e.g., &(struct Point){1, 2}).
         * Allocate temp, fill it, return its address. */
        if (node->ty) {
            int sz = node->ty->size > 0 ? node->ty->size : 4;
            int al = node->ty->align > 0 ? node->ty->align : 4;
            IRInst *tmp = entry_alloca(ctx, sz, al);
            gen_init_list(ctx, tmp, node, node->ty);
            return tmp;
        }
        return ir_build_const_int(ctx->a, ctx->cur_block, ptr_type(), 0);
    }

    case ND_COMMA:
        gen_expr(ctx, node->lhs);
        return gen_lvalue(ctx, node->rhs);

    default:
        fprintf(stderr, "irgen: not an lvalue (kind=%d)\n", node->kind);
        return ir_build_const_int(ctx->a, ctx->cur_block, ptr_type(), 0);
    }
}

/* ================================================================
 * Expression generation
 * ================================================================ */

static IRInst *gen_expr(IRGenCtx *ctx, Node *node)
{
    if (!node) {
        return ir_build_const_int(ctx->a, ctx->cur_block, i32_type(), 0);
    }

    switch (node->kind) {

    /* ------- Literals ------- */

    case ND_NUM: {
        IRType ty = ctype_to_ir(node->ty);
        return ir_build_const_int(ctx->a, ctx->cur_block, ty, (int64_t)node->ival);
    }

    case ND_FNUM: {
        IRType ty = ctype_to_ir(node->ty);
        return ir_build_const_float(ctx->a, ctx->cur_block, ty, node->fval);
    }

    case ND_STRING: {
        /* Create a global string constant */
        char name_buf[64];
        snprintf(name_buf, sizeof(name_buf), ".str.%d", ctx->str_count++);
        const char *gname = arena_strdup(ctx->a, name_buf);

        IRGlobal *g = arena_new(ctx->a, IRGlobal);
        g->name = gname;
        g->ty = make_ir_type(IR_TYPE_I8);
        g->alloc_size = node->str_len + 1;
        g->alloc_align = 1;
        g->is_string = true;
        g->string_val = node->str_val;
        g->string_len = node->str_len;
        g->next = ctx->mod->globals;
        ctx->mod->globals = g;

        /* Return a pointer to the global string */
        IRType p = ptr_type();
        IRInst *inst = ir_inst_new(ctx->a, ctx->cur_block, IR_CONST_INT, p);
        inst->ival = 0;
        inst->call_target = gname;
        ir_block_append(ctx->cur_block, inst);
        return inst;
    }

    /* ------- Variable reference ------- */

    case ND_VAR: {
        /* If the variable is an array or function type, return the address */
        if (node->ty && (node->ty->kind == TY_ARRAY || node->ty->kind == TY_FUNC)) {
            return gen_lvalue(ctx, node);
        }

        /* If it's an enum constant, return the value directly */
        if (node->var && node->var->is_enum_const) {
            IRType ty = ctype_to_ir(node->ty);
            return ir_build_const_int(ctx->a, ctx->cur_block, ty,
                                      (int64_t)node->var->enum_val);
        }

        IRInst *addr = gen_lvalue(ctx, node);
        IRType ty = ctype_to_ir(node->ty);

        /* For aggregate types (struct/union), return the address */
        if (ty.kind == IR_TYPE_AGGREGATE) {
            return addr;
        }

        return ir_build_load(ctx->a, ctx->cur_block, ty, addr);
    }

    /* ------- Member access ------- */

    case ND_MEMBER: {
        IRType ty = ctype_to_ir(node->ty);
        /* For aggregates, arrays, and functions, return the address (no load) */
        if (ty.kind == IR_TYPE_AGGREGATE ||
            (node->ty && (node->ty->kind == TY_ARRAY || node->ty->kind == TY_FUNC))) {
            return gen_lvalue(ctx, node);
        }
        Member *mem = node->member;
        if (mem && mem->bit_width > 0) {
            /* Bitfield load: extract the field from the storage unit.
             *
             * For unsigned bitfields: LSHR by bit_offset, then AND with mask.
             * For signed bitfields: shift left to put sign bit at MSB, then
             *   arithmetic shift right to sign-extend.
             *   Left shift amount:  storage_bits - bit_width - bit_offset
             *   Right shift amount: storage_bits - bit_width
             */
            IRInst *addr = gen_lvalue(ctx, node);
            IRInst *unit = ir_build_load(ctx->a, ctx->cur_block, ty, addr);
            int storage_bits = ty.size * 8;
            bool is_signed_bf = mem->ty && !mem->ty->is_unsigned;
            if (is_signed_bf) {
                /* Signed: shift-left then ASHR to sign-extend */
                int lshift_amt = storage_bits - mem->bit_width - mem->bit_offset;
                int rshift_amt = storage_bits - mem->bit_width;
                if (lshift_amt > 0) {
                    IRInst *ls = ir_build_const_int(ctx->a, ctx->cur_block, ty, lshift_amt);
                    unit = ir_build_binop(ctx->a, ctx->cur_block, IR_SHL, unit, ls);
                }
                IRInst *rs = ir_build_const_int(ctx->a, ctx->cur_block, ty, rshift_amt);
                unit = ir_build_binop(ctx->a, ctx->cur_block, IR_ASHR, unit, rs);
            } else {
                /* Unsigned: LSHR then mask */
                if (mem->bit_offset > 0) {
                    IRInst *shift = ir_build_const_int(ctx->a, ctx->cur_block, ty, mem->bit_offset);
                    unit = ir_build_binop(ctx->a, ctx->cur_block, IR_LSHR, unit, shift);
                }
                unsigned long long mask = ((1ULL << mem->bit_width) - 1);
                IRInst *mask_val = ir_build_const_int(ctx->a, ctx->cur_block, ty, (long long)mask);
                unit = ir_build_binop(ctx->a, ctx->cur_block, IR_AND, unit, mask_val);
            }
            return unit;
        }
        IRInst *addr = gen_lvalue(ctx, node);
        return ir_build_load(ctx->a, ctx->cur_block, ty, addr);
    }

    /* ------- Unary operators ------- */

    case ND_DEREF: {
        IRInst *ptr = gen_expr(ctx, node->lhs);
        IRType ty = ctype_to_ir(node->ty);
        if (ty.kind == IR_TYPE_AGGREGATE)
            return ptr;
        /* Arrays and functions decay to pointers -- no load needed */
        if (node->ty && (node->ty->kind == TY_ARRAY || node->ty->kind == TY_FUNC))
            return ptr;
        return ir_build_load(ctx->a, ctx->cur_block, ty, ptr);
    }

    case ND_ADDR:
        return gen_lvalue(ctx, node->lhs);

    case ND_NEG: {
        IRInst *val = gen_expr(ctx, node->lhs);
        if (is_float_type(node->lhs->ty))
            return ir_build_unop(ctx->a, ctx->cur_block, IR_FNEG, val);
        /* Integer promotion: promote char/short to int before negation */
        if (node->lhs->ty && node->ty) {
            val = gen_cast(ctx, val, node->lhs->ty, node->ty);
        }
        return ir_build_unop(ctx->a, ctx->cur_block, IR_NEG, val);
    }

    case ND_NOT: {
        /* Logical NOT: val == 0 */
        IRInst *val = gen_expr(ctx, node->lhs);
        IRInst *b = to_bool(ctx, val, node->lhs->ty);
        /* Invert: xor with 1 */
        IRType i1 = make_ir_type(IR_TYPE_I1);
        IRInst *one = ir_build_const_int(ctx->a, ctx->cur_block, i1, 1);
        IRInst *result = ir_build_binop(ctx->a, ctx->cur_block, IR_XOR, b, one);
        /* Zero-extend to the result type */
        IRType rty = ctype_to_ir(node->ty);
        if (!ir_type_eq(result->ty, rty))
            result = ir_build_cast(ctx->a, ctx->cur_block, IR_ZEXT, rty, result);
        return result;
    }

    case ND_BITNOT: {
        IRInst *val = gen_expr(ctx, node->lhs);
        /* Integer promotion: promote char/short to int before bitwise NOT */
        if (node->lhs->ty && node->ty) {
            val = gen_cast(ctx, val, node->lhs->ty, node->ty);
        }
        return ir_build_unop(ctx->a, ctx->cur_block, IR_NOT, val);
    }

    /* ------- Binary arithmetic ------- */

    case ND_ADD: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);

        /* Pointer arithmetic: ptr + int */
        if (is_pointer_type(node->lhs->ty)) {
            int elem_sz = pointee_size(node->lhs->ty);
            if (elem_sz > 1) {
                IRInst *scale = ir_build_const_int(ctx->a, ctx->cur_block, rhs->ty, elem_sz);
                rhs = ir_build_binop(ctx->a, ctx->cur_block, IR_MUL, rhs, scale);
            }
            if (rhs->ty.kind != IR_TYPE_I64) {
                rhs = ir_build_cast(ctx->a, ctx->cur_block,
                                    is_unsigned_type(node->rhs->ty) ? IR_ZEXT : IR_SEXT,
                                    i64_type(), rhs);
            }
            return ir_build_binop(ctx->a, ctx->cur_block, IR_ADD, lhs, rhs);
        }
        /* int + ptr */
        if (is_pointer_type(node->rhs->ty)) {
            int elem_sz = pointee_size(node->rhs->ty);
            if (elem_sz > 1) {
                IRInst *scale = ir_build_const_int(ctx->a, ctx->cur_block, lhs->ty, elem_sz);
                lhs = ir_build_binop(ctx->a, ctx->cur_block, IR_MUL, lhs, scale);
            }
            if (lhs->ty.kind != IR_TYPE_I64) {
                lhs = ir_build_cast(ctx->a, ctx->cur_block,
                                    is_unsigned_type(node->lhs->ty) ? IR_ZEXT : IR_SEXT,
                                    i64_type(), lhs);
            }
            return ir_build_binop(ctx->a, ctx->cur_block, IR_ADD, rhs, lhs);
        }

        if (is_float_type(node->ty)) {
            /* Promote integer operands to float */
            IRType fty = ctype_to_ir(node->ty);
            if (!is_float_type(node->lhs->ty))
                lhs = ir_build_cast(ctx->a, ctx->cur_block,
                    is_unsigned_type(node->lhs->ty) ? IR_UITOFP : IR_SITOFP, fty, lhs);
            if (!is_float_type(node->rhs->ty))
                rhs = ir_build_cast(ctx->a, ctx->cur_block,
                    is_unsigned_type(node->rhs->ty) ? IR_UITOFP : IR_SITOFP, fty, rhs);
            return ir_build_binop(ctx->a, ctx->cur_block, IR_FADD, lhs, rhs);
        }
        /* Integer promotion: promote char/short operands to result type */
        lhs = gen_cast(ctx, lhs, node->lhs->ty, node->ty);
        rhs = gen_cast(ctx, rhs, node->rhs->ty, node->ty);
        return ir_build_binop(ctx->a, ctx->cur_block, IR_ADD, lhs, rhs);
    }

    case ND_SUB: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);

        /* ptr - ptr */
        if (is_pointer_type(node->lhs->ty) && is_pointer_type(node->rhs->ty)) {
            IRInst *diff = ir_build_binop(ctx->a, ctx->cur_block, IR_SUB, lhs, rhs);
            int elem_sz = pointee_size(node->lhs->ty);
            if (elem_sz > 1) {
                IRInst *scale = ir_build_const_int(ctx->a, ctx->cur_block, diff->ty, elem_sz);
                diff = ir_build_binop(ctx->a, ctx->cur_block, IR_SDIV, diff, scale);
            }
            return diff;
        }

        /* ptr - int */
        if (is_pointer_type(node->lhs->ty)) {
            int elem_sz = pointee_size(node->lhs->ty);
            if (elem_sz > 1) {
                IRInst *scale = ir_build_const_int(ctx->a, ctx->cur_block, rhs->ty, elem_sz);
                rhs = ir_build_binop(ctx->a, ctx->cur_block, IR_MUL, rhs, scale);
            }
            if (rhs->ty.kind != IR_TYPE_I64) {
                rhs = ir_build_cast(ctx->a, ctx->cur_block,
                                    is_unsigned_type(node->rhs->ty) ? IR_ZEXT : IR_SEXT,
                                    i64_type(), rhs);
            }
            return ir_build_binop(ctx->a, ctx->cur_block, IR_SUB, lhs, rhs);
        }

        if (is_float_type(node->ty)) {
            IRType fty = ctype_to_ir(node->ty);
            if (!is_float_type(node->lhs->ty))
                lhs = ir_build_cast(ctx->a, ctx->cur_block,
                    is_unsigned_type(node->lhs->ty) ? IR_UITOFP : IR_SITOFP, fty, lhs);
            if (!is_float_type(node->rhs->ty))
                rhs = ir_build_cast(ctx->a, ctx->cur_block,
                    is_unsigned_type(node->rhs->ty) ? IR_UITOFP : IR_SITOFP, fty, rhs);
            return ir_build_binop(ctx->a, ctx->cur_block, IR_FSUB, lhs, rhs);
        }
        /* Integer promotion: promote char/short operands to result type */
        lhs = gen_cast(ctx, lhs, node->lhs->ty, node->ty);
        rhs = gen_cast(ctx, rhs, node->rhs->ty, node->ty);
        return ir_build_binop(ctx->a, ctx->cur_block, IR_SUB, lhs, rhs);
    }

    case ND_MUL: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        if (is_float_type(node->ty)) {
            IRType fty = ctype_to_ir(node->ty);
            if (!is_float_type(node->lhs->ty))
                lhs = ir_build_cast(ctx->a, ctx->cur_block,
                    is_unsigned_type(node->lhs->ty) ? IR_UITOFP : IR_SITOFP, fty, lhs);
            if (!is_float_type(node->rhs->ty))
                rhs = ir_build_cast(ctx->a, ctx->cur_block,
                    is_unsigned_type(node->rhs->ty) ? IR_UITOFP : IR_SITOFP, fty, rhs);
            return ir_build_binop(ctx->a, ctx->cur_block, IR_FMUL, lhs, rhs);
        }
        /* Integer promotion: promote char/short operands to result type */
        lhs = gen_cast(ctx, lhs, node->lhs->ty, node->ty);
        rhs = gen_cast(ctx, rhs, node->rhs->ty, node->ty);
        return ir_build_binop(ctx->a, ctx->cur_block, IR_MUL, lhs, rhs);
    }

    case ND_DIV: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        if (is_float_type(node->ty)) {
            IRType fty = ctype_to_ir(node->ty);
            if (!is_float_type(node->lhs->ty))
                lhs = ir_build_cast(ctx->a, ctx->cur_block,
                    is_unsigned_type(node->lhs->ty) ? IR_UITOFP : IR_SITOFP, fty, lhs);
            if (!is_float_type(node->rhs->ty))
                rhs = ir_build_cast(ctx->a, ctx->cur_block,
                    is_unsigned_type(node->rhs->ty) ? IR_UITOFP : IR_SITOFP, fty, rhs);
            return ir_build_binop(ctx->a, ctx->cur_block, IR_FDIV, lhs, rhs);
        }
        /* Integer promotion: promote char/short operands to result type */
        lhs = gen_cast(ctx, lhs, node->lhs->ty, node->ty);
        rhs = gen_cast(ctx, rhs, node->rhs->ty, node->ty);
        if (is_unsigned_type(node->ty))
            return ir_build_binop(ctx->a, ctx->cur_block, IR_UDIV, lhs, rhs);
        return ir_build_binop(ctx->a, ctx->cur_block, IR_SDIV, lhs, rhs);
    }

    case ND_MOD: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        /* Integer promotion: promote char/short operands to result type */
        lhs = gen_cast(ctx, lhs, node->lhs->ty, node->ty);
        rhs = gen_cast(ctx, rhs, node->rhs->ty, node->ty);
        if (is_unsigned_type(node->ty))
            return ir_build_binop(ctx->a, ctx->cur_block, IR_UREM, lhs, rhs);
        return ir_build_binop(ctx->a, ctx->cur_block, IR_SREM, lhs, rhs);
    }

    /* ------- Bitwise operators ------- */

    case ND_BITAND: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        /* Integer promotion: promote char/short operands to result type */
        lhs = gen_cast(ctx, lhs, node->lhs->ty, node->ty);
        rhs = gen_cast(ctx, rhs, node->rhs->ty, node->ty);
        return ir_build_binop(ctx->a, ctx->cur_block, IR_AND, lhs, rhs);
    }

    case ND_BITOR: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        /* Integer promotion: promote char/short operands to result type */
        lhs = gen_cast(ctx, lhs, node->lhs->ty, node->ty);
        rhs = gen_cast(ctx, rhs, node->rhs->ty, node->ty);
        return ir_build_binop(ctx->a, ctx->cur_block, IR_OR, lhs, rhs);
    }

    case ND_BITXOR: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        /* Integer promotion: promote char/short operands to result type */
        lhs = gen_cast(ctx, lhs, node->lhs->ty, node->ty);
        rhs = gen_cast(ctx, rhs, node->rhs->ty, node->ty);
        return ir_build_binop(ctx->a, ctx->cur_block, IR_XOR, lhs, rhs);
    }

    case ND_SHL: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        /* Integer promotion: promote char/short operands to result type */
        lhs = gen_cast(ctx, lhs, node->lhs->ty, node->ty);
        rhs = gen_cast(ctx, rhs, node->rhs->ty, node->ty);
        return ir_build_binop(ctx->a, ctx->cur_block, IR_SHL, lhs, rhs);
    }

    case ND_SHR: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        /* Integer promotion: promote char/short operands to result type */
        lhs = gen_cast(ctx, lhs, node->lhs->ty, node->ty);
        rhs = gen_cast(ctx, rhs, node->rhs->ty, node->ty);
        if (is_unsigned_type(node->ty))
            return ir_build_binop(ctx->a, ctx->cur_block, IR_LSHR, lhs, rhs);
        return ir_build_binop(ctx->a, ctx->cur_block, IR_ASHR, lhs, rhs);
    }

    /* ------- Comparisons ------- */

    case ND_EQ: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        IROpcode op;
        if (is_float_type(node->lhs->ty)) {
            op = IR_FCMP_OEQ;
        } else {
            /* Integer promotion: promote char/short operands to common type */
            Type *common = node->lhs->ty;
            if (node->rhs->ty->size > common->size)
                common = node->rhs->ty;
            else if (node->rhs->ty->size == common->size && node->rhs->ty->is_unsigned)
                common = node->rhs->ty;
            if (common->size < ty_int->size) common = ty_int;
            lhs = gen_cast(ctx, lhs, node->lhs->ty, common);
            rhs = gen_cast(ctx, rhs, node->rhs->ty, common);
            op = IR_ICMP_EQ;
        }
        IRInst *cmp = ir_build_binop(ctx->a, ctx->cur_block, op, lhs, rhs);
        IRType rty = ctype_to_ir(node->ty);
        if (!ir_type_eq(cmp->ty, rty))
            cmp = ir_build_cast(ctx->a, ctx->cur_block, IR_ZEXT, rty, cmp);
        return cmp;
    }

    case ND_NE: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        IROpcode op;
        if (is_float_type(node->lhs->ty)) {
            op = IR_FCMP_ONE;
        } else {
            /* Integer promotion: promote char/short operands to common type */
            Type *common = node->lhs->ty;
            if (node->rhs->ty->size > common->size)
                common = node->rhs->ty;
            else if (node->rhs->ty->size == common->size && node->rhs->ty->is_unsigned)
                common = node->rhs->ty;
            if (common->size < ty_int->size) common = ty_int;
            lhs = gen_cast(ctx, lhs, node->lhs->ty, common);
            rhs = gen_cast(ctx, rhs, node->rhs->ty, common);
            op = IR_ICMP_NE;
        }
        IRInst *cmp = ir_build_binop(ctx->a, ctx->cur_block, op, lhs, rhs);
        IRType rty = ctype_to_ir(node->ty);
        if (!ir_type_eq(cmp->ty, rty))
            cmp = ir_build_cast(ctx->a, ctx->cur_block, IR_ZEXT, rty, cmp);
        return cmp;
    }

    case ND_LT: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        IROpcode op;
        if (is_float_type(node->lhs->ty)) {
            op = IR_FCMP_OLT;
        } else {
            /* Integer promotion: promote char/short operands to common type */
            Type *common = node->lhs->ty;
            if (node->rhs->ty->size > common->size)
                common = node->rhs->ty;
            else if (node->rhs->ty->size == common->size && node->rhs->ty->is_unsigned)
                common = node->rhs->ty;
            if (common->size < ty_int->size) common = ty_int;
            lhs = gen_cast(ctx, lhs, node->lhs->ty, common);
            rhs = gen_cast(ctx, rhs, node->rhs->ty, common);
            if (is_unsigned_type(common))
                op = IR_ICMP_ULT;
            else
                op = IR_ICMP_SLT;
        }
        IRInst *cmp = ir_build_binop(ctx->a, ctx->cur_block, op, lhs, rhs);
        IRType rty = ctype_to_ir(node->ty);
        if (!ir_type_eq(cmp->ty, rty))
            cmp = ir_build_cast(ctx->a, ctx->cur_block, IR_ZEXT, rty, cmp);
        return cmp;
    }

    case ND_LE: {
        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        IROpcode op;
        if (is_float_type(node->lhs->ty)) {
            op = IR_FCMP_OLE;
        } else {
            /* Integer promotion: promote char/short operands to common type */
            Type *common = node->lhs->ty;
            if (node->rhs->ty->size > common->size)
                common = node->rhs->ty;
            else if (node->rhs->ty->size == common->size && node->rhs->ty->is_unsigned)
                common = node->rhs->ty;
            if (common->size < ty_int->size) common = ty_int;
            lhs = gen_cast(ctx, lhs, node->lhs->ty, common);
            rhs = gen_cast(ctx, rhs, node->rhs->ty, common);
            if (is_unsigned_type(common))
                op = IR_ICMP_ULE;
            else
                op = IR_ICMP_SLE;
        }
        IRInst *cmp = ir_build_binop(ctx->a, ctx->cur_block, op, lhs, rhs);
        IRType rty = ctype_to_ir(node->ty);
        if (!ir_type_eq(cmp->ty, rty))
            cmp = ir_build_cast(ctx->a, ctx->cur_block, IR_ZEXT, rty, cmp);
        return cmp;
    }

    /* ------- Logical operators (short-circuit) ------- */

    case ND_LOGAND: {
        IRBlock *rhs_bb = new_block(ctx, "and.rhs");
        IRBlock *end_bb = new_block(ctx, "and.end");

        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *lhs_bool = to_bool(ctx, lhs, node->lhs->ty);
        IRBlock *lhs_bb = ctx->cur_block;
        ir_build_condbr(ctx->a, ctx->cur_block, lhs_bool, rhs_bb, end_bb);

        set_block(ctx, rhs_bb);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        IRInst *rhs_bool = to_bool(ctx, rhs, node->rhs->ty);
        IRBlock *rhs_end_bb = ctx->cur_block;
        emit_br_if_needed(ctx, end_bb);

        set_block(ctx, end_bb);
        IRType rty = ctype_to_ir(node->ty);
        IRType i1 = make_ir_type(IR_TYPE_I1);
        IRInst *phi = ir_build_phi(ctx->a, ctx->cur_block, i1);
        IRInst *false_val = ir_build_const_int(ctx->a, ctx->cur_block, i1, 0);
        ir_phi_add(ctx->a, phi, false_val, lhs_bb);
        ir_phi_add(ctx->a, phi, rhs_bool, rhs_end_bb);

        IRInst *result = phi;
        if (!ir_type_eq(i1, rty))
            result = ir_build_cast(ctx->a, ctx->cur_block, IR_ZEXT, rty, result);
        return result;
    }

    case ND_LOGOR: {
        IRBlock *rhs_bb = new_block(ctx, "or.rhs");
        IRBlock *end_bb = new_block(ctx, "or.end");

        IRInst *lhs = gen_expr(ctx, node->lhs);
        IRInst *lhs_bool = to_bool(ctx, lhs, node->lhs->ty);
        IRBlock *lhs_bb = ctx->cur_block;
        ir_build_condbr(ctx->a, ctx->cur_block, lhs_bool, end_bb, rhs_bb);

        set_block(ctx, rhs_bb);
        IRInst *rhs = gen_expr(ctx, node->rhs);
        IRInst *rhs_bool = to_bool(ctx, rhs, node->rhs->ty);
        IRBlock *rhs_end_bb = ctx->cur_block;
        emit_br_if_needed(ctx, end_bb);

        set_block(ctx, end_bb);
        IRType rty = ctype_to_ir(node->ty);
        IRType i1 = make_ir_type(IR_TYPE_I1);
        IRInst *phi = ir_build_phi(ctx->a, ctx->cur_block, i1);
        IRInst *true_val = ir_build_const_int(ctx->a, ctx->cur_block, i1, 1);
        ir_phi_add(ctx->a, phi, true_val, lhs_bb);
        ir_phi_add(ctx->a, phi, rhs_bool, rhs_end_bb);

        IRInst *result = phi;
        if (!ir_type_eq(i1, rty))
            result = ir_build_cast(ctx->a, ctx->cur_block, IR_ZEXT, rty, result);
        return result;
    }

    /* ------- Assignment ------- */

    case ND_ASSIGN: {
        IRInst *rhs = gen_expr(ctx, node->rhs);
        IRInst *addr = gen_lvalue(ctx, node->lhs);

        /* Cast rhs to the lvalue type if needed */
        if (node->lhs->ty && node->rhs->ty) {
            rhs = gen_cast(ctx, rhs, node->rhs->ty, node->lhs->ty);
        }

        /* For aggregate types, emit memcpy */
        IRType lty = ctype_to_ir(node->lhs->ty);
        if (lty.kind == IR_TYPE_AGGREGATE) {
            ir_build_memcpy(ctx->a, ctx->cur_block, addr, rhs, lty.size);
            return addr;
        }

        /* Bitfield store: read-modify-write */
        if (node->lhs->kind == ND_MEMBER && node->lhs->member &&
            node->lhs->member->bit_width > 0) {
            Member *mem = node->lhs->member;
            unsigned long long mask = ((1ULL << mem->bit_width) - 1);
            /* Mask the value to the bitfield width */
            IRInst *mask_val = ir_build_const_int(ctx->a, ctx->cur_block, lty, (long long)mask);
            IRInst *masked_rhs = ir_build_binop(ctx->a, ctx->cur_block, IR_AND, rhs, mask_val);
            /* Shift to position */
            IRInst *positioned;
            if (mem->bit_offset > 0) {
                IRInst *shift = ir_build_const_int(ctx->a, ctx->cur_block, lty, mem->bit_offset);
                positioned = ir_build_binop(ctx->a, ctx->cur_block, IR_SHL, masked_rhs, shift);
            } else {
                positioned = masked_rhs;
            }
            /* Load old value */
            IRInst *old_val = ir_build_load(ctx->a, ctx->cur_block, lty, addr);
            /* Clear the target bits */
            unsigned long long clear_mask = ~(mask << mem->bit_offset);
            IRInst *clear_val = ir_build_const_int(ctx->a, ctx->cur_block, lty, (long long)clear_mask);
            IRInst *cleared = ir_build_binop(ctx->a, ctx->cur_block, IR_AND, old_val, clear_val);
            /* OR in the new bits */
            IRInst *result = ir_build_binop(ctx->a, ctx->cur_block, IR_OR, cleared, positioned);
            ir_build_store(ctx->a, ctx->cur_block, result, addr);
            return rhs;
        }

        ir_build_store(ctx->a, ctx->cur_block, rhs, addr);
        return rhs;
    }

    /* ------- Compound assignment ------- */

    case ND_COMPOUND_ASSIGN: {
        IRInst *addr = gen_lvalue(ctx, node->lhs);
        IRType lty = ctype_to_ir(node->lhs->ty);
        IRInst *old_val = ir_build_load(ctx->a, ctx->cur_block, lty, addr);
        IRInst *rhs = gen_expr(ctx, node->rhs);

        /* Determine the binary op */
        IROpcode binop;
        switch (node->compound_op) {
        case ND_ADD:    binop = is_float_type(node->ty) ? IR_FADD : IR_ADD; break;
        case ND_SUB:    binop = is_float_type(node->ty) ? IR_FSUB : IR_SUB; break;
        case ND_MUL:    binop = is_float_type(node->ty) ? IR_FMUL : IR_MUL; break;
        case ND_DIV:
            if (is_float_type(node->ty)) binop = IR_FDIV;
            else if (is_unsigned_type(node->ty)) binop = IR_UDIV;
            else binop = IR_SDIV;
            break;
        case ND_MOD:    binop = is_unsigned_type(node->ty) ? IR_UREM : IR_SREM; break;
        case ND_BITAND: binop = IR_AND; break;
        case ND_BITOR:  binop = IR_OR; break;
        case ND_BITXOR: binop = IR_XOR; break;
        case ND_SHL:    binop = IR_SHL; break;
        case ND_SHR:    binop = is_unsigned_type(node->lhs->ty) ? IR_LSHR : IR_ASHR; break;
        default:        binop = IR_ADD; break;
        }

        IRInst *new_val = ir_build_binop(ctx->a, ctx->cur_block, binop, old_val, rhs);
        /* Truncate result to match lvalue type for correct store width.
         * ir_build_binop promotes i8/i16 to i32, but we must store back
         * at the original width (e.g., char *= int -> i32 result, store as i8) */
        if (new_val->ty.size > lty.size && lty.kind != IR_TYPE_PTR) {
            new_val = ir_build_cast(ctx->a, ctx->cur_block, IR_TRUNC, lty, new_val);
        }
        ir_build_store(ctx->a, ctx->cur_block, new_val, addr);
        return new_val;
    }

    /* ------- Increment/Decrement ------- */

    case ND_PRE_INC:
    case ND_PRE_DEC: {
        IRInst *addr = gen_lvalue(ctx, node->lhs);
        IRType ty = ctype_to_ir(node->lhs->ty);
        IRInst *val = ir_build_load(ctx->a, ctx->cur_block, ty, addr);

        int64_t delta = (node->kind == ND_PRE_INC) ? 1 : -1;
        IRInst *result;

        if (is_pointer_type(node->lhs->ty)) {
            int elem_sz = pointee_size(node->lhs->ty);
            IRInst *inc = ir_build_const_int(ctx->a, ctx->cur_block, i64_type(), delta * elem_sz);
            result = ir_build_binop(ctx->a, ctx->cur_block, IR_ADD, val, inc);
        } else if (is_float_type(node->lhs->ty)) {
            IRInst *inc = ir_build_const_float(ctx->a, ctx->cur_block, ty, (double)delta);
            result = ir_build_binop(ctx->a, ctx->cur_block, IR_FADD, val, inc);
        } else {
            IRInst *inc = ir_build_const_int(ctx->a, ctx->cur_block, ty, delta);
            result = ir_build_binop(ctx->a, ctx->cur_block, IR_ADD, val, inc);
        }

        ir_build_store(ctx->a, ctx->cur_block, result, addr);
        return result;
    }

    case ND_POST_INC:
    case ND_POST_DEC: {
        IRInst *addr = gen_lvalue(ctx, node->lhs);
        IRType ty = ctype_to_ir(node->lhs->ty);
        IRInst *old_val = ir_build_load(ctx->a, ctx->cur_block, ty, addr);

        int64_t delta = (node->kind == ND_POST_INC) ? 1 : -1;
        IRInst *new_val;

        if (is_pointer_type(node->lhs->ty)) {
            int elem_sz = pointee_size(node->lhs->ty);
            IRInst *inc = ir_build_const_int(ctx->a, ctx->cur_block, i64_type(), delta * elem_sz);
            new_val = ir_build_binop(ctx->a, ctx->cur_block, IR_ADD, old_val, inc);
        } else if (is_float_type(node->lhs->ty)) {
            IRInst *inc = ir_build_const_float(ctx->a, ctx->cur_block, ty, (double)delta);
            new_val = ir_build_binop(ctx->a, ctx->cur_block, IR_FADD, old_val, inc);
        } else {
            IRInst *inc = ir_build_const_int(ctx->a, ctx->cur_block, ty, delta);
            new_val = ir_build_binop(ctx->a, ctx->cur_block, IR_ADD, old_val, inc);
        }

        ir_build_store(ctx->a, ctx->cur_block, new_val, addr);
        return old_val;
    }

    /* ------- Ternary ------- */

    case ND_TERNARY: {
        IRBlock *then_bb = new_block(ctx, "tern.then");
        IRBlock *else_bb = new_block(ctx, "tern.else");
        IRBlock *end_bb  = new_block(ctx, "tern.end");

        IRInst *cond = gen_expr(ctx, node->cond);
        IRInst *cond_bool = to_bool(ctx, cond, node->cond->ty);
        ir_build_condbr(ctx->a, ctx->cur_block, cond_bool, then_bb, else_bb);

        set_block(ctx, then_bb);
        IRInst *then_val = gen_expr(ctx, node->then);
        /* Cast in the then-block before branching */
        if (node->then->ty && node->ty)
            then_val = gen_cast(ctx, then_val, node->then->ty, node->ty);
        IRBlock *then_end = ctx->cur_block;
        emit_br_if_needed(ctx, end_bb);

        set_block(ctx, else_bb);
        IRInst *else_val = gen_expr(ctx, node->els);
        /* Cast in the else-block before branching */
        if (node->els->ty && node->ty)
            else_val = gen_cast(ctx, else_val, node->els->ty, node->ty);
        IRBlock *else_end = ctx->cur_block;
        emit_br_if_needed(ctx, end_bb);

        set_block(ctx, end_bb);
        IRType rty = ctype_to_ir(node->ty);
        if (rty.kind == IR_TYPE_VOID) {
            return ir_build_const_int(ctx->a, ctx->cur_block, i32_type(), 0);
        }
        IRInst *phi = ir_build_phi(ctx->a, ctx->cur_block, rty);
        ir_phi_add(ctx->a, phi, then_val, then_end);
        ir_phi_add(ctx->a, phi, else_val, else_end);
        return phi;
    }

    /* ------- Comma ------- */

    case ND_COMMA:
        gen_expr(ctx, node->lhs);
        return gen_expr(ctx, node->rhs);

    /* ------- Cast ------- */

    case ND_CAST: {
        IRInst *val = gen_expr(ctx, node->lhs);
        if (!node->ty || !node->lhs->ty)
            return val;
        if (node->ty->kind == TY_VOID)
            return val;
        return gen_cast(ctx, val, node->lhs->ty, node->ty);
    }

    /* ------- sizeof / alignof ------- */

    case ND_SIZEOF: {
        IRType rty = ctype_to_ir(node->ty);
        /* VLA: sizeof must be computed at runtime as nelems * elem_size */
        if (node->lhs && node->lhs->ty && node->lhs->ty->is_vla &&
            node->lhs->ty->vla_size) {
            Node *vla_size_expr = (Node *)node->lhs->ty->vla_size;
            IRInst *nelems = gen_expr(ctx, vla_size_expr);
            int elem_size = node->lhs->ty->base ? node->lhs->ty->base->size : 1;
            IRType i64ty = make_ir_type(IR_TYPE_I64);
            if (nelems->ty.kind != IR_TYPE_I64 && nelems->ty.kind != IR_TYPE_PTR)
                nelems = ir_build_cast(ctx->a, ctx->cur_block, IR_SEXT, i64ty, nelems);
            IRInst *esz = ir_build_const_int(ctx->a, ctx->cur_block, i64ty, elem_size);
            IRInst *total = ir_build_binop(ctx->a, ctx->cur_block, IR_MUL, nelems, esz);
            /* Cast to result type (usually u64 / size_t) */
            if (rty.kind != IR_TYPE_I64 && rty.kind != IR_TYPE_PTR)
                return ir_build_cast(ctx->a, ctx->cur_block, IR_ZEXT, rty, total);
            return total;
        }
        long long sz = 0;
        if (node->lhs && node->lhs->ty)
            sz = node->lhs->ty->size;
        else
            sz = node->ival;
        return ir_build_const_int(ctx->a, ctx->cur_block, rty, sz);
    }

    case ND_ALIGNOF: {
        IRType rty = ctype_to_ir(node->ty);
        long long align = 0;
        if (node->lhs && node->lhs->ty)
            align = node->lhs->ty->align;
        else
            align = node->ival;
        return ir_build_const_int(ctx->a, ctx->cur_block, rty, align);
    }

    /* ------- Function call ------- */

    case ND_CALL: {
        /* Count arguments */
        int nargs = 0;
        for (Node *arg = node->args; arg; arg = arg->next)
            nargs++;

        IRInst **args = NULL;
        if (nargs > 0) {
            args = arena_alloc(ctx->a, sizeof(IRInst *) * nargs);
            int i = 0;
            for (Node *arg = node->args; arg; arg = arg->next, i++) {
                args[i] = gen_expr(ctx, arg);
                /* Default argument promotions: promote small integer types
                 * to int (i32).  Required for variadic args and good practice
                 * for all calls since ARM64 ABI uses 64-bit registers. */
                IRType aty = args[i]->ty;
                if (aty.kind == IR_TYPE_I8 || aty.kind == IR_TYPE_I16) {
                    IROpcode widen = (arg->ty && is_unsigned_type(arg->ty))
                                        ? IR_ZEXT : IR_SEXT;
                    args[i] = ir_build_cast(ctx->a, ctx->cur_block, widen,
                                            i32_type(), args[i]);
                }
            }
        }

        IRType ret_ty;
        if (node->ty)
            ret_ty = ctype_to_ir(node->ty);
        else
            ret_ty = i32_type();

        const char *fname = node->func_name;

        /* Indirect call through function pointer */
        if (!fname && node->lhs) {
            IRInst *fptr = gen_expr(ctx, node->lhs);
            /* Build combined operands: fptr + args so set_operands
             * properly establishes use-def edges for fptr */
            int total = nargs + 1;
            IRInst **all_ops = arena_alloc(ctx->a, sizeof(IRInst *) * total);
            all_ops[0] = fptr;
            for (int i = 0; i < nargs; i++)
                all_ops[i + 1] = args[i];
            IRInst *call = ir_build_call(ctx->a, ctx->cur_block, ret_ty, NULL, all_ops, total);
            call->call_target = NULL;
            return call;
        }

        IRInst *call = ir_build_call(ctx->a, ctx->cur_block, ret_ty, fname, args, nargs);

        /* Propagate variadic info for calling convention */
        if (node->lhs && node->lhs->var && node->lhs->var->ty &&
            node->lhs->var->ty->kind == TY_FUNC) {
            Type *fty = node->lhs->var->ty;
            call->is_variadic_call = fty->is_variadic;
            call->num_fixed_args = 0;
            for (Param *p = fty->params; p; p = p->next)
                call->num_fixed_args++;
        }

        return call;
    }

    /* ------- Statement expression (GNU extension) ------- */

    case ND_STMT_EXPR: {
        IRInst *result = ir_build_const_int(ctx->a, ctx->cur_block, i32_type(), 0);
        Node *body = node->body;
        if (body && body->kind == ND_BLOCK) {
            for (Node *s = body->body; s; s = s->next) {
                if (!s->next && s->kind == ND_EXPR_STMT && s->lhs) {
                    result = gen_expr(ctx, s->lhs);
                } else {
                    gen_stmt(ctx, s);
                }
            }
        }
        return result;
    }

    /* ------- Initializer list ------- */

    case ND_INIT_LIST: {
        /* Compound literal: (Type){values...}
         * Allocate a temporary, fill it with the init list, return its address. */
        if (node->ty) {
            int size = node->ty->size > 0 ? node->ty->size : 4;
            int align = node->ty->align > 0 ? node->ty->align : 4;
            IRInst *tmp = entry_alloca(ctx, size, align);
            gen_init_list(ctx, tmp, node, node->ty);
            return tmp;
        }
        IRType rty = ctype_to_ir(node->ty);
        return ir_build_const_int(ctx->a, ctx->cur_block, rty, 0);
    }

    default:
        fprintf(stderr, "irgen: unhandled expression kind %d\n", node->kind);
        return ir_build_const_int(ctx->a, ctx->cur_block, i32_type(), 0);
    }
}

/* ================================================================
 * Statement generation
 * ================================================================ */

static void gen_stmt(IRGenCtx *ctx, Node *node)
{
    if (!node) return;

    /* If the current block is already terminated, create a new unreachable block */
    if (block_terminated(ctx)) {
        IRBlock *dead = new_block(ctx, "dead");
        set_block(ctx, dead);
    }

    switch (node->kind) {

    case ND_BLOCK:
        for (Node *s = node->body; s; s = s->next)
            gen_stmt(ctx, s);
        break;

    case ND_EXPR_STMT:
        if (node->lhs)
            gen_expr(ctx, node->lhs);
        break;

    case ND_NULL_STMT:
        break;

    case ND_RETURN: {
        if (node->lhs) {
            IRInst *val = gen_expr(ctx, node->lhs);
            if (ctx->ret_alloca) {
                /* For aggregate types, gen_expr returns the address of the
                 * struct data.  We need memcpy, not a simple store. */
                IRType ret_ty = ctype_to_ir(node->lhs->ty);
                if (ret_ty.kind == IR_TYPE_AGGREGATE && ret_ty.size > 0) {
                    ir_build_memcpy(ctx->a, ctx->cur_block, ctx->ret_alloca, val, ret_ty.size);
                } else {
                    ir_build_store(ctx->a, ctx->cur_block, val, ctx->ret_alloca);
                }
            }
        }
        ir_build_br(ctx->a, ctx->cur_block, ctx->ret_block);
        break;
    }

    case ND_IF: {
        IRBlock *then_bb = new_block(ctx, "if.then");
        IRBlock *else_bb = node->els ? new_block(ctx, "if.else") : NULL;
        IRBlock *end_bb = new_block(ctx, "if.end");

        IRInst *cond = gen_expr(ctx, node->cond);
        IRInst *cond_bool = to_bool(ctx, cond, node->cond->ty);
        ir_build_condbr(ctx->a, ctx->cur_block, cond_bool,
                        then_bb, else_bb ? else_bb : end_bb);

        set_block(ctx, then_bb);
        gen_stmt(ctx, node->then);
        emit_br_if_needed(ctx, end_bb);

        if (node->els) {
            set_block(ctx, else_bb);
            gen_stmt(ctx, node->els);
            emit_br_if_needed(ctx, end_bb);
        }

        set_block(ctx, end_bb);
        break;
    }

    case ND_FOR: {
        bool is_dowhile = node->is_dowhile;

        IRBlock *cond_bb = new_block(ctx, is_dowhile ? "dowhile.cond" : "for.cond");
        IRBlock *body_bb = new_block(ctx, is_dowhile ? "dowhile.body" : "for.body");
        IRBlock *inc_bb  = new_block(ctx, is_dowhile ? "dowhile.inc" : "for.inc");
        IRBlock *end_bb  = new_block(ctx, is_dowhile ? "dowhile.end" : "for.end");

        IRBlock *old_break = ctx->break_target;
        IRBlock *old_cont  = ctx->continue_target;
        ctx->break_target = end_bb;
        ctx->continue_target = is_dowhile ? cond_bb : inc_bb;

        if (is_dowhile) {
            /* do { body } while (cond): body executes first */
            emit_br_if_needed(ctx, body_bb);
            set_block(ctx, body_bb);
            gen_stmt(ctx, node->then);
            emit_br_if_needed(ctx, cond_bb);

            set_block(ctx, cond_bb);
            if (node->cond) {
                IRInst *cond = gen_expr(ctx, node->cond);
                IRInst *cond_bool = to_bool(ctx, cond, node->cond->ty);
                ir_build_condbr(ctx->a, ctx->cur_block, cond_bool, body_bb, end_bb);
            } else {
                ir_build_br(ctx->a, ctx->cur_block, body_bb);
            }
        } else {
            /* Regular for loop */
            if (node->init)
                gen_stmt(ctx, node->init);

            emit_br_if_needed(ctx, cond_bb);
            set_block(ctx, cond_bb);

            if (node->cond) {
                IRInst *cond = gen_expr(ctx, node->cond);
                IRInst *cond_bool = to_bool(ctx, cond, node->cond->ty);
                ir_build_condbr(ctx->a, ctx->cur_block, cond_bool, body_bb, end_bb);
            } else {
                ir_build_br(ctx->a, ctx->cur_block, body_bb);
            }

            set_block(ctx, body_bb);
            gen_stmt(ctx, node->then);
            emit_br_if_needed(ctx, inc_bb);

            set_block(ctx, inc_bb);
            if (node->inc)
                gen_expr(ctx, node->inc);
            emit_br_if_needed(ctx, cond_bb);
        }

        set_block(ctx, end_bb);

        ctx->break_target = old_break;
        ctx->continue_target = old_cont;
        break;
    }

    case ND_SWITCH: {
        IRBlock *end_bb = new_block(ctx, "switch.end");
        IRInst *val = gen_expr(ctx, node->cond);

        IRBlock *old_break = ctx->break_target;
        IRBlock *old_sw_end = ctx->switch_end;
        IRInst *old_sw_val = ctx->switch_val;
        IRBlock *old_sw_def = ctx->switch_default;

        ctx->break_target = end_bb;
        ctx->switch_end = end_bb;
        ctx->switch_val = val;
        ctx->switch_default = NULL;

        /* Collect case labels */
        CaseInfo *cases = NULL;
        CaseInfo **tail = &cases;
        IRBlock *default_bb = NULL;

        for (Node *c = node->case_next; c; c = c->case_next) {
            IRBlock *case_bb = new_block(ctx, "switch.case");
            CaseInfo *ci = arena_new(ctx->a, CaseInfo);
            ci->val = c->case_val;
            ci->bb = case_bb;
            ci->next = NULL;
            *tail = ci;
            tail = &ci->next;
        }

        if (node->default_case) {
            default_bb = new_block(ctx, "switch.default");
            ctx->switch_default = default_bb;
        }

        /* Generate dispatch chain */
        IRBlock *dispatch = new_block(ctx, "switch.dispatch");
        emit_br_if_needed(ctx, dispatch);
        set_block(ctx, dispatch);

        for (CaseInfo *ci = cases; ci; ci = ci->next) {
            IRInst *case_val = ir_build_const_int(ctx->a, ctx->cur_block, val->ty, ci->val);
            IRInst *cmp = ir_build_binop(ctx->a, ctx->cur_block, IR_ICMP_EQ, val, case_val);
            IRBlock *next_check;
            if (ci->next)
                next_check = new_block(ctx, "switch.next");
            else
                next_check = default_bb ? default_bb : end_bb;
            ir_build_condbr(ctx->a, ctx->cur_block, cmp, ci->bb, next_check);
            if (ci->next)
                set_block(ctx, next_check);
        }

        if (!cases)
            emit_br_if_needed(ctx, default_bb ? default_bb : end_bb);

        /* Generate the body, switching blocks at case/default labels.
         * gen_switch_body will set the block to each case's bb as it
         * encounters ND_CASE nodes in the AST. */
        CaseInfo *cur_case = cases;
        /* Create a dead block for code before the first case label */
        IRBlock *dead_bb = new_block(ctx, "switch.pre");
        set_block(ctx, dead_bb);

        if (node->then)
            gen_switch_body(ctx, node->then, &cur_case, default_bb, end_bb);

        emit_br_if_needed(ctx, end_bb);
        set_block(ctx, end_bb);

        ctx->break_target = old_break;
        ctx->switch_end = old_sw_end;
        ctx->switch_val = old_sw_val;
        ctx->switch_default = old_sw_def;
        break;
    }

    case ND_CASE:
        if (node->lhs)
            gen_stmt(ctx, node->lhs);
        break;

    case ND_DEFAULT:
        if (node->lhs)
            gen_stmt(ctx, node->lhs);
        break;

    case ND_BREAK:
        if (ctx->break_target)
            ir_build_br(ctx->a, ctx->cur_block, ctx->break_target);
        break;

    case ND_CONTINUE:
        if (ctx->continue_target)
            ir_build_br(ctx->a, ctx->cur_block, ctx->continue_target);
        break;

    case ND_GOTO: {
        if (node->label) {
            IRBlock *target = hashtab_get(&ctx->labels, node->label);
            if (!target) {
                target = new_block(ctx, node->label);
                hashtab_put(&ctx->labels, node->label, target);
            }
            ir_build_br(ctx->a, ctx->cur_block, target);
        }
        break;
    }

    case ND_LABEL: {
        IRBlock *label_bb = hashtab_get(&ctx->labels, node->label);
        if (!label_bb) {
            label_bb = new_block(ctx, node->label);
            hashtab_put(&ctx->labels, node->label, label_bb);
        }
        emit_br_if_needed(ctx, label_bb);
        set_block(ctx, label_bb);

        if (node->lhs)
            gen_stmt(ctx, node->lhs);
        break;
    }

    case ND_DECLARATION:
        gen_declaration(ctx, node);
        break;

    case ND_FUNCDEF:
        /* Should not appear inside a function body */
        break;

    case ND_TYPEDEF:
        break;

    default:
        gen_expr(ctx, node);
        break;
    }
}

/* ================================================================
 * Switch body generation helper
 * ================================================================ */

static void gen_switch_body(IRGenCtx *ctx, Node *node,
                            CaseInfo **cur_case, IRBlock *default_bb,
                            IRBlock *end_bb)
{
    if (!node) return;

    switch (node->kind) {
    case ND_BLOCK:
        for (Node *s = node->body; s; s = s->next)
            gen_switch_body(ctx, s, cur_case, default_bb, end_bb);
        break;

    case ND_CASE:
        if (*cur_case) {
            IRBlock *case_bb = (*cur_case)->bb;
            emit_br_if_needed(ctx, case_bb); /* fallthrough */
            set_block(ctx, case_bb);
            *cur_case = (*cur_case)->next;
        }
        if (node->lhs)
            gen_switch_body(ctx, node->lhs, cur_case, default_bb, end_bb);
        break;

    case ND_DEFAULT:
        if (default_bb) {
            emit_br_if_needed(ctx, default_bb);
            set_block(ctx, default_bb);
        }
        if (node->lhs)
            gen_switch_body(ctx, node->lhs, cur_case, default_bb, end_bb);
        break;

    case ND_FOR: {
        /* Loop with case labels inside (e.g., Duff's device).
         * We replicate the loop structure from gen_stmt but use
         * gen_switch_body for the loop body so case labels inside
         * get proper block switching. */
        bool is_dowhile = node->is_dowhile;

        IRBlock *cond_bb = new_block(ctx, is_dowhile ? "dowhile.cond" : "for.cond");
        IRBlock *body_bb = new_block(ctx, is_dowhile ? "dowhile.body" : "for.body");
        IRBlock *inc_bb  = new_block(ctx, is_dowhile ? "dowhile.inc" : "for.inc");
        IRBlock *loop_end_bb  = new_block(ctx, is_dowhile ? "dowhile.end" : "for.end");

        IRBlock *old_break = ctx->break_target;
        IRBlock *old_cont  = ctx->continue_target;
        ctx->break_target = loop_end_bb;
        ctx->continue_target = is_dowhile ? cond_bb : inc_bb;

        if (is_dowhile) {
            emit_br_if_needed(ctx, body_bb);
            set_block(ctx, body_bb);
            gen_switch_body(ctx, node->then, cur_case, default_bb, end_bb);
            emit_br_if_needed(ctx, cond_bb);

            set_block(ctx, cond_bb);
            if (node->cond) {
                IRInst *cond = gen_expr(ctx, node->cond);
                IRInst *cond_bool = to_bool(ctx, cond, node->cond->ty);
                ir_build_condbr(ctx->a, ctx->cur_block, cond_bool, body_bb, loop_end_bb);
            } else {
                ir_build_br(ctx->a, ctx->cur_block, body_bb);
            }
        } else {
            if (node->init)
                gen_stmt(ctx, node->init);

            emit_br_if_needed(ctx, cond_bb);
            set_block(ctx, cond_bb);

            if (node->cond) {
                IRInst *cond = gen_expr(ctx, node->cond);
                IRInst *cond_bool = to_bool(ctx, cond, node->cond->ty);
                ir_build_condbr(ctx->a, ctx->cur_block, cond_bool, body_bb, loop_end_bb);
            } else {
                ir_build_br(ctx->a, ctx->cur_block, body_bb);
            }

            set_block(ctx, body_bb);
            gen_switch_body(ctx, node->then, cur_case, default_bb, end_bb);
            emit_br_if_needed(ctx, inc_bb);

            set_block(ctx, inc_bb);
            if (node->inc)
                gen_expr(ctx, node->inc);
            emit_br_if_needed(ctx, cond_bb);
        }

        set_block(ctx, loop_end_bb);

        ctx->break_target = old_break;
        ctx->continue_target = old_cont;
        break;
    }

    case ND_IF: {
        /* If/else with case labels inside. */
        IRBlock *then_bb = new_block(ctx, "if.then");
        IRBlock *else_bb = node->els ? new_block(ctx, "if.else") : NULL;
        IRBlock *if_end_bb = new_block(ctx, "if.end");

        IRInst *cond = gen_expr(ctx, node->cond);
        IRInst *cond_bool = to_bool(ctx, cond, node->cond->ty);
        ir_build_condbr(ctx->a, ctx->cur_block, cond_bool,
                        then_bb, else_bb ? else_bb : if_end_bb);

        set_block(ctx, then_bb);
        gen_switch_body(ctx, node->then, cur_case, default_bb, end_bb);
        emit_br_if_needed(ctx, if_end_bb);

        if (node->els) {
            set_block(ctx, else_bb);
            gen_switch_body(ctx, node->els, cur_case, default_bb, end_bb);
            emit_br_if_needed(ctx, if_end_bb);
        }

        set_block(ctx, if_end_bb);
        break;
    }

    default:
        gen_stmt(ctx, node);
        break;
    }
}

/* ================================================================
 * Declaration generation
 * ================================================================ */

static int static_local_counter = 0;

/* Generate code for an initializer list {val1, val2, ...}
 * Stores each element at the appropriate offset in base_addr.
 * Handles arrays, structs, and nested init lists recursively. */
static void gen_init_list(IRGenCtx *ctx, IRInst *base_addr, Node *init_list, Type *ty)
{
    if (!ty || !init_list || init_list->kind != ND_INIT_LIST)
        return;

    /* First, zero-fill the entire area */
    int total_size = ty->size;
    if (total_size > 0) {
        /* Zero-fill by storing 0s in 8-byte chunks */
        IRInst *zero = ir_build_const_int(ctx->a, ctx->cur_block, make_ir_type(IR_TYPE_I64), 0);
        int off = 0;
        while (off + 8 <= total_size) {
            IRInst *ptr = (off == 0) ? base_addr :
                ir_build_gep(ctx->a, ctx->cur_block, base_addr, off);
            ir_build_store(ctx->a, ctx->cur_block, zero, ptr);
            off += 8;
        }
        if (off + 4 <= total_size) {
            IRInst *zero32 = ir_build_const_int(ctx->a, ctx->cur_block, make_ir_type(IR_TYPE_I32), 0);
            IRInst *ptr = ir_build_gep(ctx->a, ctx->cur_block, base_addr, off);
            ir_build_store(ctx->a, ctx->cur_block, zero32, ptr);
            off += 4;
        }
        while (off < total_size) {
            IRInst *zero8 = ir_build_const_int(ctx->a, ctx->cur_block, make_ir_type(IR_TYPE_I8), 0);
            IRInst *ptr = ir_build_gep(ctx->a, ctx->cur_block, base_addr, off);
            ir_build_store(ctx->a, ctx->cur_block, zero8, ptr);
            off++;
        }
    }

    if (ty->kind == TY_ARRAY && ty->base) {
        /* Array initializer: {elem0, elem1, ...} */
        int elem_size = ty->base->size;
        int idx = 0;
        for (Node *elem = init_list->body; elem; elem = elem->next, idx++) {
            /* Chained designator starting with array index: [N].field = expr */
            if (elem->desig_chain && elem->desig_chain->index >= 0) {
                int total_offset = 0;
                Type *cur_ty = ty;
                Designator *d = elem->desig_chain;
                for (; d; d = d->next) {
                    if (d->index >= 0 && cur_ty->kind == TY_ARRAY && cur_ty->base) {
                        total_offset += d->index * cur_ty->base->size;
                        cur_ty = cur_ty->base;
                        idx = d->index; /* update sequential index */
                    } else if (d->name && (cur_ty->kind == TY_STRUCT || cur_ty->kind == TY_UNION)) {
                        for (Member *m = cur_ty->members; m; m = m->next) {
                            if (m->name && strcmp(m->name, d->name) == 0) {
                                total_offset += m->offset;
                                cur_ty = m->ty;
                                break;
                            }
                        }
                    }
                }
                IRInst *ptr = (total_offset == 0) ? base_addr :
                    ir_build_gep(ctx->a, ctx->cur_block, base_addr, total_offset);
                if (elem->kind == ND_INIT_LIST) {
                    gen_init_list(ctx, ptr, elem, cur_ty);
                } else {
                    IRInst *val = gen_expr(ctx, elem);
                    IRType fty = ctype_to_ir(cur_ty);
                    if (fty.kind == IR_TYPE_AGGREGATE) {
                        ir_build_memcpy(ctx->a, ctx->cur_block, ptr, val, cur_ty->size);
                    } else {
                        if (elem->ty && cur_ty)
                            val = gen_cast(ctx, val, elem->ty, cur_ty);
                        ir_build_store(ctx->a, ctx->cur_block, val, ptr);
                    }
                }
                continue;
            }

            /* Designated array initializer: [N] = expr */
            if (elem->designator_index >= 0) {
                idx = elem->designator_index;
            }
            int offset = idx * elem_size;
            IRInst *elem_ptr = (offset == 0) ? base_addr :
                ir_build_gep(ctx->a, ctx->cur_block, base_addr, offset);

            if (elem->kind == ND_INIT_LIST) {
                /* Nested init list (e.g., 2D array) */
                gen_init_list(ctx, elem_ptr, elem, ty->base);
            } else {
                IRInst *val = gen_expr(ctx, elem);
                IRType ety = ctype_to_ir(ty->base);
                if (ety.kind == IR_TYPE_AGGREGATE) {
                    ir_build_memcpy(ctx->a, ctx->cur_block, elem_ptr, val, elem_size);
                } else {
                    if (elem->ty && ty->base)
                        val = gen_cast(ctx, val, elem->ty, ty->base);
                    ir_build_store(ctx->a, ctx->cur_block, val, elem_ptr);
                }
            }
        }
    } else if (ty->kind == TY_STRUCT || ty->kind == TY_UNION) {
        /* Struct initializer: {field0, field1, ...} */
        Member *mem = ty->members;
        for (Node *elem = init_list->body; elem; elem = elem->next) {
            /* Chained designator: .o.x = expr  or .o.in.a = expr */
            if (elem->desig_chain) {
                int total_offset = 0;
                Type *cur_ty = ty;
                Designator *d = elem->desig_chain;
                for (; d; d = d->next) {
                    if (d->name && (cur_ty->kind == TY_STRUCT || cur_ty->kind == TY_UNION)) {
                        Member *found = NULL;
                        for (Member *m = cur_ty->members; m; m = m->next) {
                            if (m->name && strcmp(m->name, d->name) == 0) {
                                found = m;
                                break;
                            }
                        }
                        if (!found) break;
                        total_offset += found->offset;
                        cur_ty = found->ty;
                        /* Set mem for next-element advancement */
                        if (!d->next) {
                            /* Find this member's next for positional continuation */
                            mem = found->next;
                        }
                    } else if (d->index >= 0 && cur_ty->kind == TY_ARRAY && cur_ty->base) {
                        total_offset += d->index * cur_ty->base->size;
                        cur_ty = cur_ty->base;
                    }
                }
                IRInst *ptr = (total_offset == 0) ? base_addr :
                    ir_build_gep(ctx->a, ctx->cur_block, base_addr, total_offset);
                if (elem->kind == ND_INIT_LIST) {
                    gen_init_list(ctx, ptr, elem, cur_ty);
                } else {
                    IRInst *val = gen_expr(ctx, elem);
                    IRType fty = ctype_to_ir(cur_ty);
                    if (fty.kind == IR_TYPE_AGGREGATE) {
                        ir_build_memcpy(ctx->a, ctx->cur_block, ptr, val, cur_ty->size);
                    } else {
                        if (elem->ty && cur_ty)
                            val = gen_cast(ctx, val, elem->ty, cur_ty);
                        ir_build_store(ctx->a, ctx->cur_block, val, ptr);
                    }
                }
                continue;
            }

            /* Designated struct initializer: .field = expr */
            if (elem->designator_name) {
                /* Find the named member */
                mem = NULL;
                for (Member *m = ty->members; m; m = m->next) {
                    if (m->name && strcmp(m->name, elem->designator_name) == 0) {
                        mem = m;
                        break;
                    }
                }
                if (!mem) {
                    /* Designator not found - skip this element */
                    continue;
                }
            }
            if (!mem) break;

            int offset = mem->offset;
            IRInst *field_ptr = (offset == 0) ? base_addr :
                ir_build_gep(ctx->a, ctx->cur_block, base_addr, offset);

            if (elem->kind == ND_INIT_LIST) {
                gen_init_list(ctx, field_ptr, elem, mem->ty);
            } else {
                IRType fty = ctype_to_ir(mem->ty);
                if (fty.kind == IR_TYPE_AGGREGATE) {
                    /* Scalar init for aggregate member (e.g. {0} for struct):
                     * area is already zero-filled, so skip unless it's a real aggregate source */
                    if (elem->kind != ND_NUM || elem->ival != 0) {
                        IRInst *val = gen_expr(ctx, elem);
                        ir_build_memcpy(ctx->a, ctx->cur_block, field_ptr, val, mem->ty->size);
                    }
                } else {
                    IRInst *val = gen_expr(ctx, elem);
                    if (elem->ty && mem->ty)
                        val = gen_cast(ctx, val, elem->ty, mem->ty);
                    ir_build_store(ctx->a, ctx->cur_block, val, field_ptr);
                }
            }
            mem = mem->next;
        }
    } else {
        /* Scalar with brace init: {value} */
        Node *elem = init_list->body;
        if (elem) {
            IRInst *val = gen_expr(ctx, elem);
            if (elem->ty && ty)
                val = gen_cast(ctx, val, elem->ty, ty);
            ir_build_store(ctx->a, ctx->cur_block, val, base_addr);
        }
    }
}

static void gen_declaration(IRGenCtx *ctx, Node *node)
{
    Obj *var = node->var;
    if (!var) return;

    /* Skip extern declarations */
    if (var->is_extern) return;

    /* Handle static local variables: emit as globals with mangled names */
    if (var->is_static && !var->is_global) {
        /* Create a unique mangled name for this static local */
        char mangled[256];
        snprintf(mangled, sizeof(mangled), "%s.%s.%d",
                 ctx->cur_func->name, var->name, static_local_counter++);
        char *perm_name = arena_alloc(ctx->a, strlen(mangled) + 1);
        strcpy(perm_name, mangled);

        /* Register as a global variable */
        IRGlobal *g = arena_new(ctx->a, IRGlobal);
        g->name = perm_name;
        g->ty = ctype_to_ir(var->ty);
        g->alloc_size = var->ty->size;
        g->alloc_align = var->ty->align;
        g->is_string = false;
        g->is_extern = false;
        g->is_static = true;  /* don't export symbol */
        /* Handle static initializer */
        {
            int64_t cval;
            if (node->lhs && eval_const_expr(node->lhs, &cval)) {
                g->has_init = true;
                g->init_val = cval;
            } else {
                g->has_init = false;  /* zero-initialized in .bss */
                g->init_val = 0;
            }
        }
        g->next = ctx->mod->globals;
        ctx->mod->globals = g;

        /* Create a reference to the global as the variable's "alloca" */
        IRType p = ptr_type();
        IRInst *ref = ir_inst_new(ctx->a, ctx->cur_block, IR_CONST_INT, p);
        ref->ival = 0;
        ref->call_target = perm_name;
        ir_block_append(ctx->cur_block, ref);

        var->ir_alloca = ref;
        hashtab_put(&ctx->locals, var->name, ref);
        return;
    }

    /* VLA: variable-length array — use malloc since the naive backend
     * cannot handle dynamic stack allocation (all offsets are sp-relative). */
    if (var->ty && var->ty->is_vla && var->ty->vla_size) {
        Node *vla_size_expr = (Node *)var->ty->vla_size;
        IRInst *nelems = gen_expr(ctx, vla_size_expr);
        /* Compute total size: nelems * sizeof(element) */
        int elem_size = var->ty->base ? var->ty->base->size : 1;
        IRType i64ty = make_ir_type(IR_TYPE_I64);
        /* Extend nelems to i64 if needed */
        if (nelems->ty.kind != IR_TYPE_I64 && nelems->ty.kind != IR_TYPE_PTR) {
            nelems = ir_build_cast(ctx->a, ctx->cur_block, IR_SEXT, i64ty, nelems);
        }
        IRInst *elem_sz = ir_build_const_int(ctx->a, ctx->cur_block, i64ty, elem_size);
        IRInst *total_sz = ir_build_binop(ctx->a, ctx->cur_block, IR_MUL, nelems, elem_sz);

        /* Call malloc to allocate VLA memory */
        IRType pty = ptr_type();
        IRInst *malloc_call = ir_inst_new(ctx->a, ctx->cur_block, IR_CALL, pty);
        malloc_call->call_target = "malloc";
        malloc_call->num_operands = 1;
        malloc_call->operands = arena_alloc(ctx->a, sizeof(IRInst *));
        malloc_call->operands[0] = total_sz;
        ir_block_append(ctx->cur_block, malloc_call);

        /* Store pointer to VLA into a regular alloca */
        IRInst *ptr_alloca = entry_alloca(ctx, 8, 8);
        ir_build_store(ctx->a, ctx->cur_block, malloc_call, ptr_alloca);

        var->ir_alloca = ptr_alloca;
        hashtab_put(&ctx->locals, var->name, ptr_alloca);
        return;
    }

    /* Create alloca for the local variable */
    int size = var->ty ? var->ty->size : 4;
    int align = var->ty ? var->ty->align : 4;
    if (size == 0) size = 1;
    if (align == 0) align = 1;

    /* Insert alloca in the entry block for better optimization.
     * We save/restore insertion point. */
    IRBlock *saved_block = ctx->cur_block;
    IRBlock *entry = ctx->cur_func->entry;

    /* Find insertion point: after existing allocas in entry block */
    IRInst *insert_after = NULL;
    for (IRInst *inst = entry->first; inst; inst = inst->next) {
        if (inst->op == IR_ALLOCA)
            insert_after = inst;
        else
            break;
    }

    /* Create the alloca instruction */
    IRType pty = ptr_type();
    IRInst *alloca_inst = ir_inst_new(ctx->a, entry, IR_ALLOCA, pty);
    alloca_inst->alloca_size = size;
    alloca_inst->alloca_align = align;

    /* Insert it in the entry block at the right position */
    if (insert_after) {
        alloca_inst->next = insert_after->next;
        alloca_inst->prev = insert_after;
        if (insert_after->next)
            insert_after->next->prev = alloca_inst;
        insert_after->next = alloca_inst;
        if (entry->last == insert_after)
            entry->last = alloca_inst;
        alloca_inst->parent = entry;
        entry->inst_count++;
    } else {
        alloca_inst->next = entry->first;
        alloca_inst->prev = NULL;
        if (entry->first)
            entry->first->prev = alloca_inst;
        entry->first = alloca_inst;
        if (!entry->last)
            entry->last = alloca_inst;
        alloca_inst->parent = entry;
        entry->inst_count++;
    }

    /* Map the variable to its alloca (both Obj pointer and hash table) */
    var->ir_alloca = alloca_inst;
    hashtab_put(&ctx->locals, var->name, alloca_inst);

    /* Restore insertion block */
    ctx->cur_block = saved_block;

    /* Handle initializer */
    if (node->lhs && node->lhs->kind == ND_INIT_LIST) {
        /* Check if this is a compound literal expression assigned to a
         * pointer (e.g., int *p = (int[]){1,2,3}).  Pointers can never
         * be directly initialized with an init list, so the init list
         * must be a compound literal expression yielding an address. */
        if (var->ty->kind == TY_PTR && node->lhs->ty) {
            IRInst *init_val = gen_expr(ctx, node->lhs);
            if (node->lhs->ty && var->ty)
                init_val = gen_cast(ctx, init_val, node->lhs->ty, var->ty);
            ir_build_store(ctx->a, ctx->cur_block, init_val, alloca_inst);
        } else {
            /* Initializer list: {val1, val2, ...}
             * Store each element at the appropriate offset in the alloca. */
            gen_init_list(ctx, alloca_inst, node->lhs, var->ty);
        }
    } else if (node->lhs) {
        IRInst *init_val = gen_expr(ctx, node->lhs);

        /* Check if this is an aggregate type that needs memcpy.
         * Note: TY_ARRAY maps to IR_TYPE_PTR in ctype_to_ir (array decay),
         * but for local array initialization we need memcpy, not pointer store. */
        bool is_aggregate = (var->ty->kind == TY_ARRAY ||
                             var->ty->kind == TY_STRUCT ||
                             var->ty->kind == TY_UNION);
        if (is_aggregate) {
            /* For char arrays initialized from string literals shorter than the array,
             * zero-fill first, then copy only the string length */
            if (var->ty->kind == TY_ARRAY && node->lhs->str_val &&
                node->lhs->str_len + 1 < size) {
                /* Zero-fill entire array */
                IRInst *zero = ir_build_const_int(ctx->a, ctx->cur_block,
                    make_ir_type(IR_TYPE_I64), 0);
                int off = 0;
                while (off + 8 <= size) {
                    IRInst *ptr = (off == 0) ? alloca_inst :
                        ir_build_gep(ctx->a, ctx->cur_block, alloca_inst, off);
                    ir_build_store(ctx->a, ctx->cur_block, zero, ptr);
                    off += 8;
                }
                if (off + 4 <= size) {
                    IRInst *z32 = ir_build_const_int(ctx->a, ctx->cur_block,
                        make_ir_type(IR_TYPE_I32), 0);
                    IRInst *ptr = ir_build_gep(ctx->a, ctx->cur_block, alloca_inst, off);
                    ir_build_store(ctx->a, ctx->cur_block, z32, ptr);
                    off += 4;
                }
                while (off < size) {
                    IRInst *z8 = ir_build_const_int(ctx->a, ctx->cur_block,
                        make_ir_type(IR_TYPE_I8), 0);
                    IRInst *ptr = ir_build_gep(ctx->a, ctx->cur_block, alloca_inst, off);
                    ir_build_store(ctx->a, ctx->cur_block, z8, ptr);
                    off++;
                }
                /* Copy string data (including null terminator) */
                ir_build_memcpy(ctx->a, ctx->cur_block, alloca_inst, init_val,
                                node->lhs->str_len + 1);
            } else {
                ir_build_memcpy(ctx->a, ctx->cur_block, alloca_inst, init_val, size);
            }
        } else {
            if (node->lhs->ty && var->ty)
                init_val = gen_cast(ctx, init_val, node->lhs->ty, var->ty);
            ir_build_store(ctx->a, ctx->cur_block, init_val, alloca_inst);
        }
    } else if (var->init_expr) {
        IRInst *init_val = gen_expr(ctx, var->init_expr);
        if (var->init_expr->ty && var->ty)
            init_val = gen_cast(ctx, init_val, var->init_expr->ty, var->ty);
        ir_build_store(ctx->a, ctx->cur_block, init_val, alloca_inst);
    }
}

/* ================================================================
 * Global variable generation
 * ================================================================ */

static void gen_global_var(IRGenCtx *ctx, Obj *var)
{
    /* Skip function declarations and function pointer globals - they don't
     * need global variable entries as the linker resolves functions. */
    if (var->ty && var->ty->kind == TY_FUNC) return;
    if (var->ty && var->ty->kind == TY_PTR && var->ty->base &&
        var->ty->base->kind == TY_FUNC) return;

    IRGlobal *g = arena_new(ctx->a, IRGlobal);
    g->name = var->name;
    g->ty = ctype_to_ir(var->ty);
    g->alloc_size = var->ty->size;
    g->alloc_align = var->ty->align;
    g->is_string = (var->string_val != NULL);
    g->string_val = var->string_val;
    g->string_len = var->string_len;
    g->is_extern = var->is_extern;
    g->is_static = var->is_static;

    /* Check for constant initializer */
    g->has_init = false;
    g->init_val = 0;
    g->init_data = NULL;
    g->init_data_len = 0;
    g->relocs = NULL;

    if (var->init_expr) {
        int64_t cval;
        if (eval_const_expr(var->init_expr, &cval)) {
            g->has_init = true;
            g->init_val = cval;
        } else if (var->init_expr->kind == ND_INIT_LIST && var->ty) {
            /* Array or struct initializer: build init_data byte array */
            int total = g->alloc_size > 0 ? g->alloc_size : var->ty->size;
            if (total > 0) {
                uint8_t *data = arena_alloc(ctx->a, total);
                memset(data, 0, total);
                bool ok = true;
                if (var->ty->kind == TY_ARRAY && var->ty->base) {
                    int elem_sz = var->ty->base->size;
                    int idx = 0;
                    for (Node *e = var->init_expr->body; e && ok; e = e->next, idx++) {
                        if (e->designator_index >= 0) idx = e->designator_index;
                        int off = idx * elem_sz;
                        if (off + elem_sz > total) { ok = false; break; }
                        if (e->kind == ND_INIT_LIST && var->ty->base->kind == TY_STRUCT) {
                            /* Nested struct in array */
                            Member *mem = var->ty->base->members;
                            for (Node *se = e->body; se && mem; se = se->next, mem = mem->next) {
                                if (se->designator_name) {
                                    mem = NULL;
                                    for (Member *m = var->ty->base->members; m; m = m->next) {
                                        if (m->name && strcmp(m->name, se->designator_name) == 0) {
                                            mem = m; break;
                                        }
                                    }
                                    if (!mem) continue;
                                }
                                int64_t v;
                                if (eval_const_expr(se, &v)) {
                                    int foff = off + mem->offset;
                                    if (mem->ty->size == 1) data[foff] = (uint8_t)v;
                                    else if (mem->ty->size == 2) memcpy(&data[foff], &v, 2);
                                    else if (mem->ty->size == 4) memcpy(&data[foff], &v, 4);
                                    else if (mem->ty->size == 8) memcpy(&data[foff], &v, 8);
                                }
                            }
                        } else {
                            int64_t v;
                            if (eval_const_expr(e, &v)) {
                                if (elem_sz == 1) data[off] = (uint8_t)v;
                                else if (elem_sz == 2) memcpy(&data[off], &v, 2);
                                else if (elem_sz == 4) memcpy(&data[off], &v, 4);
                                else if (elem_sz == 8) memcpy(&data[off], &v, 8);
                            } else ok = false;
                        }
                    }
                } else if (var->ty->kind == TY_STRUCT || var->ty->kind == TY_UNION) {
                    Member *mem = var->ty->members;
                    for (Node *e = var->init_expr->body; e && ok; e = e->next) {
                        if (e->designator_name) {
                            mem = NULL;
                            for (Member *m = var->ty->members; m; m = m->next) {
                                if (m->name && strcmp(m->name, e->designator_name) == 0) {
                                    mem = m; break;
                                }
                            }
                            if (!mem) continue;
                        }
                        if (!mem) break;
                        int off = mem->offset;
                        if (e->kind == ND_INIT_LIST && (mem->ty->kind == TY_STRUCT ||
                            mem->ty->kind == TY_ARRAY)) {
                            /* Nested init list for struct/array member */
                            if (mem->ty->kind == TY_ARRAY && mem->ty->base) {
                                int esz = mem->ty->base->size;
                                int aidx = 0;
                                for (Node *ae = e->body; ae; ae = ae->next, aidx++) {
                                    if (ae->designator_index >= 0) aidx = ae->designator_index;
                                    int64_t v;
                                    if (eval_const_expr(ae, &v)) {
                                        int aoff = off + aidx * esz;
                                        if (esz == 1) data[aoff] = (uint8_t)v;
                                        else if (esz == 2) memcpy(&data[aoff], &v, 2);
                                        else if (esz == 4) memcpy(&data[aoff], &v, 4);
                                        else if (esz == 8) memcpy(&data[aoff], &v, 8);
                                    }
                                }
                            } else if (mem->ty->kind == TY_STRUCT) {
                                Member *sm = mem->ty->members;
                                for (Node *se = e->body; se && sm; se = se->next, sm = sm->next) {
                                    if (se->designator_name) {
                                        sm = NULL;
                                        for (Member *m2 = mem->ty->members; m2; m2 = m2->next) {
                                            if (m2->name && strcmp(m2->name, se->designator_name) == 0) {
                                                sm = m2; break;
                                            }
                                        }
                                        if (!sm) continue;
                                    }
                                    int64_t v;
                                    if (eval_const_expr(se, &v)) {
                                        int foff = off + sm->offset;
                                        if (sm->ty->size == 1) data[foff] = (uint8_t)v;
                                        else if (sm->ty->size == 2) memcpy(&data[foff], &v, 2);
                                        else if (sm->ty->size == 4) memcpy(&data[foff], &v, 4);
                                        else if (sm->ty->size == 8) memcpy(&data[foff], &v, 8);
                                    }
                                }
                            }
                        } else if (e->kind == ND_VAR && e->str_val &&
                                   mem->ty->kind == TY_PTR) {
                            /* String literal pointer in struct: char *str = "hello" */
                            Obj *str_obj = e->var;
                            if (str_obj) {
                                ensure_global_registered(ctx, str_obj);
                                IRReloc *r = arena_new(ctx->a, IRReloc);
                                r->offset = off;
                                r->symbol = str_obj->name;
                                r->next = g->relocs;
                                g->relocs = r;
                            }
                        } else if (e->kind == ND_ADDR && e->lhs &&
                                   e->lhs->kind == ND_VAR && mem->ty->kind == TY_PTR) {
                            /* Pointer to global: &g_var */
                            IRReloc *r = arena_new(ctx->a, IRReloc);
                            r->offset = off;
                            r->symbol = e->lhs->var->name;
                            r->next = g->relocs;
                            g->relocs = r;
                        } else if (e->kind == ND_VAR && e->ty &&
                                   e->ty->kind == TY_ARRAY && mem->ty->kind == TY_PTR) {
                            /* Array decay to pointer: int *p = g_arr */
                            Obj *arr_obj = e->var;
                            if (arr_obj) {
                                IRReloc *r = arena_new(ctx->a, IRReloc);
                                r->offset = off;
                                r->symbol = arr_obj->name;
                                r->next = g->relocs;
                                g->relocs = r;
                            }
                        } else {
                            int64_t v;
                            if (eval_const_expr(e, &v)) {
                                if (mem->ty->size == 1) data[off] = (uint8_t)v;
                                else if (mem->ty->size == 2) memcpy(&data[off], &v, 2);
                                else if (mem->ty->size == 4) memcpy(&data[off], &v, 4);
                                else if (mem->ty->size == 8) memcpy(&data[off], &v, 8);
                            } else ok = false;
                        }
                        mem = mem->next;
                    }
                }
                if (ok) {
                    g->has_init = true;
                    g->init_data = data;
                    g->init_data_len = total;
                }
            }
        } else if (var->init_expr->kind == ND_ADDR && var->init_expr->lhs &&
                   var->init_expr->lhs->kind == ND_VAR) {
            /* Pointer to global: int *p = &g_var */
            g->has_init = true;
            IRReloc *r = arena_new(ctx->a, IRReloc);
            r->offset = 0;
            r->symbol = var->init_expr->lhs->var->name;
            r->next = NULL;
            g->relocs = r;
            g->init_data = arena_alloc(ctx->a, 8);
            memset(g->init_data, 0, 8);
            g->init_data_len = 8;
        } else if (var->init_expr->kind == ND_VAR && var->init_expr->ty &&
                   var->init_expr->ty->kind == TY_ARRAY) {
            Obj *src_var = var->init_expr->var;
            if (src_var && src_var->string_val && var->ty->kind == TY_ARRAY) {
                /* char arr[] = "string": inline the string bytes into the array */
                int total = g->alloc_size > 0 ? g->alloc_size : var->ty->size;
                if (total <= 0) total = src_var->string_len;
                uint8_t *data = arena_alloc(ctx->a, total);
                memset(data, 0, total);
                int copy_len = src_var->string_len < total ? src_var->string_len : total;
                memcpy(data, src_var->string_val, copy_len);
                g->has_init = true;
                g->init_data = data;
                g->init_data_len = total;
            } else {
                /* Array decay: int *p = g_arr or char *p = "str" */
                if (src_var) ensure_global_registered(ctx, src_var);
                g->has_init = true;
                IRReloc *r = arena_new(ctx->a, IRReloc);
                r->offset = 0;
                r->symbol = src_var ? src_var->name : var->init_expr->var->name;
                r->next = NULL;
                g->relocs = r;
                g->init_data = arena_alloc(ctx->a, 8);
                memset(g->init_data, 0, 8);
                g->init_data_len = 8;
            }
        }
    }

    g->next = ctx->mod->globals;
    ctx->mod->globals = g;
}

/* ================================================================
 * Function generation
 * ================================================================ */

static void gen_func(IRGenCtx *ctx, Node *node)
{
    Obj *fn = node->var;
    if (!fn || !fn->is_definition) return;

    /* Determine return type */
    IRType ret_ty = void_type();
    if (fn->ty && fn->ty->return_ty)
        ret_ty = ctype_to_ir(fn->ty->return_ty);

    /* Create IR function */
    IRFunc *func = ir_func_new(ctx->a, ctx->mod, fn->name, ret_ty);
    ctx->cur_func = func;

    /* Create entry block */
    IRBlock *entry = ir_block_new(ctx->a, func, "entry");
    set_block(ctx, entry);

    /* Create exit block with return */
    ctx->ret_block = new_block(ctx, "return");
    ctx->ret_alloca = NULL;

    /* If function returns a value, create an alloca for the return value */
    if (ret_ty.kind != IR_TYPE_VOID) {
        ctx->ret_alloca = ir_build_alloca(ctx->a, entry,
                                          ir_type_size(ret_ty),
                                          ret_ty.align > 0 ? ret_ty.align : 4);
    }

    /* Reset local variable and label maps */
    hashtab_clear(&ctx->locals);
    hashtab_clear(&ctx->labels);

    /* Create parameters */
    if (fn->ty && fn->ty->params) {
        int nparams = 0;
        for (Param *p = fn->ty->params; p; p = p->next)
            nparams++;

        func->params = arena_alloc(ctx->a, sizeof(IRInst *) * nparams);
        func->num_params = nparams;

        int i = 0;
        for (Param *p = fn->ty->params; p; p = p->next, i++) {
            IRType pty = ctype_to_ir(p->ty);
            /* Create a "parameter" instruction (IR_COPY serves as param placeholder) */
            IRInst *param = ir_inst_new(ctx->a, entry, IR_COPY, pty);
            ir_block_append(entry, param);
            func->params[i] = param;

            /* Create alloca for the parameter and store into it */
            if (p->name) {
                int psz = p->ty ? p->ty->size : 4;
                int palign = p->ty ? p->ty->align : 4;
                if (psz == 0) psz = 1;
                if (palign == 0) palign = 1;
                IRInst *param_alloca = ir_build_alloca(ctx->a, entry, psz, palign);
                /* For aggregate parameters, the register holds a pointer to
                 * the caller's struct. Copy the data via memcpy. */
                if (pty.kind == IR_TYPE_AGGREGATE && psz > 0) {
                    ir_build_memcpy(ctx->a, entry, param_alloca, param, psz);
                } else {
                    ir_build_store(ctx->a, entry, param, param_alloca);
                }
                hashtab_put(&ctx->locals, p->name, param_alloca);

                /* Set ir_alloca on the parameter's Obj so variable lookups
                 * use it directly instead of the flat hashtable, which can
                 * be overwritten by inner-scope shadowing variables. */
                Obj *param_obj = NULL;
                for (Obj *o = fn->locals; o; o = o->next) {
                    if (o->name && strcmp(o->name, p->name) == 0)
                        param_obj = o;  /* last match = parameter (added first) */
                }
                if (param_obj)
                    param_obj->ir_alloca = param_alloca;
            }
        }
    }

    /* Generate function body */
    if (fn->body)
        gen_stmt(ctx, fn->body);

    /* Terminate the current block if needed */
    emit_br_if_needed(ctx, ctx->ret_block);

    /* Generate the return block */
    set_block(ctx, ctx->ret_block);
    if (ctx->ret_alloca) {
        IRInst *ret_val = ir_build_load(ctx->a, ctx->cur_block, ret_ty, ctx->ret_alloca);
        ir_build_ret(ctx->a, ctx->cur_block, ret_val);
    } else {
        ir_build_retv(ctx->a, ctx->cur_block);
    }

    ctx->cur_func = NULL;
}

/* ================================================================
 * Top-level: cc_irgen
 * ================================================================ */

IRModule *cc_irgen(CompilerCtx *cc, Node *program)
{
    Arena *a = &cc->arena;
    IRModule *mod = ir_module_new(a);

    IRGenCtx ctx = {0};
    ctx.cc = cc;
    ctx.a = a;
    ctx.mod = mod;
    ctx.cur_func = NULL;
    ctx.cur_block = NULL;
    ctx.break_target = NULL;
    ctx.continue_target = NULL;
    ctx.switch_end = NULL;
    ctx.switch_val = NULL;
    ctx.switch_default = NULL;
    ctx.str_count = 0;
    ctx.ret_alloca = NULL;
    ctx.ret_block = NULL;
    hashtab_init(&ctx.locals, 64);
    hashtab_init(&ctx.labels, 16);

    if (!program) return mod;

    /* Walk top-level declarations */
    for (Node *n = program->body; n; n = n->next) {
        switch (n->kind) {
        case ND_FUNCDEF:
            gen_func(&ctx, n);
            break;

        case ND_DECLARATION:
            if (n->var && n->var->is_global)
                gen_global_var(&ctx, n->var);
            break;

        case ND_TYPEDEF:
            break;

        default:
            break;
        }
    }

    hashtab_free(&ctx.locals);
    hashtab_free(&ctx.labels);

    return mod;
}
