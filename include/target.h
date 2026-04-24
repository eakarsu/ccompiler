#ifndef TARGET_H
#define TARGET_H

#include <stdbool.h>
#include <stdio.h>

typedef struct IRModule IRModule;
typedef struct IRFunc IRFunc;
typedef struct IRInst IRInst;
typedef struct Arena Arena;

typedef enum {
    TARGET_X86_64,
    TARGET_ARM64,
} TargetArch;

typedef enum {
    ABI_SYSV,           /* System V AMD64 ABI */
    ABI_AAPCS64,        /* ARM64 standard */
    ABI_APPLE_ARM64,    /* Apple ARM64 variant */
} TargetABI;

/* Physical register descriptor */
typedef struct {
    const char *name;
    int id;
    bool is_callee_saved;
    bool is_allocable;
} RegDesc;

typedef struct Target {
    TargetArch arch;
    TargetABI abi;
    int ptr_size;
    int stack_align;

    /* Registers */
    const RegDesc *int_regs;
    int num_int_regs;
    const RegDesc *float_regs;
    int num_float_regs;

    /* ABI: argument passing registers */
    const int *arg_int_regs;
    int num_arg_int_regs;
    const int *arg_float_regs;
    int num_arg_float_regs;

    /* Callee-saved register list */
    const int *callee_saved;
    int num_callee_saved;

    /* Function pointers for target-specific operations */
    void (*emit_func)(IRFunc *func, FILE *out, struct Target *tgt, Arena *a);
    void (*emit_prologue)(FILE *out, struct Target *tgt);
    void (*emit_epilogue)(FILE *out, struct Target *tgt);
} Target;

/* Target initialization */
Target *target_x86_64(Arena *a);
Target *target_arm64(Arena *a, bool apple);

/* Code generation entry point */
void codegen(IRModule *mod, Target *tgt, FILE *out, Arena *a);

/* Register allocator */
void regalloc(IRFunc *func, Target *tgt, Arena *a);

/* Detect host target */
Target *target_host(Arena *a);

#endif /* TARGET_H */
