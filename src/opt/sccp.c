#include "cc.h"

/* Sparse Conditional Constant Propagation (SCCP)
 *
 * Combines constant propagation with dead branch elimination.
 * Uses a lattice: TOP -> constant -> BOTTOM (overdefined)
 *
 * Algorithm:
 * 1. Initialize all values to TOP
 * 2. Use two worklists: SSA edges and CFG edges
 * 3. When a value becomes constant, propagate to users
 * 4. When a branch condition is constant, only mark the taken edge executable
 */

typedef enum {
    LAT_TOP,       /* undefined / not yet computed */
    LAT_CONST,     /* known constant value */
    LAT_BOTTOM,    /* overdefined (multiple possible values) */
} LatKind;

typedef struct {
    LatKind kind;
    int64_t val;
} LatVal;

static LatVal lat_top(void) { return (LatVal){LAT_TOP, 0}; }
static LatVal lat_const(int64_t v) { return (LatVal){LAT_CONST, v}; }
static LatVal lat_bottom(void) { return (LatVal){LAT_BOTTOM, 0}; }

static LatVal lat_meet(LatVal a, LatVal b) {
    if (a.kind == LAT_TOP) return b;
    if (b.kind == LAT_TOP) return a;
    if (a.kind == LAT_BOTTOM || b.kind == LAT_BOTTOM) return lat_bottom();
    if (a.val == b.val) return a;
    return lat_bottom();
}

