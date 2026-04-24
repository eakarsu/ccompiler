int printf(const char *fmt, ...);

// Dead code elimination simulation
// Tracks which instructions are "used" and removes unreachable/unused ones

enum OpKind {
    OP_CONST,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_STORE,
    OP_RETURN,
    OP_BRANCH,
    OP_NOP
};

struct Instruction {
    int opcode;
    int dest;       // virtual reg (-1 for store/branch/return)
    int src1;       // source register or constant
    int src2;       // second source
    int live;       // 1 if instruction is live (has side effects or used result)
};

struct BasicBlock {
    struct Instruction insts[32];
    int count;
    int reachable;
};

struct CFG {
    struct BasicBlock blocks[8];
    int num_blocks;
    int edges[8][8]; // adjacency matrix for control flow
};

void cfg_init(struct CFG *cfg) {
    int i, j;
    cfg->num_blocks = 0;
    for (i = 0; i < 8; i++) {
        cfg->blocks[i].count = 0;
        cfg->blocks[i].reachable = 0;
        for (j = 0; j < 8; j++) {
            cfg->edges[i][j] = 0;
        }
    }
}

int add_block(struct CFG *cfg) {
    int idx = cfg->num_blocks;
    cfg->num_blocks = cfg->num_blocks + 1;
    return idx;
}

void add_edge(struct CFG *cfg, int from, int to) {
    cfg->edges[from][to] = 1;
}

void add_inst(struct CFG *cfg, int block, int opcode, int dest, int s1, int s2) {
    struct BasicBlock *bb = &cfg->blocks[block];
    int idx = bb->count;
    bb->insts[idx].opcode = opcode;
    bb->insts[idx].dest = dest;
    bb->insts[idx].src1 = s1;
    bb->insts[idx].src2 = s2;
    bb->insts[idx].live = 0;
    bb->count = bb->count + 1;
}

// Mark reachable blocks using DFS from entry (block 0)
void mark_reachable(struct CFG *cfg, int block) {
    if (block < 0 || block >= cfg->num_blocks) return;
    if (cfg->blocks[block].reachable) return;
    cfg->blocks[block].reachable = 1;
    int i;
    for (i = 0; i < cfg->num_blocks; i++) {
        if (cfg->edges[block][i]) {
            mark_reachable(cfg, i);
        }
    }
}

// Check if a register is used by any live instruction in any reachable block
int reg_is_used(struct CFG *cfg, int reg) {
    int b, i;
    for (b = 0; b < cfg->num_blocks; b++) {
        if (!cfg->blocks[b].reachable) continue;
        struct BasicBlock *bb = &cfg->blocks[b];
        for (i = 0; i < bb->count; i++) {
            struct Instruction *inst = &bb->insts[i];
            if (inst->opcode == OP_NOP) continue;
            if (inst->src1 == reg || inst->src2 == reg) return 1;
        }
    }
    return 0;
}

// Has side effects?
int has_side_effects(int opcode) {
    return opcode == OP_STORE || opcode == OP_RETURN || opcode == OP_BRANCH;
}

