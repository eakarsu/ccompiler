#ifndef IR_H
#define IR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "vec.h"

typedef struct Arena Arena;
typedef struct Type Type;

/* IR Type kinds (simplified from C types) */
typedef enum {
    IR_TYPE_I1,
    IR_TYPE_I8,
    IR_TYPE_I16,
    IR_TYPE_I32,
    IR_TYPE_I64,
    IR_TYPE_F32,
    IR_TYPE_F64,
    IR_TYPE_PTR,
    IR_TYPE_VOID,
    IR_TYPE_AGGREGATE,  /* struct/union passed by value */
} IRTypeKind;

typedef struct IRType {
    IRTypeKind kind;
    int size;
    int align;
    int is_unsigned; /* for i8/i16: determines ldrb vs ldrsb in backend */
} IRType;

/* SSA Value (every instruction produces a value) */
typedef struct IRInst IRInst;
typedef struct IRBlock IRBlock;
typedef struct IRFunc IRFunc;

typedef enum {
    /* Constants */
    IR_CONST_INT,
    IR_CONST_FLOAT,

    /* Memory */
    IR_ALLOCA,
    IR_LOAD,
    IR_STORE,
    IR_GEP,           /* get element pointer */

    /* Arithmetic */
    IR_ADD,
    IR_SUB,
    IR_MUL,
    IR_SDIV,
    IR_UDIV,
    IR_SREM,
    IR_UREM,
    IR_FADD,
    IR_FSUB,
    IR_FMUL,
    IR_FDIV,
    IR_NEG,
    IR_FNEG,

    /* Bitwise */
    IR_AND,
    IR_OR,
    IR_XOR,
    IR_SHL,
    IR_LSHR,           /* logical shift right */
    IR_ASHR,           /* arithmetic shift right */
    IR_NOT,

    /* Comparison */
    IR_ICMP_EQ,
    IR_ICMP_NE,
    IR_ICMP_SLT,
    IR_ICMP_SLE,
    IR_ICMP_SGT,
    IR_ICMP_SGE,
    IR_ICMP_ULT,
    IR_ICMP_ULE,
    IR_ICMP_UGT,
    IR_ICMP_UGE,
    IR_FCMP_OEQ,
    IR_FCMP_ONE,
    IR_FCMP_OLT,
    IR_FCMP_OLE,
    IR_FCMP_OGT,
    IR_FCMP_OGE,

    /* Conversion */
    IR_SEXT,
    IR_ZEXT,
    IR_TRUNC,
    IR_SITOFP,
    IR_UITOFP,
    IR_FPTOSI,
    IR_FPTOUI,
    IR_FPEXT,
    IR_FPTRUNC,
    IR_BITCAST,
    IR_PTRTOINT,
    IR_INTTOPTR,

    /* Control flow */
    IR_BR,             /* unconditional branch */
    IR_CONDBR,         /* conditional branch */
    IR_RET,
    IR_RETV,           /* return void */

    /* Other */
    IR_CALL,
    IR_PHI,
    IR_COPY,           /* for SSA construction */
    IR_MEMCPY,
    IR_MEMSET,

    IR_OP_COUNT
} IROpcode;

/* Operand reference: just an instruction pointer (SSA value) */
typedef struct {
    IRInst *inst;
} IRUse;

struct IRInst {
    IROpcode op;
    IRType ty;
    int id;               /* unique value number for printing: %1, %2... */
    IRBlock *parent;

    /* Operands */
    IRInst **operands;
    int num_operands;

    /* Constant value */
    union {
        int64_t ival;
        double fval;
    };

    /* For alloca */
    int alloca_size;
    int alloca_align;

    /* For GEP */
    int gep_offset;

    /* For call */
    const char *call_target;
    IRFunc *call_func;
    bool is_variadic_call;   /* true if calling a variadic function */
    int  num_fixed_args;     /* number of fixed (non-variadic) parameters */

    /* For PHI: parallel arrays of (value, predecessor block) */
    IRInst **phi_vals;
    IRBlock **phi_blocks;
    int phi_count;
    IRInst *phi_alloca;    /* alloca this phi was created for (used by mem2reg) */

    /* For branch */
    IRBlock *br_target;
    IRBlock *br_true;
    IRBlock *br_false;

    /* Linked list in block */
    IRInst *next;
    IRInst *prev;

    /* Use-def chain: list of instructions that use this value */
    Vec(IRInst *) users;

    /* Register allocation result */
    int reg;           /* physical register or -1 */
    int stack_slot;    /* spill slot or -1 */

    /* Flags */
    bool is_dead;
    bool visited;
};

struct IRBlock {
    const char *name;
    int id;
    IRFunc *parent;

    IRInst *first;     /* first instruction */
    IRInst *last;      /* last instruction (terminator) */
    int inst_count;

    /* CFG edges */
    Vec(IRBlock *) preds;
    Vec(IRBlock *) succs;

