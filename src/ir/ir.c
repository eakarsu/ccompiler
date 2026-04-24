/*
 * ir.c - IR helpers, printer, and verifier for mycc
 *
 * Implements IR construction, human-readable SSA printing, and
 * verification of SSA invariants.
 */

#include "cc.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* ================================================================
 * Opcode name table
 * ================================================================ */

static const char *opcode_names[IR_OP_COUNT] = {
    [IR_CONST_INT]  = "const.int",
    [IR_CONST_FLOAT]= "const.float",
    [IR_ALLOCA]     = "alloca",
    [IR_LOAD]       = "load",
    [IR_STORE]      = "store",
    [IR_GEP]        = "gep",
    [IR_ADD]        = "add",
    [IR_SUB]        = "sub",
    [IR_MUL]        = "mul",
    [IR_SDIV]       = "sdiv",
    [IR_UDIV]       = "udiv",
    [IR_SREM]       = "srem",
    [IR_UREM]       = "urem",
    [IR_FADD]       = "fadd",
    [IR_FSUB]       = "fsub",
    [IR_FMUL]       = "fmul",
    [IR_FDIV]       = "fdiv",
    [IR_NEG]        = "neg",
    [IR_FNEG]       = "fneg",
    [IR_AND]        = "and",
    [IR_OR]         = "or",
    [IR_XOR]        = "xor",
    [IR_SHL]        = "shl",
    [IR_LSHR]       = "lshr",
    [IR_ASHR]       = "ashr",
    [IR_NOT]        = "not",
    [IR_ICMP_EQ]    = "icmp.eq",
    [IR_ICMP_NE]    = "icmp.ne",
    [IR_ICMP_SLT]   = "icmp.slt",
    [IR_ICMP_SLE]   = "icmp.sle",
    [IR_ICMP_SGT]   = "icmp.sgt",
    [IR_ICMP_SGE]   = "icmp.sge",
    [IR_ICMP_ULT]   = "icmp.ult",
    [IR_ICMP_ULE]   = "icmp.ule",
    [IR_ICMP_UGT]   = "icmp.ugt",
    [IR_ICMP_UGE]   = "icmp.uge",
    [IR_FCMP_OEQ]   = "fcmp.oeq",
    [IR_FCMP_ONE]   = "fcmp.one",
    [IR_FCMP_OLT]   = "fcmp.olt",
    [IR_FCMP_OLE]   = "fcmp.ole",
    [IR_FCMP_OGT]   = "fcmp.ogt",
    [IR_FCMP_OGE]   = "fcmp.oge",
    [IR_SEXT]       = "sext",
    [IR_ZEXT]       = "zext",
    [IR_TRUNC]      = "trunc",
    [IR_SITOFP]     = "sitofp",
    [IR_UITOFP]     = "uitofp",
    [IR_FPTOSI]     = "fptosi",
    [IR_FPTOUI]     = "fptoui",
    [IR_FPEXT]      = "fpext",
    [IR_FPTRUNC]    = "fptrunc",
    [IR_BITCAST]    = "bitcast",
    [IR_PTRTOINT]   = "ptrtoint",
    [IR_INTTOPTR]   = "inttoptr",
    [IR_BR]         = "br",
    [IR_CONDBR]     = "condbr",
    [IR_RET]        = "ret",
    [IR_RETV]       = "retv",
    [IR_CALL]       = "call",
    [IR_PHI]        = "phi",
    [IR_COPY]       = "copy",
    [IR_MEMCPY]     = "memcpy",
    [IR_MEMSET]     = "memset",
};

/* ================================================================
 * Type helpers
 * ================================================================ */

bool ir_type_eq(IRType a, IRType b)
{
    return a.kind == b.kind;
}

int ir_type_size(IRType ty)
{
    switch (ty.kind) {
    case IR_TYPE_I1:        return 1;
    case IR_TYPE_I8:        return 1;
    case IR_TYPE_I16:       return 2;
    case IR_TYPE_I32:       return 4;
    case IR_TYPE_I64:       return 8;
    case IR_TYPE_F32:       return 4;
    case IR_TYPE_F64:       return 8;
    case IR_TYPE_PTR:       return 8; /* 64-bit pointer */
    case IR_TYPE_VOID:      return 0;
    case IR_TYPE_AGGREGATE: return ty.size;
    }
    return 0;
}

