int printf(const char *fmt, ...);

// Stack machine with conditional jumps and comparison operators
// New opcodes: CMP_EQ, CMP_LT, CMP_GT, JMP, JZ, JNZ, MOD

int code[256];
int code_len;

int stack[64];
int sp;

void vm_reset(void) {
    code_len = 0;
    sp = 0;
}

void emit(int op) {
    code[code_len] = op;
    code_len = code_len + 1;
}

void vm_push(int val) {
    stack[sp] = val;
    sp = sp + 1;
}

int vm_pop(void) {
    sp = sp - 1;
    return stack[sp];
}

// Opcodes
int PUSH;
int ADD;
int SUB;
int MUL;
int DIV;
int MOD;
int CMP_EQ;
int CMP_LT;
int CMP_GT;
int JMP;
int JZ;
int JNZ;
int PRINT;
int HALT;
int DUP;
int LOAD;
int STORE;

int mem[26]; // simple memory cells 0-25

void init_ops(void) {
    PUSH   = 1;
    ADD    = 2;
    SUB    = 3;
    MUL    = 4;
    DIV    = 5;
    MOD    = 6;
    CMP_EQ = 7;
    CMP_LT = 8;
    CMP_GT = 9;
    JMP    = 10;
    JZ     = 11;
    JNZ    = 12;
    PRINT  = 13;
    HALT   = 14;
    DUP    = 15;
    LOAD   = 16;
    STORE  = 17;
}

int run(void) {
    int pc;
    int op;
    int a;
    int b;
    int addr;

    sp = 0;
    pc = 0;
    while (pc < code_len) {
        op = code[pc];
        pc = pc + 1;
        if (op == PUSH) {
            vm_push(code[pc]);
            pc = pc + 1;
        } else if (op == ADD) {
            b = vm_pop(); a = vm_pop();
            vm_push(a + b);
        } else if (op == SUB) {
            b = vm_pop(); a = vm_pop();
            vm_push(a - b);
        } else if (op == MUL) {
            b = vm_pop(); a = vm_pop();
            vm_push(a * b);
        } else if (op == DIV) {
            b = vm_pop(); a = vm_pop();
            vm_push(a / b);
        } else if (op == MOD) {
            b = vm_pop(); a = vm_pop();
            vm_push(a - (a / b) * b);
        } else if (op == CMP_EQ) {
            b = vm_pop(); a = vm_pop();
            vm_push(a == b);
        } else if (op == CMP_LT) {
            b = vm_pop(); a = vm_pop();
            vm_push(a < b);
        } else if (op == CMP_GT) {
            b = vm_pop(); a = vm_pop();
            vm_push(a > b);
        } else if (op == JMP) {
            pc = code[pc];
        } else if (op == JZ) {
            addr = code[pc];
            pc = pc + 1;
            a = vm_pop();
            if (a == 0) pc = addr;
        } else if (op == JNZ) {
            addr = code[pc];
            pc = pc + 1;
            a = vm_pop();
            if (a != 0) pc = addr;
        } else if (op == PRINT) {
            printf("vm: %d\n", vm_pop());
        } else if (op == DUP) {
            a = vm_pop();
            vm_push(a);
            vm_push(a);
        } else if (op == LOAD) {
            addr = vm_pop();
            vm_push(mem[addr]);
        } else if (op == STORE) {
            a = vm_pop();
            addr = vm_pop();
            mem[addr] = a;
        } else if (op == HALT) {
            return 0;
        }
    }
    return 0;
}