// DCE pass: mark side-effect instructions as live, then propagate
int dce_pass(struct CFG *cfg) {
    int eliminated = 0;
    int b, i;

    // First, mark all side-effect instructions as live
    for (b = 0; b < cfg->num_blocks; b++) {
        if (!cfg->blocks[b].reachable) continue;
        struct BasicBlock *bb = &cfg->blocks[b];
        for (i = 0; i < bb->count; i++) {
            if (has_side_effects(bb->insts[i].opcode)) {
                bb->insts[i].live = 1;
            }
        }
    }

    // Mark instructions whose results are used
    for (b = 0; b < cfg->num_blocks; b++) {
        if (!cfg->blocks[b].reachable) continue;
        struct BasicBlock *bb = &cfg->blocks[b];
        for (i = 0; i < bb->count; i++) {
            struct Instruction *inst = &bb->insts[i];
            if (inst->opcode == OP_NOP) continue;
            if (inst->dest >= 0 && reg_is_used(cfg, inst->dest)) {
                inst->live = 1;
            }
        }
    }

    // Eliminate dead instructions
    for (b = 0; b < cfg->num_blocks; b++) {
        if (!cfg->blocks[b].reachable) continue;
        struct BasicBlock *bb = &cfg->blocks[b];
        for (i = 0; i < bb->count; i++) {
            if (!bb->insts[i].live && bb->insts[i].opcode != OP_NOP) {
                bb->insts[i].opcode = OP_NOP;
                eliminated = eliminated + 1;
            }
        }
    }

    // Eliminate all instructions in unreachable blocks
    for (b = 0; b < cfg->num_blocks; b++) {
        if (!cfg->blocks[b].reachable) {
            struct BasicBlock *bb = &cfg->blocks[b];
            for (i = 0; i < bb->count; i++) {
                if (bb->insts[i].opcode != OP_NOP) {
                    bb->insts[i].opcode = OP_NOP;
                    eliminated = eliminated + 1;
                }
            }
        }
    }

    return eliminated;
}

int count_live(struct CFG *cfg) {
    int c = 0;
    int b, i;
    for (b = 0; b < cfg->num_blocks; b++) {
        struct BasicBlock *bb = &cfg->blocks[b];
        for (i = 0; i < bb->count; i++) {
            if (bb->insts[i].opcode != OP_NOP) c = c + 1;
        }
    }
    return c;
}

int count_total_insts(struct CFG *cfg) {
    int c = 0;
    int b;
    for (b = 0; b < cfg->num_blocks; b++) {
        c = c + cfg->blocks[b].count;
    }
    return c;
}