IRType ir_type_from_ctype(Type *cty)
{
    IRType t = {0};
    if (!cty) {
        t.kind = IR_TYPE_VOID;
        return t;
    }

    t.is_unsigned = cty->is_unsigned;

    switch (cty->kind) {
    case TY_VOID:
        t.kind = IR_TYPE_VOID;
        break;
    case TY_BOOL:
        t.kind = IR_TYPE_I1;
        t.size = 1;
        t.align = 1;
        break;
    case TY_CHAR:
        t.kind = IR_TYPE_I8;
        t.size = 1;
        t.align = 1;
        break;
    case TY_SHORT:
        t.kind = IR_TYPE_I16;
        t.size = 2;
        t.align = 2;
        break;
    case TY_INT:
    case TY_ENUM:
        t.kind = IR_TYPE_I32;
        t.size = 4;
        t.align = 4;
        break;
    case TY_LONG:
    case TY_LLONG:
        t.kind = IR_TYPE_I64;
        t.size = 8;
        t.align = 8;
        break;
    case TY_FLOAT:
        t.kind = IR_TYPE_F32;
        t.size = 4;
        t.align = 4;
        break;
    case TY_DOUBLE:
    case TY_LDOUBLE:
        t.kind = IR_TYPE_F64;
        t.size = 8;
        t.align = 8;
        break;
    case TY_PTR:
    case TY_ARRAY:
    case TY_FUNC:
        t.kind = IR_TYPE_PTR;
        t.size = 8;
        t.align = 8;
        break;
    case TY_STRUCT:
    case TY_UNION:
        t.kind = IR_TYPE_AGGREGATE;
        t.size = cty->size;
        t.align = cty->align;
        break;
    }
    return t;
}

static const char *ir_type_name(IRType ty)
{
    switch (ty.kind) {
    case IR_TYPE_I1:        return "i1";
    case IR_TYPE_I8:        return "i8";
    case IR_TYPE_I16:       return "i16";
    case IR_TYPE_I32:       return "i32";
    case IR_TYPE_I64:       return "i64";
    case IR_TYPE_F32:       return "f32";
    case IR_TYPE_F64:       return "f64";
    case IR_TYPE_PTR:       return "ptr";
    case IR_TYPE_VOID:      return "void";
    case IR_TYPE_AGGREGATE: return "agg";
    }
    return "???";
}

/* ================================================================
 * IR Construction
 * ================================================================ */

IRModule *ir_module_new(Arena *a)
{
    IRModule *mod = arena_new(a, IRModule);
    mod->arena = a;
    mod->funcs = NULL;
    mod->globals = NULL;
    return mod;
}

IRFunc *ir_func_new(Arena *a, IRModule *mod, const char *name, IRType ret_type)
{
    IRFunc *func = arena_new(a, IRFunc);
    func->name = name;
    func->ret_type = ret_type;
    func->params = NULL;
    func->num_params = 0;
    func->entry = NULL;
    func->blocks = NULL;
    func->block_count = 0;
    func->next_inst_id = 0;
    func->next_block_id = 0;
    func->stack_size = 0;
    func->next = NULL;

    /* Append to module's function list */
    if (!mod->funcs) {
        mod->funcs = func;
    } else {
        IRFunc *f = mod->funcs;
        while (f->next)
            f = f->next;
        f->next = func;
    }

    return func;
}

