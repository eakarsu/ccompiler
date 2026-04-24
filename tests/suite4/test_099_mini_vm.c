int printf(const char *fmt, ...);

enum Opcode {
    OP_NOP,
    OP_LOAD,     /* load immediate: reg = imm */
    OP_ADD,      /* reg[a] = reg[b] + reg[c] */
    OP_SUB,      /* reg[a] = reg[b] - reg[c] */
    OP_MUL,      /* reg[a] = reg[b] * reg[c] */
    OP_DIV,      /* reg[a] = reg[b] / reg[c] */
    OP_MOV,      /* reg[a] = reg[b] */
    OP_STORE,    /* mem[imm] = reg[a] */
    OP_LOADM,    /* reg[a] = mem[imm] */
    OP_CMP,      /* set flag: reg[a] - reg[b] */
    OP_JMP,      /* pc = imm */
    OP_JZ,       /* if flag == 0, pc = imm */
    OP_INC,      /* reg[a] = reg[a] + 1 */
    OP_DEC,      /* reg[a] = reg[a] - 1 */
    OP_PRINT,    /* print reg[a] */
    OP_HALT
};

struct Instruction {
    int opcode;
    int a;
    int b;
    int c;
    int imm;
};

struct VM {
    int regs[8];
    int mem[32];
    int pc;
    int flag;    /* result of last CMP */
    int halted;
    int cycle_count;
    int output[16];
    int output_count;
};

void vm_init(struct VM *vm) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        vm->regs[i] = 0;
    }
    for (i = 0; i < 32; i = i + 1) {
        vm->mem[i] = 0;
    }
    vm->pc = 0;
    vm->flag = 0;
    vm->halted = 0;
    vm->cycle_count = 0;
    vm->output_count = 0;
}

void vm_step(struct VM *vm, struct Instruction *prog) {
    if (vm->halted) return;
    struct Instruction inst = prog[vm->pc];
    vm->pc = vm->pc + 1;
    vm->cycle_count = vm->cycle_count + 1;

    switch (inst.opcode) {
        case OP_NOP:
            break;
        case OP_LOAD:
            vm->regs[inst.a] = inst.imm;
            break;
        case OP_ADD:
            vm->regs[inst.a] = vm->regs[inst.b] + vm->regs[inst.c];
            break;
        case OP_SUB:
            vm->regs[inst.a] = vm->regs[inst.b] - vm->regs[inst.c];
            break;
        case OP_MUL:
            vm->regs[inst.a] = vm->regs[inst.b] * vm->regs[inst.c];
            break;
        case OP_DIV:
            if (vm->regs[inst.c] != 0) {
                vm->regs[inst.a] = vm->regs[inst.b] / vm->regs[inst.c];
            }
            break;
        case OP_MOV:
            vm->regs[inst.a] = vm->regs[inst.b];
            break;
        case OP_STORE:
            if (inst.imm >= 0 && inst.imm < 32) {
                vm->mem[inst.imm] = vm->regs[inst.a];
            }
            break;
        case OP_LOADM:
            if (inst.imm >= 0 && inst.imm < 32) {
                vm->regs[inst.a] = vm->mem[inst.imm];
            }
            break;
        case OP_CMP:
            vm->flag = vm->regs[inst.a] - vm->regs[inst.b];
            break;
        case OP_JMP:
            vm->pc = inst.imm;
            break;
        case OP_JZ:
            if (vm->flag == 0) {
                vm->pc = inst.imm;
            }
            break;
        case OP_INC:
            vm->regs[inst.a] = vm->regs[inst.a] + 1;
            break;
        case OP_DEC:
            vm->regs[inst.a] = vm->regs[inst.a] - 1;
            break;
        case OP_PRINT:
            if (vm->output_count < 16) {
                vm->output[vm->output_count] = vm->regs[inst.a];
                vm->output_count = vm->output_count + 1;
            }
            break;
        case OP_HALT:
            vm->halted = 1;
            break;
    }
}

void vm_run(struct VM *vm, struct Instruction *prog, int max_cycles) {
    while (!vm->halted && vm->cycle_count < max_cycles) {
        vm_step(vm, prog);
    }
}

