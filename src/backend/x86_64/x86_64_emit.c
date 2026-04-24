/*
 * x86_64_emit.c - AT&T syntax assembly emission for x86-64
 *
 * This is the main code-generation driver.  It walks the IR module,
 * runs register allocation on each function, then emits AT&T-syntax
 * x86-64 assembly suitable for GNU as / clang integrated assembler.
 *
 * Strategy (naive, spill-everything):
 *   - Every IR value is assigned a unique stack slot by regalloc.
 *   - For each IR instruction we load operands into scratch registers
 *     (%rax, %rcx, %rdx, %xmm0, %xmm1), perform the operation, and
 *     store the result back to the stack.
 *   - Function calls follow the System V AMD64 ABI.
 */

#include "ir.h"
#include "target.h"
#include "arena.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Imports from x86_64_isel.c                                        */
/* ------------------------------------------------------------------ */

extern char        x86_isel_suffix(IRType ty);
extern int         x86_isel_slot_size(IRType ty);
extern const char *x86_isel_reg64(int reg);
extern const char *x86_isel_reg32(int reg);
extern const char *x86_isel_reg16(int reg);
extern const char *x86_isel_reg8(int reg);
extern const char *x86_isel_regname(int reg, IRType ty);
extern const char *x86_isel_xmm(int reg);
extern bool        x86_isel_is_float(IRType ty);
extern bool        x86_isel_is_int(IRType ty);
extern int         x86_isel_arg_int_reg(int idx);
extern int         x86_isel_arg_float_reg(int idx);
extern const char *x86_isel_cc(IROpcode op);
extern bool        x86_isel_is_icmp(IROpcode op);
extern bool        x86_isel_is_fcmp(IROpcode op);
extern bool        x86_isel_is_cmp(IROpcode op);

/* x86-64 register IDs (must stay in sync with x86_64_target.c) */
enum {
    X86_RAX = 0, X86_RBX, X86_RCX, X86_RDX,
    X86_RSI, X86_RDI, X86_RBP, X86_RSP,
    X86_R8, X86_R9, X86_R10, X86_R11,
    X86_R12, X86_R13, X86_R14, X86_R15,
};

/* ------------------------------------------------------------------ */
/*  Tiny helpers                                                      */
/* ------------------------------------------------------------------ */

/* Align value up to 'align' (must be power of 2). */
static int align_up(int value, int align)
{
    return (value + align - 1) & ~(align - 1);
}

/* Emit a label for a basic block.  Format: .LBB<func_id>_<block_id>  */
static void emit_label(FILE *out, IRFunc *func, IRBlock *bb)
{
    (void)func;
    fprintf(out, ".LBB%s_%d:\n", func->name, bb->id);
}

/* Return the block label string (callers use it in jump targets). */
static void emit_block_ref(FILE *out, IRFunc *func, IRBlock *bb)
{
    fprintf(out, ".LBB%s_%d", func->name, bb->id);
}

/* Memory operand for an instruction's stack slot: offset(%rbp). */
static void emit_slot(FILE *out, IRInst *inst)
{
    assert(inst->stack_slot != 0 && "value has no stack slot assigned");
    fprintf(out, "%d(%%rbp)", inst->stack_slot);
}

/* ------------------------------------------------------------------ */
/*  Load an IR value into a scratch register                          */
/* ------------------------------------------------------------------ */

/* Load an integer/pointer value into the given GPR.
 * If the value is a constant, emit a movq $imm, %reg.
 * Otherwise emit a movq slot(%rbp), %reg.                           */
static void emit_load_int(FILE *out, IRInst *val, int reg)
{
    if (val->op == IR_CONST_INT) {
        fprintf(out, "    movq $%" PRId64 ", %%%s\n", val->ival,
                x86_isel_reg64(reg));
    } else {
        /* Load from stack slot.  Always load 64 bits then narrow as needed
         * by using the sub-register in the actual operation. */
        fprintf(out, "    movq %d(%%rbp), %%%s\n", val->stack_slot,
                x86_isel_reg64(reg));
    }
}

/* Load a float/double into the given XMM register. */
static void emit_load_float(FILE *out, IRInst *val, int xmm)
{
    if (val->ty.kind == IR_TYPE_F32) {
        fprintf(out, "    movss %d(%%rbp), %%%s\n", val->stack_slot,
                x86_isel_xmm(xmm));
    } else {
        fprintf(out, "    movsd %d(%%rbp), %%%s\n", val->stack_slot,
                x86_isel_xmm(xmm));
    }
}

/* Store a GPR to the instruction's stack slot. */
static void emit_store_int(FILE *out, IRInst *inst, int reg)
{
    fprintf(out, "    movq %%%s, %d(%%rbp)\n", x86_isel_reg64(reg),
            inst->stack_slot);
}

/* Store an XMM register to the instruction's stack slot. */
static void emit_store_float(FILE *out, IRInst *inst, int xmm)
{
    if (inst->ty.kind == IR_TYPE_F32) {
        fprintf(out, "    movss %%%s, %d(%%rbp)\n", x86_isel_xmm(xmm),
                inst->stack_slot);
    } else {
        fprintf(out, "    movsd %%%s, %d(%%rbp)\n", x86_isel_xmm(xmm),
                inst->stack_slot);
    }
}