IRBlock *ir_block_new(Arena *a, IRFunc *func, const char *name)
{
    IRBlock *bb = arena_new(a, IRBlock);
    bb->name = name;
    bb->id = func->next_block_id++;
    bb->parent = func;
    bb->first = NULL;
    bb->last = NULL;
    bb->inst_count = 0;
    bb->idom = NULL;
    bb->dom_order = -1;
    bb->loop_depth = 0;
    bb->next = NULL;
    bb->prev = NULL;
    vec_init(&bb->preds);
    vec_init(&bb->succs);
    vec_init(&bb->dom_children);
    vec_init(&bb->dom_frontier);

    /* Append block to the function's block list */
    if (!func->blocks) {
        func->blocks = bb;
        func->entry = bb;
    } else {
        IRBlock *last = func->blocks;
        while (last->next)
            last = last->next;
        last->next = bb;
        bb->prev = last;
    }
    func->block_count++;

    return bb;
}

IRInst *ir_inst_new(Arena *a, IRBlock *block, IROpcode op, IRType ty)
{
    IRInst *inst = arena_new(a, IRInst);
    inst->op = op;
    inst->ty = ty;
    inst->id = block->parent->next_inst_id++;
    inst->parent = block;
    inst->operands = NULL;
    inst->num_operands = 0;
    inst->ival = 0;
    inst->alloca_size = 0;
    inst->alloca_align = 0;
    inst->gep_offset = 0;
    inst->call_target = NULL;
    inst->call_func = NULL;
    inst->phi_vals = NULL;
    inst->phi_blocks = NULL;
    inst->phi_count = 0;
    inst->br_target = NULL;
    inst->br_true = NULL;
    inst->br_false = NULL;
    inst->next = NULL;
    inst->prev = NULL;
    vec_init(&inst->users);
    inst->reg = -1;
    inst->stack_slot = 0;
    inst->is_dead = false;
    inst->visited = false;

    return inst;
}

void ir_block_append(IRBlock *block, IRInst *inst)
{
    inst->parent = block;
    if (!block->first) {
        block->first = inst;
        block->last = inst;
        inst->prev = NULL;
        inst->next = NULL;
    } else {
        inst->prev = block->last;
        inst->next = NULL;
        block->last->next = inst;
        block->last = inst;
    }
    block->inst_count++;
}

void ir_block_insert_before(IRBlock *block, IRInst *before, IRInst *inst)
{
    inst->parent = block;
    inst->next = before;
    inst->prev = before->prev;
    if (before->prev) {
        before->prev->next = inst;
    } else {
        block->first = inst;
    }
    before->prev = inst;
    block->inst_count++;
}

/* Add a use edge: record that 'user' uses 'def' */
static void add_use(IRInst *def, IRInst *user)
{
    if (def) {
        vec_push(&def->users, user);
    }
}

/* Set operands for an instruction and update use-def chains */
static void set_operands(Arena *a, IRInst *inst, IRInst **ops, int count)
{
    inst->num_operands = count;
    if (count > 0) {
        inst->operands = arena_alloc(a, sizeof(IRInst *) * count);
        for (int i = 0; i < count; i++) {
            inst->operands[i] = ops[i];
            add_use(ops[i], inst);
        }
    }
}

/* ================================================================
 * Instruction Builders
 * ================================================================ */

