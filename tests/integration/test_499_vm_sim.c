int printf(const char *fmt, ...);
// EXPECT: === Virtual Machine Simulation ===\nProgram 1: Compute 3 + 4 * 5\n23\nCycles: 7\nProgram 2: Factorial of 5 (iterative)\n120\nCycles: 110\nProgram 3: Sum 1 to 10\nCycles: 200\nStack pointer: 1

enum Opcode {
    VM_PUSH, VM_POP, VM_ADD, VM_SUB, VM_MUL, VM_DIV,
    VM_DUP, VM_SWAP, VM_LOAD, VM_STORE,
    VM_JMP, VM_JZ, VM_CMP, VM_PRINT, VM_HALT
};

struct VM {
    int stack[32];
    int sp;
    int memory[16];
    int code[64];
    int pc;
    int running;
    int cycles;
    int flags;
};

const char *opcode_name(int op) {
    if (op == VM_PUSH) return "PUSH";
    if (op == VM_POP) return "POP";
    if (op == VM_ADD) return "ADD";
    if (op == VM_SUB) return "SUB";
    if (op == VM_MUL) return "MUL";
    if (op == VM_DIV) return "DIV";
    if (op == VM_DUP) return "DUP";
    if (op == VM_SWAP) return "SWAP";
    if (op == VM_LOAD) return "LOAD";
    if (op == VM_STORE) return "STORE";
    if (op == VM_JMP) return "JMP";
    if (op == VM_JZ) return "JZ";
    if (op == VM_CMP) return "CMP";
    if (op == VM_PRINT) return "PRINT";
    return "HALT";
}

void init_vm(struct VM *vm) {
    vm->sp = 0;
    vm->pc = 0;
    vm->running = 1;
    vm->cycles = 0;
    vm->flags = 0;
    int i;
    for (i = 0; i < 32; i++) vm->stack[i] = 0;
    for (i = 0; i < 16; i++) vm->memory[i] = 0;
    for (i = 0; i < 64; i++) vm->code[i] = 0;
}

void push(struct VM *vm, int val) {
    if (vm->sp < 32) {
        vm->stack[vm->sp] = val;
        vm->sp++;
    }
}

int pop(struct VM *vm) {
    if (vm->sp > 0) {
        vm->sp--;
        return vm->stack[vm->sp];
    }
    return 0;
}

int peek(struct VM *vm) {
    if (vm->sp > 0) return vm->stack[vm->sp - 1];
    return 0;
}

void step(struct VM *vm) {
    if (!vm->running) return;
    vm->cycles++;

    int op = vm->code[vm->pc];
    vm->pc++;

    int a, b, addr;

    if (op == VM_PUSH) {
        push(vm, vm->code[vm->pc]);
        vm->pc++;
    } else if (op == VM_POP) {
        pop(vm);
    } else if (op == VM_ADD) {
        b = pop(vm); a = pop(vm);
        push(vm, a + b);
    } else if (op == VM_SUB) {
        b = pop(vm); a = pop(vm);
        push(vm, a - b);
    } else if (op == VM_MUL) {
        b = pop(vm); a = pop(vm);
        push(vm, a * b);
    } else if (op == VM_DIV) {
        b = pop(vm); a = pop(vm);
        push(vm, b != 0 ? a / b : 0);
    } else if (op == VM_DUP) {
        push(vm, peek(vm));
    } else if (op == VM_SWAP) {
        b = pop(vm); a = pop(vm);
        push(vm, b); push(vm, a);
    } else if (op == VM_LOAD) {
        addr = pop(vm);
        if (addr >= 0 && addr < 16) push(vm, vm->memory[addr]);
        else push(vm, 0);
    } else if (op == VM_STORE) {
        addr = pop(vm); a = pop(vm);
        if (addr >= 0 && addr < 16) vm->memory[addr] = a;
    } else if (op == VM_JMP) {
        vm->pc = vm->code[vm->pc];
    } else if (op == VM_JZ) {
        addr = vm->code[vm->pc];
        vm->pc++;
        if (pop(vm) == 0) vm->pc = addr;
    } else if (op == VM_CMP) {
        b = pop(vm); a = pop(vm);
        vm->flags = (a == b) ? 0 : ((a < b) ? -1 : 1);
        push(vm, vm->flags);
    } else if (op == VM_PRINT) {
        printf("%d\n", pop(vm));
    } else if (op == VM_HALT) {
        vm->running = 0;
    }
}

void run(struct VM *vm) {
    while (vm->running && vm->cycles < 200) {
        step(vm);
    }
}

int main(void) {
    struct VM vm;

    printf("=== Virtual Machine Simulation ===\n");

    printf("Program 1: Compute 3 + 4 * 5\n");
    init_vm(&vm);
    int i = 0;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 4;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 5;
    vm.code[i++] = VM_MUL;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 3;
    vm.code[i++] = VM_ADD;
    vm.code[i++] = VM_PRINT;
    vm.code[i++] = VM_HALT;
    run(&vm);
    printf("Cycles: %d\n", vm.cycles);

    printf("Program 2: Factorial of 5 (iterative)\n");
    init_vm(&vm);
    i = 0;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 0;
    vm.code[i++] = VM_STORE;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 5;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_STORE;

    int loop_start = i;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_LOAD;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 0;
    vm.code[i++] = VM_CMP;
    int jz_addr = i;
    vm.code[i++] = VM_JZ; vm.code[i++] = 0;

    vm.code[i++] = VM_PUSH; vm.code[i++] = 0;
    vm.code[i++] = VM_LOAD;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_LOAD;
    vm.code[i++] = VM_MUL;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 0;
    vm.code[i++] = VM_STORE;

    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_LOAD;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_SUB;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_STORE;

    vm.code[i++] = VM_JMP; vm.code[i++] = loop_start;

    vm.code[jz_addr + 1] = i;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 0;
    vm.code[i++] = VM_LOAD;
    vm.code[i++] = VM_PRINT;
    vm.code[i++] = VM_HALT;

    run(&vm);
    printf("Cycles: %d\n", vm.cycles);

    printf("Program 3: Sum 1 to 10\n");
    init_vm(&vm);
    i = 0;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 0;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 0;
    vm.code[i++] = VM_STORE;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 10;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_STORE;

    loop_start = i;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_LOAD;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 0;
    vm.code[i++] = VM_CMP;
    jz_addr = i;
    vm.code[i++] = VM_JZ; vm.code[i++] = 0;

    vm.code[i++] = VM_PUSH; vm.code[i++] = 0;
    vm.code[i++] = VM_LOAD;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_LOAD;
    vm.code[i++] = VM_ADD;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 0;
    vm.code[i++] = VM_STORE;

    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_LOAD;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_SUB;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 1;
    vm.code[i++] = VM_STORE;

    vm.code[i++] = VM_JMP; vm.code[i++] = loop_start;

    vm.code[jz_addr + 1] = i;
    vm.code[i++] = VM_PUSH; vm.code[i++] = 0;
    vm.code[i++] = VM_LOAD;
    vm.code[i++] = VM_PRINT;
    vm.code[i++] = VM_HALT;

    run(&vm);
    printf("Cycles: %d\n", vm.cycles);

    printf("Stack pointer: %d\n", vm.sp);

    return 0;
}
