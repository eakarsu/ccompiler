int printf(const char *fmt, ...);

struct VM {
    int stack[32];
    int sp;
    int program[64];
    int pc;
    int running;
};

enum {
    OP_PUSH = 1,
    OP_POP  = 2,
    OP_ADD  = 3,
    OP_SUB  = 4,
    OP_MUL  = 5,
    OP_DIV  = 6,
    OP_DUP  = 7,
    OP_SWAP = 8,
    OP_NEG  = 9,
    OP_OVER = 10,
    OP_MOD  = 11,
    OP_HALT = 0
};

void vm_init(struct VM *vm) {
    vm->sp = 0;
    vm->pc = 0;
    vm->running = 1;
    int i = 0;
    while (i < 32) {
        vm->stack[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < 64) {
        vm->program[i] = 0;
        i = i + 1;
    }
}

void vm_push(struct VM *vm, int val) {
    if (vm->sp < 32) {
        vm->stack[vm->sp] = val;
        vm->sp = vm->sp + 1;
    }
}

int vm_pop(struct VM *vm) {
    if (vm->sp > 0) {
        vm->sp = vm->sp - 1;
        return vm->stack[vm->sp];
    }
    return 0;
}

int vm_peek(struct VM *vm) {
    if (vm->sp > 0) {
        return vm->stack[vm->sp - 1];
    }
    return 0;
}

void vm_step(struct VM *vm) {
    int op = vm->program[vm->pc];
    int a;
    int b;
    int tmp;
    vm->pc = vm->pc + 1;

    switch (op) {
        case OP_PUSH:
            a = vm->program[vm->pc];
            vm->pc = vm->pc + 1;
            vm_push(vm, a);
            break;
        case OP_POP:
            vm_pop(vm);
            break;
        case OP_ADD:
            b = vm_pop(vm);
            a = vm_pop(vm);
            vm_push(vm, a + b);
            break;
        case OP_SUB:
            b = vm_pop(vm);
            a = vm_pop(vm);
            vm_push(vm, a - b);
            break;
        case OP_MUL:
            b = vm_pop(vm);
            a = vm_pop(vm);
            vm_push(vm, a * b);
            break;
        case OP_DIV:
            b = vm_pop(vm);
            a = vm_pop(vm);
            if (b != 0) vm_push(vm, a / b);
            else vm_push(vm, 0);
            break;
        case OP_MOD:
            b = vm_pop(vm);
            a = vm_pop(vm);
            if (b != 0) vm_push(vm, a % b);
            else vm_push(vm, 0);
            break;
        case OP_DUP:
            a = vm_peek(vm);
            vm_push(vm, a);
            break;
        case OP_SWAP:
            b = vm_pop(vm);
            a = vm_pop(vm);
            vm_push(vm, b);
            vm_push(vm, a);
            break;
        case OP_NEG:
            a = vm_pop(vm);
            vm_push(vm, -a);
            break;
        case OP_OVER:
            if (vm->sp >= 2) {
                a = vm->stack[vm->sp - 2];
                vm_push(vm, a);
            }
            break;
        case OP_HALT:
            vm->running = 0;
            break;
    }
}

void vm_run(struct VM *vm) {
    while (vm->running && vm->pc < 64) {
        vm_step(vm);
    }
}

void vm_load(struct VM *vm, int prog[], int n) {
    int i = 0;
    while (i < n && i < 64) {
        vm->program[i] = prog[i];
        i = i + 1;
    }
}

int main(void) {
    struct VM vm;

    vm_init(&vm);
    int p1[7] = {OP_PUSH, 10, OP_PUSH, 20, OP_ADD, OP_HALT, 0};
    vm_load(&vm, p1, 7);
    vm_run(&vm);
    // EXPECT: add=30
    printf("add=%d\n", vm_peek(&vm));
    // EXPECT: sp=1
    printf("sp=%d\n", vm.sp);

    vm_init(&vm);
    int p2[7] = {OP_PUSH, 50, OP_PUSH, 30, OP_SUB, OP_HALT, 0};
    vm_load(&vm, p2, 7);
    vm_run(&vm);
    // EXPECT: sub=20
    printf("sub=%d\n", vm_peek(&vm));

    vm_init(&vm);
    int p3[7] = {OP_PUSH, 6, OP_PUSH, 7, OP_MUL, OP_HALT, 0};
    vm_load(&vm, p3, 7);
    vm_run(&vm);
    // EXPECT: mul=42
    printf("mul=%d\n", vm_peek(&vm));

    vm_init(&vm);
    int p4[7] = {OP_PUSH, 100, OP_PUSH, 4, OP_DIV, OP_HALT, 0};
    vm_load(&vm, p4, 7);
    vm_run(&vm);
    // EXPECT: div=25
    printf("div=%d\n", vm_peek(&vm));

    vm_init(&vm);
    int p5[5] = {OP_PUSH, 5, OP_DUP, OP_MUL, OP_HALT};
    vm_load(&vm, p5, 5);
    vm_run(&vm);
    // EXPECT: dup_mul=25
    printf("dup_mul=%d\n", vm_peek(&vm));

    vm_init(&vm);
    int p6[9] = {
        OP_PUSH, 3, OP_PUSH, 7,
        OP_SWAP, OP_SUB, OP_HALT, 0, 0
    };
    vm_load(&vm, p6, 9);
    vm_run(&vm);
    // EXPECT: swap_sub=4
    printf("swap_sub=%d\n", vm_peek(&vm));

    vm_init(&vm);
    int p7[5] = {OP_PUSH, 42, OP_NEG, OP_HALT, 0};
    vm_load(&vm, p7, 5);
    vm_run(&vm);
    // EXPECT: neg=-42
    printf("neg=%d\n", vm_peek(&vm));

    vm_init(&vm);
    int p8[7] = {OP_PUSH, 17, OP_PUSH, 5, OP_MOD, OP_HALT, 0};
    vm_load(&vm, p8, 7);
    vm_run(&vm);
    // EXPECT: mod=2
    printf("mod=%d\n", vm_peek(&vm));

    vm_init(&vm);
    int p9[12] = {
        OP_PUSH, 2, OP_PUSH, 3,
        OP_OVER, OP_ADD, OP_MUL,
        OP_HALT, 0, 0, 0, 0
    };
    vm_load(&vm, p9, 12);
    vm_run(&vm);
    // EXPECT: over=10
    printf("over=%d\n", vm_peek(&vm));

    vm_init(&vm);
    int p10[16] = {
        OP_PUSH, 10,
        OP_PUSH, 20,
        OP_PUSH, 30,
        OP_ADD,
        OP_ADD,
        OP_PUSH, 2,
        OP_DIV,
        OP_HALT,
        0, 0, 0, 0
    };
    vm_load(&vm, p10, 16);
    vm_run(&vm);
    // EXPECT: complex=30
    printf("complex=%d\n", vm_peek(&vm));

    vm_init(&vm);
    int p11[4] = {OP_PUSH, 0, OP_HALT, 0};
    vm_load(&vm, p11, 4);
    vm_run(&vm);
    // EXPECT: zero=0
    printf("zero=%d\n", vm_peek(&vm));

    vm_init(&vm);
    int p12[14] = {
        OP_PUSH, 5,
        OP_DUP,
        OP_PUSH, 1,
        OP_SUB,
        OP_DUP,
        OP_PUSH, 1,
        OP_SUB,
        OP_ADD,
        OP_ADD,
        OP_HALT,
        0
    };
    vm_load(&vm, p12, 14);
    vm_run(&vm);
    // EXPECT: chain=12
    printf("chain=%d\n", vm_peek(&vm));

    // EXPECT: final_sp=1
    printf("final_sp=%d\n", vm.sp);

    return 0;
}