int main(void) {
    init_ops();

    // Test 1: Simple comparison
    vm_reset();
    emit(PUSH); emit(5);
    emit(PUSH); emit(5);
    emit(CMP_EQ);
    emit(PRINT);
    emit(HALT);
    run();
    printf("test1 done\n"); // EXPECT: vm: 1
    // EXPECT: test1 done

    // Test 2: Less than
    vm_reset();
    emit(PUSH); emit(3);
    emit(PUSH); emit(7);
    emit(CMP_LT);
    emit(PRINT);
    emit(HALT);
    run();
    printf("test2 done\n"); // EXPECT: vm: 1
    // EXPECT: test2 done

    // Test 3: Modulo
    vm_reset();
    emit(PUSH); emit(17);
    emit(PUSH); emit(5);
    emit(MOD);
    emit(PRINT);
    emit(HALT);
    run();
    printf("test3 done\n"); // EXPECT: vm: 2
    // EXPECT: test3 done

    // Test 4: Conditional jump (if 5==5 print 100, else print 200)
    vm_reset();
    emit(PUSH); emit(5);    // 0,1
    emit(PUSH); emit(5);    // 2,3
    emit(CMP_EQ);           // 4
    emit(JZ); emit(11);     // 5,6  if false, jump to 11
    emit(PUSH); emit(100);  // 7,8
    emit(PRINT);            // 9
    emit(JMP); emit(14);    // 10,11  jump to halt
    emit(PUSH); emit(200);  // 12,13 (actually 11,12 - recount)
    emit(PRINT);            // 14 (actually 13)
    emit(HALT);             // 15 (actually 14)
    // Let me recount: indices 0-14
    // 0:PUSH 1:5 2:PUSH 3:5 4:CMP_EQ 5:JZ 6:11 7:PUSH 8:100 9:PRINT 10:JMP 11:14 12:PUSH 13:200 14:PRINT 15:HALT
    // if equal (1), JZ won't jump -> push 100, print, jmp to 15 (HALT) -> but 14 is PRINT
    // Fix: JMP target should be 15
    vm_reset();
    emit(PUSH); emit(5);    // 0,1
    emit(PUSH); emit(5);    // 2,3
    emit(CMP_EQ);           // 4
    emit(JZ); emit(12);     // 5,6: if false jump to 12
    emit(PUSH); emit(100);  // 7,8
    emit(PRINT);            // 9
    emit(JMP); emit(15);    // 10,11: jump to HALT
    emit(PUSH); emit(200);  // 12,13
    emit(PRINT);            // 14
    emit(HALT);             // 15
    run();
    printf("test4 done\n"); // EXPECT: vm: 100
    // EXPECT: test4 done

    // Test 5: Loop - sum 1 to 5 using memory
    // mem[0] = counter (1), mem[1] = sum (0)
    // Loop: if counter > 5, break. sum += counter. counter += 1.
    vm_reset();
    // Initialize: mem[0]=1, mem[1]=0
    emit(PUSH); emit(0); emit(PUSH); emit(1); emit(STORE);  // 0-4: mem[0]=1
    emit(PUSH); emit(1); emit(PUSH); emit(0); emit(STORE);  // 5-9: mem[1]=0
    // Loop start at 10:
    // Load counter
    emit(PUSH); emit(0); emit(LOAD);  // 10-12: push mem[0] (counter)
    emit(PUSH); emit(5);              // 13,14: push 5
    emit(CMP_GT);                     // 15: counter > 5?
    emit(JNZ); emit(38);              // 16,17: if true, jump to end
    // sum += counter
    emit(PUSH); emit(1); emit(LOAD);  // 18-20: push mem[1] (sum)
    emit(PUSH); emit(0); emit(LOAD);  // 21-23: push mem[0] (counter)
    emit(ADD);                         // 24: sum + counter
    emit(PUSH); emit(1); emit(PUSH);  // 25-27
    // Need: push addr(1), push new_sum, store
    // Actually: stack has new_sum. Need to store to mem[1].
    // STORE pops: addr then value. So we need: push 1, push new_sum... no
    // STORE: a=pop (value), addr=pop (address). So: push addr, push value, store
    // We have new_sum on stack. Need to push 1 under it... use a different approach
    vm_reset();
    // mem[0]=counter=1, mem[1]=sum=0
    emit(PUSH); emit(0); emit(PUSH); emit(1); emit(STORE);  // 0-4
    emit(PUSH); emit(1); emit(PUSH); emit(0); emit(STORE);  // 5-9
    // Loop at 10:
    emit(PUSH); emit(0); emit(LOAD);  // 10-12: counter
    emit(PUSH); emit(5);              // 13-14
    emit(CMP_GT);                     // 15: counter > 5?
    emit(JNZ); emit(34);              // 16-17: jump to print if done
    // sum = sum + counter
    emit(PUSH); emit(1);              // 18-19: addr for sum
    emit(PUSH); emit(1); emit(LOAD);  // 20-22: old sum
    emit(PUSH); emit(0); emit(LOAD);  // 23-25: counter
    emit(ADD);                         // 26: new sum
    emit(STORE);                       // 27: mem[1] = new sum
    // counter = counter + 1
    emit(PUSH); emit(0);              // 28-29: addr for counter
    emit(PUSH); emit(0); emit(LOAD);  // 30-32: old counter
    emit(PUSH); emit(1); emit(ADD);   // 33-35: counter+1
    // Oops, that's already past 34. Let me recount more carefully.
    vm_reset();
    // 0: PUSH 0   -> 0,1
    // 2: PUSH 1   -> 2,3
    // 4: STORE    -> 4      mem[0]=1
    // 5: PUSH 1   -> 5,6
    // 7: PUSH 0   -> 7,8
    // 9: STORE    -> 9      mem[1]=0
    emit(PUSH); emit(0); emit(PUSH); emit(1); emit(STORE);
    emit(PUSH); emit(1); emit(PUSH); emit(0); emit(STORE);
    // Loop at pc=10
    // 10: PUSH 0   -> 10,11
    // 12: LOAD     -> 12     (counter on stack)
    // 13: PUSH 5   -> 13,14
    // 15: CMP_GT   -> 15     (counter > 5?)
    // 16: JNZ 36   -> 16,17  (if done, jump to print)
    emit(PUSH); emit(0); emit(LOAD);
    emit(PUSH); emit(5);
    emit(CMP_GT);
    emit(JNZ); emit(36);
    // sum = sum + counter
    // 18: PUSH 1   -> 18,19
    // 20: PUSH 1   -> 20,21
    // 22: LOAD     -> 22     (sum)
    // 23: PUSH 0   -> 23,24
    // 25: LOAD     -> 25     (counter)
    // 26: ADD      -> 26
    // 27: STORE    -> 27     mem[1] = sum+counter
    emit(PUSH); emit(1);
    emit(PUSH); emit(1); emit(LOAD);
    emit(PUSH); emit(0); emit(LOAD);
    emit(ADD);
    emit(STORE);
    // counter = counter + 1
    // 28: PUSH 0   -> 28,29
    // 30: PUSH 0   -> 30,31
    // 32: LOAD     -> 32     (counter)
    // 33: PUSH 1   -> 33,34
    // 35: ADD      -> 35     (counter+1)
    // 36: STORE    -> wait, 36 is our jump target for "done"
    // Need to fit store + jump before 36
    // Let me recalculate jump target
    emit(PUSH); emit(0);
    emit(PUSH); emit(0); emit(LOAD);
    emit(PUSH); emit(1);
    emit(ADD);
    emit(STORE);
    // 36: JMP 10   -> 36,37
    emit(JMP); emit(10);
    // 38: done - print sum
    // But JNZ target was 36... that would jump to JMP 10, creating infinite loop
    // Fix: JNZ should jump to 38
    // Let me redo with correct offsets
    vm_reset();
    emit(PUSH); emit(0); emit(PUSH); emit(1); emit(STORE);  // 0-4: mem[0]=1
    emit(PUSH); emit(1); emit(PUSH); emit(0); emit(STORE);  // 5-9: mem[1]=0
    // pc=10: loop start
    emit(PUSH); emit(0); emit(LOAD);    // 10,11,12
    emit(PUSH); emit(5); emit(CMP_GT);  // 13,14,15
    emit(JNZ); emit(38);                // 16,17
    emit(PUSH); emit(1);                // 18,19
    emit(PUSH); emit(1); emit(LOAD);    // 20,21,22
    emit(PUSH); emit(0); emit(LOAD);    // 23,24,25
    emit(ADD); emit(STORE);             // 26,27
    emit(PUSH); emit(0);                // 28,29
    emit(PUSH); emit(0); emit(LOAD);    // 30,31,32
    emit(PUSH); emit(1); emit(ADD);     // 33,34,35
    emit(STORE);                         // 36
    emit(JMP); emit(10);                // 37,38 -- oops 37 is JMP, 38 is 10
    // JNZ target 38 = operand of JMP (value 10), not an instruction!
    // Fix: JNZ should target 39
    vm_reset();
    emit(PUSH); emit(0); emit(PUSH); emit(1); emit(STORE);  // 0-4
    emit(PUSH); emit(1); emit(PUSH); emit(0); emit(STORE);  // 5-9
    emit(PUSH); emit(0); emit(LOAD);    // 10,11,12
    emit(PUSH); emit(5); emit(CMP_GT);  // 13,14,15
    emit(JNZ); emit(39);                // 16,17
    emit(PUSH); emit(1);                // 18,19
    emit(PUSH); emit(1); emit(LOAD);    // 20,21,22
    emit(PUSH); emit(0); emit(LOAD);    // 23,24,25
    emit(ADD); emit(STORE);             // 26,27
    emit(PUSH); emit(0);                // 28,29
    emit(PUSH); emit(0); emit(LOAD);    // 30,31,32
    emit(PUSH); emit(1); emit(ADD);     // 33,34,35
    emit(STORE);                         // 36
    emit(JMP); emit(10);                // 37,38
    // pc=39: print result
    emit(PUSH); emit(1); emit(LOAD);    // 39,40,41
    emit(PRINT);                         // 42
    emit(HALT);                          // 43
    run();
    printf("test5 done\n"); // EXPECT: vm: 15
    // EXPECT: test5 done

    printf("all jump tests passed\n"); // EXPECT: all jump tests passed
    return 0;
}