struct Instruction make_inst(int op, int a, int b, int c, int imm) {
    struct Instruction inst;
    inst.opcode = op;
    inst.a = a;
    inst.b = b;
    inst.c = c;
    inst.imm = imm;
    return inst;
}

int main(void) {
    struct VM vm;

    /* Program 1: compute 10 + 20 and print it */
    struct Instruction prog1[5];
    prog1[0] = make_inst(OP_LOAD, 0, 0, 0, 10);    /* r0 = 10 */
    prog1[1] = make_inst(OP_LOAD, 1, 0, 0, 20);    /* r1 = 20 */
    prog1[2] = make_inst(OP_ADD, 2, 0, 1, 0);      /* r2 = r0 + r1 */
    prog1[3] = make_inst(OP_PRINT, 2, 0, 0, 0);    /* print r2 */
    prog1[4] = make_inst(OP_HALT, 0, 0, 0, 0);

    vm_init(&vm);
    vm_run(&vm, prog1, 100);

    // EXPECT: Prog1 output: 30
    printf("Prog1 output: %d\n", vm.output[0]);
    // EXPECT: Prog1 cycles: 5
    printf("Prog1 cycles: %d\n", vm.cycle_count);
    // EXPECT: Prog1 halted: 1
    printf("Prog1 halted: %d\n", vm.halted);

    /* Program 2: compute 5 * 7 - 3 */
    struct Instruction prog2[6];
    prog2[0] = make_inst(OP_LOAD, 0, 0, 0, 5);
    prog2[1] = make_inst(OP_LOAD, 1, 0, 0, 7);
    prog2[2] = make_inst(OP_MUL, 2, 0, 1, 0);      /* r2 = 35 */
    prog2[3] = make_inst(OP_LOAD, 3, 0, 0, 3);
    prog2[4] = make_inst(OP_SUB, 4, 2, 3, 0);      /* r4 = 35 - 3 = 32 */
    prog2[5] = make_inst(OP_HALT, 0, 0, 0, 0);

    vm_init(&vm);
    vm_run(&vm, prog2, 100);
    // EXPECT: Prog2 r4: 32
    printf("Prog2 r4: %d\n", vm.regs[4]);
    // EXPECT: Prog2 r2: 35
    printf("Prog2 r2: %d\n", vm.regs[2]);

    /* Program 3: store to memory and load back */
    struct Instruction prog3[6];
    prog3[0] = make_inst(OP_LOAD, 0, 0, 0, 42);
    prog3[1] = make_inst(OP_STORE, 0, 0, 0, 5);    /* mem[5] = 42 */
    prog3[2] = make_inst(OP_LOAD, 0, 0, 0, 0);     /* r0 = 0 (clear) */
    prog3[3] = make_inst(OP_LOADM, 1, 0, 0, 5);    /* r1 = mem[5] */
    prog3[4] = make_inst(OP_PRINT, 1, 0, 0, 0);
    prog3[5] = make_inst(OP_HALT, 0, 0, 0, 0);

    vm_init(&vm);
    vm_run(&vm, prog3, 100);
    // EXPECT: Prog3 mem load: 42
    printf("Prog3 mem load: %d\n", vm.output[0]);
    // EXPECT: Prog3 r0 cleared: 0
    printf("Prog3 r0 cleared: %d\n", vm.regs[0]);

    /* Program 4: count from 0 to 4 using loop (JMP, CMP, JZ) */
    /* r0 = counter, r1 = limit (5)
       0: LOAD r0, 0
       1: LOAD r1, 5
       2: CMP r0, r1       <- loop start
       3: JZ 7              <- if r0 == r1, jump to HALT
       4: PRINT r0
       5: INC r0
       6: JMP 2             <- back to loop
       7: HALT
    */
    struct Instruction prog4[8];
    prog4[0] = make_inst(OP_LOAD, 0, 0, 0, 0);
    prog4[1] = make_inst(OP_LOAD, 1, 0, 0, 5);
    prog4[2] = make_inst(OP_CMP, 0, 1, 0, 0);
    prog4[3] = make_inst(OP_JZ, 0, 0, 0, 7);
    prog4[4] = make_inst(OP_PRINT, 0, 0, 0, 0);
    prog4[5] = make_inst(OP_INC, 0, 0, 0, 0);
    prog4[6] = make_inst(OP_JMP, 0, 0, 0, 2);
    prog4[7] = make_inst(OP_HALT, 0, 0, 0, 0);

    vm_init(&vm);
    vm_run(&vm, prog4, 1000);
    // EXPECT: Loop outputs: 5
    printf("Loop outputs: %d\n", vm.output_count);
    // EXPECT: Loop out[0]: 0
    printf("Loop out[0]: %d\n", vm.output[0]);
    // EXPECT: Loop out[1]: 1
    printf("Loop out[1]: %d\n", vm.output[1]);
    // EXPECT: Loop out[4]: 4
    printf("Loop out[4]: %d\n", vm.output[4]);
    // EXPECT: Loop final r0: 5
    printf("Loop final r0: %d\n", vm.regs[0]);

    /* Program 5: compute sum 1+2+3+4+5 = 15 */
    /* r0 = i (counter), r1 = limit (6), r2 = sum
       0: LOAD r0, 1
       1: LOAD r1, 6
       2: LOAD r2, 0
       3: CMP r0, r1
       4: JZ 8
       5: ADD r2, r2, r0
       6: INC r0
       7: JMP 3
       8: PRINT r2
       9: HALT
    */
    struct Instruction prog5[10];
    prog5[0] = make_inst(OP_LOAD, 0, 0, 0, 1);
    prog5[1] = make_inst(OP_LOAD, 1, 0, 0, 6);
    prog5[2] = make_inst(OP_LOAD, 2, 0, 0, 0);
    prog5[3] = make_inst(OP_CMP, 0, 1, 0, 0);
    prog5[4] = make_inst(OP_JZ, 0, 0, 0, 8);
    prog5[5] = make_inst(OP_ADD, 2, 2, 0, 0);
    prog5[6] = make_inst(OP_INC, 0, 0, 0, 0);
    prog5[7] = make_inst(OP_JMP, 0, 0, 0, 3);
    prog5[8] = make_inst(OP_PRINT, 2, 0, 0, 0);
    prog5[9] = make_inst(OP_HALT, 0, 0, 0, 0);

    vm_init(&vm);
    vm_run(&vm, prog5, 1000);
    // EXPECT: Sum 1-5: 15
    printf("Sum 1-5: %d\n", vm.output[0]);
    /* 3 loads + 5 iterations of (CMP+JZ+ADD+INC+JMP=5) + final (CMP+JZ) + PRINT + HALT
       = 3 + 25 + 2 + 2 = 32 */
    // EXPECT: Sum cycles: 32
    printf("Sum cycles: %d\n", vm.cycle_count);
    // EXPECT: Sum halted: 1
    printf("Sum halted: %d\n", vm.halted);
    // EXPECT: Sum final r0: 6
    printf("Sum final r0: %d\n", vm.regs[0]);

    /* Program 6: MOV and DIV test */
    struct Instruction prog6[7];
    prog6[0] = make_inst(OP_LOAD, 0, 0, 0, 100);
    prog6[1] = make_inst(OP_LOAD, 1, 0, 0, 4);
    prog6[2] = make_inst(OP_DIV, 2, 0, 1, 0);      /* r2 = 100/4 = 25 */
    prog6[3] = make_inst(OP_MOV, 3, 2, 0, 0);       /* r3 = r2 = 25 */
    prog6[4] = make_inst(OP_PRINT, 2, 0, 0, 0);
    prog6[5] = make_inst(OP_PRINT, 3, 0, 0, 0);
    prog6[6] = make_inst(OP_HALT, 0, 0, 0, 0);

    vm_init(&vm);
    vm_run(&vm, prog6, 100);
    // EXPECT: Div result: 25
    printf("Div result: %d\n", vm.output[0]);
    // EXPECT: Mov result: 25
    printf("Mov result: %d\n", vm.output[1]);
    // EXPECT: Prog6 outputs: 2
    printf("Prog6 outputs: %d\n", vm.output_count);

    return 0;
}
