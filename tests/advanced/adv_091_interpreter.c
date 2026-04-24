// EXPECT: Program 1: 3+4=7
// EXPECT: Program 2: (5*3)-(2+1)=12
// EXPECT: Program 3: 10-3-2=5
// EXPECT: Program 4: 2*3*4=24
// EXPECT: Program 5: factorial-like 1*2*3*4*5=120
// EXPECT: All interpreter tests passed!

int printf(const char *fmt, ...);

// Opcodes for our bytecode VM
enum Opcode {
    OP_PUSH,    // push immediate value onto stack
    OP_ADD,     // pop two, push sum
    OP_SUB,     // pop two, push difference (second - first)
    OP_MUL,     // pop two, push product
    OP_DUP,     // duplicate top of stack
    OP_SWAP,    // swap top two stack elements
    OP_PRINT,   // pop and print top of stack
    OP_HALT     // stop execution
};

// Instruction: opcode + optional operand
struct Instruction {
    int opcode;
    int operand;  // used only by OP_PUSH
};

// Virtual machine state
struct VM {
    int stack[64];
    int sp;           // stack pointer
    struct Instruction program[64];
    int pc;           // program counter
    int program_len;
    int last_printed;
};

void vm_init(struct VM *vm) {
    vm->sp = 0;
    vm->pc = 0;
    vm->program_len = 0;
    vm->last_printed = 0;
    int i;
    for (i = 0; i < 64; i++) {
        vm->stack[i] = 0;
    }
}

void vm_emit(struct VM *vm, int opcode, int operand) {
    vm->program[vm->program_len].opcode = opcode;
    vm->program[vm->program_len].operand = operand;
    vm->program_len = vm->program_len + 1;
}

void vm_push(struct VM *vm, int val) {
    vm->stack[vm->sp] = val;
    vm->sp = vm->sp + 1;
}

int vm_pop(struct VM *vm) {
    vm->sp = vm->sp - 1;
    return vm->stack[vm->sp];
}

int vm_run(struct VM *vm) {
    int a;
    int b;
    int running = 1;
    vm->pc = 0;

    while (running && vm->pc < vm->program_len) {
        struct Instruction inst = vm->program[vm->pc];
        vm->pc = vm->pc + 1;

        if (inst.opcode == OP_PUSH) {
            vm_push(vm, inst.operand);
        } else if (inst.opcode == OP_ADD) {
            a = vm_pop(vm);
            b = vm_pop(vm);
            vm_push(vm, b + a);
        } else if (inst.opcode == OP_SUB) {
            a = vm_pop(vm);
            b = vm_pop(vm);
            vm_push(vm, b - a);
        } else if (inst.opcode == OP_MUL) {
            a = vm_pop(vm);
            b = vm_pop(vm);
            vm_push(vm, b * a);
        } else if (inst.opcode == OP_DUP) {
            a = vm->stack[vm->sp - 1];
            vm_push(vm, a);
        } else if (inst.opcode == OP_SWAP) {
            a = vm_pop(vm);
            b = vm_pop(vm);
            vm_push(vm, a);
            vm_push(vm, b);
        } else if (inst.opcode == OP_PRINT) {
            vm->last_printed = vm_pop(vm);
        } else if (inst.opcode == OP_HALT) {
            running = 0;
        }
    }
    return vm->last_printed;
}

int main(void) {
    struct VM vm;
    int result;

    // Program 1: 3 + 4 = 7
    vm_init(&vm);
    vm_emit(&vm, OP_PUSH, 3);
    vm_emit(&vm, OP_PUSH, 4);
    vm_emit(&vm, OP_ADD, 0);
    vm_emit(&vm, OP_PRINT, 0);
    vm_emit(&vm, OP_HALT, 0);
    result = vm_run(&vm);
    printf("Program 1: 3+4=%d\n", result);

    // Program 2: (5 * 3) - (2 + 1) = 12
    vm_init(&vm);
    vm_emit(&vm, OP_PUSH, 5);
    vm_emit(&vm, OP_PUSH, 3);
    vm_emit(&vm, OP_MUL, 0);
    vm_emit(&vm, OP_PUSH, 2);
    vm_emit(&vm, OP_PUSH, 1);
    vm_emit(&vm, OP_ADD, 0);
    vm_emit(&vm, OP_SUB, 0);
    vm_emit(&vm, OP_PRINT, 0);
    vm_emit(&vm, OP_HALT, 0);
    result = vm_run(&vm);
    printf("Program 2: (5*3)-(2+1)=%d\n", result);

    // Program 3: 10 - 3 - 2 = 5
    vm_init(&vm);
    vm_emit(&vm, OP_PUSH, 10);
    vm_emit(&vm, OP_PUSH, 3);
    vm_emit(&vm, OP_SUB, 0);
    vm_emit(&vm, OP_PUSH, 2);
    vm_emit(&vm, OP_SUB, 0);
    vm_emit(&vm, OP_PRINT, 0);
    vm_emit(&vm, OP_HALT, 0);
    result = vm_run(&vm);
    printf("Program 3: 10-3-2=%d\n", result);

    // Program 4: 2 * 3 * 4 = 24
    vm_init(&vm);
    vm_emit(&vm, OP_PUSH, 2);
    vm_emit(&vm, OP_PUSH, 3);
    vm_emit(&vm, OP_MUL, 0);
    vm_emit(&vm, OP_PUSH, 4);
    vm_emit(&vm, OP_MUL, 0);
    vm_emit(&vm, OP_PRINT, 0);
    vm_emit(&vm, OP_HALT, 0);
    result = vm_run(&vm);
    printf("Program 4: 2*3*4=%d\n", result);

    // Program 5: factorial-like: push 1, then multiply by 2,3,4,5
    vm_init(&vm);
    vm_emit(&vm, OP_PUSH, 1);
    vm_emit(&vm, OP_PUSH, 2);
    vm_emit(&vm, OP_MUL, 0);
    vm_emit(&vm, OP_PUSH, 3);
    vm_emit(&vm, OP_MUL, 0);
    vm_emit(&vm, OP_PUSH, 4);
    vm_emit(&vm, OP_MUL, 0);
    vm_emit(&vm, OP_PUSH, 5);
    vm_emit(&vm, OP_MUL, 0);
    vm_emit(&vm, OP_PRINT, 0);
    vm_emit(&vm, OP_HALT, 0);
    result = vm_run(&vm);
    printf("Program 5: factorial-like 1*2*3*4*5=%d\n", result);

    printf("All interpreter tests passed!\n");
    return 0;
}
