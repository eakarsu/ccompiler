int printf(const char *fmt, ...);

/* Simple 3-stage pipeline: Fetch, Decode, Execute */

enum Opcode {
    OP_NOP,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_LOAD,
    OP_STORE,
    OP_HALT
};

struct Instruction {
    int opcode;
    int rd;
    int rs1;
    int rs2;
    int imm;
};

struct PipelineStage {
    struct Instruction inst;
    int valid;
    int stalled;
};

struct CPU {
    int regs[8];
    int memory[32];
    struct Instruction program[16];
    int pc;
    int program_size;
    struct PipelineStage fetch_stage;
    struct PipelineStage decode_stage;
    struct PipelineStage execute_stage;
    int cycles;
    int instructions_executed;
    int stalls;
    int halted;
};

void cpu_init(struct CPU *cpu) {
    int i;
    cpu->pc = 0;
    cpu->program_size = 0;
    cpu->cycles = 0;
    cpu->instructions_executed = 0;
    cpu->stalls = 0;
    cpu->halted = 0;
    cpu->fetch_stage.valid = 0;
    cpu->fetch_stage.stalled = 0;
    cpu->decode_stage.valid = 0;
    cpu->decode_stage.stalled = 0;
    cpu->execute_stage.valid = 0;
    cpu->execute_stage.stalled = 0;
    i = 0;
    while (i < 8) {
        cpu->regs[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < 32) {
        cpu->memory[i] = 0;
        i = i + 1;
    }
}

void add_instruction(struct CPU *cpu, int opcode, int rd, int rs1, int rs2, int imm) {
    int idx;
    idx = cpu->program_size;
    if (idx >= 16) return;
    cpu->program[idx].opcode = opcode;
    cpu->program[idx].rd = rd;
    cpu->program[idx].rs1 = rs1;
    cpu->program[idx].rs2 = rs2;
    cpu->program[idx].imm = imm;
    cpu->program_size = cpu->program_size + 1;
}

/* Check for data hazard: decode stage reads register that execute stage writes */
int has_data_hazard(struct CPU *cpu) {
    int exec_rd;
    if (cpu->decode_stage.valid == 0 || cpu->execute_stage.valid == 0) return 0;
    exec_rd = cpu->execute_stage.inst.rd;
    if (cpu->execute_stage.inst.opcode == OP_STORE || cpu->execute_stage.inst.opcode == OP_NOP ||
        cpu->execute_stage.inst.opcode == OP_HALT) return 0;
    if (cpu->decode_stage.inst.rs1 == exec_rd) return 1;
    if (cpu->decode_stage.inst.opcode != OP_LOAD && cpu->decode_stage.inst.opcode != OP_STORE) {
        if (cpu->decode_stage.inst.rs2 == exec_rd) return 1;
    }
    return 0;
}

/* Execute one pipeline stage */
void execute_stage_do(struct CPU *cpu) {
    struct Instruction *inst;
    if (cpu->execute_stage.valid == 0) return;
    inst = &cpu->execute_stage.inst;
    if (inst->opcode == OP_ADD) {
        cpu->regs[inst->rd] = cpu->regs[inst->rs1] + cpu->regs[inst->rs2];
    } else if (inst->opcode == OP_SUB) {
        cpu->regs[inst->rd] = cpu->regs[inst->rs1] - cpu->regs[inst->rs2];
    } else if (inst->opcode == OP_MUL) {
        cpu->regs[inst->rd] = cpu->regs[inst->rs1] * cpu->regs[inst->rs2];
    } else if (inst->opcode == OP_LOAD) {
        if (inst->imm >= 0 && inst->imm < 32) {
            cpu->regs[inst->rd] = cpu->memory[inst->imm];
        }
    } else if (inst->opcode == OP_STORE) {
        if (inst->imm >= 0 && inst->imm < 32) {
            cpu->memory[inst->imm] = cpu->regs[inst->rs1];
        }
    } else if (inst->opcode == OP_HALT) {
        cpu->halted = 1;
    }
    if (inst->opcode != OP_NOP) {
        cpu->instructions_executed = cpu->instructions_executed + 1;
    }
}

void pipeline_cycle(struct CPU *cpu) {
    int hazard;
    cpu->cycles = cpu->cycles + 1;

    /* Execute */
    execute_stage_do(cpu);

    /* Check for hazard before advancing decode -> execute */
    hazard = has_data_hazard(cpu);

    if (hazard) {
        /* Stall: insert bubble into execute, keep decode and fetch */
        cpu->execute_stage.valid = 0;
        cpu->stalls = cpu->stalls + 1;
    } else {
        /* Normal advance: decode -> execute */
        cpu->execute_stage = cpu->decode_stage;
        /* fetch -> decode */
        cpu->decode_stage = cpu->fetch_stage;
        /* Fetch next instruction */
        cpu->fetch_stage.valid = 0;
        if (cpu->pc < cpu->program_size && cpu->halted == 0) {
            cpu->fetch_stage.inst = cpu->program[cpu->pc];
            cpu->fetch_stage.valid = 1;
            cpu->pc = cpu->pc + 1;
        }
    }
}

void run_cpu(struct CPU *cpu) {
    int max_cycles;
    max_cycles = 100;
    while (cpu->halted == 0 && cpu->cycles < max_cycles) {
        pipeline_cycle(cpu);
        /* Check if pipeline is drained */
        if (cpu->halted && cpu->execute_stage.valid == 0 &&
            cpu->decode_stage.valid == 0 && cpu->fetch_stage.valid == 0) {
            break;
        }
    }
}

int main(void) {
    struct CPU cpu;
    int i;

    cpu_init(&cpu);

    /* Program: compute (5+3)*2 - 1 */
    /* R1 = 5 (via LOAD from mem[0], preset to 5) */
    /* R2 = 3 (via LOAD from mem[1], preset to 3) */
    /* R3 = R1 + R2 */
    /* R4 = 2 (via LOAD from mem[2], preset to 2) */
    /* R5 = R3 * R4 */
    /* R6 = 1 (via LOAD from mem[3], preset to 1) */
    /* R7 = R5 - R6 */
    /* STORE R7 to mem[4] */
    /* HALT */

    cpu.memory[0] = 5;
    cpu.memory[1] = 3;
    cpu.memory[2] = 2;
    cpu.memory[3] = 1;

    add_instruction(&cpu, OP_LOAD, 1, 0, 0, 0);
    add_instruction(&cpu, OP_LOAD, 2, 0, 0, 1);
    add_instruction(&cpu, OP_ADD, 3, 1, 2, 0);
    add_instruction(&cpu, OP_LOAD, 4, 0, 0, 2);
    add_instruction(&cpu, OP_MUL, 5, 3, 4, 0);
    add_instruction(&cpu, OP_LOAD, 6, 0, 0, 3);
    add_instruction(&cpu, OP_SUB, 7, 5, 6, 0);
    add_instruction(&cpu, OP_STORE, 0, 7, 0, 4);
    add_instruction(&cpu, OP_HALT, 0, 0, 0, 0);

    // EXPECT: 9
    printf("%d\n", cpu.program_size);

    run_cpu(&cpu);

    /* R1 = 5 */
    // EXPECT: 5
    printf("%d\n", cpu.regs[1]);

    /* R2 = 3 */
    // EXPECT: 3
    printf("%d\n", cpu.regs[2]);

    /* R3 = 5 + 3 = 8 */
    // EXPECT: 8
    printf("%d\n", cpu.regs[3]);

    /* R4 = 2 */
    // EXPECT: 2
    printf("%d\n", cpu.regs[4]);

    /* R5 = 8 * 2 = 16 */
    // EXPECT: 16
    printf("%d\n", cpu.regs[5]);

    /* R6 = 1 */
    // EXPECT: 1
    printf("%d\n", cpu.regs[6]);

    /* R7 = 16 - 1 = 15 */
    // EXPECT: 15
    printf("%d\n", cpu.regs[7]);

    /* Memory[4] = 15 (stored result) */
    // EXPECT: 15
    printf("%d\n", cpu.memory[4]);

    // EXPECT: 1
    printf("%d\n", cpu.halted);

    /* Check pipeline stats */
    // EXPECT: 9
    printf("%d\n", cpu.instructions_executed);

    /* Stalls should be > 0 due to data hazards */
    /* ADD r3,r1,r2 depends on LOAD r2; MUL r5,r3,r4 depends on ADD r3; etc */
    /* Print stalls > 0 check */
    result_check:;
    {
        int has_stalls;
        has_stalls = cpu.stalls > 0 ? 1 : 0;
        // EXPECT: 1
        printf("%d\n", has_stalls);
    }

    /* Verify cycles > instructions (due to pipeline + stalls) */
    {
        int more_cycles;
        more_cycles = cpu.cycles > cpu.instructions_executed ? 1 : 0;
        // EXPECT: 1
        printf("%d\n", more_cycles);
    }

    /* Second simple test: no hazards (all independent loads) */
    cpu_init(&cpu);
    cpu.memory[0] = 10;
    cpu.memory[1] = 20;
    cpu.memory[2] = 30;

    add_instruction(&cpu, OP_LOAD, 1, 0, 0, 0);
    add_instruction(&cpu, OP_LOAD, 2, 0, 0, 1);
    add_instruction(&cpu, OP_LOAD, 3, 0, 0, 2);
    add_instruction(&cpu, OP_HALT, 0, 0, 0, 0);

    run_cpu(&cpu);

    // EXPECT: 10
    printf("%d\n", cpu.regs[1]);
    // EXPECT: 20
    printf("%d\n", cpu.regs[2]);
    // EXPECT: 30
    printf("%d\n", cpu.regs[3]);
    // EXPECT: 0
    printf("%d\n", cpu.stalls);

    return 0;
}