/* ------------------------------------------------------------------ */
/*  Integer binary operations                                         */
/* ------------------------------------------------------------------ */

static void emit_int_binop(FILE *out, IRInst *inst)
{
    IRInst *lhs = inst->operands[0];
    IRInst *rhs = inst->operands[1];

    /* Load lhs -> %rax, rhs -> %rcx */
    emit_load_int(out, lhs, X86_RAX);
    emit_load_int(out, rhs, X86_RCX);

    switch (inst->op) {
    case IR_ADD:
        fprintf(out, "    addq %%rcx, %%rax\n");
        break;
    case IR_SUB:
        fprintf(out, "    subq %%rcx, %%rax\n");
        break;
    case IR_MUL:
        fprintf(out, "    imulq %%rcx, %%rax\n");
        break;
    case IR_SDIV:
        fprintf(out, "    cqto\n");             /* sign-extend rax -> rdx:rax */
        fprintf(out, "    idivq %%rcx\n");      /* result in rax */
        break;
    case IR_UDIV:
        fprintf(out, "    xorq %%rdx, %%rdx\n"); /* zero-extend */
        fprintf(out, "    divq %%rcx\n");
        break;
    case IR_SREM:
        fprintf(out, "    cqto\n");
        fprintf(out, "    idivq %%rcx\n");
        /* Remainder is in %rdx, move to %rax for storage. */
        fprintf(out, "    movq %%rdx, %%rax\n");
        break;
    case IR_UREM:
        fprintf(out, "    xorq %%rdx, %%rdx\n");
        fprintf(out, "    divq %%rcx\n");
        fprintf(out, "    movq %%rdx, %%rax\n");
        break;
    case IR_AND:
        fprintf(out, "    andq %%rcx, %%rax\n");
        break;
    case IR_OR:
        fprintf(out, "    orq %%rcx, %%rax\n");
        break;
    case IR_XOR:
        fprintf(out, "    xorq %%rcx, %%rax\n");
        break;
    case IR_SHL:
        /* Shift amount must be in %cl. */
        fprintf(out, "    shlq %%cl, %%rax\n");
        break;
    case IR_LSHR:
        fprintf(out, "    shrq %%cl, %%rax\n");
        break;
    case IR_ASHR:
        fprintf(out, "    sarq %%cl, %%rax\n");
        break;
    default:
        assert(0 && "unhandled integer binop");
    }

    /* Store result from %rax to slot. */
    emit_store_int(out, inst, X86_RAX);
}

/* ------------------------------------------------------------------ */
/*  Floating-point binary operations                                  */
/* ------------------------------------------------------------------ */

static void emit_float_binop(FILE *out, IRInst *inst)
{
    IRInst *lhs = inst->operands[0];
    IRInst *rhs = inst->operands[1];
    bool is_f32 = (inst->ty.kind == IR_TYPE_F32);
    const char *s = is_f32 ? "ss" : "sd";

    emit_load_float(out, lhs, 0);   /* xmm0 */
    emit_load_float(out, rhs, 1);   /* xmm1 */

    switch (inst->op) {
    case IR_FADD: fprintf(out, "    add%s %%xmm1, %%xmm0\n", s); break;
    case IR_FSUB: fprintf(out, "    sub%s %%xmm1, %%xmm0\n", s); break;
    case IR_FMUL: fprintf(out, "    mul%s %%xmm1, %%xmm0\n", s); break;
    case IR_FDIV: fprintf(out, "    div%s %%xmm1, %%xmm0\n", s); break;
    default:      assert(0 && "unhandled float binop");
    }

    emit_store_float(out, inst, 0);
}

/* ------------------------------------------------------------------ */
/*  Unary operations                                                  */
/* ------------------------------------------------------------------ */

static void emit_unop(FILE *out, IRInst *inst)
{
    IRInst *operand = inst->operands[0];

    switch (inst->op) {
    case IR_NEG:
        emit_load_int(out, operand, X86_RAX);
        fprintf(out, "    negq %%rax\n");
        emit_store_int(out, inst, X86_RAX);
        break;

    case IR_NOT:
        emit_load_int(out, operand, X86_RAX);
        fprintf(out, "    notq %%rax\n");
        emit_store_int(out, inst, X86_RAX);
        break;

    case IR_FNEG: {
        bool is_f32 = (inst->ty.kind == IR_TYPE_F32);
        emit_load_float(out, operand, 0);
        /* XOR with sign-bit mask.  We build the mask in a GPR, push to
         * the stack, then load into xmm1. */
        if (is_f32) {
            fprintf(out, "    movl $0x80000000, %%eax\n");
            fprintf(out, "    movd %%eax, %%xmm1\n");
            fprintf(out, "    xorps %%xmm1, %%xmm0\n");
        } else {
            fprintf(out, "    movabsq $0x8000000000000000, %%rax\n");
            fprintf(out, "    movq %%rax, %%xmm1\n");
            fprintf(out, "    xorpd %%xmm1, %%xmm0\n");
        }
        emit_store_float(out, inst, 0);
        break;
    }

    default:
        assert(0 && "unhandled unary op");
    }
}

/* ------------------------------------------------------------------ */
/*  Comparison                                                        */
/* ------------------------------------------------------------------ */

