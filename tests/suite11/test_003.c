int printf(const char *fmt, ...);

// Register-based VM with 8 registers
// Opcodes: LOAD_IMM, ADD_RR, SUB_RR, MUL_RR, MOV, CMP, JZ, JMP, PRINT, HALT

enum RegOp {
    R_LOAD  = 1,
    R_ADD   = 2,
    R_SUB   = 3,
    R_MUL   = 4,
    R_MOV   = 5,
    R_CMP   = 6,
    R_JZ    = 7,
    R_JMP   = 8,
    R_PRINT = 9,
    R_HALT  = 10,
    R_DIV   = 11
};

struct RegVM {
    int regs[8];
    int flag;
    int code[128];
    int pc;
    int running;
};

void rvm_init(struct RegVM *vm) {
    int i;
    for (i = 0; i < 8; i++) vm->regs[i] = 0;
    for (i = 0; i < 128; i++) vm->code[i] = 0;
    vm->flag = 0;
    vm->pc = 0;
    vm->running = 1;
}

void rvm_run(struct RegVM *vm) {
    while (vm->running) {
        int op = vm->code[vm->pc];
        vm->pc = vm->pc + 1;

        if (op == R_LOAD) {
            int reg = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            int val = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            vm->regs[reg] = val;
        } else if (op == R_ADD) {
            int dst = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            int src = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            vm->regs[dst] = vm->regs[dst] + vm->regs[src];
        } else if (op == R_SUB) {
            int dst = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            int src = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            vm->regs[dst] = vm->regs[dst] - vm->regs[src];
        } else if (op == R_MUL) {
            int dst = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            int src = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            vm->regs[dst] = vm->regs[dst] * vm->regs[src];
        } else if (op == R_DIV) {
            int dst = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            int src = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            vm->regs[dst] = vm->regs[dst] / vm->regs[src];
        } else if (op == R_MOV) {
            int dst = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            int src = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            vm->regs[dst] = vm->regs[src];
        } else if (op == R_CMP) {
            int a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            int b = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            vm->flag = (vm->regs[a] == vm->regs[b]) ? 1 : 0;
        } else if (op == R_JZ) {
            int target = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            if (vm->flag == 0) {
                vm->pc = target;
            }
        } else if (op == R_JMP) {
            int target = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            vm->pc = target;
        } else if (op == R_PRINT) {
            int reg = vm->code[vm->pc]; vm->pc = vm->pc + 1;
            printf("r%d = %d\n", reg, vm->regs[reg]);
        } else if (op == R_HALT) {
            vm->running = 0;
        }
    }
}

int main() {
    struct RegVM vm;
    rvm_init(&vm);

    // Program: compute 5 * 3 + 7 = 22
    int i = 0;
    vm.code[i] = R_LOAD; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 5; i = i + 1;
    vm.code[i] = R_LOAD; i = i + 1; vm.code[i] = 1; i = i + 1; vm.code[i] = 3; i = i + 1;
    vm.code[i] = R_MUL;  i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 1; i = i + 1;
    vm.code[i] = R_LOAD; i = i + 1; vm.code[i] = 2; i = i + 1; vm.code[i] = 7; i = i + 1;
    vm.code[i] = R_ADD;  i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 2; i = i + 1;
    vm.code[i] = R_PRINT; i = i + 1; vm.code[i] = 0; i = i + 1;
    // LOAD r3=10, LOAD r4=2, DIV r3/r4 => 5, print
    vm.code[i] = R_LOAD; i = i + 1; vm.code[i] = 3; i = i + 1; vm.code[i] = 10; i = i + 1;
    vm.code[i] = R_LOAD; i = i + 1; vm.code[i] = 4; i = i + 1; vm.code[i] = 2; i = i + 1;
    vm.code[i] = R_DIV;  i = i + 1; vm.code[i] = 3; i = i + 1; vm.code[i] = 4; i = i + 1;
    vm.code[i] = R_PRINT; i = i + 1; vm.code[i] = 3; i = i + 1;
    // MOV r5 = r0, print r5
    vm.code[i] = R_MOV;  i = i + 1; vm.code[i] = 5; i = i + 1; vm.code[i] = 0; i = i + 1;
    vm.code[i] = R_PRINT; i = i + 1; vm.code[i] = 5; i = i + 1;
    vm.code[i] = R_HALT;

    rvm_run(&vm);
    // EXPECT: r0 = 22
    // EXPECT: r3 = 5
    // EXPECT: r5 = 22

    printf("pc at halt: %d\n", vm.pc);
    // EXPECT: pc at halt: 34

    // Test 2: countdown loop with JZ/JMP
    rvm_init(&vm);
    i = 0;
    // r0 = 3 (counter), r1 = 1 (decrement), r2 = 0 (target)
    vm.code[i] = R_LOAD; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 3; i = i + 1;
    vm.code[i] = R_LOAD; i = i + 1; vm.code[i] = 1; i = i + 1; vm.code[i] = 1; i = i + 1;
    vm.code[i] = R_LOAD; i = i + 1; vm.code[i] = 2; i = i + 1; vm.code[i] = 0; i = i + 1;
    // loop: print r0
    int loop_start = i;
    vm.code[i] = R_PRINT; i = i + 1; vm.code[i] = 0; i = i + 1;
    // sub r0, r1
    vm.code[i] = R_SUB; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 1; i = i + 1;
    // cmp r0, r2 => flag=1 if equal
    vm.code[i] = R_CMP; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 2; i = i + 1;
    // jz loop_start (if flag==0, not equal, jump back)
    vm.code[i] = R_JZ; i = i + 1; vm.code[i] = loop_start; i = i + 1;
    vm.code[i] = R_HALT;

    rvm_run(&vm);
    // EXPECT: r0 = 3
    // EXPECT: r0 = 2
    // EXPECT: r0 = 1

    printf("final r0: %d\n", vm.regs[0]);
    // EXPECT: final r0: 0

    return 0;
}
