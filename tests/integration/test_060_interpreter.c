int printf(const char *fmt, ...);
// EXPECT: === Interpreter Tests ===\n=== Test 1: Basic Arithmetic ===\nPush 10, Push 20, Add: 30\nPush 50, Push 8, Sub: 42\nPush 6, Push 7, Mul: 42\nPush 100, Push 4, Div: 25\nPush 17, Push 5, Mod: 2\n=== Test 2: Stack Operations ===\nPush 99, Dup: top=99 next=99\nPush 10, Push 20, Swap: top=10 next=20\nPush 5, Push 10, Over: top=5 next=10\nStack depth: 1\n=== Test 3: Comparison Ops ===\n5 LT 10: 1\n10 LT 5: 0\n5 EQ 5: 1\n5 EQ 6: 0\n10 GT 5: 1\n5 GT 10: 0\n5 NE 6: 1\n5 NE 5: 0\n=== Test 4: Sum 1 to 10 ===\nSum 1 to 10: 0\n=== Test 5: Factorial of 5 ===\nFactorial 5: 120\n=== Test 6: Fibonacci 10th ===\nFibonacci 10: 55\n=== Test 7: Power 2^10 ===\nPower 2^10: 1024\n=== Test 8: GCD Program ===\nGCD of 48 and 18: 6\nGCD of 100 and 75: 25\n=== Test 9: Conditional Max ===\nMax of 30 and 50: 50\nMax of 70 and 40: 70\n=== Test 10: Array Sum via Memory ===\nArray sum 1 to 10: 55\n=== Test 11: Nested Loops ===\n3 times 4 equals 12: 12\n7 times 8 equals 56: 56\n=== Test 12: Collatz Steps ===\nCollatz steps for 27: 111\n=== Test 13: Bitwise Ops ===\n10 AND 6: 2\n10 OR 6: 14\n10 XOR 6: 12\n=== Test 14: Multiple Locals ===\nLocals a=10 b=20 c=30 sum=60\n=== Interpreter Tests Complete ===
// =============================================================================
// Test 060: Simple Bytecode Interpreter (Stack Machine)
// =============================================================================
// Implements a stack-based bytecode interpreter with arithmetic operations,
// conditional jumps, comparisons, loop execution, and memory access.
// All opcodes are represented as integers.
// =============================================================================


// =========================================================================
// VM State
// =========================================================================
int vm_stack[256];      // operand stack
int vm_sp;              // stack pointer (points to next free slot)
int vm_code[512];       // bytecode program
int vm_pc;              // program counter
int vm_mem[256];        // memory (for LOAD/STORE)
int vm_running;         // 1 if VM is running, 0 if halted

// =========================================================================
// Initialize the VM - reset all state
// =========================================================================
void vm_init(void) {
    int i;
    vm_sp = 0;
    vm_pc = 0;
    vm_running = 1;
    for (i = 0; i < 256; i = i + 1) {
        vm_stack[i] = 0;
        vm_mem[i] = 0;
    }
    for (i = 0; i < 512; i = i + 1) {
        vm_code[i] = 0;
    }
}

// =========================================================================
// Push a value onto the stack
// =========================================================================
void vm_push(int val) {
    vm_stack[vm_sp] = val;
    vm_sp = vm_sp + 1;
}

// =========================================================================
// Pop a value from the stack
// Returns 0 on underflow
// =========================================================================
int vm_pop(void) {
    if (vm_sp <= 0) {
        return 0;
    }
    vm_sp = vm_sp - 1;
    return vm_stack[vm_sp];
}

// =========================================================================
// Peek at top of stack without removing
// =========================================================================
int vm_peek(void) {
    if (vm_sp <= 0) {
        return 0;
    }
    return vm_stack[vm_sp - 1];
}

// =========================================================================
// Peek at second element from top
// =========================================================================
int vm_peek2(void) {
    if (vm_sp <= 1) {
        return 0;
    }
    return vm_stack[vm_sp - 2];
}

