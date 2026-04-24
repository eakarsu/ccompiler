/*
 * x86_64_target.c - Target descriptor for x86-64 (System V AMD64 ABI)
 *
 * Defines all physical registers, argument-passing conventions, and
 * callee-saved register sets for the System V AMD64 ABI.
 */

#include "target.h"
#include "arena.h"

#include <string.h>

/* ------------------------------------------------------------------ */
/*  Physical register IDs                                             */
/* ------------------------------------------------------------------ */

enum {
    /* General-purpose registers */
    REG_RAX = 0,
    REG_RBX,
    REG_RCX,
    REG_RDX,
    REG_RSI,
    REG_RDI,
    REG_RBP,
    REG_RSP,
    REG_R8,
    REG_R9,
    REG_R10,
    REG_R11,
    REG_R12,
    REG_R13,
    REG_R14,
    REG_R15,
    NUM_INT_REGS,

    /* XMM floating-point registers */
    REG_XMM0 = 0,
    REG_XMM1,
    REG_XMM2,
    REG_XMM3,
    REG_XMM4,
    REG_XMM5,
    REG_XMM6,
    REG_XMM7,
    REG_XMM8,
    REG_XMM9,
    REG_XMM10,
    REG_XMM11,
    REG_XMM12,
    REG_XMM13,
    REG_XMM14,
    REG_XMM15,
    NUM_FLOAT_REGS,
};

/* ------------------------------------------------------------------ */
/*  Integer register descriptors                                      */
/* ------------------------------------------------------------------ */

static const RegDesc x86_64_int_regs[NUM_INT_REGS] = {
    /*  name    id          callee_saved  allocable */
    { "rax",   REG_RAX,    false,        true  },
    { "rbx",   REG_RBX,    true,         true  },
    { "rcx",   REG_RCX,    false,        true  },
    { "rdx",   REG_RDX,    false,        true  },
    { "rsi",   REG_RSI,    false,        true  },
    { "rdi",   REG_RDI,    false,        true  },
    { "rbp",   REG_RBP,    true,         false },  /* frame pointer, not allocable */
    { "rsp",   REG_RSP,    false,        false },  /* stack pointer, not allocable */
    { "r8",    REG_R8,     false,        true  },
    { "r9",    REG_R9,     false,        true  },
    { "r10",   REG_R10,    false,        true  },
    { "r11",   REG_R11,    false,        true  },
    { "r12",   REG_R12,    true,         true  },
    { "r13",   REG_R13,    true,         true  },
    { "r14",   REG_R14,    true,         true  },
    { "r15",   REG_R15,    true,         true  },
};

/* ------------------------------------------------------------------ */
/*  XMM register descriptors                                          */
/* ------------------------------------------------------------------ */

static const RegDesc x86_64_float_regs[NUM_FLOAT_REGS] = {
    { "xmm0",  REG_XMM0,  false, true },
    { "xmm1",  REG_XMM1,  false, true },
    { "xmm2",  REG_XMM2,  false, true },
    { "xmm3",  REG_XMM3,  false, true },
    { "xmm4",  REG_XMM4,  false, true },
    { "xmm5",  REG_XMM5,  false, true },
    { "xmm6",  REG_XMM6,  false, true },
    { "xmm7",  REG_XMM7,  false, true },
    { "xmm8",  REG_XMM8,  false, true },
    { "xmm9",  REG_XMM9,  false, true },
    { "xmm10", REG_XMM10, false, true },
    { "xmm11", REG_XMM11, false, true },
    { "xmm12", REG_XMM12, false, true },
    { "xmm13", REG_XMM13, false, true },
    { "xmm14", REG_XMM14, false, true },
    { "xmm15", REG_XMM15, false, true },
};

/* ------------------------------------------------------------------ */
/*  System V AMD64 ABI: integer argument registers                    */
/*  Order: rdi, rsi, rdx, rcx, r8, r9                                */
/* ------------------------------------------------------------------ */

static const int sysv_arg_int_regs[] = {
    REG_RDI, REG_RSI, REG_RDX, REG_RCX, REG_R8, REG_R9,
};

/* ------------------------------------------------------------------ */
/*  System V AMD64 ABI: floating-point argument registers             */
/*  Order: xmm0 .. xmm7                                              */
/* ------------------------------------------------------------------ */

static const int sysv_arg_float_regs[] = {
    REG_XMM0, REG_XMM1, REG_XMM2, REG_XMM3,
    REG_XMM4, REG_XMM5, REG_XMM6, REG_XMM7,
};

/* ------------------------------------------------------------------ */
/*  System V AMD64 ABI: callee-saved registers                        */
/*  rbx, rbp, r12, r13, r14, r15                                     */
/* ------------------------------------------------------------------ */

static const int sysv_callee_saved[] = {
    REG_RBX, REG_RBP, REG_R12, REG_R13, REG_R14, REG_R15,
};

/* ------------------------------------------------------------------ */
/*  Forward declarations for emit functions (defined in x86_64_emit.c)*/
/* ------------------------------------------------------------------ */

extern void x86_64_emit_func(IRFunc *func, FILE *out, Target *tgt, Arena *a);
extern void x86_64_emit_prologue(FILE *out, Target *tgt);
extern void x86_64_emit_epilogue(FILE *out, Target *tgt);

/* ------------------------------------------------------------------ */
/*  target_x86_64() - construct the x86-64 target descriptor          */
/* ------------------------------------------------------------------ */

Target *target_x86_64(Arena *a)
{
    Target *t = arena_new(a, Target);

    t->arch        = TARGET_X86_64;
    t->abi         = ABI_SYSV;
    t->ptr_size    = 8;
    t->stack_align = 16;

    /* Integer registers */
    t->int_regs     = x86_64_int_regs;
    t->num_int_regs = NUM_INT_REGS;

    /* Floating-point registers */
    t->float_regs     = x86_64_float_regs;
    t->num_float_regs = NUM_FLOAT_REGS;

    /* Argument passing (System V) */
    t->arg_int_regs     = sysv_arg_int_regs;
    t->num_arg_int_regs = (int)(sizeof(sysv_arg_int_regs) / sizeof(sysv_arg_int_regs[0]));

    t->arg_float_regs     = sysv_arg_float_regs;
    t->num_arg_float_regs = (int)(sizeof(sysv_arg_float_regs) / sizeof(sysv_arg_float_regs[0]));

    /* Callee-saved registers */
    t->callee_saved     = sysv_callee_saved;
    t->num_callee_saved = (int)(sizeof(sysv_callee_saved) / sizeof(sysv_callee_saved[0]));

    /* Emit function pointers */
    t->emit_func     = x86_64_emit_func;
    t->emit_prologue = x86_64_emit_prologue;
    t->emit_epilogue = x86_64_emit_epilogue;

    return t;
}

/* ------------------------------------------------------------------ */
/*  target_host() - detect current platform and return target         */
/* ------------------------------------------------------------------ */

Target *target_host(Arena *a)
{
#if defined(__x86_64__) || defined(_M_X64)
    return target_x86_64(a);
#elif defined(__aarch64__) || defined(_M_ARM64)
    #if defined(__APPLE__)
        return target_arm64(a, true);
    #else
        return target_arm64(a, false);
    #endif
#else
    /* Default to x86-64 if architecture is unknown */
    return target_x86_64(a);
#endif
}
