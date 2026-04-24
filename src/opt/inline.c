#include "cc.h"

/* Function inlining with a simple cost model.
 * Inline small functions at call sites.
 */

static int compute_cost(IRFunc *func) {
    int cost = 0;
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            cost++;
            /* Calls are expensive */
            if (inst->op == IR_CALL) cost += 5;
        }
    }
    return cost;
}

static bool should_inline(IRFunc *callee, OptLevel level) {
    if (!callee || !callee->entry) return false;
    /* Don't inline recursive functions */
    for (IRBlock *bb = callee->blocks; bb; bb = bb->next) {
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            if (inst->op == IR_CALL && inst->call_target &&
                strcmp(inst->call_target, callee->name) == 0)
                return false;
        }
    }

    int cost = compute_cost(callee);

    switch (level) {
    case OPT_O0:
    case OPT_O1:
        return false;  /* No inlining at O0/O1 */
    case OPT_O2:
        return cost <= 20;  /* Small functions only */
    case OPT_O3:
        return cost <= 100; /* More aggressive */
    }
    return false;
}

/* Find a function by name in the module */
static IRFunc *find_func(IRModule *mod, const char *name) {
    for (IRFunc *f = mod->funcs; f; f = f->next) {
        if (f->name && name && strcmp(f->name, name) == 0 && f->entry)
            return f;
    }
    return NULL;
}

/* Clone an instruction into a new block, remapping values through a map.
 * Returns the cloned instruction. */
static IRInst *clone_inst(Arena *a, IRBlock *dest_bb, IRInst *orig,
                          IRInst **val_map, int map_size) {
    IRInst *clone = arena_new(a, IRInst);
    *clone = *orig;

    /* Assign new ID in dest function */
    clone->id = dest_bb->parent->next_inst_id++;
    clone->parent = dest_bb;
    clone->next = NULL;
    clone->prev = NULL;
    memset(&clone->users, 0, sizeof(clone->users));

    /* Remap operands */
    if (orig->num_operands > 0) {
        clone->operands = arena_alloc(a, sizeof(IRInst *) * orig->num_operands);
        for (int i = 0; i < orig->num_operands; i++) {
            IRInst *op = orig->operands[i];
            if (op && op->id < map_size && val_map[op->id])
                clone->operands[i] = val_map[op->id];
            else
                clone->operands[i] = op;
        }
    }

    /* Remap phi values */
    if (orig->op == IR_PHI && orig->phi_count > 0) {
        clone->phi_vals = arena_alloc(a, sizeof(IRInst *) * orig->phi_count);
        clone->phi_blocks = arena_alloc(a, sizeof(IRBlock *) * orig->phi_count);
        for (int i = 0; i < orig->phi_count; i++) {
            IRInst *pv = orig->phi_vals[i];
            if (pv && pv->id < map_size && val_map[pv->id])
                clone->phi_vals[i] = val_map[pv->id];
            else
                clone->phi_vals[i] = pv;
            clone->phi_blocks[i] = orig->phi_blocks[i]; /* remapped later */
        }
    }

    /* Link into block */
    if (dest_bb->last) {
        dest_bb->last->next = clone;
        clone->prev = dest_bb->last;
        dest_bb->last = clone;
    } else {
        dest_bb->first = dest_bb->last = clone;
    }
    dest_bb->inst_count++;

    return clone;
}