int main() {
    struct CFG cfg;

    // Test 1: Simple dead code
    printf("=== DCE Test 1: Unused computation ===\n");
    // EXPECT: === DCE Test 1: Unused computation ===
    cfg_init(&cfg);
    int b0 = add_block(&cfg);
    // r0 = 5, r1 = 10, r2 = r0+r1 (unused), r3 = r0*2, STORE r3, RETURN
    add_inst(&cfg, b0, OP_CONST, 0, 5, 0);
    add_inst(&cfg, b0, OP_CONST, 1, 10, 0);
    add_inst(&cfg, b0, OP_ADD, 2, 0, 1);    // dead: r2 unused
    add_inst(&cfg, b0, OP_MUL, 3, 0, 1);
    add_inst(&cfg, b0, OP_STORE, -1, 3, 0);
    add_inst(&cfg, b0, OP_RETURN, -1, 3, 0);

    printf("before: %d insts\n", count_total_insts(&cfg));
    // EXPECT: before: 6 insts
    mark_reachable(&cfg, 0);
    int elim1 = dce_pass(&cfg);
    printf("eliminated: %d\n", elim1);
    // EXPECT: eliminated: 1
    printf("after: %d live\n", count_live(&cfg));
    // EXPECT: after: 5 live

    // Test 2: Unreachable block
    printf("=== DCE Test 2: Unreachable block ===\n");
    // EXPECT: === DCE Test 2: Unreachable block ===
    cfg_init(&cfg);
    b0 = add_block(&cfg);   // block 0: entry
    int b1 = add_block(&cfg); // block 1: reachable
    int b2 = add_block(&cfg); // block 2: unreachable

    add_edge(&cfg, 0, 1); // 0 -> 1, but NOT 0 -> 2

    add_inst(&cfg, b0, OP_CONST, 0, 42, 0);
    add_inst(&cfg, b0, OP_BRANCH, -1, 1, 0);

    add_inst(&cfg, b1, OP_STORE, -1, 0, 0);
    add_inst(&cfg, b1, OP_RETURN, -1, 0, 0);

    add_inst(&cfg, b2, OP_CONST, 5, 999, 0);  // unreachable
    add_inst(&cfg, b2, OP_STORE, -1, 5, 0);   // unreachable

    printf("before: %d insts\n", count_total_insts(&cfg));
    // EXPECT: before: 6 insts
    mark_reachable(&cfg, 0);
    printf("reachable blocks: %d %d %d\n",
           cfg.blocks[0].reachable, cfg.blocks[1].reachable, cfg.blocks[2].reachable);
    // EXPECT: reachable blocks: 1 1 0
    int elim2 = dce_pass(&cfg);
    printf("eliminated: %d\n", elim2);
    // EXPECT: eliminated: 2
    printf("after: %d live\n", count_live(&cfg));
    // EXPECT: after: 4 live

    // Test 3: Chain of dead code
    printf("=== DCE Test 3: Dead chain ===\n");
    // EXPECT: === DCE Test 3: Dead chain ===
    cfg_init(&cfg);
    b0 = add_block(&cfg);
    // r0=1, r1=2, r2=r0+r1, r3=r2*r2, r4=100, RETURN r4
    // r0,r1,r2,r3 are all dead because only r4 is returned
    add_inst(&cfg, b0, OP_CONST, 0, 1, 0);
    add_inst(&cfg, b0, OP_CONST, 1, 2, 0);
    add_inst(&cfg, b0, OP_ADD, 2, 0, 1);
    add_inst(&cfg, b0, OP_MUL, 3, 2, 2);
    add_inst(&cfg, b0, OP_CONST, 4, 100, 0);
    add_inst(&cfg, b0, OP_RETURN, -1, 4, 0);

    mark_reachable(&cfg, 0);
    int elim3 = dce_pass(&cfg);
    printf("eliminated: %d\n", elim3);
    // EXPECT: eliminated: 1
    printf("after: %d live\n", count_live(&cfg));
    // EXPECT: after: 5 live

    // Test 4: All live (everything used)
    printf("=== DCE Test 4: All live ===\n");
    // EXPECT: === DCE Test 4: All live ===
    cfg_init(&cfg);
    b0 = add_block(&cfg);
    add_inst(&cfg, b0, OP_CONST, 0, 7, 0);
    add_inst(&cfg, b0, OP_CONST, 1, 3, 0);
    add_inst(&cfg, b0, OP_ADD, 2, 0, 1);
    add_inst(&cfg, b0, OP_RETURN, -1, 2, 0);

    mark_reachable(&cfg, 0);
    int elim4 = dce_pass(&cfg);
    printf("eliminated: %d\n", elim4);
    // EXPECT: eliminated: 0
    printf("after: %d live\n", count_live(&cfg));
    // EXPECT: after: 4 live

    // Test 5: Diamond CFG with one dead path
    printf("=== DCE Test 5: Diamond CFG ===\n");
    // EXPECT: === DCE Test 5: Diamond CFG ===
    cfg_init(&cfg);
    b0 = add_block(&cfg); // entry
    b1 = add_block(&cfg); // then
    b2 = add_block(&cfg); // else
    int b3 = add_block(&cfg); // merge

    add_edge(&cfg, 0, 1);
    add_edge(&cfg, 0, 2);
    add_edge(&cfg, 1, 3);
    add_edge(&cfg, 2, 3);

    add_inst(&cfg, b0, OP_CONST, 0, 1, 0);
    add_inst(&cfg, b0, OP_BRANCH, -1, 0, 0);
    add_inst(&cfg, b1, OP_CONST, 1, 10, 0);
    add_inst(&cfg, b2, OP_CONST, 2, 20, 0);
    add_inst(&cfg, b3, OP_STORE, -1, 1, 0);
    add_inst(&cfg, b3, OP_RETURN, -1, 0, 0);

    mark_reachable(&cfg, 0);
    printf("all reachable: %d\n",
        cfg.blocks[0].reachable + cfg.blocks[1].reachable +
        cfg.blocks[2].reachable + cfg.blocks[3].reachable);
    // EXPECT: all reachable: 4
    int elim5 = dce_pass(&cfg);
    printf("eliminated: %d\n", elim5);
    // EXPECT: eliminated: 1
    printf("after: %d live\n", count_live(&cfg));
    // EXPECT: after: 5 live

    printf("done\n");
    // EXPECT: done
    return 0;
}
