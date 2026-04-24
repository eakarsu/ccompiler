int printf(const char *fmt, ...);

// Simple stack-based calculator VM
// Operations: PUSH, ADD, SUB, MUL, DIV, DUP, SWAP, PRINT, HALT

enum Opcode {
    OP_PUSH,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_DUP,
    OP_SWAP,
    OP_PRINT,
    OP_HALT,
    OP_NEG,
    OP_MOD,
    OP_OVER
};

typedef struct {
    int stack[64];
    int sp;
    int program[128];
    int pc;
    int running;
} VM;

void vm_init(VM *vm) {
    vm->sp = 0;
    vm->pc = 0;
    vm->running = 1;
    int i = 0;
    while (i < 64) {
        vm->stack[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < 128) {
        vm->program[i] = 0;
        i = i + 1;
    }
}

void vm_push(VM *vm, int val) {
    vm->stack[vm->sp] = val;
    vm->sp = vm->sp + 1;
}

int vm_pop(VM *vm) {
    vm->sp = vm->sp - 1;
    return vm->stack[vm->sp];
}

int vm_peek(VM *vm) {
    return vm->stack[vm->sp - 1];
}

void vm_step(VM *vm) {
    int op = vm->program[vm->pc];
    vm->pc = vm->pc + 1;
    int a;
    int b;
    int tmp;

    if (op == OP_PUSH) {
        a = vm->program[vm->pc];
        vm->pc = vm->pc + 1;
        vm_push(vm, a);
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
    } else if (op == OP_DUP) {
        a = vm_peek(vm);
        vm_push(vm, a);
    } else if (op == OP_SWAP) {
        b = vm_pop(vm);
        a = vm_pop(vm);
        vm_push(vm, b);
        vm_push(vm, a);
    } else if (op == OP_PRINT) {
        a = vm_peek(vm);
        printf("%d\n", a);
    } else if (op == OP_NEG) {
        a = vm_pop(vm);
        vm_push(vm, -a);
    } else if (op == OP_MOD) {
        b = vm_pop(vm);
        a = vm_pop(vm);
        vm_push(vm, a % b);
    } else if (op == OP_OVER) {
        a = vm->stack[vm->sp - 2];
        vm_push(vm, a);
    } else if (op == OP_HALT) {
        vm->running = 0;
    }
}

void vm_run(VM *vm) {
    while (vm->running) {
        vm_step(vm);
    }
}

void set_program(VM *vm, int *prog, int len) {
    int i = 0;
    while (i < len) {
        vm->program[i] = prog[i];
        i = i + 1;
    }
}

int main(void) {
    VM vm;

    // Test 1: push 3, push 4, add, print => 7
    vm_init(&vm);
    int prog1[7];
    prog1[0] = OP_PUSH; prog1[1] = 3;
    prog1[2] = OP_PUSH; prog1[3] = 4;
    prog1[4] = OP_ADD;
    prog1[5] = OP_PRINT;
    prog1[6] = OP_HALT;
    set_program(&vm, prog1, 7);
    vm_run(&vm);
    // EXPECT: 7

    // Test 2: push 10, push 3, sub, print => 7
    vm_init(&vm);
    int prog2[7];
    prog2[0] = OP_PUSH; prog2[1] = 10;
    prog2[2] = OP_PUSH; prog2[3] = 3;
    prog2[4] = OP_SUB;
    prog2[5] = OP_PRINT;
    prog2[6] = OP_HALT;
    set_program(&vm, prog2, 7);
    vm_run(&vm);
    // EXPECT: 7

    // Test 3: push 6, push 7, mul, print => 42
    vm_init(&vm);
    int prog3[7];
    prog3[0] = OP_PUSH; prog3[1] = 6;
    prog3[2] = OP_PUSH; prog3[3] = 7;
    prog3[4] = OP_MUL;
    prog3[5] = OP_PRINT;
    prog3[6] = OP_HALT;
    set_program(&vm, prog3, 7);
    vm_run(&vm);
    // EXPECT: 42

    // Test 4: push 20, push 4, div, print => 5
    vm_init(&vm);
    int prog4[7];
    prog4[0] = OP_PUSH; prog4[1] = 20;
    prog4[2] = OP_PUSH; prog4[3] = 4;
    prog4[4] = OP_DIV;
    prog4[5] = OP_PRINT;
    prog4[6] = OP_HALT;
    set_program(&vm, prog4, 7);
    vm_run(&vm);
    // EXPECT: 5

    // Test 5: push 5, dup, mul, print => 25
    vm_init(&vm);
    int prog5[6];
    prog5[0] = OP_PUSH; prog5[1] = 5;
    prog5[2] = OP_DUP;
    prog5[3] = OP_MUL;
    prog5[4] = OP_PRINT;
    prog5[5] = OP_HALT;
    set_program(&vm, prog5, 6);
    vm_run(&vm);
    // EXPECT: 25

    // Test 6: push 3, push 7, swap, sub, print => 4
    vm_init(&vm);
    int prog6[8];
    prog6[0] = OP_PUSH; prog6[1] = 3;
    prog6[2] = OP_PUSH; prog6[3] = 7;
    prog6[4] = OP_SWAP;
    prog6[5] = OP_SUB;
    prog6[6] = OP_PRINT;
    prog6[7] = OP_HALT;
    set_program(&vm, prog6, 8);
    vm_run(&vm);
    // EXPECT: 4

    // Test 7: compute (2 + 3) * (4 + 1) = 25
    vm_init(&vm);
    int prog7[15];
    prog7[0] = OP_PUSH; prog7[1] = 2;
    prog7[2] = OP_PUSH; prog7[3] = 3;
    prog7[4] = OP_ADD;
    prog7[5] = OP_PUSH; prog7[6] = 4;
    prog7[7] = OP_PUSH; prog7[8] = 1;
    prog7[9] = OP_ADD;
    prog7[10] = OP_MUL;
    prog7[11] = OP_PRINT;
    prog7[12] = OP_HALT;
    set_program(&vm, prog7, 13);
    vm_run(&vm);
    // EXPECT: 25

    // Test 8: neg => push 7, neg, print => -7
    vm_init(&vm);
    int prog8[5];
    prog8[0] = OP_PUSH; prog8[1] = 7;
    prog8[2] = OP_NEG;
    prog8[3] = OP_PRINT;
    prog8[4] = OP_HALT;
    set_program(&vm, prog8, 5);
    vm_run(&vm);
    // EXPECT: -7

    // Test 9: mod => push 17, push 5, mod, print => 2
    vm_init(&vm);
    int prog9[7];
    prog9[0] = OP_PUSH; prog9[1] = 17;
    prog9[2] = OP_PUSH; prog9[3] = 5;
    prog9[4] = OP_MOD;
    prog9[5] = OP_PRINT;
    prog9[6] = OP_HALT;
    set_program(&vm, prog9, 7);
    vm_run(&vm);
    // EXPECT: 2

    // Test 10: over => push 3, push 5, over, print => 3, then add print => 8
    vm_init(&vm);
    int prog10[10];
    prog10[0] = OP_PUSH; prog10[1] = 3;
    prog10[2] = OP_PUSH; prog10[3] = 5;
    prog10[4] = OP_OVER;
    prog10[5] = OP_PRINT;
    prog10[6] = OP_ADD;
    prog10[7] = OP_PRINT;
    prog10[8] = OP_HALT;
    set_program(&vm, prog10, 9);
    vm_run(&vm);
    // EXPECT: 3
    // EXPECT: 8

    // Test 11: stack depth tracking
    vm_init(&vm);
    vm_push(&vm, 10);
    vm_push(&vm, 20);
    vm_push(&vm, 30);
    printf("sp=%d\n", vm.sp);
    // EXPECT: sp=3
    vm_pop(&vm);
    printf("sp=%d\n", vm.sp);
    // EXPECT: sp=2

    printf("all stack vm tests passed\n");
    // EXPECT: all stack vm tests passed

    return 0;
}