static void emit_cmp(FILE *out, IRInst *inst)
{
    IRInst *lhs = inst->operands[0];
    IRInst *rhs = inst->operands[1];
    const char *cc = x86_isel_cc(inst->op);

    if (x86_isel_is_icmp(inst->op)) {
        emit_load_int(out, lhs, X86_RAX);
        emit_load_int(out, rhs, X86_RCX);
        fprintf(out, "    cmpq %%rcx, %%rax\n");
        fprintf(out, "    set%s %%al\n", cc);
        fprintf(out, "    movzbq %%al, %%rax\n");
    } else {
        /* Float comparison */
        bool is_f32 = (lhs->ty.kind == IR_TYPE_F32);
        emit_load_float(out, lhs, 0);
        emit_load_float(out, rhs, 1);
        if (is_f32)
            fprintf(out, "    ucomiss %%xmm1, %%xmm0\n");
        else
            fprintf(out, "    ucomisd %%xmm1, %%xmm0\n");
        fprintf(out, "    set%s %%al\n", cc);
        /* For ordered comparisons, also check parity flag (NaN). */
        fprintf(out, "    setnp %%cl\n");
        fprintf(out, "    andb %%cl, %%al\n");
        fprintf(out, "    movzbq %%al, %%rax\n");
    }

    emit_store_int(out, inst, X86_RAX);
}

/* ------------------------------------------------------------------ */
/*  Conversion / cast                                                 */
/* ------------------------------------------------------------------ */

static void emit_cast(FILE *out, IRInst *inst)
{
    IRInst *src = inst->operands[0];

    switch (inst->op) {
    case IR_SEXT:
        emit_load_int(out, src, X86_RAX);
        /* movsx from source width.  Since we always load 64 bits and operate
         * on the full register, sign extension from narrower types is needed. */
        switch (src->ty.kind) {
        case IR_TYPE_I1:
        case IR_TYPE_I8:
            fprintf(out, "    movsbq %%al, %%rax\n");
            break;
        case IR_TYPE_I16:
            fprintf(out, "    movswq %%ax, %%rax\n");
            break;
        case IR_TYPE_I32:
            fprintf(out, "    movslq %%eax, %%rax\n");
            break;
        default:
            break; /* already 64-bit, nothing to do */
        }
        emit_store_int(out, inst, X86_RAX);
        break;

    case IR_ZEXT:
        emit_load_int(out, src, X86_RAX);
        switch (src->ty.kind) {
        case IR_TYPE_I1:
        case IR_TYPE_I8:
            fprintf(out, "    movzbq %%al, %%rax\n");
            break;
        case IR_TYPE_I16:
            fprintf(out, "    movzwq %%ax, %%rax\n");
            break;
        case IR_TYPE_I32:
            /* Writing to a 32-bit register implicitly zero-extends. */
            fprintf(out, "    movl %%eax, %%eax\n");
            break;
        default:
            break;
        }
        emit_store_int(out, inst, X86_RAX);
        break;

    case IR_TRUNC:
        emit_load_int(out, src, X86_RAX);
        /* Truncation is implicit: we just store fewer bytes.
         * But since our slots are 8 bytes, we mask. */
        switch (inst->ty.kind) {
        case IR_TYPE_I1:
            fprintf(out, "    andq $1, %%rax\n");
            break;
        case IR_TYPE_I8:
            fprintf(out, "    andq $0xff, %%rax\n");
            break;
        case IR_TYPE_I16:
            fprintf(out, "    andq $0xffff, %%rax\n");
            break;
        case IR_TYPE_I32:
            fprintf(out, "    movl %%eax, %%eax\n"); /* zero-extends */
            break;
        default:
            break;
        }
        emit_store_int(out, inst, X86_RAX);
        break;

    case IR_SITOFP:
        emit_load_int(out, src, X86_RAX);
        if (inst->ty.kind == IR_TYPE_F32)
            fprintf(out, "    cvtsi2ssq %%rax, %%xmm0\n");
        else
            fprintf(out, "    cvtsi2sdq %%rax, %%xmm0\n");
        emit_store_float(out, inst, 0);
        break;

    case IR_UITOFP:
        /* Simple path: treat as signed 64-bit (correct for values < 2^63). */
        emit_load_int(out, src, X86_RAX);
        if (inst->ty.kind == IR_TYPE_F32)
            fprintf(out, "    cvtsi2ssq %%rax, %%xmm0\n");
        else
            fprintf(out, "    cvtsi2sdq %%rax, %%xmm0\n");
        emit_store_float(out, inst, 0);
        break;

    case IR_FPTOSI:
        emit_load_float(out, src, 0);
        if (src->ty.kind == IR_TYPE_F32)
            fprintf(out, "    cvttss2siq %%xmm0, %%rax\n");
        else
            fprintf(out, "    cvttsd2siq %%xmm0, %%rax\n");
        emit_store_int(out, inst, X86_RAX);
        break;

    case IR_FPTOUI:
        emit_load_float(out, src, 0);
        if (src->ty.kind == IR_TYPE_F32)
            fprintf(out, "    cvttss2siq %%xmm0, %%rax\n");
        else
            fprintf(out, "    cvttsd2siq %%xmm0, %%rax\n");
        emit_store_int(out, inst, X86_RAX);
        break;

    case IR_FPEXT:
        emit_load_float(out, src, 0);
        fprintf(out, "    cvtss2sd %%xmm0, %%xmm0\n");
        emit_store_float(out, inst, 0);
        break;

    case IR_FPTRUNC:
        emit_load_float(out, src, 0);
        fprintf(out, "    cvtsd2ss %%xmm0, %%xmm0\n");
        emit_store_float(out, inst, 0);
        break;

    case IR_BITCAST:
    case IR_PTRTOINT:
    case IR_INTTOPTR:
        /* All the same bit-pattern, just copy the slot. */
        if (x86_isel_is_float(src->ty) && x86_isel_is_int(inst->ty)) {
            emit_load_float(out, src, 0);
            fprintf(out, "    movq %%xmm0, %%rax\n");
            emit_store_int(out, inst, X86_RAX);
        } else if (x86_isel_is_int(src->ty) && x86_isel_is_float(inst->ty)) {
            emit_load_int(out, src, X86_RAX);
            fprintf(out, "    movq %%rax, %%xmm0\n");
            emit_store_float(out, inst, 0);
        } else {
            emit_load_int(out, src, X86_RAX);
            emit_store_int(out, inst, X86_RAX);
        }
        break;

    default:
        assert(0 && "unhandled cast op");
    }
}