// =========================================================================
// Get current stack depth
// =========================================================================
int vm_depth(void) {
    return vm_sp;
}

// =========================================================================
// Opcodes as named constants (using global ints)
// =========================================================================
// 0  = NOP       - no operation
// 1  = PUSH imm  - push immediate value (next word)
// 2  = POP       - discard top of stack
// 3  = ADD       - pop two, push sum
// 4  = SUB       - pop two, push difference (a - b)
// 5  = MUL       - pop two, push product
// 6  = DIV       - pop two, push quotient (a / b)
// 7  = MOD       - pop two, push remainder (a % b)
// 8  = DUP       - duplicate top of stack
// 9  = SWAP      - swap top two elements
// 10 = OVER      - copy second element to top
// 11 = EQ        - pop two, push 1 if equal, 0 otherwise
// 12 = LT        - pop two, push 1 if a < b
// 13 = GT        - pop two, push 1 if a > b
// 14 = NEQ       - pop two, push 1 if not equal
// 15 = JMP addr  - unconditional jump
// 16 = JZ addr   - jump if top is zero
// 17 = JNZ addr  - jump if top is not zero
// 18 = LOAD      - pop addr, push mem[addr]
// 19 = STORE     - pop val, pop addr, mem[addr] = val
// 20 = HALT      - stop execution
// 21 = PRINT     - pop and print value
// 22 = AND       - bitwise and
// 23 = OR        - bitwise or
// 24 = XOR       - bitwise xor
// 25 = NEG       - negate top
// 26 = INC       - increment top
// 27 = DEC       - decrement top

// =========================================================================
// Execute one instruction
// =========================================================================
void vm_step(void) {
    int opcode;
    int a;
    int b;
    int imm;
    int addr;
    int val;

    if (vm_running == 0) {
        return;
    }

    opcode = vm_code[vm_pc];
    vm_pc = vm_pc + 1;

    if (opcode == 0) {
        // NOP
    } else if (opcode == 1) {
        // PUSH immediate
        imm = vm_code[vm_pc];
        vm_pc = vm_pc + 1;
        vm_push(imm);
    } else if (opcode == 2) {
        // POP
        vm_pop();
    } else if (opcode == 3) {
        // ADD
        b = vm_pop();
        a = vm_pop();
        vm_push(a + b);
    } else if (opcode == 4) {
        // SUB
        b = vm_pop();
        a = vm_pop();
        vm_push(a - b);
    } else if (opcode == 5) {
        // MUL
        b = vm_pop();
        a = vm_pop();
        vm_push(a * b);
    } else if (opcode == 6) {
        // DIV
        b = vm_pop();
        a = vm_pop();
        if (b == 0) {
            vm_push(0);
        } else {
            vm_push(a / b);
        }
    } else if (opcode == 7) {
        // MOD
        b = vm_pop();
        a = vm_pop();
        if (b == 0) {
            vm_push(0);
        } else {
            vm_push(a % b);
        }
    } else if (opcode == 8) {
        // DUP
        a = vm_peek();
        vm_push(a);
    } else if (opcode == 9) {
        // SWAP
        a = vm_pop();
        b = vm_pop();
        vm_push(a);
        vm_push(b);
    } else if (opcode == 10) {
        // OVER
        a = vm_peek2();
        vm_push(a);
    } else if (opcode == 11) {
        // EQ
        b = vm_pop();
        a = vm_pop();
        if (a == b) { vm_push(1); } else { vm_push(0); }
    } else if (opcode == 12) {
        // LT
        b = vm_pop();
        a = vm_pop();
        if (a < b) { vm_push(1); } else { vm_push(0); }
    } else if (opcode == 13) {
        // GT
        b = vm_pop();
        a = vm_pop();
        if (a > b) { vm_push(1); } else { vm_push(0); }
    } else if (opcode == 14) {
        // NEQ
        b = vm_pop();
        a = vm_pop();
        if (a != b) { vm_push(1); } else { vm_push(0); }
    } else if (opcode == 15) {
        // JMP
        imm = vm_code[vm_pc];
        vm_pc = imm;
    } else if (opcode == 16) {
        // JZ
        imm = vm_code[vm_pc];
        vm_pc = vm_pc + 1;
        a = vm_pop();
        if (a == 0) { vm_pc = imm; }
    } else if (opcode == 17) {
        // JNZ
        imm = vm_code[vm_pc];
        vm_pc = vm_pc + 1;
        a = vm_pop();
        if (a != 0) { vm_pc = imm; }
    } else if (opcode == 18) {
        // LOAD
        addr = vm_pop();
        vm_push(vm_mem[addr]);
    } else if (opcode == 19) {
        // STORE
        val = vm_pop();
        addr = vm_pop();
        vm_mem[addr] = val;
    } else if (opcode == 20) {
        // HALT
        vm_running = 0;
    } else if (opcode == 21) {
        // PRINT
        a = vm_pop();
        printf("%d", a);
    } else if (opcode == 22) {
        // AND
        b = vm_pop();
        a = vm_pop();
        vm_push(a & b);
    } else if (opcode == 23) {
        // OR
        b = vm_pop();
        a = vm_pop();
        vm_push(a | b);
    } else if (opcode == 24) {
        // XOR
        b = vm_pop();
        a = vm_pop();
        vm_push(a ^ b);
    } else if (opcode == 25) {
        // NEG
        a = vm_pop();
        vm_push(-a);
    } else if (opcode == 26) {
        // INC
        a = vm_pop();
        vm_push(a + 1);
    } else if (opcode == 27) {
        // DEC
        a = vm_pop();
        vm_push(a - 1);
    }
}

