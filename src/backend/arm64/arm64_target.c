#include "cc.h"

/* ARM64 register definitions */
enum {
    ARM64_X0 = 0, ARM64_X1, ARM64_X2, ARM64_X3,
    ARM64_X4, ARM64_X5, ARM64_X6, ARM64_X7,
    ARM64_X8, ARM64_X9, ARM64_X10, ARM64_X11,
    ARM64_X12, ARM64_X13, ARM64_X14, ARM64_X15,
    ARM64_X16, ARM64_X17, ARM64_X18,
    ARM64_X19, ARM64_X20, ARM64_X21, ARM64_X22,
    ARM64_X23, ARM64_X24, ARM64_X25, ARM64_X26,
    ARM64_X27, ARM64_X28,
    ARM64_FP,   /* x29 = frame pointer */
    ARM64_LR,   /* x30 = link register */
    ARM64_SP,   /* x31 = stack pointer (not allocable) */
    ARM64_XREG_COUNT,
};

enum {
    ARM64_V0 = 0, ARM64_V1, ARM64_V2, ARM64_V3,
    ARM64_V4, ARM64_V5, ARM64_V6, ARM64_V7,
    ARM64_V8, ARM64_V9, ARM64_V10, ARM64_V11,
    ARM64_V12, ARM64_V13, ARM64_V14, ARM64_V15,
    ARM64_V16, ARM64_V17, ARM64_V18, ARM64_V19,
    ARM64_V20, ARM64_V21, ARM64_V22, ARM64_V23,
    ARM64_V24, ARM64_V25, ARM64_V26, ARM64_V27,
    ARM64_V28, ARM64_V29, ARM64_V30, ARM64_V31,
    ARM64_VREG_COUNT,
};

void arm64_emit_func(IRFunc *func, FILE *out, Target *tgt, Arena *a);

static RegDesc arm64_int_regs[] = {
    {"x0", ARM64_X0, false, true},
    {"x1", ARM64_X1, false, true},
    {"x2", ARM64_X2, false, true},
    {"x3", ARM64_X3, false, true},
    {"x4", ARM64_X4, false, true},
    {"x5", ARM64_X5, false, true},
    {"x6", ARM64_X6, false, true},
    {"x7", ARM64_X7, false, true},
    {"x8", ARM64_X8, false, true},  /* indirect result */
    {"x9", ARM64_X9, false, true},
    {"x10", ARM64_X10, false, true},
    {"x11", ARM64_X11, false, true},
    {"x12", ARM64_X12, false, true},
    {"x13", ARM64_X13, false, true},
    {"x14", ARM64_X14, false, true},
    {"x15", ARM64_X15, false, true},
    {"x16", ARM64_X16, false, false},  /* IP0, platform reserved */
    {"x17", ARM64_X17, false, false},  /* IP1, platform reserved */
    {"x18", ARM64_X18, false, false},  /* platform register */
    {"x19", ARM64_X19, true, true},
    {"x20", ARM64_X20, true, true},
    {"x21", ARM64_X21, true, true},
    {"x22", ARM64_X22, true, true},
    {"x23", ARM64_X23, true, true},
    {"x24", ARM64_X24, true, true},
    {"x25", ARM64_X25, true, true},
    {"x26", ARM64_X26, true, true},
    {"x27", ARM64_X27, true, true},
    {"x28", ARM64_X28, true, true},
    {"x29", ARM64_FP, true, false},    /* frame pointer */
    {"x30", ARM64_LR, false, false},   /* link register */
    {"sp", ARM64_SP, false, false},
};

static RegDesc arm64_float_regs[] = {
    {"v0", ARM64_V0, false, true},
    {"v1", ARM64_V1, false, true},
    {"v2", ARM64_V2, false, true},
    {"v3", ARM64_V3, false, true},
    {"v4", ARM64_V4, false, true},
    {"v5", ARM64_V5, false, true},
    {"v6", ARM64_V6, false, true},
    {"v7", ARM64_V7, false, true},
    {"v8", ARM64_V8, true, true},
    {"v9", ARM64_V9, true, true},
    {"v10", ARM64_V10, true, true},
    {"v11", ARM64_V11, true, true},
    {"v12", ARM64_V12, true, true},
    {"v13", ARM64_V13, true, true},
    {"v14", ARM64_V14, true, true},
    {"v15", ARM64_V15, true, true},
    {"v16", ARM64_V16, false, true},
    {"v17", ARM64_V17, false, true},
    {"v18", ARM64_V18, false, true},
    {"v19", ARM64_V19, false, true},
    {"v20", ARM64_V20, false, true},
    {"v21", ARM64_V21, false, true},
    {"v22", ARM64_V22, false, true},
    {"v23", ARM64_V23, false, true},
    {"v24", ARM64_V24, false, true},
    {"v25", ARM64_V25, false, true},
    {"v26", ARM64_V26, false, true},
    {"v27", ARM64_V27, false, true},
    {"v28", ARM64_V28, false, true},
    {"v29", ARM64_V29, false, true},
    {"v30", ARM64_V30, false, true},
    {"v31", ARM64_V31, false, true},
};

/* AAPCS64: x0-x7 for integer args, v0-v7 for float args */
static int arm64_arg_int_regs[] = {
    ARM64_X0, ARM64_X1, ARM64_X2, ARM64_X3,
    ARM64_X4, ARM64_X5, ARM64_X6, ARM64_X7,
};

static int arm64_arg_float_regs[] = {
    ARM64_V0, ARM64_V1, ARM64_V2, ARM64_V3,
    ARM64_V4, ARM64_V5, ARM64_V6, ARM64_V7,
};

static int arm64_callee_saved[] = {
    ARM64_X19, ARM64_X20, ARM64_X21, ARM64_X22,
    ARM64_X23, ARM64_X24, ARM64_X25, ARM64_X26,
    ARM64_X27, ARM64_X28, ARM64_FP,
};

Target *target_arm64(Arena *a, bool apple) {
    Target *t = arena_new(a, Target);
    t->arch = TARGET_ARM64;
    t->abi = apple ? ABI_APPLE_ARM64 : ABI_AAPCS64;
    t->ptr_size = 8;
    t->stack_align = 16;

    t->int_regs = arm64_int_regs;
    t->num_int_regs = ARM64_XREG_COUNT;
    t->float_regs = arm64_float_regs;
    t->num_float_regs = ARM64_VREG_COUNT;

    t->arg_int_regs = arm64_arg_int_regs;
    t->num_arg_int_regs = 8;
    t->arg_float_regs = arm64_arg_float_regs;
    t->num_arg_float_regs = 8;

    t->callee_saved = arm64_callee_saved;
    t->num_callee_saved = sizeof(arm64_callee_saved) / sizeof(int);

    t->emit_func = arm64_emit_func;

    return t;
}