/* ------------------------------------------------------------------ */
/*  Memory operations                                                 */
/* ------------------------------------------------------------------ */

static void emit_alloca(FILE *out, IRInst *inst)
{
    /* Alloca values are represented as the address of their stack slot.
     * We compute the address with leaq. */
    fprintf(out, "    leaq %d(%%rbp), %%rax\n", inst->stack_slot);
    emit_store_int(out, inst, X86_RAX);
}

static void emit_load(FILE *out, IRInst *inst)
{
    IRInst *ptr = inst->operands[0];

    /* Load the pointer value. */
    emit_load_int(out, ptr, X86_RAX);

    if (x86_isel_is_float(inst->ty)) {
        if (inst->ty.kind == IR_TYPE_F32)
            fprintf(out, "    movss (%%rax), %%xmm0\n");
        else
            fprintf(out, "    movsd (%%rax), %%xmm0\n");
        emit_store_float(out, inst, 0);
    } else {
        /* Emit a sized load. */
        switch (inst->ty.kind) {
        case IR_TYPE_I1:
        case IR_TYPE_I8:
            fprintf(out, "    movzbq (%%rax), %%rax\n");
            break;
        case IR_TYPE_I16:
            fprintf(out, "    movzwq (%%rax), %%rax\n");
            break;
        case IR_TYPE_I32:
            fprintf(out, "    movl (%%rax), %%eax\n");
            break;
        default:
            fprintf(out, "    movq (%%rax), %%rax\n");
            break;
        }
        emit_store_int(out, inst, X86_RAX);
    }
}

static void emit_store(FILE *out, IRInst *inst)
{
    IRInst *val = inst->operands[0];
    IRInst *ptr = inst->operands[1];

    /* Load pointer into %rcx (not %rax, since we need %rax for the value). */
    emit_load_int(out, ptr, X86_RCX);

    if (x86_isel_is_float(val->ty)) {
        emit_load_float(out, val, 0);
        if (val->ty.kind == IR_TYPE_F32)
            fprintf(out, "    movss %%xmm0, (%%rcx)\n");
        else
            fprintf(out, "    movsd %%xmm0, (%%rcx)\n");
    } else {
        emit_load_int(out, val, X86_RAX);
        switch (val->ty.kind) {
        case IR_TYPE_I1:
        case IR_TYPE_I8:
            fprintf(out, "    movb %%al, (%%rcx)\n");
            break;
        case IR_TYPE_I16:
            fprintf(out, "    movw %%ax, (%%rcx)\n");
            break;
        case IR_TYPE_I32:
            fprintf(out, "    movl %%eax, (%%rcx)\n");
            break;
        default:
            fprintf(out, "    movq %%rax, (%%rcx)\n");
            break;
        }
    }
}

static void emit_gep(FILE *out, IRInst *inst)
{
    IRInst *base = inst->operands[0];

    emit_load_int(out, base, X86_RAX);
    if (inst->gep_offset != 0) {
        fprintf(out, "    addq $%d, %%rax\n", inst->gep_offset);
    }
    emit_store_int(out, inst, X86_RAX);
}

/* ------------------------------------------------------------------ */
/*  Constant materialization                                          */
/* ------------------------------------------------------------------ */

/* Float/double constants get stored to their stack slots from a GPR
 * (we load the bit pattern as an integer immediate). */
static void emit_const_float(FILE *out, IRInst *inst)
{
    union { double d; uint64_t u; } bits;
    bits.d = inst->fval;

    if (inst->ty.kind == IR_TYPE_F32) {
        union { float f; uint32_t u; } f32;
        f32.f = (float)inst->fval;
        fprintf(out, "    movl $%" PRIu32 ", %%eax\n", f32.u);
        fprintf(out, "    movl %%eax, %d(%%rbp)\n", inst->stack_slot);
    } else {
        fprintf(out, "    movabsq $%" PRIu64 ", %%rax\n", bits.u);
        fprintf(out, "    movq %%rax, %d(%%rbp)\n", inst->stack_slot);
    }
}