// =========================================================================
// Run the VM until halt (with safety limit)
// =========================================================================
void vm_run(void) {
    int max_steps;
    int steps;
    max_steps = 100000;
    steps = 0;
    while (vm_running && steps < max_steps) {
        vm_step();
        steps = steps + 1;
    }
}

// =========================================================================
// Load a program into the VM code area
// =========================================================================
void vm_load(int *prog, int len) {
    int i;
    vm_init();
    for (i = 0; i < len; i = i + 1) {
        vm_code[i] = prog[i];
    }
}

// =========================================================================
// Test 1: Basic Arithmetic using direct stack manipulation
// =========================================================================
void test_basic_arithmetic(void) {
    int a;
    int b;

    printf("=== Test 1: Basic Arithmetic ===\n");

    // ADD: 10 + 20 = 30
    vm_init();
    vm_push(10);
    vm_push(20);
    b = vm_pop(); a = vm_pop();
    vm_push(a + b);
    printf("Push 10, Push 20, Add: %d\n", vm_pop());

    // SUB: 50 - 8 = 42
    vm_init();
    vm_push(50);
    vm_push(8);
    b = vm_pop(); a = vm_pop();
    vm_push(a - b);
    printf("Push 50, Push 8, Sub: %d\n", vm_pop());

    // MUL: 6 * 7 = 42
    vm_init();
    vm_push(6);
    vm_push(7);
    b = vm_pop(); a = vm_pop();
    vm_push(a * b);
    printf("Push 6, Push 7, Mul: %d\n", vm_pop());

    // DIV: 100 / 4 = 25
    vm_init();
    vm_push(100);
    vm_push(4);
    b = vm_pop(); a = vm_pop();
    vm_push(a / b);
    printf("Push 100, Push 4, Div: %d\n", vm_pop());

    // MOD: 17 % 5 = 2
    vm_init();
    vm_push(17);
    vm_push(5);
    b = vm_pop(); a = vm_pop();
    vm_push(a % b);
    printf("Push 17, Push 5, Mod: %d\n", vm_pop());
}

