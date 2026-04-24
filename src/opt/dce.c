#include "cc.h"

/* Dead Code Elimination
 * Remove instructions whose results are never used.
 * Iteratively removes dead instructions until fixpoint.
 */

static bool has_side_effects(IRInst *inst) {
    switch (inst->op) {
    case IR_STORE:
    case IR_CALL:
    case IR_BR:
    case IR_CONDBR:
    case IR_RET:
    case IR_RETV:
    case IR_MEMCPY:
    case IR_MEMSET:
        return true;
    default:
        return false;
    }
}

static bool is_dead(IRInst *inst) {
    if (has_side_effects(inst)) return false;
    if (inst->op == IR_PHI && inst->users.len == 0) return true;
    if (inst->users.len == 0 && inst->op != IR_BR && inst->op != IR_CONDBR &&
        inst->op != IR_RET && inst->op != IR_RETV) {
        return true;
    }
    return false;
}

static void remove_from_users(IRInst *inst) {
    /* Remove this instruction from the user lists of its operands */
    for (int i = 0; i < inst->num_operands; i++) {
        IRInst *op = inst->operands[i];
        if (!op) continue;
        for (int j = 0; j < op->users.len; j++) {
            if (op->users.data[j] == inst) {
                /* Remove by swapping with last */
                op->users.data[j] = op->users.data[op->users.len - 1];
                op->users.len--;
                break;
            }
        }
    }
    /* Same for phi operands */
    if (inst->op == IR_PHI) {
        for (int i = 0; i < inst->phi_count; i++) {
            IRInst *val = inst->phi_vals[i];
            if (!val) continue;
            for (int j = 0; j < val->users.len; j++) {
                if (val->users.data[j] == inst) {
                    val->users.data[j] = val->users.data[val->users.len - 1];
                    val->users.len--;
                    break;
                }
            }
        }
    }
}

bool opt_dce(IRFunc *func) {
    bool changed = false;
    bool progress = true;

    while (progress) {
        progress = false;
        for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
            IRInst *inst = bb->first;
            while (inst) {
                IRInst *next = inst->next;
                if (is_dead(inst)) {
                    remove_from_users(inst);
                    /* Unlink */
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
    }

    /* Remove unreachable blocks */
    /* Mark reachable blocks via BFS from entry */
    if (!func->entry) return changed;

    Vec(IRBlock *) worklist = {0};

    /* Reset visited (reuse dom_order as marker) */
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        bb->dom_order = 0;
    }
    func->entry->dom_order = 1;
    vec_push(&worklist, func->entry);

    int head = 0;
    while (head < worklist.len) {
        IRBlock *bb = worklist.data[head++];
        for (int i = 0; i < bb->succs.len; i++) {
            IRBlock *succ = bb->succs.data[i];
            if (succ->dom_order == 0) {
                succ->dom_order = 1;
                vec_push(&worklist, succ);
            }
        }
    }

    /* Remove unreachable blocks */
    IRBlock **prev_ptr = &func->blocks;
    for (IRBlock *bb = func->blocks; bb; ) {
        IRBlock *next = bb->next;
        if (bb->dom_order == 0) {
            *prev_ptr = next;
            if (next) next->prev = bb->prev;
            func->block_count--;
            changed = true;
        } else {
            prev_ptr = &bb->next;
        }
        bb = next;
    }

    vec_free(&worklist);
    return changed;
}