static void emit_const_int(FILE *out, IRInst *inst)
{
    /* Integer constants are materialized inline at each use site,
     * but we also store them to a slot for address-taken values. */
    fprintf(out, "    movq $%" PRId64 ", %%rax\n", inst->ival);
    emit_store_int(out, inst, X86_RAX);
}

/* ------------------------------------------------------------------ */
/*  Control flow                                                      */
/* ------------------------------------------------------------------ */

static void emit_br(FILE *out, IRInst *inst, IRFunc *func)
{
    fprintf(out, "    jmp ");
    emit_block_ref(out, func, inst->br_target);
    fprintf(out, "\n");
}

static void emit_condbr(FILE *out, IRInst *inst, IRFunc *func)
{
    IRInst *cond = inst->operands[0];

    emit_load_int(out, cond, X86_RAX);
    fprintf(out, "    testq %%rax, %%rax\n");
    fprintf(out, "    jne ");
    emit_block_ref(out, func, inst->br_true);
    fprintf(out, "\n");
    fprintf(out, "    jmp ");
    emit_block_ref(out, func, inst->br_false);
    fprintf(out, "\n");
}

static void emit_ret(FILE *out, IRInst *inst)
{
    if (inst->num_operands > 0 && inst->operands[0]) {
        IRInst *retval = inst->operands[0];
        if (x86_isel_is_float(retval->ty)) {
            emit_load_float(out, retval, 0);  /* return in %xmm0 */
        } else {
            emit_load_int(out, retval, X86_RAX);  /* return in %rax */
        }
    }

    /* Epilogue: restore frame and return. */
    fprintf(out, "    movq %%rbp, %%rsp\n");
    fprintf(out, "    popq %%rbp\n");
    fprintf(out, "    retq\n");
}

static void emit_retv(FILE *out, IRInst *inst)
{
    (void)inst;
    fprintf(out, "    movq %%rbp, %%rsp\n");
    fprintf(out, "    popq %%rbp\n");
    fprintf(out, "    retq\n");
}

/* ------------------------------------------------------------------ */
/*  Function call (System V AMD64 ABI)                                */
/* ------------------------------------------------------------------ */

static void emit_call(FILE *out, IRInst *inst)
{
    int int_arg_idx   = 0;
    int float_arg_idx = 0;
    int stack_args    = 0;

    /*
     * Pass 1: count how many arguments spill to the stack so we can
     * allocate the argument area.
     */
    for (int i = 0; i < inst->num_operands; i++) {
        IRInst *arg = inst->operands[i];
        if (x86_isel_is_float(arg->ty)) {
            if (float_arg_idx >= 8) stack_args++;
            float_arg_idx++;
        } else {
            if (int_arg_idx >= 6) stack_args++;
            int_arg_idx++;
        }
    }

    /* Ensure 16-byte stack alignment including the stack arguments. */
    int stack_space = stack_args * 8;
    stack_space = align_up(stack_space, 16);
    if (stack_space > 0) {
        fprintf(out, "    subq $%d, %%rsp\n", stack_space);
    }

    /*
     * Pass 2: place arguments in registers / on the stack.
     * We iterate in order and keep separate int/float counters.
     */
    int_arg_idx   = 0;
    float_arg_idx = 0;
    int stack_off  = 0;

    for (int i = 0; i < inst->num_operands; i++) {
        IRInst *arg = inst->operands[i];

        if (x86_isel_is_float(arg->ty)) {
            int xreg = x86_isel_arg_float_reg(float_arg_idx);
            if (xreg >= 0) {
                emit_load_float(out, arg, xreg);
            } else {
                /* Spill to stack argument area. */
                emit_load_float(out, arg, 0);
                if (arg->ty.kind == IR_TYPE_F32)
                    fprintf(out, "    movss %%xmm0, %d(%%rsp)\n", stack_off);
                else
                    fprintf(out, "    movsd %%xmm0, %d(%%rsp)\n", stack_off);
                stack_off += 8;
            }
            float_arg_idx++;
        } else {
            int reg = x86_isel_arg_int_reg(int_arg_idx);
            if (reg >= 0) {
                emit_load_int(out, arg, reg);
            } else {
                /* Spill to stack argument area. */
                emit_load_int(out, arg, X86_RAX);
                fprintf(out, "    movq %%rax, %d(%%rsp)\n", stack_off);
                stack_off += 8;
            }
            int_arg_idx++;
        }
    }

    /* For variadic functions (or as a safety measure), set %al to the
     * number of floating-point arguments passed in XMM registers. */
    int num_xmm_used = float_arg_idx < 8 ? float_arg_idx : 8;
    fprintf(out, "    movb $%d, %%al\n", num_xmm_used);

    /* Emit the call. */
    if (inst->call_target) {
#if defined(__APPLE__)
        fprintf(out, "    callq _%s\n", inst->call_target);
#else
        fprintf(out, "    callq %s@PLT\n", inst->call_target);
#endif
    } else {
        /* Indirect call through first operand (function pointer). */
        fprintf(out, "    callq *%%rax\n");
    }

    /* Reclaim stack argument area. */
    if (stack_space > 0) {
        fprintf(out, "    addq $%d, %%rsp\n", stack_space);
    }

    /* Store the return value. */
    if (inst->ty.kind != IR_TYPE_VOID && inst->stack_slot != 0) {
        if (x86_isel_is_float(inst->ty)) {
            emit_store_float(out, inst, 0);  /* return value in %xmm0 */
        } else {
            emit_store_int(out, inst, X86_RAX);  /* return value in %rax */
        }
    }
}

