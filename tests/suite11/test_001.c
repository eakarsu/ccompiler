int printf(const char *fmt, ...);

// Bytecode interpreter: a simple stack-based bytecode VM
// Opcodes: PUSH=1, ADD=2, SUB=3, MUL=4, DIV=5, PRINT=6, HALT=7

enum Opcode {
    OP_PUSH  = 1,
    OP_ADD   = 2,
    OP_SUB   = 3,
    OP_MUL   = 4,
    OP_DIV   = 5,
    OP_PRINT = 6,
    OP_HALT  = 7
};

struct VM {
    int stack[64];
    int sp;
    int program[128];
    int pc;
    int running;
};

void vm_init(struct VM *vm) {
    vm->sp = 0;
    vm->pc = 0;
    vm->running = 1;
    int i;
    for (i = 0; i < 64; i++) {
        vm->stack[i] = 0;
    }
    for (i = 0; i < 128; i++) {
        vm->program[i] = 0;
    }
}

void vm_push(struct VM *vm, int val) {
    vm->stack[vm->sp] = val;
    vm->sp = vm->sp + 1;
}

int vm_pop(struct VM *vm) {
    vm->sp = vm->sp - 1;
    return vm->stack[vm->sp];
}

void vm_step(struct VM *vm) {
    int op = vm->program[vm->pc];
    vm->pc = vm->pc + 1;
    int a;
    int b;

    if (op == OP_PUSH) {
        int val = vm->program[vm->pc];
        vm->pc = vm->pc + 1;
        vm_push(vm, val);
    } else if (op == OP_ADD) {
        b = vm_pop(vm);
        a = vm_pop(vm);
        vm_push(vm, a + b);
    } else if (op == OP_SUB) {
        b = vm_pop(vm);
        a = vm_pop(vm);
        vm_push(vm, a - b);
    } else if (op == OP_MUL) {
        b = vm_pop(vm);
        a = vm_pop(vm);
        vm_push(vm, a * b);
    } else if (op == OP_DIV) {
        b = vm_pop(vm);
        a = vm_pop(vm);
        vm_push(vm, a / b);
    } else if (op == OP_PRINT) {
        a = vm_pop(vm);
        printf("out: %d\n", a);
    } else if (op == OP_HALT) {
        vm->running = 0;
    }
}

void vm_run(struct VM *vm) {
    while (vm->running) {
        vm_step(vm);
    }
}

int main() {
    struct VM vm;
    vm_init(&vm);

    // Program: push 10, push 20, add, print => 30
    int idx = 0;
    vm.program[idx] = OP_PUSH; idx = idx + 1;
    vm.program[idx] = 10;     idx = idx + 1;
    vm.program[idx] = OP_PUSH; idx = idx + 1;
    vm.program[idx] = 20;     idx = idx + 1;
    vm.program[idx] = OP_ADD;  idx = idx + 1;
    vm.program[idx] = OP_PRINT; idx = idx + 1;

    // push 100, push 3, mul, print => 300
    vm.program[idx] = OP_PUSH; idx = idx + 1;
    vm.program[idx] = 100;    idx = idx + 1;
    vm.program[idx] = OP_PUSH; idx = idx + 1;
    vm.program[idx] = 3;      idx = idx + 1;
    vm.program[idx] = OP_MUL;  idx = idx + 1;
    vm.program[idx] = OP_PRINT; idx = idx + 1;

    // push 50, push 8, sub, print => 42
    vm.program[idx] = OP_PUSH; idx = idx + 1;
    vm.program[idx] = 50;     idx = idx + 1;
    vm.program[idx] = OP_PUSH; idx = idx + 1;
    vm.program[idx] = 8;      idx = idx + 1;
    vm.program[idx] = OP_SUB;  idx = idx + 1;
    vm.program[idx] = OP_PRINT; idx = idx + 1;

    // push 144, push 12, div, print => 12
    vm.program[idx] = OP_PUSH; idx = idx + 1;
    vm.program[idx] = 144;    idx = idx + 1;
    vm.program[idx] = OP_PUSH; idx = idx + 1;
    vm.program[idx] = 12;     idx = idx + 1;
    vm.program[idx] = OP_DIV;  idx = idx + 1;
    vm.program[idx] = OP_PRINT; idx = idx + 1;

    vm.program[idx] = OP_HALT;

    vm_run(&vm);
    // EXPECT: out: 30
    // EXPECT: out: 300
    // EXPECT: out: 42
    // EXPECT: out: 12

    printf("sp after run: %d\n", vm.sp);
    // EXPECT: sp after run: 0

    printf("instructions executed: %d\n", vm.pc);
    // EXPECT: instructions executed: 25

    return 0;
}