IRInst *ir_build_const_int(Arena *a, IRBlock *bb, IRType ty, int64_t val)
{
    IRInst *inst = ir_inst_new(a, bb, IR_CONST_INT, ty);
    inst->ival = val;
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_const_float(Arena *a, IRBlock *bb, IRType ty, double val)
{
    IRInst *inst = ir_inst_new(a, bb, IR_CONST_FLOAT, ty);
    inst->fval = val;
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_alloca(Arena *a, IRBlock *bb, int size, int align)
{
    IRType ptr_ty = { IR_TYPE_PTR, 8, 8 };
    IRInst *inst = ir_inst_new(a, bb, IR_ALLOCA, ptr_ty);
    inst->alloca_size = size;
    inst->alloca_align = align;
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_load(Arena *a, IRBlock *bb, IRType ty, IRInst *ptr)
{
    IRInst *inst = ir_inst_new(a, bb, IR_LOAD, ty);
    IRInst *ops[] = { ptr };
    set_operands(a, inst, ops, 1);
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_store(Arena *a, IRBlock *bb, IRInst *val, IRInst *ptr)
{
    IRType void_ty = { IR_TYPE_VOID, 0, 0 };
    IRInst *inst = ir_inst_new(a, bb, IR_STORE, void_ty);
    IRInst *ops[] = { val, ptr };
    set_operands(a, inst, ops, 2);
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_memcpy(Arena *a, IRBlock *bb, IRInst *dst, IRInst *src, int size)
{
    IRType void_ty = { IR_TYPE_VOID, 0, 0 };
    IRInst *inst = ir_inst_new(a, bb, IR_MEMCPY, void_ty);
    inst->ival = size;
    IRInst *ops[] = { dst, src };
    set_operands(a, inst, ops, 2);
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_gep(Arena *a, IRBlock *bb, IRInst *ptr, int offset)
{
    IRType ptr_ty = { IR_TYPE_PTR, 8, 8 };
    IRInst *inst = ir_inst_new(a, bb, IR_GEP, ptr_ty);
    inst->gep_offset = offset;
    IRInst *ops[] = { ptr };
    set_operands(a, inst, ops, 1);
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_binop(Arena *a, IRBlock *bb, IROpcode op, IRInst *lhs, IRInst *rhs)
{
    /* Result type: comparisons produce i1, arithmetic inherits from lhs
     * with C integer promotion (result is at least i32) */
    IRType ty;
    if (op >= IR_ICMP_EQ && op <= IR_FCMP_OGE) {
        ty = (IRType){ IR_TYPE_I1, 1, 1 };
    } else {
        ty = lhs->ty;
        /* Integer promotion: result of integer arithmetic is at least 4 bytes */
        if (ty.kind != IR_TYPE_F32 && ty.kind != IR_TYPE_F64 &&
            ty.kind != IR_TYPE_PTR && ty.kind != IR_TYPE_AGGREGATE &&
            ty.size < 4) {
            ty = (IRType){ IR_TYPE_I32, 4, 4 };
        }
    }

    IRInst *inst = ir_inst_new(a, bb, op, ty);
    IRInst *ops[] = { lhs, rhs };
    set_operands(a, inst, ops, 2);
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_unop(Arena *a, IRBlock *bb, IROpcode op, IRInst *operand)
{
    IRInst *inst = ir_inst_new(a, bb, op, operand->ty);
    IRInst *ops[] = { operand };
    set_operands(a, inst, ops, 1);
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_cast(Arena *a, IRBlock *bb, IROpcode op, IRType target, IRInst *src)
{
    IRInst *inst = ir_inst_new(a, bb, op, target);
    IRInst *ops[] = { src };
    set_operands(a, inst, ops, 1);
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_call(Arena *a, IRBlock *bb, IRType ret_ty, const char *target,
                      IRInst **args, int nargs)
{
    IRInst *inst = ir_inst_new(a, bb, IR_CALL, ret_ty);
    inst->call_target = target;
    set_operands(a, inst, args, nargs);
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_br(Arena *a, IRBlock *bb, IRBlock *target)
{
    IRType void_ty = { IR_TYPE_VOID, 0, 0 };
    IRInst *inst = ir_inst_new(a, bb, IR_BR, void_ty);
    inst->br_target = target;
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_condbr(Arena *a, IRBlock *bb, IRInst *cond,
                        IRBlock *true_bb, IRBlock *false_bb)
{
    IRType void_ty = { IR_TYPE_VOID, 0, 0 };
    IRInst *inst = ir_inst_new(a, bb, IR_CONDBR, void_ty);
    inst->br_true = true_bb;
    inst->br_false = false_bb;
    IRInst *ops[] = { cond };
    set_operands(a, inst, ops, 1);
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_ret(Arena *a, IRBlock *bb, IRInst *val)
{
    IRInst *inst = ir_inst_new(a, bb, IR_RET, val->ty);
    IRInst *ops[] = { val };
    set_operands(a, inst, ops, 1);
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_retv(Arena *a, IRBlock *bb)
{
    IRType void_ty = { IR_TYPE_VOID, 0, 0 };
    IRInst *inst = ir_inst_new(a, bb, IR_RETV, void_ty);
    ir_block_append(bb, inst);
    return inst;
}

IRInst *ir_build_phi(Arena *a, IRBlock *bb, IRType ty)
{
    IRInst *inst = ir_inst_new(a, bb, IR_PHI, ty);
    inst->phi_vals = NULL;
    inst->phi_blocks = NULL;
    inst->phi_count = 0;
    ir_block_append(bb, inst);
    return inst;
}

void ir_phi_add(Arena *a, IRInst *phi, IRInst *val, IRBlock *pred)
{
    assert(phi->op == IR_PHI);
    int n = phi->phi_count + 1;

    /* Reallocate arrays using arena. We copy old data to new arrays. */
    IRInst **new_vals = arena_alloc(a, sizeof(IRInst *) * n);
    IRBlock **new_blocks = arena_alloc(a, sizeof(IRBlock *) * n);

    if (phi->phi_count > 0) {
        memcpy(new_vals, phi->phi_vals, sizeof(IRInst *) * phi->phi_count);
        memcpy(new_blocks, phi->phi_blocks, sizeof(IRBlock *) * phi->phi_count);
    }

    new_vals[phi->phi_count] = val;
    new_blocks[phi->phi_count] = pred;

    phi->phi_vals = new_vals;
    phi->phi_blocks = new_blocks;
    phi->phi_count = n;

    add_use(val, phi);
}

/* ================================================================
 * IR Printer
 * ================================================================ */

static void print_value(IRInst *inst, FILE *out)
{
    if (!inst) {
        fprintf(out, "undef");
        return;
    }
    if (inst->op == IR_CONST_INT) {
        fprintf(out, "%lld", (long long)inst->ival);
    } else if (inst->op == IR_CONST_FLOAT) {
        fprintf(out, "%g", inst->fval);
    } else {
        fprintf(out, "%%%d", inst->id);
    }
}

static void print_type(IRType ty, FILE *out)
{
    fprintf(out, "%s", ir_type_name(ty));
}

static bool is_terminator(IROpcode op)
{
    return op == IR_BR || op == IR_CONDBR || op == IR_RET || op == IR_RETV;
}

static void print_inst(IRInst *inst, FILE *out)
{
    /* Terminators and stores don't produce a value */
    bool has_result = !is_terminator(inst->op) && inst->op != IR_STORE;

    if (has_result) {
        fprintf(out, "  %%%d = ", inst->id);
    } else {
        fprintf(out, "  ");
    }

    fprintf(out, "%s ", opcode_names[inst->op] ? opcode_names[inst->op] : "???");

    switch (inst->op) {
    case IR_CONST_INT:
        print_type(inst->ty, out);
        fprintf(out, " %lld", (long long)inst->ival);
        break;

    case IR_CONST_FLOAT:
        print_type(inst->ty, out);
        fprintf(out, " %g", inst->fval);
        break;

    case IR_ALLOCA:
        fprintf(out, "size=%d, align=%d", inst->alloca_size, inst->alloca_align);
        break;

    case IR_LOAD:
        print_type(inst->ty, out);
        fprintf(out, ", ");
        print_value(inst->operands[0], out);
        break;

    case IR_STORE:
        print_value(inst->operands[0], out);
        fprintf(out, ", ");
        print_value(inst->operands[1], out);
        break;

    case IR_GEP:
        print_value(inst->operands[0], out);
        fprintf(out, ", offset %d", inst->gep_offset);
        break;

    case IR_ADD: case IR_SUB: case IR_MUL:
    case IR_SDIV: case IR_UDIV: case IR_SREM: case IR_UREM:
    case IR_FADD: case IR_FSUB: case IR_FMUL: case IR_FDIV:
    case IR_AND: case IR_OR: case IR_XOR:
    case IR_SHL: case IR_LSHR: case IR_ASHR:
    case IR_ICMP_EQ: case IR_ICMP_NE:
    case IR_ICMP_SLT: case IR_ICMP_SLE:
    case IR_ICMP_SGT: case IR_ICMP_SGE:
    case IR_ICMP_ULT: case IR_ICMP_ULE:
    case IR_ICMP_UGT: case IR_ICMP_UGE:
    case IR_FCMP_OEQ: case IR_FCMP_ONE:
    case IR_FCMP_OLT: case IR_FCMP_OLE:
    case IR_FCMP_OGT: case IR_FCMP_OGE:
        print_type(inst->ty, out);
        fprintf(out, " ");
        print_value(inst->operands[0], out);
        fprintf(out, ", ");
        print_value(inst->operands[1], out);
        break;

    case IR_NEG: case IR_FNEG: case IR_NOT:
        print_type(inst->ty, out);
        fprintf(out, " ");
        print_value(inst->operands[0], out);
        break;

    case IR_SEXT: case IR_ZEXT: case IR_TRUNC:
    case IR_SITOFP: case IR_UITOFP:
    case IR_FPTOSI: case IR_FPTOUI:
    case IR_FPEXT: case IR_FPTRUNC:
    case IR_BITCAST: case IR_PTRTOINT: case IR_INTTOPTR:
        print_value(inst->operands[0], out);
        fprintf(out, " to ");
        print_type(inst->ty, out);
        break;

    case IR_BR:
        fprintf(out, "label %%%s", inst->br_target->name);
        break;

    case IR_CONDBR:
        print_value(inst->operands[0], out);
        fprintf(out, ", label %%%s, label %%%s",
                inst->br_true->name, inst->br_false->name);
        break;

    case IR_RET:
        print_type(inst->ty, out);
        fprintf(out, " ");
        print_value(inst->operands[0], out);
        break;

    case IR_RETV:
        fprintf(out, "void");
        break;

    case IR_CALL:
        print_type(inst->ty, out);
        fprintf(out, " @%s(", inst->call_target ? inst->call_target : "???");
        for (int i = 0; i < inst->num_operands; i++) {
            if (i > 0) fprintf(out, ", ");
            print_type(inst->operands[i]->ty, out);
            fprintf(out, " ");
            print_value(inst->operands[i], out);
        }
        fprintf(out, ")");
        break;

    case IR_PHI:
        print_type(inst->ty, out);
        for (int i = 0; i < inst->phi_count; i++) {
            fprintf(out, " [");
            print_value(inst->phi_vals[i], out);
            fprintf(out, ", %%%s]", inst->phi_blocks[i]->name);
            if (i + 1 < inst->phi_count) fprintf(out, ",");
        }
        break;

    case IR_COPY:
        print_type(inst->ty, out);
        fprintf(out, " ");
        if (inst->num_operands > 0)
            print_value(inst->operands[0], out);
        break;

    case IR_MEMCPY:
        /* memcpy dest, src, size */
        if (inst->num_operands >= 2) {
            print_value(inst->operands[0], out);
            fprintf(out, ", ");
            print_value(inst->operands[1], out);
            fprintf(out, ", %lld", (long long)inst->ival);
        }
        break;

    case IR_MEMSET:
        /* memset dest, val, size */
        if (inst->num_operands >= 2) {
            print_value(inst->operands[0], out);
            fprintf(out, ", ");
            print_value(inst->operands[1], out);
            fprintf(out, ", %lld", (long long)inst->ival);
        }
        break;

    default:
        fprintf(out, "???");
        break;
    }

    fprintf(out, "\n");
}

void ir_print_func(IRFunc *func, FILE *out)
{
    fprintf(out, "define ");
    print_type(func->ret_type, out);
    fprintf(out, " @%s(", func->name);

    for (int i = 0; i < func->num_params; i++) {
        if (i > 0) fprintf(out, ", ");
        print_type(func->params[i]->ty, out);
        fprintf(out, " %%%d", func->params[i]->id);
    }

    fprintf(out, ") {\n");

    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        fprintf(out, "%s:", bb->name);

        /* Print predecessor info as a comment */
        if (bb->preds.len > 0) {
            fprintf(out, "                              ; preds:");
            for (int i = 0; i < bb->preds.len; i++) {
                fprintf(out, " %%%s", bb->preds.data[i]->name);
                if (i + 1 < bb->preds.len) fprintf(out, ",");
            }
        }
        fprintf(out, "\n");

        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            print_inst(inst, out);
        }

        fprintf(out, "\n");
    }

    fprintf(out, "}\n\n");
}

void ir_print_module(IRModule *mod, FILE *out)
{
    /* Print globals */
    for (IRGlobal *g = mod->globals; g; g = g->next) {
        if (g->is_extern) {
            fprintf(out, "declare @%s\n", g->name);
        } else if (g->is_string) {
            fprintf(out, "@%s = constant [%d x i8] \"", g->name, g->string_len);
            for (int i = 0; i < g->string_len; i++) {
                char c = g->string_val[i];
                if (c >= 32 && c < 127 && c != '"' && c != '\\') {
                    fputc(c, out);
                } else {
                    fprintf(out, "\\%02x", (unsigned char)c);
                }
            }
            fprintf(out, "\"\n");
        } else if (g->has_init) {
            fprintf(out, "@%s = global %s %lld\n", g->name,
                    ir_type_name(g->ty), (long long)g->init_val);
        } else {
            fprintf(out, "@%s = global %s\n", g->name, ir_type_name(g->ty));
        }
    }

    if (mod->globals) fprintf(out, "\n");

    /* Print functions */
    for (IRFunc *func = mod->funcs; func; func = func->next) {
        ir_print_func(func, out);
    }
}

/* ================================================================
 * IR Verifier
 * ================================================================ */

static bool verify_block_terminates(IRFunc *func, IRBlock *bb, FILE *err)
{
    if (!bb->last) {
        fprintf(err, "verify: block '%s' in @%s has no instructions\n",
                bb->name, func->name);
        return false;
    }
    if (!is_terminator(bb->last->op)) {
        fprintf(err, "verify: block '%s' in @%s does not end with terminator (ends with %s)\n",
                bb->name, func->name,
                opcode_names[bb->last->op] ? opcode_names[bb->last->op] : "???");
        return false;
    }

    /* Check no terminator appears before the last instruction */
    for (IRInst *inst = bb->first; inst && inst != bb->last; inst = inst->next) {
        if (is_terminator(inst->op)) {
            fprintf(err, "verify: block '%s' in @%s has terminator %%%d not at end\n",
                    bb->name, func->name, inst->id);
            return false;
        }
    }

    return true;
}

/* Check that all operand uses are defined before use within the same function.
 * In SSA form, a definition dominates all its uses. For pre-SSA IR with allocas,
 * we relax this to just checking that the operand belongs to the same function. */
static bool verify_operands(IRFunc *func, IRBlock *bb, IRInst *inst, FILE *err)
{
    for (int i = 0; i < inst->num_operands; i++) {
        IRInst *op = inst->operands[i];
        if (!op) {
            fprintf(err, "verify: %%%d in @%s block '%s' has NULL operand %d\n",
                    inst->id, func->name, bb->name, i);
            return false;
        }
        /* Check the operand belongs to the same function */
        if (op->parent && op->parent->parent != func) {
            fprintf(err, "verify: %%%d in @%s uses %%%d from different function\n",
                    inst->id, func->name, op->id);
            return false;
        }
    }

    /* Verify PHI incoming blocks */
    if (inst->op == IR_PHI) {
        for (int i = 0; i < inst->phi_count; i++) {
            if (!inst->phi_vals[i]) {
                fprintf(err, "verify: phi %%%d in @%s has NULL val at index %d\n",
                        inst->id, func->name, i);
                return false;
            }
            if (!inst->phi_blocks[i]) {
                fprintf(err, "verify: phi %%%d in @%s has NULL block at index %d\n",
                        inst->id, func->name, i);
                return false;
            }
        }
    }

    /* Verify branch targets */
    if (inst->op == IR_BR) {
        if (!inst->br_target) {
            fprintf(err, "verify: br %%%d in @%s has NULL target\n",
                    inst->id, func->name);
            return false;
        }
        if (inst->br_target->parent != func) {
            fprintf(err, "verify: br %%%d in @%s targets block in different function\n",
                    inst->id, func->name);
            return false;
        }
    }

    if (inst->op == IR_CONDBR) {
        if (!inst->br_true || !inst->br_false) {
            fprintf(err, "verify: condbr %%%d in @%s has NULL branch target\n",
                    inst->id, func->name);
            return false;
        }
        if (inst->br_true->parent != func || inst->br_false->parent != func) {
            fprintf(err, "verify: condbr %%%d in @%s targets block in different function\n",
                    inst->id, func->name);
            return false;
        }
    }

    return true;
}

/* Check that binops have the right number of operands and matching types */
static bool verify_types(IRFunc *func, IRInst *inst, FILE *err)
{
    switch (inst->op) {
    case IR_ADD: case IR_SUB: case IR_MUL:
    case IR_SDIV: case IR_UDIV: case IR_SREM: case IR_UREM:
    case IR_FADD: case IR_FSUB: case IR_FMUL: case IR_FDIV:
    case IR_AND: case IR_OR: case IR_XOR:
    case IR_SHL: case IR_LSHR: case IR_ASHR:
        if (inst->num_operands != 2) {
            fprintf(err, "verify: %%%d in @%s: binop has %d operands (expected 2)\n",
                    inst->id, func->name, inst->num_operands);
            return false;
        }
        break;

    case IR_ICMP_EQ: case IR_ICMP_NE:
    case IR_ICMP_SLT: case IR_ICMP_SLE:
    case IR_ICMP_SGT: case IR_ICMP_SGE:
    case IR_ICMP_ULT: case IR_ICMP_ULE:
    case IR_ICMP_UGT: case IR_ICMP_UGE:
    case IR_FCMP_OEQ: case IR_FCMP_ONE:
    case IR_FCMP_OLT: case IR_FCMP_OLE:
    case IR_FCMP_OGT: case IR_FCMP_OGE:
        if (inst->num_operands != 2) {
            fprintf(err, "verify: %%%d in @%s: cmp has %d operands (expected 2)\n",
                    inst->id, func->name, inst->num_operands);
            return false;
        }
        break;

    case IR_NEG: case IR_FNEG: case IR_NOT:
        if (inst->num_operands != 1) {
            fprintf(err, "verify: %%%d in @%s: unop has %d operands (expected 1)\n",
                    inst->id, func->name, inst->num_operands);
            return false;
        }
        break;

    case IR_LOAD:
        if (inst->num_operands != 1) {
            fprintf(err, "verify: %%%d in @%s: load has %d operands (expected 1)\n",
                    inst->id, func->name, inst->num_operands);
            return false;
        }
        break;

    case IR_STORE:
        if (inst->num_operands != 2) {
            fprintf(err, "verify: %%%d in @%s: store has %d operands (expected 2)\n",
                    inst->id, func->name, inst->num_operands);
            return false;
        }
        break;

    case IR_RET:
        if (inst->num_operands != 1) {
            fprintf(err, "verify: %%%d in @%s: ret has %d operands (expected 1)\n",
                    inst->id, func->name, inst->num_operands);
            return false;
        }
        break;

    case IR_CONDBR:
        if (inst->num_operands != 1) {
            fprintf(err, "verify: %%%d in @%s: condbr has %d operands (expected 1)\n",
                    inst->id, func->name, inst->num_operands);
            return false;
        }
        break;

    default:
        break;
    }

    return true;
}

bool ir_verify_func(IRFunc *func)
{
    FILE *err = stderr;
    bool ok = true;

    if (!func->entry) {
        fprintf(err, "verify: @%s has no entry block\n", func->name);
        return false;
    }

    if (func->block_count == 0) {
        fprintf(err, "verify: @%s has block_count==0 but has entry block\n", func->name);
        return false;
    }

    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        /* Each block must have at least one instruction (the terminator) */
        if (!verify_block_terminates(func, bb, err))
            ok = false;

        /* Check each instruction */
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            if (inst->parent != bb) {
                fprintf(err, "verify: %%%d parent mismatch in @%s\n",
                        inst->id, func->name);
                ok = false;
            }
            if (!verify_operands(func, bb, inst, err))
                ok = false;
            if (!verify_types(func, inst, err))
                ok = false;
        }
    }

    return ok;
}