/* ------------------------------------------------------------------ */
/*  PHI nodes                                                         */
/* ------------------------------------------------------------------ */

/* PHI nodes are resolved by the register allocator (copies inserted at
 * the end of predecessor blocks).  At this stage, PHIs with stack slots
 * are effectively copies that have already been lowered -- we just need
 * to emit a move from the appropriate predecessor's value.
 *
 * In the naive spill-everything approach, PHI resolution is done by
 * emitting copies at the end of each predecessor.  For the initial
 * implementation we handle PHIs in a simplistic way: we emit nothing
 * here.  Instead, the regalloc pass inserts IR_COPY instructions at
 * the end of predecessor blocks.
 */
static void emit_phi(FILE *out, IRInst *inst)
{
    (void)out;
    (void)inst;
    /* PHIs handled by copies inserted during regalloc. */
}

/* ------------------------------------------------------------------ */
/*  Copy (used for PHI lowering and reg-to-reg moves)                 */
/* ------------------------------------------------------------------ */

static void emit_copy(FILE *out, IRInst *inst)
{
    IRInst *src = inst->operands[0];

    if (x86_isel_is_float(inst->ty)) {
        emit_load_float(out, src, 0);
        emit_store_float(out, inst, 0);
    } else {
        emit_load_int(out, src, X86_RAX);
        emit_store_int(out, inst, X86_RAX);
    }
}

/* ------------------------------------------------------------------ */
/*  Memcpy / Memset                                                   */
/* ------------------------------------------------------------------ */

static void emit_memcpy(FILE *out, IRInst *inst)
{
    /* operands: [dest, src, size] */
    IRInst *dst = inst->operands[0];
    IRInst *src = inst->operands[1];
    IRInst *sz  = inst->operands[2];

    emit_load_int(out, dst, X86_RDI);
    emit_load_int(out, src, X86_RSI);
    emit_load_int(out, sz,  X86_RDX);
    fprintf(out, "    movb $0, %%al\n");
#if defined(__APPLE__)
    fprintf(out, "    callq _memcpy\n");
#else
    fprintf(out, "    callq memcpy@PLT\n");
#endif
}

static void emit_memset(FILE *out, IRInst *inst)
{
    /* operands: [dest, val, size] */
    IRInst *dst = inst->operands[0];
    IRInst *val = inst->operands[1];
    IRInst *sz  = inst->operands[2];

    emit_load_int(out, dst, X86_RDI);
    emit_load_int(out, val, X86_RSI);
    emit_load_int(out, sz,  X86_RDX);
    fprintf(out, "    movb $0, %%al\n");
#if defined(__APPLE__)
    fprintf(out, "    callq _memset\n");
#else
    fprintf(out, "    callq memset@PLT\n");
#endif
}

/* ------------------------------------------------------------------ */
/*  Emit one instruction                                              */
/* ------------------------------------------------------------------ */

static void emit_inst(FILE *out, IRInst *inst, IRFunc *func)
{
    /* Skip dead instructions. */
    if (inst->is_dead) return;

    /* Emit a comment with the IR value number for debugging. */
    fprintf(out, "    # %%%d\n", inst->id);

    switch (inst->op) {
    /* Constants */
    case IR_CONST_INT:
        emit_const_int(out, inst);
        break;
    case IR_CONST_FLOAT:
        emit_const_float(out, inst);
        break;

    /* Memory */
    case IR_ALLOCA:
        emit_alloca(out, inst);
        break;
    case IR_LOAD:
        emit_load(out, inst);
        break;
    case IR_STORE:
        emit_store(out, inst);
        break;
    case IR_GEP:
        emit_gep(out, inst);
        break;

    /* Integer arithmetic */
    case IR_ADD: case IR_SUB: case IR_MUL:
    case IR_SDIV: case IR_UDIV: case IR_SREM: case IR_UREM:
    case IR_AND: case IR_OR: case IR_XOR:
    case IR_SHL: case IR_LSHR: case IR_ASHR:
        emit_int_binop(out, inst);
        break;

    /* Float arithmetic */
    case IR_FADD: case IR_FSUB: case IR_FMUL: case IR_FDIV:
        emit_float_binop(out, inst);
        break;

    /* Unary */
    case IR_NEG: case IR_NOT: case IR_FNEG:
        emit_unop(out, inst);
        break;

    /* Comparison */
    case IR_ICMP_EQ: case IR_ICMP_NE:
    case IR_ICMP_SLT: case IR_ICMP_SLE: case IR_ICMP_SGT: case IR_ICMP_SGE:
    case IR_ICMP_ULT: case IR_ICMP_ULE: case IR_ICMP_UGT: case IR_ICMP_UGE:
    case IR_FCMP_OEQ: case IR_FCMP_ONE:
    case IR_FCMP_OLT: case IR_FCMP_OLE: case IR_FCMP_OGT: case IR_FCMP_OGE:
        emit_cmp(out, inst);
        break;

    /* Conversions */
    case IR_SEXT: case IR_ZEXT: case IR_TRUNC:
    case IR_SITOFP: case IR_UITOFP: case IR_FPTOSI: case IR_FPTOUI:
    case IR_FPEXT: case IR_FPTRUNC:
    case IR_BITCAST: case IR_PTRTOINT: case IR_INTTOPTR:
        emit_cast(out, inst);
        break;

    /* Control flow */
    case IR_BR:
        emit_br(out, inst, func);
        break;
    case IR_CONDBR:
        emit_condbr(out, inst, func);
        break;
    case IR_RET:
        emit_ret(out, inst);
        break;
    case IR_RETV:
        emit_retv(out, inst);
        break;

    /* Other */
    case IR_CALL:
        emit_call(out, inst);
        break;
    case IR_PHI:
        emit_phi(out, inst);
        break;
    case IR_COPY:
        emit_copy(out, inst);
        break;
    case IR_MEMCPY:
        emit_memcpy(out, inst);
        break;
    case IR_MEMSET:
        emit_memset(out, inst);
        break;

    default:
        fprintf(out, "    # UNIMPLEMENTED: op %d\n", inst->op);
        break;
    }
}

