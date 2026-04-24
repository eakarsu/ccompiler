/*
 * regalloc.c - Register allocator for mycc
 *
 * Current implementation: naive stack-based allocation.
 * Every IR value is assigned a unique stack slot below %rbp.
 * No physical registers are assigned (reg == -1 for all values).
 *
 * Stack layout (growing downward):
 *
 *   +-------------------+  <-- %rbp
 *   |  saved %rbp       |
 *   +-------------------+  -8(%rbp)
 *   |  slot for %%%1     |
 *   +-------------------+  -16(%rbp)
 *   |  slot for %%%2     |
 *   +-------------------+  ...
 *   |        ...        |
 *   +-------------------+  <-- %rsp (aligned to 16)
 *
 * Alloca instructions receive extra space beyond the value slots.
 * The alloca's "result" is the address of that space (i.e. the
 * stack_slot stores the offset that leaq will use).
 *
 * This file will be replaced with a linear-scan allocator once
 * the naive backend is working end-to-end.
 */

#include "ir.h"
#include "target.h"
#include "arena.h"

#include <assert.h>

/* Align value up to 'align' (must be power of 2). */
static int ra_align_up(int value, int align)
{
    return (value + align - 1) & ~(align - 1);
}

/* Return the byte width we need for one stack slot of this type. */
static int ra_slot_size(IRType ty)
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
    case IR_TYPE_AGGREGATE: return ty.size > 0 ? ty.size : 8;
    default:            return 8;
    }
}

/* ------------------------------------------------------------------ */
/*  regalloc() - Assign stack slots to every value in a function      */
/* ------------------------------------------------------------------ */

void regalloc(IRFunc *func, Target *tgt, Arena *a)
{
    (void)tgt;
    (void)a;

    /*
     * offset tracks the current bottom of the stack frame (negative
     * displacement from %rbp).  We grow downward.
     */
    int offset = 0;

    /* Track the maximum alignment required by any alloca.  The entire frame
     * must be aligned to this value so that sp-relative addresses computed
     * from the alloca offsets are actually aligned at runtime. */
    int max_alloca_align = 16;

    /*
     * Pass 1: Assign slots to function parameters.
     *
     * Parameters arrive in registers (or on the stack for the > 6th/8th
     * integer/float arg).  In the naive backend we immediately spill all
     * register-passed parameters to the stack frame in the prologue, so
     * each parameter gets a stack slot here.
     */
    for (int i = 0; i < func->num_params; i++) {
        IRInst *param = func->params[i];
        param->reg = -1;

        int sz = ra_slot_size(param->ty);
        if (sz == 0) sz = 8;  /* defensive */

        /* Always allocate at least 8 bytes, aligned to 8, for uniform addressing. */
        int alloc = sz < 8 ? 8 : ra_align_up(sz, 8);
        offset -= alloc;
        param->stack_slot = offset;
    }

    /*
     * Pass 2: Walk every instruction in every block and assign slots.
     */
    for (IRBlock *bb = func->entry; bb; bb = bb->next) {
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            inst->reg = -1;

            /* Skip instructions that do not produce a value. */
            if (inst->op == IR_STORE || inst->op == IR_BR ||
                inst->op == IR_CONDBR || inst->op == IR_RETV ||
                inst->op == IR_MEMCPY || inst->op == IR_MEMSET) {
                inst->stack_slot = 0;
                continue;
            }

            /* IR_RET produces no usable value either. */
            if (inst->op == IR_RET) {
                inst->stack_slot = 0;
                continue;
            }

            /* Parameters were already assigned in pass 1. */
            if (inst->stack_slot != 0) continue;

            /* Alloca: reserve space for the allocation itself. */
            if (inst->op == IR_ALLOCA) {
                int alloca_al = inst->alloca_align > 0 ? inst->alloca_align : 8;
                int alloca_sz = inst->alloca_size  > 0 ? inst->alloca_size  : 8;

                /* Ensure minimum 8-byte allocation and alignment for the
                 * naive backend, which loads/stores 64-bit registers. */
                if (alloca_al < 8) alloca_al = 8;
                if (alloca_sz < 8) alloca_sz = 8;
                alloca_sz = ra_align_up(alloca_sz, alloca_al); /* align size to alloca alignment */

                /* Track maximum alignment for frame alignment below. */
                if (alloca_al > max_alloca_align) max_alloca_align = alloca_al;

                /* Reserve space for the actual allocation. */
                offset -= alloca_sz;

                /* Align the offset to the requested alignment. */
                offset = -(ra_align_up(-offset, alloca_al));

                /* The alloca "value" is the address of the reserved space.
                 * We store that offset; the emitter uses leaq offset(%rbp). */
                inst->stack_slot = offset;
                continue;
            }

            /* Normal value: allocate an 8-byte slot (we promote everything
             * to 64 bits in the naive backend for simplicity). */
            int sz = ra_slot_size(inst->ty);
            if (sz == 0) {
                /* Void-typed calls that are still in the IR. */
                inst->stack_slot = 0;
                continue;
            }

            int alloc = sz < 8 ? 8 : ra_align_up(sz, 8);
            offset -= alloc;
            inst->stack_slot = offset;
        }
    }

    /*
     * Record total frame size (positive number).  The emitter will
     * emit `subq $frame_size, %rsp` in the prologue.
     */
    func->stack_size = -offset;

    /* Align the local-variable area to the maximum alloca alignment.
     * This guarantees that the sp-relative addresses computed for each
     * alloca are actually aligned at runtime.  The ARM64/x86-64 ABI
     * guarantees SP is at least 16-byte aligned on entry; by aligning
     * stack_size to max_alloca_align (>= 16) the frame we subtract from
     * SP will preserve that alignment for every alloca slot. */
    func->stack_size = ra_align_up(func->stack_size, max_alloca_align);
    func->max_alloca_align = max_alloca_align;
}
