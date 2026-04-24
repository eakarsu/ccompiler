/*
 * x86_64_isel.c - Instruction selection for x86-64
 *
 * Naive "spill-everything" approach: every IR value is assigned a stack slot.
 * Operands are loaded into scratch registers, the operation is performed,
 * and the result is stored back to its stack slot.
 *
 * This file provides helpers used by the emitter (x86_64_emit.c) to lower
 * IR instructions to x86-64 machine operations.  It is not a full MachineIR
 * layer -- the emitter directly prints AT&T assembly using these helpers.
 */

#include "ir.h"
#include "target.h"
#include "arena.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

/* ------------------------------------------------------------------ */
/*  x86-64 register IDs (must match x86_64_target.c)                  */
/* ------------------------------------------------------------------ */

enum {
    X86_RAX = 0,
    X86_RBX,
    X86_RCX,
    X86_RDX,
    X86_RSI,
    X86_RDI,
    X86_RBP,
    X86_RSP,
    X86_R8,
    X86_R9,
    X86_R10,
    X86_R11,
    X86_R12,
    X86_R13,
    X86_R14,
    X86_R15,
};

/* ------------------------------------------------------------------ */
/*  Suffix helpers                                                    */
/* ------------------------------------------------------------------ */

/* Return the AT&T operand-size suffix for an IR type. */
char x86_isel_suffix(IRType ty)
{
    switch (ty.kind) {
    case IR_TYPE_I1:
    case IR_TYPE_I8:    return 'b';
    case IR_TYPE_I16:   return 'w';
    case IR_TYPE_I32:   return 'l';
    case IR_TYPE_I64:
    case IR_TYPE_PTR:   return 'q';
    case IR_TYPE_F32:   return 's';  /* movss, addss, ... */
    case IR_TYPE_F64:   return 'd';  /* movsd, addsd, ... */
    default:            return 'q';
    }
}

/* Return the byte size of an IR type for stack slot allocation. */
int x86_isel_slot_size(IRType ty)
{
    switch (ty.kind) {
    case IR_TYPE_I1:
    case IR_TYPE_I8:    return 1;
    case IR_TYPE_I16:   return 2;
    case IR_TYPE_I32:
    case IR_TYPE_F32:   return 4;
    case IR_TYPE_I64:
    case IR_TYPE_F64:
    case IR_TYPE_PTR:   return 8;
    case IR_TYPE_VOID:  return 0;
    default:            return 8;
    }
}

/* Return the name of a 64-bit GPR. */
const char *x86_isel_reg64(int reg)
{
    static const char *names[] = {
        "rax","rbx","rcx","rdx","rsi","rdi","rbp","rsp",
        "r8","r9","r10","r11","r12","r13","r14","r15",
    };
    assert(reg >= 0 && reg < 16);
    return names[reg];
}

/* Return the 32-bit sub-register name. */
const char *x86_isel_reg32(int reg)
{
    static const char *names[] = {
        "eax","ebx","ecx","edx","esi","edi","ebp","esp",
        "r8d","r9d","r10d","r11d","r12d","r13d","r14d","r15d",
    };
    assert(reg >= 0 && reg < 16);
    return names[reg];
}

/* Return the 16-bit sub-register name. */
const char *x86_isel_reg16(int reg)
{
    static const char *names[] = {
        "ax","bx","cx","dx","si","di","bp","sp",
        "r8w","r9w","r10w","r11w","r12w","r13w","r14w","r15w",
    };
    assert(reg >= 0 && reg < 16);
    return names[reg];
}

/* Return the 8-bit low sub-register name. */
const char *x86_isel_reg8(int reg)
{
    static const char *names[] = {
        "al","bl","cl","dl","sil","dil","bpl","spl",
        "r8b","r9b","r10b","r11b","r12b","r13b","r14b","r15b",
    };
    assert(reg >= 0 && reg < 16);
    return names[reg];
}

/* Return the appropriate sub-register name based on type width. */
const char *x86_isel_regname(int reg, IRType ty)
{
    switch (ty.kind) {
    case IR_TYPE_I1:
    case IR_TYPE_I8:    return x86_isel_reg8(reg);
    case IR_TYPE_I16:   return x86_isel_reg16(reg);
    case IR_TYPE_I32:   return x86_isel_reg32(reg);
    case IR_TYPE_I64:
    case IR_TYPE_PTR:   return x86_isel_reg64(reg);
    default:            return x86_isel_reg64(reg);
    }
}

