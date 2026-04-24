#include "cc.h"

/* Global Value Numbering (GVN)
 * Eliminates redundant computations by assigning value numbers
 * to expressions and replacing duplicates.
 */

typedef struct {
    IROpcode op;
    int operand_vns[3];
    int num_operands;
} GVNKey;

typedef struct {
    GVNKey key;
    IRInst *leader;
} GVNEntry;

#define GVN_TABLE_SIZE 1024

static int gvn_hash(GVNKey *k) {
    unsigned h = k->op * 2654435761u;
    for (int i = 0; i < k->num_operands; i++) {
        h ^= (unsigned)k->operand_vns[i] * 2246822519u;
        h = (h << 13) | (h >> 19);
    }
    return (int)(h % GVN_TABLE_SIZE);
}

bool opt_gvn(Arena *a, IRFunc *func) {
    (void)a;
    bool changed = false;

    /* Value number table */
    int max_id = func->next_inst_id;
    int *vn = calloc(max_id, sizeof(int));
    int next_vn = 1;

    /* Hash table for expressions */
    GVNEntry *table = calloc(GVN_TABLE_SIZE, sizeof(GVNEntry));

    /* Assign initial value numbers (unique for each instruction) */
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            if (inst->op == IR_CONST_INT) {
                /* Constants with same value should get same VN */
                GVNKey key = {.op = IR_CONST_INT, .num_operands = 0};
                key.operand_vns[0] = (int)inst->ival;
                key.num_operands = 1;

                int h = gvn_hash(&key);
                bool found = false;
                for (int probe = 0; probe < GVN_TABLE_SIZE; probe++) {
                    int idx = (h + probe) % GVN_TABLE_SIZE;
                    if (!table[idx].leader) {
                        table[idx].key = key;
                        table[idx].leader = inst;
                        vn[inst->id] = next_vn++;
                        break;
                    }
                    if (table[idx].key.op == key.op &&
                        table[idx].key.num_operands == key.num_operands &&
                        table[idx].key.operand_vns[0] == key.operand_vns[0]) {
                        vn[inst->id] = vn[table[idx].leader->id];
                        found = true;
                        break;
                    }
                }
                if (!found && vn[inst->id] == 0) {
                    vn[inst->id] = next_vn++;
                }
            } else if (inst->op == IR_ADD || inst->op == IR_SUB ||
                       inst->op == IR_MUL || inst->op == IR_AND ||
                       inst->op == IR_OR || inst->op == IR_XOR ||
                       inst->op == IR_SHL || inst->op == IR_LSHR ||
                       inst->op == IR_ASHR ||
                       inst->op == IR_ICMP_EQ || inst->op == IR_ICMP_NE ||
                       inst->op == IR_ICMP_SLT || inst->op == IR_ICMP_SLE ||
                       inst->op == IR_ICMP_SGT || inst->op == IR_ICMP_SGE) {
                if (inst->num_operands >= 2 &&
                    vn[inst->operands[0]->id] > 0 &&
                    vn[inst->operands[1]->id] > 0) {
                    GVNKey key = {
                        .op = inst->op,
                        .num_operands = 2,
                    };
                    key.operand_vns[0] = vn[inst->operands[0]->id];
                    key.operand_vns[1] = vn[inst->operands[1]->id];

                    int h = gvn_hash(&key);
                    bool found = false;
                    for (int probe = 0; probe < GVN_TABLE_SIZE; probe++) {
                        int idx = (h + probe) % GVN_TABLE_SIZE;
                        if (!table[idx].leader) {
                            table[idx].key = key;
                            table[idx].leader = inst;
                            vn[inst->id] = next_vn++;
                            break;
                        }
                        if (table[idx].key.op == key.op &&
                            table[idx].key.num_operands == key.num_operands &&
                            table[idx].key.operand_vns[0] == key.operand_vns[0] &&
                            table[idx].key.operand_vns[1] == key.operand_vns[1]) {
                            /* Found duplicate! Replace uses only if leader dominates */
                            IRInst *leader = table[idx].leader;
                            if (!ir_dominates(leader->parent, inst->parent)) {
                                /* Leader doesn't dominate; give unique VN */
                                vn[inst->id] = next_vn++;
                                found = true;
                                break;
                            }
                            vn[inst->id] = vn[leader->id];
                            for (int u = 0; u < inst->users.len; u++) {
                                IRInst *user = inst->users.data[u];
                                for (int j = 0; j < user->num_operands; j++) {
                                    if (user->operands[j] == inst) {
                                        user->operands[j] = leader;
                                    }
                                }
                            }
                            inst->is_dead = true;
                            changed = true;
                            found = true;
                            break;
                        }
                    }
                    if (!found && vn[inst->id] == 0) {
                        vn[inst->id] = next_vn++;
                    }
                } else {
                    vn[inst->id] = next_vn++;
                }
            } else {
                vn[inst->id] = next_vn++;
            }
        }
    }

    free(vn);
    free(table);
    return changed;
}
