// stress_080_interpreter_vm.c - Stack-based VM interpreter
// EXPECT: factorial_10=3628800
// EXPECT: fib=0,1,1,2,3,5,8,13,21,34
// EXPECT: prime_7=1
// EXPECT: prime_12=0
// EXPECT: prime_23=1
// EXPECT: prime_1=0
// EXPECT: prime_2=1
// EXPECT: stack_dup_add=20
// EXPECT: stack_swap_sub=4
// EXPECT: stack_over_add=13
// EXPECT: stack_rot_top=1
// EXPECT: stack_rot_mid=3
// EXPECT: stack_rot_bot=2

int printf(const char *fmt, ...);

// Opcodes
enum {
    OP_PUSH = 0,
    OP_POP = 1,
    OP_ADD = 2,
    OP_SUB = 3,
    OP_MUL = 4,
    OP_DIV = 5,
    OP_MOD = 6,
    OP_DUP = 7,
    OP_SWAP = 8,
    OP_OVER = 9,
    OP_ROT = 10,
    OP_JMP = 11,
    OP_JZ = 12,
    OP_JNZ = 13,
    OP_HALT = 14,
    OP_PRINT = 15,
    OP_LT = 16,
    OP_EQ = 17,
    OP_NEG = 18,
    OP_STORE = 19,
    OP_LOAD = 20
};

typedef struct {
    int code[256];
    int code_len;
    int stack[64];
    int sp;
    int vars[16];     // local variables
    int output[32];
    int out_count;
    int halted;
} VM;

void vm_init(VM *vm) {
    vm->code_len = 0;
    vm->sp = 0;
    vm->out_count = 0;
    vm->halted = 0;
    int i;
    for (i = 0; i < 16; i++) vm->vars[i] = 0;
}

void vm_emit(VM *vm, int op) {
    vm->code[vm->code_len++] = op;
}

void vm_emit2(VM *vm, int op, int arg) {
    vm->code[vm->code_len++] = op;
    vm->code[vm->code_len++] = arg;
}

void vm_push(VM *vm, int val) {
    vm->stack[vm->sp++] = val;
}

int vm_pop(VM *vm) {
    return vm->stack[--vm->sp];
}

int vm_peek(VM *vm) {
    return vm->stack[vm->sp - 1];
}

void vm_run(VM *vm) {
    int pc = 0;
    int limit = 10000; // prevent infinite loops
    vm->halted = 0;
    while (pc < vm->code_len && !vm->halted && limit > 0) {
        limit--;
        int op = vm->code[pc++];
        int a;
        int b;
        int c;
        int tmp;
        switch (op) {
        case OP_PUSH:
            vm_push(vm, vm->code[pc++]);
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
            vm_push(vm, a / b);
            break;
        case OP_MOD:
            b = vm_pop(vm);
            a = vm_pop(vm);
            vm_push(vm, a % b);
            break;
        case OP_DUP:
            vm_push(vm, vm_peek(vm));
            break;
        case OP_SWAP:
            a = vm_pop(vm);
            b = vm_pop(vm);
            vm_push(vm, a);
            vm_push(vm, b);
            break;
        case OP_OVER:
            // Copy second element to top
            a = vm->stack[vm->sp - 2];
            vm_push(vm, a);
            break;
        case OP_ROT:
            // Rotate top 3: (a b c -- b c a)
            c = vm_pop(vm);
            b = vm_pop(vm);
            a = vm_pop(vm);
            vm_push(vm, b);
            vm_push(vm, c);
            vm_push(vm, a);
            break;
        case OP_JMP:
            pc = vm->code[pc];
            break;
        case OP_JZ:
            tmp = vm->code[pc++];
            a = vm_pop(vm);
            if (a == 0) pc = tmp;
            break;
        case OP_JNZ:
            tmp = vm->code[pc++];
            a = vm_pop(vm);
            if (a != 0) pc = tmp;
            break;
        case OP_HALT:
            vm->halted = 1;
            break;
        case OP_PRINT:
            a = vm_pop(vm);
            vm->output[vm->out_count++] = a;
            break;
        case OP_LT:
            b = vm_pop(vm);
            a = vm_pop(vm);
            vm_push(vm, a < b ? 1 : 0);
            break;
        case OP_EQ:
            b = vm_pop(vm);
            a = vm_pop(vm);
            vm_push(vm, a == b ? 1 : 0);
            break;
        case OP_NEG:
            a = vm_pop(vm);
            vm_push(vm, -a);
            break;
        case OP_STORE:
            tmp = vm->code[pc++]; // variable index
            a = vm_pop(vm);
            vm->vars[tmp] = a;
            break;
        case OP_LOAD:
            tmp = vm->code[pc++]; // variable index
            vm_push(vm, vm->vars[tmp]);
            break;
        }
    }
}