/* Return the XMM register name. */
const char *x86_isel_xmm(int reg)
{
    static const char *names[] = {
        "xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7",
        "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15",
    };
    assert(reg >= 0 && reg < 16);
    return names[reg];
}

/* ------------------------------------------------------------------ */
/*  Operand location                                                  */
/* ------------------------------------------------------------------ */

/* Describes where an IR value lives at emission time. */
typedef enum {
    XLOC_STACK,       /* value in stack slot: offset(%rbp) */
    XLOC_CONST_INT,   /* immediate integer constant */
    XLOC_CONST_FLOAT, /* floating-point constant (in .rodata) */
    XLOC_GLOBAL,      /* global symbol address */
} XLocKind;

typedef struct {
    XLocKind kind;
    int stack_offset;     /* negative offset from %rbp */
    int64_t imm;          /* immediate value */
    double fval;          /* float constant */
    const char *sym;      /* global symbol name */
} XLoc;

/* Resolve the location of an IR instruction's result value. */
XLoc x86_isel_loc(IRInst *inst)
{
    XLoc loc = {0};

    if (inst->op == IR_CONST_INT) {
        loc.kind = XLOC_CONST_INT;
        loc.imm  = inst->ival;
        return loc;
    }

    if (inst->op == IR_CONST_FLOAT) {
        loc.kind = XLOC_CONST_FLOAT;
        loc.fval = inst->fval;
        return loc;
    }

    /* Everything else lives in its assigned stack slot. */
    loc.kind         = XLOC_STACK;
    loc.stack_offset = inst->stack_slot;
    return loc;
}

/* ------------------------------------------------------------------ */
/*  Predicate helpers                                                  */
/* ------------------------------------------------------------------ */

/* Does this IR type need an XMM register? */
bool x86_isel_is_float(IRType ty)
{
    return ty.kind == IR_TYPE_F32 || ty.kind == IR_TYPE_F64;
}

/* Does this IR type use integer/pointer registers? */
bool x86_isel_is_int(IRType ty)
{
    return !x86_isel_is_float(ty) && ty.kind != IR_TYPE_VOID;
}

/* ------------------------------------------------------------------ */
/*  System V ABI: argument register mapping                           */
/* ------------------------------------------------------------------ */

/*
 * Classify arguments for a call / function entry.
 * Returns the register id for argument index 'idx' among integer args,
 * or -1 if the argument should be passed on the stack.
 */
int x86_isel_arg_int_reg(int idx)
{
    static const int regs[] = { X86_RDI, X86_RSI, X86_RDX, X86_RCX, X86_R8, X86_R9 };
    if (idx < 0 || idx >= 6) return -1;
    return regs[idx];
}

/*
 * Float argument register: xmm0..xmm7.
 * Returns -1 if the argument should be passed on the stack.
 */
int x86_isel_arg_float_reg(int idx)
{
    if (idx < 0 || idx >= 8) return -1;
    return idx;  /* XMM register ids are 0..15 */
}

/* ------------------------------------------------------------------ */
/*  Condition code mapping                                            */
/* ------------------------------------------------------------------ */

/* Map an IR comparison opcode to x86 condition code suffix string. */
const char *x86_isel_cc(IROpcode op)
{
    switch (op) {
    case IR_ICMP_EQ:   return "e";
    case IR_ICMP_NE:   return "ne";
    case IR_ICMP_SLT:  return "l";
    case IR_ICMP_SLE:  return "le";
    case IR_ICMP_SGT:  return "g";
    case IR_ICMP_SGE:  return "ge";
    case IR_ICMP_ULT:  return "b";
    case IR_ICMP_ULE:  return "be";
    case IR_ICMP_UGT:  return "a";
    case IR_ICMP_UGE:  return "ae";
    case IR_FCMP_OEQ:  return "e";
    case IR_FCMP_ONE:  return "ne";
    case IR_FCMP_OLT:  return "b";
    case IR_FCMP_OLE:  return "be";
    case IR_FCMP_OGT:  return "a";
    case IR_FCMP_OGE:  return "ae";
    default:           return "e";
    }
}

/* Is this an integer comparison opcode? */
bool x86_isel_is_icmp(IROpcode op)
{
    return op >= IR_ICMP_EQ && op <= IR_ICMP_UGE;
}

/* Is this a floating-point comparison opcode? */
bool x86_isel_is_fcmp(IROpcode op)
{
    return op >= IR_FCMP_OEQ && op <= IR_FCMP_OGE;
}

/* Is this a comparison opcode (integer or float)? */
bool x86_isel_is_cmp(IROpcode op)
{
    return x86_isel_is_icmp(op) || x86_isel_is_fcmp(op);
}
