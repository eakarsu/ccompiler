int printf(const char *fmt, ...);

// Register-based VM with 4 registers (R0-R3)
// Instructions: LOAD_IMM, MOVE, ADD, SUB, MUL, DIV, CMP, JZ, JNZ, PRINT, HALT

enum RegOp {
    R_LOAD_IMM,
    R_MOVE,
    R_ADD,
    R_SUB,
    R_MUL,
    R_DIV,
    R_MOD,
    R_CMP,
    R_JZ,
    R_JNZ,
    R_JMP,
    R_PRINT,
    R_HALT,
    R_AND,
    R_OR,
    R_INC,
    R_DEC
};

typedef struct {
    int regs[4];
    int code[256];
    int pc;
    int cmp_flag;
    int running;
} RegVM;

void rvm_init(RegVM *vm) {
    int i = 0;
    while (i < 4) { vm->regs[i] = 0; i = i + 1; }
    i = 0;
    while (i < 256) { vm->code[i] = 0; i = i + 1; }
    vm->pc = 0;
    vm->cmp_flag = 0;
    vm->running = 1;
}

void rvm_step(RegVM *vm) {
    int op = vm->code[vm->pc];
    vm->pc = vm->pc + 1;
    int a;
    int b;
    int val;

    if (op == R_LOAD_IMM) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        val = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->regs[a] = val;
    } else if (op == R_MOVE) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        b = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->regs[a] = vm->regs[b];
    } else if (op == R_ADD) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        b = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->regs[a] = vm->regs[a] + vm->regs[b];
    } else if (op == R_SUB) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        b = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->regs[a] = vm->regs[a] - vm->regs[b];
    } else if (op == R_MUL) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        b = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->regs[a] = vm->regs[a] * vm->regs[b];
    } else if (op == R_DIV) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        b = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->regs[a] = vm->regs[a] / vm->regs[b];
    } else if (op == R_MOD) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        b = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->regs[a] = vm->regs[a] % vm->regs[b];
    } else if (op == R_CMP) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        b = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->cmp_flag = vm->regs[a] - vm->regs[b];
    } else if (op == R_JZ) {
        val = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        if (vm->cmp_flag == 0) vm->pc = val;
    } else if (op == R_JNZ) {
        val = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        if (vm->cmp_flag != 0) vm->pc = val;
    } else if (op == R_JMP) {
        val = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->pc = val;
    } else if (op == R_PRINT) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        printf("R%d=%d\n", a, vm->regs[a]);
    } else if (op == R_AND) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        b = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->regs[a] = vm->regs[a] & vm->regs[b];
    } else if (op == R_OR) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        b = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->regs[a] = vm->regs[a] | vm->regs[b];
    } else if (op == R_INC) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->regs[a] = vm->regs[a] + 1;
    } else if (op == R_DEC) {
        a = vm->code[vm->pc]; vm->pc = vm->pc + 1;
        vm->regs[a] = vm->regs[a] - 1;
    } else if (op == R_HALT) {
        vm->running = 0;
    }
}

void rvm_run(RegVM *vm) {
    while (vm->running) {
        rvm_step(vm);
    }
}

