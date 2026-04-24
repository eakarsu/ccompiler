int printf(const char *fmt, ...);
// EXPECT: === Program 1: (3 + 4) * 5 ===\n35\n=== Program 2: 10 - 3 * 2 ===\n4\n=== Program 3: factorial(5) ===\n120\n=== Program 4: 17 mod 5 ===\n2
// Simple stack-based bytecode interpreter

enum OpCode {
    OP_PUSH = 0,
    OP_POP = 1,
    OP_ADD = 2,
    OP_SUB = 3,
    OP_MUL = 4,
    OP_DIV = 5,
    OP_DUP = 6,
    OP_SWAP = 7,
    OP_PRINT = 8,
    OP_HALT = 9,
    OP_NEG = 10,
    OP_MOD = 11
};

struct VM {
    int code[64];
    int code_len;
    int stack[32];
    int sp;
    int ip;
    int running;
};

void vm_init(struct VM *vm) {
    int i;
    for (i = 0; i < 64; i++) vm->code[i] = 0;
    for (i = 0; i < 32; i++) vm->stack[i] = 0;
    vm->code_len = 0;
    vm->sp = 0;
    vm->ip = 0;
    vm->running = 1;
}

void vm_emit(struct VM *vm, int op) {
    vm->code[vm->code_len] = op;
    vm->code_len++;
}

void vm_push(struct VM *vm, int val) {
    vm->stack[vm->sp] = val;
    vm->sp++;
}

int vm_pop(struct VM *vm) {
    vm->sp--;
    return vm->stack[vm->sp];
}

void vm_run(struct VM *vm) {
    int op;
    int a;
    int b;
    int tmp;
    while (vm->running && vm->ip < vm->code_len) {
        op = vm->code[vm->ip];
        vm->ip++;
        if (op == OP_PUSH) {
            vm_push(vm, vm->code[vm->ip]);
            vm->ip++;
        } else if (op == OP_POP) {
            vm_pop(vm);
        } else if (op == OP_ADD) {
            b = vm_pop(vm); a = vm_pop(vm);
            vm_push(vm, a + b);
        } else if (op == OP_SUB) {
            b = vm_pop(vm); a = vm_pop(vm);
            vm_push(vm, a - b);
        } else if (op == OP_MUL) {
            b = vm_pop(vm); a = vm_pop(vm);
            vm_push(vm, a * b);
        } else if (op == OP_DIV) {
            b = vm_pop(vm); a = vm_pop(vm);
            vm_push(vm, a / b);
        } else if (op == OP_MOD) {
            b = vm_pop(vm); a = vm_pop(vm);
            vm_push(vm, a % b);
        } else if (op == OP_DUP) {
            a = vm->stack[vm->sp - 1];
            vm_push(vm, a);
        } else if (op == OP_SWAP) {
            tmp = vm->stack[vm->sp - 1];
            vm->stack[vm->sp - 1] = vm->stack[vm->sp - 2];
            vm->stack[vm->sp - 2] = tmp;
        } else if (op == OP_NEG) {
            vm->stack[vm->sp - 1] = -vm->stack[vm->sp - 1];
        } else if (op == OP_PRINT) {
            printf("%d\n", vm_pop(vm));
        } else if (op == OP_HALT) {
            vm->running = 0;
        }
    }
}

int main(void) {
    struct VM vm;

    printf("=== Program 1: (3 + 4) * 5 ===\n");
    vm_init(&vm);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 3);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 4);
    vm_emit(&vm, OP_ADD);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 5);
    vm_emit(&vm, OP_MUL);
    vm_emit(&vm, OP_PRINT);
    vm_emit(&vm, OP_HALT);
    vm_run(&vm);

    printf("=== Program 2: 10 - 3 * 2 ===\n");
    vm_init(&vm);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 3);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 2);
    vm_emit(&vm, OP_MUL);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 10);
    vm_emit(&vm, OP_SWAP);
    vm_emit(&vm, OP_SUB);
    vm_emit(&vm, OP_PRINT);
    vm_emit(&vm, OP_HALT);
    vm_run(&vm);

    printf("=== Program 3: factorial(5) ===\n");
    vm_init(&vm);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 1);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 2);
    vm_emit(&vm, OP_MUL);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 3);
    vm_emit(&vm, OP_MUL);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 4);
    vm_emit(&vm, OP_MUL);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 5);
    vm_emit(&vm, OP_MUL);
    vm_emit(&vm, OP_PRINT);
    vm_emit(&vm, OP_HALT);
    vm_run(&vm);

    printf("=== Program 4: 17 mod 5 ===\n");
    vm_init(&vm);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 17);
    vm_emit(&vm, OP_PUSH); vm_emit(&vm, 5);
    vm_emit(&vm, OP_MOD);
    vm_emit(&vm, OP_PRINT);
    vm_emit(&vm, OP_HALT);
    vm_run(&vm);

    return 0;
}
