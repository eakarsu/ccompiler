#include "cc.h"

/* Instruction combining / algebraic simplification / peephole optimizations
 *
 * Patterns:
 * - x + 0 -> x
 * - x * 1 -> x
 * - x * 0 -> 0
 * - x - x -> 0
 * - x & 0 -> 0
 * - x | 0 -> x
 * - x ^ 0 -> x
 * - x ^ x -> 0
 * - x << 0 -> x
 * - x >> 0 -> x
 * - double negation: --x -> x
 * - strength reduction: x * 2^n -> x << n
 * - comparison simplification
 */

static bool is_const_int(IRInst *inst, int64_t val) {
    return inst && inst->op == IR_CONST_INT && inst->ival == val
        && inst->call_target == NULL;  /* exclude global symbol references */
}

static bool is_power_of_2(int64_t val) {
    return val > 0 && (val & (val - 1)) == 0;
}

static int log2_of(int64_t val) {
    int n = 0;
    while (val > 1) { val >>= 1; n++; }
    return n;
}

/* Create a const_int and insert it before 'before' in the same block.
 * This avoids ir_build_const_int which appends to the end of the block
 * (potentially after the terminator). */
static IRInst *make_const_before(Arena *a, IRBlock *bb, IRInst *before, IRType ty, int64_t val) {
    IRInst *inst = ir_inst_new(a, bb, IR_CONST_INT, ty);
    inst->ival = val;
    ir_block_insert_before(bb, before, inst);
    return inst;
}

static void replace_inst(IRInst *old, IRInst *new_val) {
    for (int i = 0; i < old->users.len; i++) {
        IRInst *user = old->users.data[i];
        for (int j = 0; j < user->num_operands; j++) {
            if (user->operands[j] == old) {
                user->operands[j] = new_val;
            }
        }
        if (user->op == IR_PHI) {
            for (int j = 0; j < user->phi_count; j++) {
                if (user->phi_vals[j] == old) {
                    user->phi_vals[j] = new_val;
                }
            }
        }
        vec_push(&new_val->users, user);
    }
    old->users.len = 0;
    old->is_dead = true;
}

bool opt_instcombine(Arena *a, IRFunc *func) {
    bool changed = false;

    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            if (inst->is_dead) continue;
            if (inst->num_operands < 2) continue;

            IRInst *lhs = inst->operands[0];
            IRInst *rhs = inst->operands[1];

            switch (inst->op) {
            case IR_ADD:
                /* x + 0 -> x */
                if (is_const_int(rhs, 0)) {
                    replace_inst(inst, lhs);
                    changed = true;
                } else if (is_const_int(lhs, 0)) {
                    replace_inst(inst, rhs);
                    changed = true;
                }
                break;

            case IR_SUB:
                /* x - 0 -> x */
                if (is_const_int(rhs, 0)) {
                    replace_inst(inst, lhs);
                    changed = true;
                }
                /* x - x -> 0 */
                else if (lhs == rhs) {
                    IRInst *zero = make_const_before(a, bb, inst, inst->ty, 0);
                    replace_inst(inst, zero);
                    changed = true;
                }
                break;

            case IR_MUL:
                /* x * 0 -> 0 */
                if (is_const_int(rhs, 0) || is_const_int(lhs, 0)) {
                    IRInst *zero = make_const_before(a, bb, inst, inst->ty, 0);
                    replace_inst(inst, zero);
                    changed = true;
                }
                /* x * 1 -> x */
                else if (is_const_int(rhs, 1)) {
                    replace_inst(inst, lhs);
                    changed = true;
                } else if (is_const_int(lhs, 1)) {
                    replace_inst(inst, rhs);
                    changed = true;
                }
                /* x * 2^n -> x << n (strength reduction) */
                else if (rhs->op == IR_CONST_INT && is_power_of_2(rhs->ival)) {
                    IRInst *shift = make_const_before(a, bb, inst, inst->ty, log2_of(rhs->ival));
                    inst->op = IR_SHL;
                    inst->operands[1] = shift;
                    changed = true;
                }
                break;

            case IR_SDIV:
            case IR_UDIV:
                /* x / 1 -> x */
                if (is_const_int(rhs, 1)) {
                    replace_inst(inst, lhs);
                    changed = true;
                }
                break;

            case IR_AND:
                /* x & 0 -> 0 */
                if (is_const_int(rhs, 0) || is_const_int(lhs, 0)) {
                    IRInst *zero = make_const_before(a, bb, inst, inst->ty, 0);
                    replace_inst(inst, zero);
                    changed = true;
                }
                /* x & x -> x */
                else if (lhs == rhs) {
                    replace_inst(inst, lhs);
                    changed = true;
                }
                break;

            case IR_OR:
                /* x | 0 -> x */
                if (is_const_int(rhs, 0)) {
                    replace_inst(inst, lhs);
                    changed = true;
                } else if (is_const_int(lhs, 0)) {
                    replace_inst(inst, rhs);
                    changed = true;
                }
                /* x | x -> x */
                else if (lhs == rhs) {
                    replace_inst(inst, lhs);
                    changed = true;
                }
                break;

            case IR_XOR:
                /* x ^ 0 -> x */
                if (is_const_int(rhs, 0)) {
                    replace_inst(inst, lhs);
                    changed = true;
                } else if (is_const_int(lhs, 0)) {
                    replace_inst(inst, rhs);
                    changed = true;
                }
                /* x ^ x -> 0 */
                else if (lhs == rhs) {
                    IRInst *zero = make_const_before(a, bb, inst, inst->ty, 0);
                    replace_inst(inst, zero);
                    changed = true;
                }
                break;

            case IR_SHL:
            case IR_LSHR:
            case IR_ASHR:
                /* x << 0 -> x, x >> 0 -> x */
                if (is_const_int(rhs, 0)) {
                    replace_inst(inst, lhs);
                    changed = true;
                }
                break;

            case IR_ICMP_EQ:
                /* x == x -> 1 */
                if (lhs == rhs) {
                    IRInst *one = make_const_before(a, bb, inst, inst->ty, 1);
                    replace_inst(inst, one);
                    changed = true;
                }
                break;

            case IR_ICMP_NE:
                /* x != x -> 0 */
                if (lhs == rhs) {
                    IRInst *zero = make_const_before(a, bb, inst, inst->ty, 0);
                    replace_inst(inst, zero);
                    changed = true;
                }
                break;

            default:
                break;
            }
        }
    }

    return changed;
}