/* ------------------------------------------------------------------ */
/*  Emit a complete function                                          */
/* ------------------------------------------------------------------ */

void x86_64_emit_func(IRFunc *func, FILE *out, Target *tgt, Arena *a)
{
    (void)tgt;
    (void)a;

    /* Function label. */
#if defined(__APPLE__)
    fprintf(out, "    .globl _%s\n", func->name);
    fprintf(out, "_%s:\n", func->name);
#else
    fprintf(out, "    .globl %s\n", func->name);
    fprintf(out, "    .type %s, @function\n", func->name);
    fprintf(out, "%s:\n", func->name);
#endif

    /* Prologue: set up stack frame. */
    fprintf(out, "    pushq %%rbp\n");
    fprintf(out, "    movq %%rsp, %%rbp\n");

    /* Reserve stack space (computed by regalloc). */
    int frame_size = align_up(func->stack_size, 16);
    if (frame_size > 0) {
        fprintf(out, "    subq $%d, %%rsp\n", frame_size);
    }

    /* Copy register-passed parameters to their stack slots.
     * System V: int args in rdi, rsi, rdx, rcx, r8, r9
     *           float args in xmm0..xmm7                            */
    {
        int int_idx   = 0;
        int float_idx = 0;
        for (int i = 0; i < func->num_params; i++) {
            IRInst *param = func->params[i];
            if (param->stack_slot == 0) continue;  /* should not happen */

            if (x86_isel_is_float(param->ty)) {
                int xreg = x86_isel_arg_float_reg(float_idx);
                if (xreg >= 0) {
                    if (param->ty.kind == IR_TYPE_F32)
                        fprintf(out, "    movss %%%s, %d(%%rbp)\n",
                                x86_isel_xmm(xreg), param->stack_slot);
                    else
                        fprintf(out, "    movsd %%%s, %d(%%rbp)\n",
                                x86_isel_xmm(xreg), param->stack_slot);
                }
                float_idx++;
            } else {
                int reg = x86_isel_arg_int_reg(int_idx);
                if (reg >= 0) {
                    fprintf(out, "    movq %%%s, %d(%%rbp)\n",
                            x86_isel_reg64(reg), param->stack_slot);
                }
                int_idx++;
            }
        }
    }

    /* Emit each basic block. */
    for (IRBlock *bb = func->entry; bb; bb = bb->next) {
        /* Block label (skip entry if it is the first block). */
        if (bb != func->entry || bb->preds.len > 0) {
            emit_label(out, func, bb);
        }

        /* Emit each instruction. */
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            emit_inst(out, inst, func);
        }
    }

    /* Size directive (ELF only). */
#if !defined(__APPLE__)
    fprintf(out, "    .size %s, .-%s\n", func->name, func->name);
#endif
    fprintf(out, "\n");
}

/* ------------------------------------------------------------------ */
/*  Standalone prologue / epilogue (for Target function pointers)     */
/* ------------------------------------------------------------------ */

void x86_64_emit_prologue(FILE *out, Target *tgt)
{
    (void)tgt;
    fprintf(out, "    pushq %%rbp\n");
    fprintf(out, "    movq %%rsp, %%rbp\n");
}

void x86_64_emit_epilogue(FILE *out, Target *tgt)
{
    (void)tgt;
    fprintf(out, "    movq %%rbp, %%rsp\n");
    fprintf(out, "    popq %%rbp\n");
    fprintf(out, "    retq\n");
}

/* ------------------------------------------------------------------ */
/*  Emit globals and string literals                                  */
/* ------------------------------------------------------------------ */