int main(void) {
    RegVM vm;
    int i;

    // Test 1: load and print
    rvm_init(&vm);
    i = 0;
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 42; i = i + 1;
    vm.code[i] = R_PRINT; i = i + 1; vm.code[i] = 0; i = i + 1;
    vm.code[i] = R_HALT; i = i + 1;
    rvm_run(&vm);
    // EXPECT: R0=42

    // Test 2: add two registers
    rvm_init(&vm);
    i = 0;
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 15; i = i + 1;
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 1; i = i + 1; vm.code[i] = 27; i = i + 1;
    vm.code[i] = R_ADD; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 1; i = i + 1;
    vm.code[i] = R_PRINT; i = i + 1; vm.code[i] = 0; i = i + 1;
    vm.code[i] = R_HALT; i = i + 1;
    rvm_run(&vm);
    // EXPECT: R0=42

    // Test 3: multiply
    rvm_init(&vm);
    i = 0;
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 6; i = i + 1;
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 1; i = i + 1; vm.code[i] = 7; i = i + 1;
    vm.code[i] = R_MUL; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 1; i = i + 1;
    vm.code[i] = R_PRINT; i = i + 1; vm.code[i] = 0; i = i + 1;
    vm.code[i] = R_HALT; i = i + 1;
    rvm_run(&vm);
    // EXPECT: R0=42

    // Test 4: loop counting down from 5 to 0
    // R0 = counter (5), R1 = 0 for comparison
    rvm_init(&vm);
    i = 0;
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 5; i = i + 1; // 0: R0=5
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 1; i = i + 1; vm.code[i] = 0; i = i + 1; // 3: R1=0
    // loop start at pc=6
    vm.code[i] = R_CMP; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 1; i = i + 1;     // 6: CMP R0,R1
    vm.code[i] = R_JZ; i = i + 1; vm.code[i] = 15; i = i + 1;                                   // 9: JZ end(15)
    vm.code[i] = R_DEC; i = i + 1; vm.code[i] = 0; i = i + 1;                                   // 11: DEC R0
    vm.code[i] = R_JMP; i = i + 1; vm.code[i] = 6; i = i + 1;                                   // 13: JMP loop(6)
    vm.code[i] = R_PRINT; i = i + 1; vm.code[i] = 0; i = i + 1;                                 // 15: PRINT R0
    vm.code[i] = R_HALT; i = i + 1;                                                               // 17: HALT
    rvm_run(&vm);
    // EXPECT: R0=0

    // Test 5: compute factorial(5) = 120
    // R0=n(5), R1=result(1), R2=0, R3=temp
    rvm_init(&vm);
    i = 0;
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 5; i = i + 1;  // 0: R0=5
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 1; i = i + 1; vm.code[i] = 1; i = i + 1;  // 3: R1=1
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 2; i = i + 1; vm.code[i] = 0; i = i + 1;  // 6: R2=0
    // loop at pc=9: while R0 > 0
    vm.code[i] = R_CMP; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 2; i = i + 1;       // 9: CMP R0,R2
    vm.code[i] = R_JZ; i = i + 1; vm.code[i] = 21; i = i + 1;                                    // 12: JZ end(21)
    vm.code[i] = R_MUL; i = i + 1; vm.code[i] = 1; i = i + 1; vm.code[i] = 0; i = i + 1;       // 14: R1 *= R0
    vm.code[i] = R_DEC; i = i + 1; vm.code[i] = 0; i = i + 1;                                    // 17: R0--
    vm.code[i] = R_JMP; i = i + 1; vm.code[i] = 9; i = i + 1;                                    // 19: JMP loop(9)
    vm.code[i] = R_PRINT; i = i + 1; vm.code[i] = 1; i = i + 1;                                  // 21: PRINT R1
    vm.code[i] = R_HALT; i = i + 1;                                                                // 23: HALT
    rvm_run(&vm);
    // EXPECT: R1=120

    // Test 6: bitwise AND and OR
    rvm_init(&vm);
    i = 0;
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 15; i = i + 1;
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 1; i = i + 1; vm.code[i] = 6; i = i + 1;
    vm.code[i] = R_AND; i = i + 1; vm.code[i] = 0; i = i + 1; vm.code[i] = 1; i = i + 1;
    vm.code[i] = R_PRINT; i = i + 1; vm.code[i] = 0; i = i + 1;
    vm.code[i] = R_HALT; i = i + 1;
    rvm_run(&vm);
    // EXPECT: R0=6

    // Test 7: move instruction
    rvm_init(&vm);
    i = 0;
    vm.code[i] = R_LOAD_IMM; i = i + 1; vm.code[i] = 2; i = i + 1; vm.code[i] = 99; i = i + 1;
    vm.code[i] = R_MOVE; i = i + 1; vm.code[i] = 3; i = i + 1; vm.code[i] = 2; i = i + 1;
    vm.code[i] = R_PRINT; i = i + 1; vm.code[i] = 3; i = i + 1;
    vm.code[i] = R_HALT; i = i + 1;
    rvm_run(&vm);
    // EXPECT: R3=99

    printf("register vm done\n");
    // EXPECT: register vm done

    return 0;
}