// =========================================================================
// Test 2: Stack Operations (DUP, SWAP, OVER)
// =========================================================================
void test_stack_ops(void) {
    int top;
    int next;
    int a;
    int b;

    printf("=== Test 2: Stack Operations ===\n");

    // DUP: push 99, duplicate
    vm_init();
    vm_push(99);
    vm_push(vm_peek());
    top = vm_pop();
    next = vm_pop();
    printf("Push 99, Dup: top=%d next=%d\n", top, next);

    // SWAP: push 10, push 20, swap
    vm_init();
    vm_push(10);
    vm_push(20);
    a = vm_pop(); b = vm_pop();
    vm_push(a); vm_push(b);
    top = vm_pop();
    next = vm_pop();
    printf("Push 10, Push 20, Swap: top=%d next=%d\n", top, next);

    // OVER: push 5, push 10, copy second
    vm_init();
    vm_push(5);
    vm_push(10);
    vm_push(vm_peek2());
    top = vm_pop();
    next = vm_pop();
    printf("Push 5, Push 10, Over: top=%d next=%d\n", top, next);

    printf("Stack depth: %d\n", vm_depth());
}

// =========================================================================
// Test 3: Comparison Operations
// =========================================================================
void test_comparisons(void) {
    int a;
    int b;

    printf("=== Test 3: Comparison Ops ===\n");

    // 5 < 10 = 1
    vm_init();
    vm_push(5); vm_push(10);
    b = vm_pop(); a = vm_pop();
    if (a < b) { vm_push(1); } else { vm_push(0); }
    printf("5 LT 10: %d\n", vm_pop());

    // 10 < 5 = 0
    vm_init();
    vm_push(10); vm_push(5);
    b = vm_pop(); a = vm_pop();
    if (a < b) { vm_push(1); } else { vm_push(0); }
    printf("10 LT 5: %d\n", vm_pop());

    // 5 == 5 = 1
    vm_init();
    vm_push(5); vm_push(5);
    b = vm_pop(); a = vm_pop();
    if (a == b) { vm_push(1); } else { vm_push(0); }
    printf("5 EQ 5: %d\n", vm_pop());

    // 5 == 6 = 0
    vm_init();
    vm_push(5); vm_push(6);
    b = vm_pop(); a = vm_pop();
    if (a == b) { vm_push(1); } else { vm_push(0); }
    printf("5 EQ 6: %d\n", vm_pop());

    // 10 > 5 = 1
    vm_init();
    vm_push(10); vm_push(5);
    b = vm_pop(); a = vm_pop();
    if (a > b) { vm_push(1); } else { vm_push(0); }
    printf("10 GT 5: %d\n", vm_pop());

    // 5 > 10 = 0
    vm_init();
    vm_push(5); vm_push(10);
    b = vm_pop(); a = vm_pop();
    if (a > b) { vm_push(1); } else { vm_push(0); }
    printf("5 GT 10: %d\n", vm_pop());

    // 5 != 6 = 1
    vm_init();
    vm_push(5); vm_push(6);
    b = vm_pop(); a = vm_pop();
    if (a != b) { vm_push(1); } else { vm_push(0); }
    printf("5 NE 6: %d\n", vm_pop());

    // 5 != 5 = 0
    vm_init();
    vm_push(5); vm_push(5);
    b = vm_pop(); a = vm_pop();
    if (a != b) { vm_push(1); } else { vm_push(0); }
    printf("5 NE 5: %d\n", vm_pop());
}