int main(void) {
    VM vm;

    // ---- Program 1: Compute factorial of 10 ----
    // Algorithm: var[0] = n = 10, var[1] = result = 1
    // loop: if n <= 1 goto end; result = result * n; n = n - 1; goto loop
    vm_init(&vm);
    // Initialize: n=10, result=1
    vm_emit2(&vm, OP_PUSH, 10);
    vm_emit2(&vm, OP_STORE, 0);     // var[0] = 10 (n)
    vm_emit2(&vm, OP_PUSH, 1);
    vm_emit2(&vm, OP_STORE, 1);     // var[1] = 1 (result)
    // Loop start (pc = 8)
    vm_emit2(&vm, OP_LOAD, 0);      // push n
    vm_emit2(&vm, OP_PUSH, 1);
    vm_emit(  &vm, OP_LT);          // n < 1?
    // Actually we want: if n <= 1, stop. So: push n, push 2, LT => n < 2
    // Let me redo. The code above pushes n, then 1, then LT => (n < 1).
    // We want to stop when n <= 1, i.e., n < 2.
    // Let me restart the program.
    vm_init(&vm);
    vm_emit2(&vm, OP_PUSH, 10);     // pc=0,1
    vm_emit2(&vm, OP_STORE, 0);     // pc=2,3: var[0] = n = 10
    vm_emit2(&vm, OP_PUSH, 1);      // pc=4,5
    vm_emit2(&vm, OP_STORE, 1);     // pc=6,7: var[1] = result = 1
    // Loop start at pc=8
    vm_emit2(&vm, OP_LOAD, 0);      // pc=8,9: push n
    vm_emit2(&vm, OP_PUSH, 2);      // pc=10,11
    vm_emit( &vm, OP_LT);           // pc=12: n < 2?
    // if true (n < 2), jump to end
    int jz_patch = vm.code_len;
    vm_emit2(&vm, OP_JNZ, 0);       // pc=13,14: JNZ to end (patch later)
    // result = result * n
    vm_emit2(&vm, OP_LOAD, 1);      // pc=15,16: push result
    vm_emit2(&vm, OP_LOAD, 0);      // pc=17,18: push n
    vm_emit( &vm, OP_MUL);          // pc=19: result * n
    vm_emit2(&vm, OP_STORE, 1);     // pc=20,21: store result
    // n = n - 1
    vm_emit2(&vm, OP_LOAD, 0);      // pc=22,23: push n
    vm_emit2(&vm, OP_PUSH, 1);      // pc=24,25
    vm_emit( &vm, OP_SUB);          // pc=26: n - 1
    vm_emit2(&vm, OP_STORE, 0);     // pc=27,28: store n
    vm_emit2(&vm, OP_JMP, 8);       // pc=29,30: goto loop
    // End:
    int end_pc = vm.code_len;       // pc=31
    vm.code[jz_patch + 1] = end_pc; // patch jump target
    vm_emit2(&vm, OP_LOAD, 1);      // push result
    vm_emit( &vm, OP_PRINT);
    vm_emit( &vm, OP_HALT);

    vm_run(&vm);
    printf("factorial_10=%d\n", vm.output[0]);

    // ---- Program 2: Fibonacci - compute first 10 fib numbers ----
    // fib(0)=0, fib(1)=1, fib(n)=fib(n-1)+fib(n-2)
    // Iterative: a=0, b=1, print a, for i=1..9: t=a+b; a=b; b=t; print a
    vm_init(&vm);
    // var[0] = a = 0, var[1] = b = 1, var[2] = counter = 10
    vm_emit2(&vm, OP_PUSH, 0);
    vm_emit2(&vm, OP_STORE, 0);    // a = 0
    vm_emit2(&vm, OP_PUSH, 1);
    vm_emit2(&vm, OP_STORE, 1);    // b = 1
    vm_emit2(&vm, OP_PUSH, 10);
    vm_emit2(&vm, OP_STORE, 2);    // counter = 10
    // Loop start
    int fib_loop = vm.code_len;
    // Print a
    vm_emit2(&vm, OP_LOAD, 0);
    vm_emit( &vm, OP_PRINT);
    // t = a + b
    vm_emit2(&vm, OP_LOAD, 0);
    vm_emit2(&vm, OP_LOAD, 1);
    vm_emit( &vm, OP_ADD);
    vm_emit2(&vm, OP_STORE, 3);    // var[3] = t
    // a = b
    vm_emit2(&vm, OP_LOAD, 1);
    vm_emit2(&vm, OP_STORE, 0);
    // b = t
    vm_emit2(&vm, OP_LOAD, 3);
    vm_emit2(&vm, OP_STORE, 1);
    // counter--
    vm_emit2(&vm, OP_LOAD, 2);
    vm_emit2(&vm, OP_PUSH, 1);
    vm_emit( &vm, OP_SUB);
    vm_emit( &vm, OP_DUP);
    vm_emit2(&vm, OP_STORE, 2);
    // if counter > 0 (counter != 0), loop
    vm_emit2(&vm, OP_JNZ, fib_loop);
    vm_emit( &vm, OP_HALT);

    vm_run(&vm);
    printf("fib=");
    int i;
    for (i = 0; i < vm.out_count; i++) {
        if (i > 0) printf(",");
        printf("%d", vm.output[i]);
    }
    printf("\n");

    // ---- Program 3: Check if numbers are prime ----
    // For each number to check: trial division
    // Test: 7 (prime), 12 (not), 23 (prime), 1 (not), 2 (prime)
    // We'll run separate mini-programs for simplicity, storing result
    int test_nums[5];
    test_nums[0] = 7;
    test_nums[1] = 12;
    test_nums[2] = 23;
    test_nums[3] = 1;
    test_nums[4] = 2;
    int prime_results[5];
    int ti;

    for (ti = 0; ti < 5; ti++) {
        int n = test_nums[ti];
        vm_init(&vm);
        // var[0] = n, var[1] = divisor (starts at 2), var[2] = is_prime result
        vm_emit2(&vm, OP_PUSH, n);
        vm_emit2(&vm, OP_STORE, 0);
        vm_emit2(&vm, OP_PUSH, 2);
        vm_emit2(&vm, OP_STORE, 1);    // divisor = 2
        vm_emit2(&vm, OP_PUSH, 1);
        vm_emit2(&vm, OP_STORE, 2);    // assume prime

        // if n < 2: not prime
        vm_emit2(&vm, OP_LOAD, 0);
        vm_emit2(&vm, OP_PUSH, 2);
        vm_emit( &vm, OP_LT);          // n < 2?
        int not_prime_jmp = vm.code_len;
        vm_emit2(&vm, OP_JNZ, 0);      // patch: goto not_prime

        // Loop: while divisor * divisor <= n
        int prime_loop = vm.code_len;
        vm_emit2(&vm, OP_LOAD, 1);     // divisor
        vm_emit2(&vm, OP_LOAD, 1);     // divisor
        vm_emit( &vm, OP_MUL);         // divisor * divisor
        vm_emit2(&vm, OP_LOAD, 0);     // n
        // We want: if divisor*divisor > n, goto done (is prime)
        // stack: d*d, n. We check if n < d*d
        vm_emit( &vm, OP_SWAP);        // n, d*d
        vm_emit( &vm, OP_LT);          // n < d*d ?
        int is_prime_jmp = vm.code_len;
        vm_emit2(&vm, OP_JNZ, 0);      // patch: goto is_prime

        // Check n % divisor == 0
        vm_emit2(&vm, OP_LOAD, 0);     // n
        vm_emit2(&vm, OP_LOAD, 1);     // divisor
        vm_emit( &vm, OP_MOD);         // n % divisor
        int found_factor_jmp = vm.code_len;
        vm_emit2(&vm, OP_JZ, 0);       // patch: if 0, goto not_prime

        // divisor++
        vm_emit2(&vm, OP_LOAD, 1);
        vm_emit2(&vm, OP_PUSH, 1);
        vm_emit( &vm, OP_ADD);
        vm_emit2(&vm, OP_STORE, 1);
        vm_emit2(&vm, OP_JMP, prime_loop);

        // not_prime:
        int not_prime_pc = vm.code_len;
        vm.code[not_prime_jmp + 1] = not_prime_pc;
        vm.code[found_factor_jmp + 1] = not_prime_pc;
        vm_emit2(&vm, OP_PUSH, 0);
        vm_emit2(&vm, OP_STORE, 2);
        int done_jmp = vm.code_len;
        vm_emit2(&vm, OP_JMP, 0);      // patch: goto done

        // is_prime:
        int is_prime_pc = vm.code_len;
        vm.code[is_prime_jmp + 1] = is_prime_pc;
        // var[2] already 1

        // done:
        int done_pc = vm.code_len;
        vm.code[done_jmp + 1] = done_pc;
        vm_emit2(&vm, OP_LOAD, 2);
        vm_emit( &vm, OP_PRINT);
        vm_emit( &vm, OP_HALT);

        vm_run(&vm);
        prime_results[ti] = vm.output[0];
    }

    printf("prime_7=%d\n", prime_results[0]);
    printf("prime_12=%d\n", prime_results[1]);
    printf("prime_23=%d\n", prime_results[2]);
    printf("prime_1=%d\n", prime_results[3]);
    printf("prime_2=%d\n", prime_results[4]);

    // ---- Program 4: Stack manipulation test ----
    vm_init(&vm);
    // Test DUP, SWAP, OVER, ROT
    vm_emit2(&vm, OP_PUSH, 10);
    vm_emit( &vm, OP_DUP);
    vm_emit( &vm, OP_ADD);          // 10+10=20
    vm_emit( &vm, OP_PRINT);        // print 20

    vm_emit2(&vm, OP_PUSH, 3);
    vm_emit2(&vm, OP_PUSH, 7);
    vm_emit( &vm, OP_SWAP);         // 7, 3
    vm_emit( &vm, OP_SUB);          // 7-3=4
    vm_emit( &vm, OP_PRINT);        // print 4

    vm_emit2(&vm, OP_PUSH, 5);
    vm_emit2(&vm, OP_PUSH, 8);
    vm_emit( &vm, OP_OVER);         // 5, 8, 5
    vm_emit( &vm, OP_ADD);          // 5, 13
    vm_emit( &vm, OP_PRINT);        // print 13

    vm_emit2(&vm, OP_PUSH, 1);
    vm_emit2(&vm, OP_PUSH, 2);
    vm_emit2(&vm, OP_PUSH, 3);
    vm_emit( &vm, OP_ROT);          // (1,2,3) -> (2,3,1)
    vm_emit( &vm, OP_PRINT);        // print 1 (top after rot)
    vm_emit( &vm, OP_PRINT);        // print 3
    vm_emit( &vm, OP_PRINT);        // print 2

    vm_emit( &vm, OP_HALT);
    vm_run(&vm);

    printf("stack_dup_add=%d\n", vm.output[0]);
    printf("stack_swap_sub=%d\n", vm.output[1]);
    printf("stack_over_add=%d\n", vm.output[2]);
    printf("stack_rot_top=%d\n", vm.output[3]);
    printf("stack_rot_mid=%d\n", vm.output[4]);
    printf("stack_rot_bot=%d\n", vm.output[5]);

    return 0;
}
