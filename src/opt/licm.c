#include "cc.h"

/* Loop-Invariant Code Motion (LICM)
 * Move instructions that don't change within a loop to the preheader.
 */

static bool is_loop_invariant(IRInst *inst, IRBlock **loop_blocks, int num_loop_blocks) {
    /* Instructions with side effects can't be moved */
    if (inst->op == IR_STORE || inst->op == IR_CALL || inst->op == IR_LOAD ||
        inst->op == IR_BR || inst->op == IR_CONDBR || inst->op == IR_RET ||
        inst->op == IR_RETV || inst->op == IR_PHI || inst->op == IR_ALLOCA) {
        return false;
    }

    /* All operands must be defined outside the loop or be loop-invariant themselves */
    for (int i = 0; i < inst->num_operands; i++) {
        IRInst *op = inst->operands[i];
        if (!op) continue;

        bool in_loop = false;
        for (int j = 0; j < num_loop_blocks; j++) {
            if (op->parent == loop_blocks[j]) {
                in_loop = true;
                break;
            }
        }
        if (in_loop) return false;  /* Operand defined in loop */
    }

    return true;
}

bool opt_licm(Arena *a, IRFunc *func) {
    (void)a;
    bool changed = false;

    /* For each block, if it has loop_depth > 0, it's in a loop */
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        if (bb->loop_depth == 0) continue;

        /* Find all blocks in this loop (same depth or deeper) */
        Vec(IRBlock *) loop_blocks = {0};
        for (IRBlock *lb = func->blocks; lb; lb = lb->next) {
            if (lb->loop_depth >= bb->loop_depth) {
                vec_push(&loop_blocks, lb);
            }
        }

        /* Find preheader (predecessor of loop header not in loop) */
        IRBlock *preheader = NULL;
        for (int i = 0; i < bb->preds.len; i++) {
            IRBlock *pred = bb->preds.data[i];
            bool in_loop = false;
            for (int j = 0; j < loop_blocks.len; j++) {
                if (loop_blocks.data[j] == pred) { in_loop = true; break; }
            }
            if (!in_loop) {
                preheader = pred;
                break;
            }
        }

        if (!preheader) {
            vec_free(&loop_blocks);
            continue;
        }

        /* Try to hoist loop-invariant instructions */
        for (int li = 0; li < loop_blocks.len; li++) {
            IRBlock *loop_bb = loop_blocks.data[li];
            IRInst *inst = loop_bb->first;
            while (inst) {
                IRInst *next = inst->next;
                if (is_loop_invariant(inst, loop_blocks.data, loop_blocks.len)) {
                    /* Move to preheader (before terminator) */
                    /* Unlink from current block */
                    if (inst->prev) inst->prev->next = inst->next;
                    else loop_bb->first = inst->next;
                    if (inst->next) inst->next->prev = inst->prev;
                    else loop_bb->last = inst->prev;
                    loop_bb->inst_count--;

                    /* Insert before preheader's terminator.
                     * Scan backward to find the actual terminator, since
                     * other passes may have placed instructions after it. */
                    IRInst *term = preheader->last;
                    while (term && term->op != IR_BR && term->op != IR_CONDBR &&
                           term->op != IR_RET && term->op != IR_RETV) {
                        term = term->prev;
                    }
                    if (term && (term->op == IR_BR || term->op == IR_CONDBR ||
                                 term->op == IR_RET || term->op == IR_RETV)) {
                        inst->prev = term->prev;
                        inst->next = term;
                        if (term->prev) term->prev->next = inst;
                        else preheader->first = inst;
                        term->prev = inst;
                    } else {
                        /* No terminator, append */
                        inst->prev = preheader->last;
                        inst->next = NULL;
                        if (preheader->last) preheader->last->next = inst;
                        else preheader->first = inst;
                        preheader->last = inst;
                    }
                    inst->parent = preheader;
                    preheader->inst_count++;

                    changed = true;
                }
                inst = next;
            }
        }

        vec_free(&loop_blocks);
    }

    return changed;
}