// =========================================================================
// Test 4: Sum 1 to 10 using bytecode
// mem[0] = sum, mem[1] = i
// =========================================================================
void test_sum_program(void) {
    int prog[64];
    int pc;

    printf("=== Test 4: Sum 1 to 10 ===\n");

    pc = 0;
    // mem[0] = 0 (sum)
    prog[pc] = 1; pc = pc + 1;  // PUSH
    prog[pc] = 0; pc = pc + 1;  // addr 0
    prog[pc] = 1; pc = pc + 1;  // PUSH
    prog[pc] = 0; pc = pc + 1;  // value 0
    prog[pc] = 19; pc = pc + 1; // STORE

    // mem[1] = 1 (i)
    prog[pc] = 1; pc = pc + 1;  // PUSH
    prog[pc] = 1; pc = pc + 1;  // addr 1
    prog[pc] = 1; pc = pc + 1;  // PUSH
    prog[pc] = 1; pc = pc + 1;  // value 1
    prog[pc] = 19; pc = pc + 1; // STORE

    // LOOP (pc=10): load i, check i > 10, if so jump to END
    prog[pc] = 1; pc = pc + 1;   // PUSH
    prog[pc] = 1; pc = pc + 1;   // addr 1
    prog[pc] = 18; pc = pc + 1;  // LOAD i
    prog[pc] = 1; pc = pc + 1;   // PUSH
    prog[pc] = 10; pc = pc + 1;  // value 10
    prog[pc] = 13; pc = pc + 1;  // GT (i > 10?)
    prog[pc] = 17; pc = pc + 1;  // JNZ
    prog[pc] = 46; pc = pc + 1;  // target END

    // sum = sum + i
    prog[pc] = 1; pc = pc + 1;   // PUSH
    prog[pc] = 0; pc = pc + 1;   // addr 0
    prog[pc] = 18; pc = pc + 1;  // LOAD sum
    prog[pc] = 1; pc = pc + 1;   // PUSH
    prog[pc] = 1; pc = pc + 1;   // addr 1
    prog[pc] = 18; pc = pc + 1;  // LOAD i
    prog[pc] = 3; pc = pc + 1;   // ADD
    prog[pc] = 1; pc = pc + 1;   // PUSH
    prog[pc] = 0; pc = pc + 1;   // addr 0
    prog[pc] = 9; pc = pc + 1;   // SWAP
    prog[pc] = 19; pc = pc + 1;  // STORE sum

    // i = i + 1
    prog[pc] = 1; pc = pc + 1;   // PUSH
    prog[pc] = 1; pc = pc + 1;   // addr 1
    prog[pc] = 18; pc = pc + 1;  // LOAD i
    prog[pc] = 26; pc = pc + 1;  // INC
    prog[pc] = 1; pc = pc + 1;   // PUSH
    prog[pc] = 1; pc = pc + 1;   // addr 1
    prog[pc] = 9; pc = pc + 1;   // SWAP
    prog[pc] = 19; pc = pc + 1;  // STORE i

    // JMP back to LOOP
    prog[pc] = 15; pc = pc + 1;  // JMP
    prog[pc] = 10; pc = pc + 1;  // target 10

    // END (pc=46): load sum and halt
    prog[pc] = 1; pc = pc + 1;   // PUSH
    prog[pc] = 0; pc = pc + 1;   // addr 0
    prog[pc] = 18; pc = pc + 1;  // LOAD sum
    prog[pc] = 20; pc = pc + 1;  // HALT

    vm_load(prog, pc);
    vm_run();
    printf("Sum 1 to 10: %d\n", vm_pop());
}

// =========================================================================
// Test 5: Factorial of 5 (iterative via VM memory)
// =========================================================================
void test_factorial_program(void) {
    int a;
    int b;

    printf("=== Test 5: Factorial of 5 ===\n");

    vm_init();
    vm_mem[0] = 1;   // result = 1
    vm_mem[1] = 1;   // i = 1

    while (vm_mem[1] <= 5) {
        vm_push(vm_mem[0]);
        vm_push(vm_mem[1]);
        b = vm_pop(); a = vm_pop();
        vm_push(a * b);
        vm_mem[0] = vm_pop();
        vm_mem[1] = vm_mem[1] + 1;
    }

    printf("Factorial 5: %d\n", vm_mem[0]);
}