bool opt_sccp(Arena *a, IRFunc *func) {
    (void)a;
    if (!func->entry) return false;

    /* Assign indices to all instructions */
    int num_insts = 0;
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            inst->visited = false;
            num_insts++;
        }
    }

    if (num_insts == 0) return false;

    /* Allocate lattice array (use inst->id as index) */
    int max_id = func->next_inst_id;
    LatVal *lat = calloc(max_id, sizeof(LatVal));
    bool *block_exec = calloc(func->next_block_id, sizeof(bool));

    /* Initialize all to TOP */
    for (int i = 0; i < max_id; i++) lat[i] = lat_top();

    /* Mark entry block executable */
    block_exec[func->entry->id] = true;

    /* Worklists */
    Vec(IRInst *) ssa_worklist = {0};
    Vec(IRBlock *) cfg_worklist = {0};
    vec_push(&cfg_worklist, func->entry);

    bool changed = false;

    while (cfg_worklist.len > 0 || ssa_worklist.len > 0) {
        /* Process CFG worklist */
        while (cfg_worklist.len > 0) {
            IRBlock *bb = vec_pop(&cfg_worklist);

            for (IRInst *inst = bb->first; inst; inst = inst->next) {
                LatVal old = lat[inst->id];
                LatVal new_val = lat_top();

                switch (inst->op) {
                case IR_CONST_INT:
                    new_val = lat_const(inst->ival);
                    break;

                case IR_ADD: case IR_SUB: case IR_MUL:
                case IR_SDIV: case IR_SREM:
                case IR_AND: case IR_OR: case IR_XOR:
                case IR_SHL: case IR_LSHR: case IR_ASHR: {
                    if (inst->num_operands < 2) { new_val = lat_bottom(); break; }
                    LatVal l = lat[inst->operands[0]->id];
                    LatVal r = lat[inst->operands[1]->id];
                    if (l.kind == LAT_BOTTOM || r.kind == LAT_BOTTOM) {
                        new_val = lat_bottom();
                    } else if (l.kind == LAT_CONST && r.kind == LAT_CONST) {
                        int64_t result = 0;
                        switch (inst->op) {
                        case IR_ADD: result = l.val + r.val; break;
                        case IR_SUB: result = l.val - r.val; break;
                        case IR_MUL: result = l.val * r.val; break;
                        case IR_SDIV: result = r.val ? l.val / r.val : 0; break;
                        case IR_SREM: result = r.val ? l.val % r.val : 0; break;
                        case IR_AND: result = l.val & r.val; break;
                        case IR_OR: result = l.val | r.val; break;
                        case IR_XOR: result = l.val ^ r.val; break;
                        case IR_SHL: result = l.val << r.val; break;
                        case IR_LSHR: result = (int64_t)((uint64_t)l.val >> r.val); break;
                        case IR_ASHR: result = l.val >> r.val; break;
                        default: break;
                        }
                        new_val = lat_const(result);
                    }
                    break;
                }

                case IR_ICMP_EQ: case IR_ICMP_NE:
                case IR_ICMP_SLT: case IR_ICMP_SLE:
                case IR_ICMP_SGT: case IR_ICMP_SGE: {
                    if (inst->num_operands < 2) { new_val = lat_bottom(); break; }
                    LatVal l = lat[inst->operands[0]->id];
                    LatVal r = lat[inst->operands[1]->id];
                    if (l.kind == LAT_BOTTOM || r.kind == LAT_BOTTOM) {
                        new_val = lat_bottom();
                    } else if (l.kind == LAT_CONST && r.kind == LAT_CONST) {
                        bool result = false;
                        switch (inst->op) {
                        case IR_ICMP_EQ: result = l.val == r.val; break;
                        case IR_ICMP_NE: result = l.val != r.val; break;
                        case IR_ICMP_SLT: result = l.val < r.val; break;
                        case IR_ICMP_SLE: result = l.val <= r.val; break;
                        case IR_ICMP_SGT: result = l.val > r.val; break;
                        case IR_ICMP_SGE: result = l.val >= r.val; break;
                        default: break;
                        }
                        new_val = lat_const(result ? 1 : 0);
                    }
                    break;
                }

                case IR_PHI: {
                    new_val = lat_top();
                    for (int i = 0; i < inst->phi_count; i++) {
                        if (!block_exec[inst->phi_blocks[i]->id]) continue;
                        if (inst->phi_vals[i]) {
                            new_val = lat_meet(new_val, lat[inst->phi_vals[i]->id]);
                        } else {
                            new_val = lat_bottom();
                        }
                    }
                    break;
                }

                default:
                    new_val = lat_bottom();
                    break;
                }

                lat[inst->id] = new_val;

                if (old.kind != new_val.kind || old.val != new_val.val) {
                    /* Value changed, add users to SSA worklist */
                    for (int i = 0; i < inst->users.len; i++) {
                        vec_push(&ssa_worklist, inst->users.data[i]);
                    }
                }

                /* Handle terminators */
                if (inst->op == IR_CONDBR && new_val.kind != LAT_TOP) {
                    /* Skip - handled via lattice changes to the condition */
                    if (inst->num_operands >= 1) {
                        LatVal cond = lat[inst->operands[0]->id];
                        if (cond.kind == LAT_CONST) {
                            IRBlock *target = cond.val ? inst->br_true : inst->br_false;
                            if (target && !block_exec[target->id]) {
                                block_exec[target->id] = true;
                                vec_push(&cfg_worklist, target);
                            }
                        } else if (cond.kind == LAT_BOTTOM) {
                            if (inst->br_true && !block_exec[inst->br_true->id]) {
                                block_exec[inst->br_true->id] = true;
                                vec_push(&cfg_worklist, inst->br_true);
                            }
                            if (inst->br_false && !block_exec[inst->br_false->id]) {
                                block_exec[inst->br_false->id] = true;
                                vec_push(&cfg_worklist, inst->br_false);
                            }
                        }
                    }
                } else if (inst->op == IR_BR) {
                    if (inst->br_target && !block_exec[inst->br_target->id]) {
                        block_exec[inst->br_target->id] = true;
                        vec_push(&cfg_worklist, inst->br_target);
                    }
                }
            }
        }

        /* Process SSA worklist */
        while (ssa_worklist.len > 0) {
            IRInst *inst = vec_pop(&ssa_worklist);
            if (!inst->parent || !block_exec[inst->parent->id]) continue;
            /* Re-evaluate this instruction - push its block to cfg worklist */
            vec_push(&cfg_worklist, inst->parent);
        }
    }

    /* Replace constants */
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            if (lat[inst->id].kind == LAT_CONST && inst->op != IR_CONST_INT) {
                /* Replace with constant */
                inst->op = IR_CONST_INT;
                inst->ival = lat[inst->id].val;
                inst->num_operands = 0;
                changed = true;
            }
        }
    }

    free(lat);
    free(block_exec);
    vec_free(&ssa_worklist);
    vec_free(&cfg_worklist);
    return changed;
}
