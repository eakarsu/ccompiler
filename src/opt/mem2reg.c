#include "cc.h"

/* mem2reg: Promote alloca instructions to SSA registers.
 * This is the most impactful single optimization, converting
 * the alloca/load/store pattern from IR generation into clean SSA.
 *
 * Algorithm:
 * 1. Find promotable allocas (only used by loads and stores)
 * 2. For each alloca, find blocks with stores (def blocks)
 * 3. Insert phi nodes at dominance frontiers
 * 4. Rename variables using dominator tree traversal
 */

/* Add use-def edge: record that 'user' references 'def' */
static void mem2reg_add_use(IRInst *def, IRInst *user) {
    if (def) vec_push(&def->users, user);
}

/* Remove use-def edge: remove 'user' from 'def's user list */
static void mem2reg_remove_use(IRInst *def, IRInst *user) {
    if (!def) return;
    for (int i = 0; i < def->users.len; i++) {
        if (def->users.data[i] == user) {
            def->users.data[i] = def->users.data[--def->users.len];
            return;
        }
    }
}

static bool is_promotable(IRInst *alloca) {
    if (alloca->op != IR_ALLOCA) return false;

    /* Don't promote aggregate-typed allocas (structs/unions).
     * These are accessed via IR_MEMCPY which mem2reg can't handle. */
    if (alloca->alloca_size > 8) return false;

    /* Check all users: only loads and stores to this alloca are allowed */
    for (int i = 0; i < alloca->users.len; i++) {
        IRInst *user = alloca->users.data[i];
        if (user->op == IR_LOAD) {
            /* Load from this alloca is fine */
            if (user->operands[0] != alloca) return false;
        } else if (user->op == IR_STORE) {
            /* Store to this alloca is fine (alloca must be the pointer operand) */
            if (user->num_operands < 2 || user->operands[1] != alloca) return false;
        } else {
            /* Other uses (GEP, passed to function, etc.) prevent promotion */
            return false;
        }
    }
    return true;
}

/* Get the value type stored in an alloca (from its load users) */
static IRType get_alloca_val_type(IRInst *alloca) {
    for (int i = 0; i < alloca->users.len; i++) {
        IRInst *user = alloca->users.data[i];
        if (user->op == IR_LOAD) return user->ty;
    }
    /* Fallback: derive from alloca size */
    switch (alloca->alloca_size) {
    case 1: return (IRType){IR_TYPE_I8};
    case 2: return (IRType){IR_TYPE_I16};
    case 4: return (IRType){IR_TYPE_I32};
    default: return (IRType){IR_TYPE_I64};
    }
}

static void insert_phi_nodes(Arena *a, IRFunc *func, IRInst *alloca) {
    /* Find blocks that contain stores to this alloca (defining blocks) */
    Vec(IRBlock *) def_blocks = {0};

    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            if (inst->op == IR_STORE && inst->num_operands >= 2 &&
                inst->operands[1] == alloca) {
                vec_push(&def_blocks, bb);
                break;
            }
        }
    }

    /* Insert phi nodes at dominance frontier of each defining block */
    Vec(IRBlock *) worklist = {0};
    Vec(IRBlock *) phi_blocks = {0};

    for (int i = 0; i < def_blocks.len; i++) {
        vec_push(&worklist, def_blocks.data[i]);
    }

    while (worklist.len > 0) {
        IRBlock *bb = vec_pop(&worklist);
        for (int i = 0; i < bb->dom_frontier.len; i++) {
            IRBlock *df = bb->dom_frontier.data[i];

            /* Check if we already inserted a phi here */
            bool already = false;
            for (int j = 0; j < phi_blocks.len; j++) {
                if (phi_blocks.data[j] == df) { already = true; break; }
            }
            if (already) continue;

            /* Insert phi node at beginning of df block */
            IRType val_ty = get_alloca_val_type(alloca);
            IRInst *phi = ir_build_phi(a, df, val_ty);
            phi->phi_alloca = alloca;  /* Tag phi with its source alloca */
            /* Move phi to front of block */
            if (phi->prev) {
                phi->prev->next = phi->next;
                if (phi->next) phi->next->prev = phi->prev;
                if (df->last == phi) df->last = phi->prev;
                phi->prev = NULL;
                phi->next = df->first;
                if (df->first) df->first->prev = phi;
                df->first = phi;
            }

            vec_push(&phi_blocks, df);
            vec_push(&worklist, df);
        }
    }

    vec_free(&def_blocks);
    vec_free(&worklist);
    vec_free(&phi_blocks);
}

