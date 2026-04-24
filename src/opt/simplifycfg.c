#include "cc.h"

/* CFG Simplification
 * - Remove empty blocks (only contain an unconditional branch)
 * - Merge blocks with single predecessor/successor
 * - Simplify branches with constant conditions
 */

static bool is_empty_block(IRBlock *bb) {
    /* A block with only a branch terminator */
    if (!bb->first) return true;
    if (bb->first == bb->last && bb->first->op == IR_BR) return true;
    return false;
}

static bool try_merge_blocks(IRBlock *bb) {
    /* Merge bb with its single successor if the successor has only one predecessor */
    if (bb->succs.len != 1) return false;
    IRBlock *succ = bb->succs.data[0];
    if (succ->preds.len != 1) return false;
    if (succ == bb) return false;  /* self-loop */

    /* Remove bb's terminator */
    IRInst *term = bb->last;
    if (term && term->op == IR_BR) {
        if (term->prev) {
            term->prev->next = NULL;
            bb->last = term->prev;
        } else {
            bb->first = NULL;
            bb->last = NULL;
        }
        bb->inst_count--;
    }

    /* Append succ's instructions to bb */
    if (succ->first) {
        if (bb->last) {
            bb->last->next = succ->first;
            succ->first->prev = bb->last;
        } else {
            bb->first = succ->first;
        }
        bb->last = succ->last;

        /* Update parent pointers */
        for (IRInst *inst = succ->first; inst; inst = inst->next) {
            inst->parent = bb;
            bb->inst_count++;
        }
    }

    /* Update CFG: bb now has succ's successors */
    bb->succs.len = 0;
    for (int i = 0; i < succ->succs.len; i++) {
        vec_push(&bb->succs, succ->succs.data[i]);
        /* Update predecessor of succ's successors */
        IRBlock *ss = succ->succs.data[i];
        for (int j = 0; j < ss->preds.len; j++) {
            if (ss->preds.data[j] == succ) {
                ss->preds.data[j] = bb;
            }
        }
        /* Update PHI nodes in succ's successors: replace succ with bb */
        for (IRInst *phi = ss->first; phi; phi = phi->next) {
            if (phi->op != IR_PHI) break;
            for (int j = 0; j < phi->phi_count; j++) {
                if (phi->phi_blocks[j] == succ) {
                    phi->phi_blocks[j] = bb;
                }
            }
        }
    }

    /* Mark succ as dead (will be removed later) */
    succ->first = NULL;
    succ->last = NULL;
    succ->inst_count = 0;

    return true;
}

bool opt_simplifycfg(IRFunc *func) {
    bool changed = false;
    bool progress = true;

    while (progress) {
        progress = false;

        for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
            if (!bb->first) continue;

            /* Simplify constant conditional branches */
            IRInst *term = bb->last;
            if (term && term->op == IR_CONDBR && term->num_operands >= 1) {
                IRInst *cond = term->operands[0];
                if (cond && cond->op == IR_CONST_INT) {
                    /* Replace with unconditional branch */
                    IRBlock *target = cond->ival ? term->br_true : term->br_false;
                    IRBlock *dropped = cond->ival ? term->br_false : term->br_true;
                    term->op = IR_BR;
                    term->br_target = target;
                    term->num_operands = 0;

                    /* Update CFG */
                    bb->succs.len = 0;
                    vec_push(&bb->succs, target);

                    /* Remove bb from dropped block's pred list */
                    for (int j = 0; j < dropped->preds.len; j++) {
                        if (dropped->preds.data[j] == bb) {
                            dropped->preds.data[j] = dropped->preds.data[--dropped->preds.len];
                            break;
                        }
                    }

                    /* Remove phi entries for bb in dropped block */
                    for (IRInst *phi = dropped->first; phi; phi = phi->next) {
                        if (phi->op != IR_PHI) break;
                        for (int j = 0; j < phi->phi_count; j++) {
                            if (phi->phi_blocks[j] == bb) {
                                /* Remove this entry by shifting */
                                for (int k = j; k < phi->phi_count - 1; k++) {
                                    phi->phi_vals[k] = phi->phi_vals[k + 1];
                                    phi->phi_blocks[k] = phi->phi_blocks[k + 1];
                                }
                                phi->phi_count--;
                                j--;
                                break;
                            }
                        }
                    }

                    changed = true;
                    progress = true;
                }
            }

            /* Try to merge with successor */
            if (try_merge_blocks(bb)) {
                changed = true;
                progress = true;
            }

            /* Simplify single-entry phi nodes */
            for (IRInst *inst = bb->first; inst; ) {
                IRInst *next = inst->next;
                if (inst->op != IR_PHI) break;
                if (inst->phi_count == 1) {
                    /* Replace with the single value */
                    IRInst *val = inst->phi_vals[0];
                    for (int u = 0; u < inst->users.len; u++) {
                        IRInst *user = inst->users.data[u];
                        for (int j = 0; j < user->num_operands; j++) {
                            if (user->operands[j] == inst)
                                user->operands[j] = val;
                        }
                        if (user->op == IR_PHI) {
                            for (int j = 0; j < user->phi_count; j++) {
                                if (user->phi_vals[j] == inst)
                                    user->phi_vals[j] = val;
                            }
                        }
                    }
                    /* Remove from block */
                    if (inst->prev) inst->prev->next = inst->next;
                    else bb->first = inst->next;
                    if (inst->next) inst->next->prev = inst->prev;
                    else bb->last = inst->prev;
                    bb->inst_count--;
                    changed = true;
                    progress = true;
                } else if (inst->phi_count == 0) {
                    /* Dead phi - remove */
                    if (inst->prev) inst->prev->next = inst->next;
                    else bb->first = inst->next;
                    if (inst->next) inst->next->prev = inst->prev;
                    else bb->last = inst->prev;
                    bb->inst_count--;
                    changed = true;
                    progress = true;
                }
                inst = next;
            }
        }

        /* Remove empty/dead blocks */
        for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
            if (bb == func->entry) continue;
            if (bb->preds.len == 0 && bb->first) {
                /* Unreachable block - clear it */
                bb->first = NULL;
                bb->last = NULL;
                bb->inst_count = 0;
                changed = true;
                progress = true;
            }
        }
    }

    return changed;
}