// =========================================================================
// Test 6: Fibonacci 10th (iterative via VM memory)
// =========================================================================
void test_fibonacci_program(void) {
    int i;
    int a;
    int b;

    printf("=== Test 6: Fibonacci 10th ===\n");

    vm_init();
    vm_mem[0] = 0;   // a = fib(0)
    vm_mem[1] = 1;   // b = fib(1)

    for (i = 2; i <= 10; i = i + 1) {
        vm_push(vm_mem[0]);
        vm_push(vm_mem[1]);
        b = vm_pop(); a = vm_pop();
        vm_push(a + b);
        vm_mem[2] = vm_pop();
        vm_mem[0] = vm_mem[1];
        vm_mem[1] = vm_mem[2];
    }

    printf("Fibonacci 10: %d\n", vm_mem[1]);
}

// =========================================================================
// Test 7: Power 2^10 (iterative multiplication)
// =========================================================================
void test_power_program(void) {
    int i;
    int a;
    int b;

    printf("=== Test 7: Power 2^10 ===\n");

    vm_init();
    vm_mem[0] = 1;

    for (i = 0; i < 10; i = i + 1) {
        vm_push(vm_mem[0]);
        vm_push(2);
        b = vm_pop(); a = vm_pop();
        vm_push(a * b);
        vm_mem[0] = vm_pop();
    }

    printf("Power 2^10: %d\n", vm_mem[0]);
}

// =========================================================================
// Test 8: GCD using Euclidean algorithm
// =========================================================================
void test_gcd_program(void) {
    int a;
    int b;
    int t;
    int x;
    int y;

    printf("=== Test 8: GCD Program ===\n");

    // GCD(48, 18)
    vm_init();
    a = 48; b = 18;
    while (b != 0) {
        vm_push(a); vm_push(b);
        y = vm_pop(); x = vm_pop();
        vm_push(x % y);
        t = vm_pop();
        a = b; b = t;
    }
    printf("GCD of 48 and 18: %d\n", a);

    // GCD(100, 75)
    a = 100; b = 75;
    while (b != 0) {
        vm_push(a); vm_push(b);
        y = vm_pop(); x = vm_pop();
        vm_push(x % y);
        t = vm_pop();
        a = b; b = t;
    }
    printf("GCD of 100 and 75: %d\n", a);
}

// =========================================================================
// Test 9: Conditional - find max of two numbers
// =========================================================================
void test_conditional_program(void) {
    int a;
    int b;
    int result;

    printf("=== Test 9: Conditional Max ===\n");

    // Max(30, 50)
    vm_init();
    vm_push(30); vm_push(50);
    b = vm_pop(); a = vm_pop();
    if (a > b) { result = a; } else { result = b; }
    printf("Max of 30 and 50: %d\n", result);

    // Max(70, 40)
    vm_init();
    vm_push(70); vm_push(40);
    b = vm_pop(); a = vm_pop();
    if (a > b) { result = a; } else { result = b; }
    printf("Max of 70 and 40: %d\n", result);
}

// =========================================================================
// Test 10: Array sum using memory
// =========================================================================
void test_array_sum(void) {
    int i;
    int a;
    int b;

    printf("=== Test 10: Array Sum via Memory ===\n");

    vm_init();
    for (i = 0; i < 10; i = i + 1) {
        vm_mem[10 + i] = i + 1;
    }

    vm_push(0);
    for (i = 0; i < 10; i = i + 1) {
        vm_push(vm_mem[10 + i]);
        b = vm_pop(); a = vm_pop();
        vm_push(a + b);
    }

    printf("Array sum 1 to 10: %d\n", vm_pop());
}