typedef struct {
    Vec(IRInst *) stack;  /* stack of current values */
} RenameState;

static IRInst *get_current_val(RenameState *state) {
    if (state->stack.len == 0) return NULL;
    return vec_last(&state->stack);
}

static void rename_block(Arena *a, IRBlock *bb, IRInst *alloca, RenameState *state) {
    int saved_len = state->stack.len;

    /* Process all instructions in the block */
    IRInst *inst = bb->first;
    while (inst) {
        IRInst *next = inst->next;

        if (inst->op == IR_PHI && inst->phi_alloca == alloca) {
            /* Phi node for this alloca: push as the current definition */
            vec_push(&state->stack, inst);
        } else if (inst->op == IR_LOAD && inst->operands[0] == alloca) {
            /* Replace load with current value */
            IRInst *val = get_current_val(state);
            if (val) {
                /* Replace all uses of this load with the current value */
                for (int i = 0; i < inst->users.len; i++) {
                    IRInst *user = inst->users.data[i];
                    for (int j = 0; j < user->num_operands; j++) {
                        if (user->operands[j] == inst) {
                            user->operands[j] = val;
                            mem2reg_add_use(val, user);
                        }
                    }
                    /* Update phi vals too */
                    if (user->op == IR_PHI) {
                        for (int j = 0; j < user->phi_count; j++) {
                            if (user->phi_vals[j] == inst) {
                                user->phi_vals[j] = val;
                                mem2reg_add_use(val, user);
                            }
                        }
                    }
                }
            }
            inst->is_dead = true;
        } else if (inst->op == IR_STORE && inst->num_operands >= 2 &&
                   inst->operands[1] == alloca) {
            /* Store: the stored value becomes the current definition */
            vec_push(&state->stack, inst->operands[0]);
            inst->is_dead = true;
        }

        inst = next;
    }

    /* Fill in phi nodes in successor blocks (only for our alloca) */
    for (int i = 0; i < bb->succs.len; i++) {
        IRBlock *succ = bb->succs.data[i];
        for (IRInst *phi = succ->first; phi; phi = phi->next) {
            if (phi->op != IR_PHI) break;
            if (phi->phi_alloca != alloca) continue;
            IRInst *val = get_current_val(state);
            if (val) {
                ir_phi_add(a, phi, val, bb);
            }
        }
    }

    /* Recurse into dominator tree children */
    for (int i = 0; i < bb->dom_children.len; i++) {
        rename_block(a, bb->dom_children.data[i], alloca, state);
    }

    /* Restore stack */
    state->stack.len = saved_len;
}

/* Remove dead instructions marked during rename */
static void remove_dead_insts(IRFunc *func) {
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        IRInst *inst = bb->first;
        while (inst) {
            IRInst *next = inst->next;
            if (inst->is_dead) {
                /* Unlink from block */
                if (inst->prev) inst->prev->next = inst->next;
                else bb->first = inst->next;
                if (inst->next) inst->next->prev = inst->prev;
                else bb->last = inst->prev;
                bb->inst_count--;
            }
            inst = next;
        }
    }
}

bool opt_mem2reg(Arena *a, IRFunc *func) {
    bool changed = false;

    /* Find all promotable allocas */
    Vec(IRInst *) allocas = {0};
    IRBlock *entry = func->entry;
    if (!entry) goto done;

    for (IRInst *inst = entry->first; inst; inst = inst->next) {
        if (inst->op == IR_ALLOCA && is_promotable(inst)) {
            vec_push(&allocas, inst);
        }
    }

    if (allocas.len == 0) goto done;

    /* For each promotable alloca */
    for (int i = 0; i < allocas.len; i++) {
        IRInst *alloc = allocas.data[i];

        /* Insert phi nodes */
        insert_phi_nodes(a, func, alloc);

        /* Rename variables */
        RenameState state = {0};
        rename_block(a, func->entry, alloc, &state);
        vec_free(&state.stack);

        /* Mark alloca as dead */
        alloc->is_dead = true;
        changed = true;
    }

    /* Remove dead instructions */
    if (changed) {
        remove_dead_insts(func);
    }

done:
    vec_free(&allocas);
    return changed;
}