/* Inline a call site: replace the call with the callee's body */
static bool inline_call(Arena *a, IRFunc *caller, IRBlock *call_block,
                        IRInst *call_inst, IRFunc *callee) {
    /* We need to:
     * 1. Create a continuation block (code after the call)
     * 2. Clone all callee blocks into the caller
     * 3. Replace parameter references with call arguments
     * 4. Replace ret instructions with branches to continuation
     * 5. Replace all uses of call result with returned value
     */

    /* Build value map: callee inst ID -> caller inst */
    int map_size = callee->next_inst_id + 1;
    IRInst **val_map = arena_alloc(a, sizeof(IRInst *) * map_size);
    memset(val_map, 0, sizeof(IRInst *) * map_size);

    /* Map callee parameters to call arguments */
    for (int i = 0; i < callee->num_params && i < call_inst->num_operands; i++) {
        if (callee->params[i])
            val_map[callee->params[i]->id] = call_inst->operands[i];
    }

    /* Split call_block: instructions after call go to continuation block */
    IRBlock *cont_bb = arena_new(a, IRBlock);
    char *cont_name = arena_alloc(a, strlen(caller->name) + 32);
    sprintf(cont_name, "%s_inline_cont", caller->name);
    cont_bb->name = cont_name;
    cont_bb->id = caller->next_block_id++;
    cont_bb->parent = caller;

    /* Move instructions after call_inst to cont_bb */
    IRInst *after_call = call_inst->next;
    if (after_call) {
        cont_bb->first = after_call;
        cont_bb->last = call_block->last;
        after_call->prev = NULL;
        call_block->last = call_inst;
        call_inst->next = NULL;

        for (IRInst *inst = cont_bb->first; inst; inst = inst->next) {
            inst->parent = cont_bb;
            cont_bb->inst_count++;
            call_block->inst_count--;
        }
    }

    /* Insert cont_bb after call_block in block list */
    cont_bb->next = call_block->next;
    cont_bb->prev = call_block;
    if (call_block->next) call_block->next->prev = cont_bb;
    call_block->next = cont_bb;

    /* Block map: callee block ID -> cloned block in caller */
    int bmap_size = callee->next_block_id + 1;
    IRBlock **block_map = arena_alloc(a, sizeof(IRBlock *) * bmap_size);
    memset(block_map, 0, sizeof(IRBlock *) * bmap_size);

    /* Create cloned blocks for all callee blocks */
    IRBlock *last_inserted = cont_bb;
    for (IRBlock *bb = callee->blocks; bb; bb = bb->next) {
        IRBlock *new_bb = arena_new(a, IRBlock);
        char *bname = arena_alloc(a, strlen(caller->name) + strlen(bb->name) + 16);
        sprintf(bname, "%s_inline_%s", caller->name, bb->name);
        new_bb->name = bname;
        new_bb->id = caller->next_block_id++;
        new_bb->parent = caller;

        /* Insert after last_inserted */
        new_bb->next = last_inserted->next;
        new_bb->prev = last_inserted;
        if (last_inserted->next) last_inserted->next->prev = new_bb;
        last_inserted->next = new_bb;
        last_inserted = new_bb;

        block_map[bb->id] = new_bb;
        caller->block_count++;
    }

    /* Collect return values for creating a result phi if needed */
    Vec(IRInst *) ret_vals = {0};
    Vec(IRBlock *) ret_blocks = {0};

    /* Clone instructions from callee into cloned blocks */
    for (IRBlock *bb = callee->blocks; bb; bb = bb->next) {
        IRBlock *new_bb = block_map[bb->id];
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            if (inst->op == IR_RET) {
                /* Replace return with branch to continuation */
                if (inst->num_operands > 0) {
                    IRInst *retval = inst->operands[0];
                    if (retval && retval->id < map_size && val_map[retval->id])
                        retval = val_map[retval->id];
                    vec_push(&ret_vals, retval);
                    vec_push(&ret_blocks, new_bb);
                }
                /* Build branch to cont */
                IRInst *br = arena_new(a, IRInst);
                br->op = IR_BR;
                br->id = caller->next_inst_id++;
                br->parent = new_bb;
                br->br_target = cont_bb;
                if (new_bb->last) {
                    new_bb->last->next = br;
                    br->prev = new_bb->last;
                    new_bb->last = br;
                } else {
                    new_bb->first = new_bb->last = br;
                }
                new_bb->inst_count++;
            } else if (inst->op == IR_RETV) {
                /* Void return: branch to continuation */
                IRInst *br = arena_new(a, IRInst);
                br->op = IR_BR;
                br->id = caller->next_inst_id++;
                br->parent = new_bb;
                br->br_target = cont_bb;
                if (new_bb->last) {
                    new_bb->last->next = br;
                    br->prev = new_bb->last;
                    new_bb->last = br;
                } else {
                    new_bb->first = new_bb->last = br;
                }
                new_bb->inst_count++;
            } else {
                IRInst *c = clone_inst(a, new_bb, inst, val_map, map_size);
                val_map[inst->id] = c;
            }
        }

        /* Remap branch targets in cloned block's terminator */
        IRInst *term = new_bb->last;
        if (term) {
            if (term->op == IR_BR && term->br_target != cont_bb) {
                if (term->br_target && term->br_target->id < bmap_size &&
                    block_map[term->br_target->id])
                    term->br_target = block_map[term->br_target->id];
            }
            if (term->op == IR_CONDBR) {
                if (term->br_true && term->br_true->id < bmap_size &&
                    block_map[term->br_true->id])
                    term->br_true = block_map[term->br_true->id];
                if (term->br_false && term->br_false->id < bmap_size &&
                    block_map[term->br_false->id])
                    term->br_false = block_map[term->br_false->id];
            }
        }

        /* Remap phi predecessor blocks */
        for (IRInst *inst = new_bb->first; inst; inst = inst->next) {
            if (inst->op == IR_PHI) {
                for (int i = 0; i < inst->phi_count; i++) {
                    IRBlock *pb = inst->phi_blocks[i];
                    if (pb && pb->id < bmap_size && block_map[pb->id])
                        inst->phi_blocks[i] = block_map[pb->id];
                }
            }
        }
    }

    /* Replace call instruction in call_block with branch to callee entry */
    /* Remove call from block */
    if (call_inst->prev)
        call_inst->prev->next = NULL;
    else
        call_block->first = NULL;
    call_block->last = call_inst->prev;
    call_block->inst_count--;

    /* Add branch to inlined entry */
    IRBlock *inline_entry = block_map[callee->entry->id];
    IRInst *br = arena_new(a, IRInst);
    br->op = IR_BR;
    br->id = caller->next_inst_id++;
    br->parent = call_block;
    br->br_target = inline_entry;
    if (call_block->last) {
        call_block->last->next = br;
        br->prev = call_block->last;
        call_block->last = br;
    } else {
        call_block->first = call_block->last = br;
    }
    call_block->inst_count++;

    /* Replace uses of call result with returned value(s) */
    if (ret_vals.len == 1) {
        /* Single return: directly replace */
        IRInst *retval = ret_vals.data[0];
        for (int i = 0; i < call_inst->users.len; i++) {
            IRInst *user = call_inst->users.data[i];
            for (int j = 0; j < user->num_operands; j++) {
                if (user->operands[j] == call_inst)
                    user->operands[j] = retval;
            }
            if (user->op == IR_PHI) {
                for (int j = 0; j < user->phi_count; j++) {
                    if (user->phi_vals[j] == call_inst)
                        user->phi_vals[j] = retval;
                }
            }
        }
    } else if (ret_vals.len > 1) {
        /* Multiple returns: create phi in continuation block */
        IRInst *phi = ir_build_phi(a, cont_bb, callee->ret_type);
        /* Move phi to front */
        if (phi->prev) {
            phi->prev->next = phi->next;
            if (phi->next) phi->next->prev = phi->prev;
            if (cont_bb->last == phi) cont_bb->last = phi->prev;
            phi->prev = NULL;
            phi->next = cont_bb->first;
            if (cont_bb->first) cont_bb->first->prev = phi;
            cont_bb->first = phi;
        }
        for (int i = 0; i < ret_vals.len; i++) {
            ir_phi_add(a, phi, ret_vals.data[i], ret_blocks.data[i]);
        }
        /* Replace call uses with phi */
        for (int i = 0; i < call_inst->users.len; i++) {
            IRInst *user = call_inst->users.data[i];
            for (int j = 0; j < user->num_operands; j++) {
                if (user->operands[j] == call_inst)
                    user->operands[j] = phi;
            }
        }
    }

    vec_free(&ret_vals);
    vec_free(&ret_blocks);
    return true;
}

bool opt_inline(Arena *a, IRModule *mod) {
    bool changed = false;

    for (IRFunc *caller = mod->funcs; caller; caller = caller->next) {
        if (!caller->entry) continue;

        for (IRBlock *bb = caller->blocks; bb; bb = bb->next) {
            for (IRInst *inst = bb->first; inst; inst = inst->next) {
                if (inst->op != IR_CALL || !inst->call_target) continue;

                IRFunc *callee = find_func(mod, inst->call_target);
                if (!callee || callee == caller) continue;
                if (!should_inline(callee, OPT_O2)) continue;

                if (inline_call(a, caller, bb, inst, callee)) {
                    changed = true;
                    /* Restart scanning this function since CFG changed */
                    goto next_func;
                }
            }
        }
        next_func:;
    }

    return changed;
}