static void emit_globals(IRModule *mod, FILE *out)
{
    for (IRGlobal *g = mod->globals; g; g = g->next) {
        if (g->is_extern) continue;  /* extern declarations have no definition */

        if (g->is_string) {
            /* String literals go in .rodata (or .cstring on macOS). */
#if defined(__APPLE__)
            fprintf(out, "    .section __TEXT,__cstring,cstring_literals\n");
#else
            fprintf(out, "    .section .rodata\n");
#endif
            /* Emit the label. */
#if defined(__APPLE__)
            fprintf(out, "_%s:\n", g->name);
#else
            fprintf(out, "%s:\n", g->name);
#endif
            /* Emit string bytes.  Use .ascii if no null terminator desired,
             * but C strings always have one so use .asciz. */
            fprintf(out, "    .asciz \"");
            for (int i = 0; i < g->string_len; i++) {
                unsigned char c = (unsigned char)g->string_val[i];
                if (c == '\\')       fprintf(out, "\\\\");
                else if (c == '"')   fprintf(out, "\\\"");
                else if (c == '\n')  fprintf(out, "\\n");
                else if (c == '\t')  fprintf(out, "\\t");
                else if (c == '\0')  fprintf(out, "\\0");
                else if (c >= 0x20 && c < 0x7f)
                    fputc(c, out);
                else
                    fprintf(out, "\\%03o", c);
            }
            fprintf(out, "\"\n");
        } else {
            /* Data globals. */
            if (g->is_static) {
                /* Local (file-scope) symbol, no .globl. */
            } else {
#if defined(__APPLE__)
                fprintf(out, "    .globl _%s\n", g->name);
#else
                fprintf(out, "    .globl %s\n", g->name);
#endif
            }

            /* Use alloc_size for actual storage (arrays, structs need full size). */
            int sz = g->alloc_size > 0 ? g->alloc_size : g->ty.size;
            int al = g->alloc_align > 0 ? g->alloc_align : (g->ty.align > 0 ? g->ty.align : 1);

            if (g->has_init) {
                fprintf(out, "    .data\n");
            } else {
                /* Uninitialized: BSS. */
#if defined(__APPLE__)
                fprintf(out, "    .zerofill __DATA,__bss,_%s,%d,%d\n",
                        g->name, sz, al);
                continue;
#else
                fprintf(out, "    .bss\n");
#endif
            }

            /* Alignment. */
            fprintf(out, "    .align %d\n", al);

#if defined(__APPLE__)
            fprintf(out, "_%s:\n", g->name);
#else
            fprintf(out, "%s:\n", g->name);
#endif

            if (g->has_init) {
                if (g->init_data && g->init_data_len > 0) {
                    /* Byte-level init data for arrays/structs/pointer globals */
                    int off = 0;
                    while (off < g->init_data_len) {
                        /* Check for relocation at this offset */
                        IRReloc *rel = NULL;
                        for (IRReloc *r = g->relocs; r; r = r->next) {
                            if (r->offset == off) { rel = r; break; }
                        }
                        if (rel) {
#if defined(__APPLE__)
                            fprintf(out, "    .quad _%s\n", rel->symbol);
#else
                            fprintf(out, "    .quad %s\n", rel->symbol);
#endif
                            off += 8;
                        } else {
                            /* Emit in natural chunks: try 4, 2, 1 */
                            if (off + 4 <= g->init_data_len && (off % 4 == 0)) {
                                int32_t v;
                                memcpy(&v, &g->init_data[off], 4);
                                fprintf(out, "    .long %d\n", v);
                                off += 4;
                            } else if (off + 2 <= g->init_data_len && (off % 2 == 0)) {
                                int16_t v;
                                memcpy(&v, &g->init_data[off], 2);
                                fprintf(out, "    .short %d\n", v);
                                off += 2;
                            } else {
                                fprintf(out, "    .byte %d\n", g->init_data[off]);
                                off += 1;
                            }
                        }
                    }
                } else {
                    switch (sz) {
                    case 1: fprintf(out, "    .byte %" PRId64 "\n",   g->init_val); break;
                    case 2: fprintf(out, "    .short %" PRId64 "\n",  g->init_val); break;
                    case 4: fprintf(out, "    .long %" PRId64 "\n",   g->init_val); break;
                    case 8: fprintf(out, "    .quad %" PRId64 "\n",   g->init_val); break;
                    default:
                        /* Aggregate: zero-fill for now. */
                        fprintf(out, "    .zero %d\n", sz);
                        break;
                    }
                }
            } else {
                /* BSS (non-Apple). */
                fprintf(out, "    .zero %d\n", sz);
            }
        }

        fprintf(out, "\n");
    }
}

/* ------------------------------------------------------------------ */
/*  codegen() - Main code generation entry point                      */
/* ------------------------------------------------------------------ */

void codegen(IRModule *mod, Target *tgt, FILE *out, Arena *a)
{
    /* Emit assembly header. */
    fprintf(out, "# Generated by mycc (x86-64 backend)\n");

    /* Emit the text section. */
    fprintf(out, "    .text\n\n");

    /* Process each function: register allocate then emit. */
    for (IRFunc *func = mod->funcs; func; func = func->next) {
        regalloc(func, tgt, a);
        tgt->emit_func(func, out, tgt, a);
    }

    /* Emit globals and string literals. */
    emit_globals(mod, out);

    /* macOS requires a linker hint for stack probing. */
#if defined(__APPLE__)
    fprintf(out, ".subsections_via_symbols\n");
#endif
    /* ELF: mark the stack as non-executable. */
#if !defined(__APPLE__)
    fprintf(out, "    .section .note.GNU-stack,\"\",@progbits\n");
#endif
}