    /* Dominator info */
    IRBlock *idom;     /* immediate dominator */
    Vec(IRBlock *) dom_children;
    Vec(IRBlock *) dom_frontier;
    int dom_order;     /* DFS order for dominance checks */

    /* Loop info */
    int loop_depth;

    /* Linked list */
    IRBlock *next;
    IRBlock *prev;
};

struct IRFunc {
    const char *name;
    IRType ret_type;
    IRInst **params;     /* parameter instructions */
    int num_params;

    IRBlock *entry;      /* first block */
    IRBlock *blocks;     /* linked list of all blocks */
    int block_count;
    int next_inst_id;
    int next_block_id;

    /* For register allocation */
    int stack_size;
    int max_alloca_align; /* maximum alignment required by any alloca (>= 16) */

    IRFunc *next;        /* linked list in module */
};

/* Relocation entry for global pointer initializers */
typedef struct IRReloc {
    int offset;          /* offset within init_data */
    const char *symbol;  /* symbol name to relocate to */
    struct IRReloc *next;
} IRReloc;

typedef struct IRGlobal {
    const char *name;
    IRType ty;
    int alloc_size;     /* Actual allocation size in bytes (for arrays, structs) */
    int alloc_align;    /* Actual alignment (may differ from ty.align for arrays) */
    bool is_string;
    const char *string_val;
    int string_len;
    int64_t init_val;
    bool has_init;
    bool is_extern;
    bool is_static;
    /* Byte-level init data for arrays/structs */
    uint8_t *init_data;
    int init_data_len;
    IRReloc *relocs;    /* relocation entries for pointer initializers */
    struct IRGlobal *next;
} IRGlobal;

typedef struct IRModule {
    IRFunc *funcs;
    IRGlobal *globals;
    Arena *arena;
} IRModule;

/* IR construction */
IRModule *ir_module_new(Arena *a);
IRFunc   *ir_func_new(Arena *a, IRModule *mod, const char *name, IRType ret_type);
IRBlock  *ir_block_new(Arena *a, IRFunc *func, const char *name);
IRInst   *ir_inst_new(Arena *a, IRBlock *block, IROpcode op, IRType ty);
void      ir_block_append(IRBlock *block, IRInst *inst);
void      ir_block_insert_before(IRBlock *block, IRInst *before, IRInst *inst);

/* Instruction builders */
IRInst *ir_build_const_int(Arena *a, IRBlock *bb, IRType ty, int64_t val);
IRInst *ir_build_const_float(Arena *a, IRBlock *bb, IRType ty, double val);
IRInst *ir_build_alloca(Arena *a, IRBlock *bb, int size, int align);
IRInst *ir_build_load(Arena *a, IRBlock *bb, IRType ty, IRInst *ptr);
IRInst *ir_build_store(Arena *a, IRBlock *bb, IRInst *val, IRInst *ptr);
IRInst *ir_build_memcpy(Arena *a, IRBlock *bb, IRInst *dst, IRInst *src, int size);
IRInst *ir_build_gep(Arena *a, IRBlock *bb, IRInst *ptr, int offset);
IRInst *ir_build_binop(Arena *a, IRBlock *bb, IROpcode op, IRInst *lhs, IRInst *rhs);
IRInst *ir_build_unop(Arena *a, IRBlock *bb, IROpcode op, IRInst *operand);
IRInst *ir_build_cast(Arena *a, IRBlock *bb, IROpcode op, IRType target, IRInst *src);
IRInst *ir_build_call(Arena *a, IRBlock *bb, IRType ret_ty, const char *target,
                      IRInst **args, int nargs);
IRInst *ir_build_br(Arena *a, IRBlock *bb, IRBlock *target);
IRInst *ir_build_condbr(Arena *a, IRBlock *bb, IRInst *cond,
                        IRBlock *true_bb, IRBlock *false_bb);
IRInst *ir_build_ret(Arena *a, IRBlock *bb, IRInst *val);
IRInst *ir_build_retv(Arena *a, IRBlock *bb);
IRInst *ir_build_phi(Arena *a, IRBlock *bb, IRType ty);
void    ir_phi_add(Arena *a, IRInst *phi, IRInst *val, IRBlock *pred);

/* IR utilities */
void ir_print_module(IRModule *mod, FILE *out);
void ir_print_func(IRFunc *func, FILE *out);
bool ir_verify_func(IRFunc *func);
bool ir_type_eq(IRType a, IRType b);
int  ir_type_size(IRType ty);
IRType ir_type_from_ctype(Type *cty);

/* CFG utilities */
void ir_compute_cfg(IRFunc *func);
void ir_compute_dominators(IRFunc *func);
void ir_compute_dom_frontiers(IRFunc *func);
void ir_compute_loop_info(IRFunc *func);
bool ir_dominates(IRBlock *a, IRBlock *b);

#endif /* IR_H */