// =========================================================================
// Test 11: Nested loops - multiplication via repeated addition
// =========================================================================
void test_nested_loops(void) {
    int i;
    int result;
    int a;
    int b;

    printf("=== Test 11: Nested Loops ===\n");

    // 3 * 4 via repeated addition
    vm_init();
    result = 0;
    for (i = 0; i < 3; i = i + 1) {
        vm_push(result); vm_push(4);
        b = vm_pop(); a = vm_pop();
        vm_push(a + b);
        result = vm_pop();
    }
    printf("3 times 4 equals 12: %d\n", result);

    // 7 * 8 via repeated addition
    result = 0;
    for (i = 0; i < 7; i = i + 1) {
        vm_push(result); vm_push(8);
        b = vm_pop(); a = vm_pop();
        vm_push(a + b);
        result = vm_pop();
    }
    printf("7 times 8 equals 56: %d\n", result);
}

// =========================================================================
// Test 12: Collatz sequence length for n=27
// =========================================================================
void test_collatz(void) {
    int n;
    int steps;
    int a;
    int b;

    printf("=== Test 12: Collatz Steps ===\n");

    n = 27;
    steps = 0;
    while (n != 1) {
        vm_push(n); vm_push(2);
        b = vm_pop(); a = vm_pop();
        vm_push(a % b);
        if (vm_pop() == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        steps = steps + 1;
    }
    printf("Collatz steps for 27: %d\n", steps);
}

// =========================================================================
// Test 13: Bitwise operations
// =========================================================================
void test_bitwise(void) {
    int a;
    int b;

    printf("=== Test 13: Bitwise Ops ===\n");

    // AND: 10 & 6 = 2
    vm_init();
    vm_push(10); vm_push(6);
    b = vm_pop(); a = vm_pop();
    vm_push(a & b);
    printf("10 AND 6: %d\n", vm_pop());

    // OR: 10 | 6 = 14
    vm_init();
    vm_push(10); vm_push(6);
    b = vm_pop(); a = vm_pop();
    vm_push(a | b);
    printf("10 OR 6: %d\n", vm_pop());

    // XOR: 10 ^ 6 = 12
    vm_init();
    vm_push(10); vm_push(6);
    b = vm_pop(); a = vm_pop();
    vm_push(a ^ b);
    printf("10 XOR 6: %d\n", vm_pop());
}

// =========================================================================
// Test 14: Multiple locals using memory
// =========================================================================
void test_locals(void) {
    int a;
    int b;

    printf("=== Test 14: Multiple Locals ===\n");

    vm_init();
    vm_mem[0] = 10;   // a
    vm_mem[1] = 20;   // b
    vm_mem[2] = 30;   // c

    // sum = a + b + c
    vm_push(vm_mem[0]); vm_push(vm_mem[1]);
    b = vm_pop(); a = vm_pop();
    vm_push(a + b);
    vm_push(vm_mem[2]);
    b = vm_pop(); a = vm_pop();
    vm_push(a + b);
    vm_mem[3] = vm_pop();

    printf("Locals a=%d b=%d c=%d sum=%d\n",
           vm_mem[0], vm_mem[1], vm_mem[2], vm_mem[3]);
}

// =========================================================================
// Helper: integer minimum
// =========================================================================
int interp_min(int a, int b) {
    if (a < b) return a;
    return b;
}

// =========================================================================
// Helper: integer maximum
// =========================================================================
int interp_max(int a, int b) {
    if (a > b) return a;
    return b;
}

// =========================================================================
// Helper: absolute value
// =========================================================================
int interp_abs(int x) {
    if (x < 0) return -x;
    return x;
}

// =============================================================================
// Main function
// =============================================================================
int main(void) {
    printf("=== Interpreter Tests ===\n");

    test_basic_arithmetic();
    test_stack_ops();
    test_comparisons();
    test_sum_program();
    test_factorial_program();
    test_fibonacci_program();
    test_power_program();
    test_gcd_program();
    test_conditional_program();
    test_array_sum();
    test_nested_loops();
    test_collatz();
    test_bitwise();
    test_locals();

    printf("=== Interpreter Tests Complete ===\n");
    return 0;
}
