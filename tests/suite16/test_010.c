int printf(const char *fmt, ...);

// Multi-statement mini language interpreter
// Language: assignment, print, if-then, while-do, arithmetic
// Programs are represented as instruction arrays (bytecode)
// Each instruction: opcode, operands
//
// Opcodes:
//   SET var imm    - set variable to immediate
//   ADD var var var - var = var + var
//   SUB var var var
//   MUL var var var
//   LT  var var var - var = (var < var)
//   EQ  var var var - var = (var == var)
//   JZ  var addr   - jump if var == 0
//   JMP addr       - unconditional jump
//   PRT var        - print variable
//   HLT            - halt
//   SETI var var   - set var to value of another var
//   ADDI var var imm - var = var + imm
//   GT  var var var  - var = (var > var)
//   MOD var var var  - var = var % var

int vars[26]; // variables a-z mapped to 0-25
int prog[512];
int prog_len;

int I_SET;
int I_ADD;
int I_SUB;
int I_MUL;
int I_LT;
int I_EQ;
int I_JZ;
int I_JMP;
int I_PRT;
int I_HLT;
int I_SETI;
int I_ADDI;
int I_GT;
int I_MOD;

void init_ops(void) {
    I_SET  = 1;
    I_ADD  = 2;
    I_SUB  = 3;
    I_MUL  = 4;
    I_LT   = 5;
    I_EQ   = 6;
    I_JZ   = 7;
    I_JMP  = 8;
    I_PRT  = 9;
    I_HLT  = 10;
    I_SETI = 11;
    I_ADDI = 12;
    I_GT   = 13;
    I_MOD  = 14;
}

void reset(void) {
    int i;
    prog_len = 0;
    for (i = 0; i < 26; i = i + 1) vars[i] = 0;
}

void emit(int v) {
    prog[prog_len] = v;
    prog_len = prog_len + 1;
}

void execute(void) {
    int pc;
    int op;
    int d;
    int s1;
    int s2;
    int imm;

    pc = 0;
    while (pc < prog_len) {
        op = prog[pc];
        pc = pc + 1;
        if (op == I_SET) {
            d = prog[pc]; pc = pc + 1;
            imm = prog[pc]; pc = pc + 1;
            vars[d] = imm;
        } else if (op == I_ADD) {
            d = prog[pc]; pc = pc + 1;
            s1 = prog[pc]; pc = pc + 1;
            s2 = prog[pc]; pc = pc + 1;
            vars[d] = vars[s1] + vars[s2];
        } else if (op == I_SUB) {
            d = prog[pc]; pc = pc + 1;
            s1 = prog[pc]; pc = pc + 1;
            s2 = prog[pc]; pc = pc + 1;
            vars[d] = vars[s1] - vars[s2];
        } else if (op == I_MUL) {
            d = prog[pc]; pc = pc + 1;
            s1 = prog[pc]; pc = pc + 1;
            s2 = prog[pc]; pc = pc + 1;
            vars[d] = vars[s1] * vars[s2];
        } else if (op == I_LT) {
            d = prog[pc]; pc = pc + 1;
            s1 = prog[pc]; pc = pc + 1;
            s2 = prog[pc]; pc = pc + 1;
            vars[d] = (vars[s1] < vars[s2]);
        } else if (op == I_GT) {
            d = prog[pc]; pc = pc + 1;
            s1 = prog[pc]; pc = pc + 1;
            s2 = prog[pc]; pc = pc + 1;
            vars[d] = (vars[s1] > vars[s2]);
        } else if (op == I_EQ) {
            d = prog[pc]; pc = pc + 1;
            s1 = prog[pc]; pc = pc + 1;
            s2 = prog[pc]; pc = pc + 1;
            vars[d] = (vars[s1] == vars[s2]);
        } else if (op == I_MOD) {
            d = prog[pc]; pc = pc + 1;
            s1 = prog[pc]; pc = pc + 1;
            s2 = prog[pc]; pc = pc + 1;
            vars[d] = vars[s1] - (vars[s1] / vars[s2]) * vars[s2];
        } else if (op == I_JZ) {
            d = prog[pc]; pc = pc + 1;
            imm = prog[pc]; pc = pc + 1;
            if (vars[d] == 0) pc = imm;
        } else if (op == I_JMP) {
            imm = prog[pc]; pc = pc + 1;
            pc = imm;
        } else if (op == I_PRT) {
            d = prog[pc]; pc = pc + 1;
            printf("v%d = %d\n", d, vars[d]);
        } else if (op == I_SETI) {
            d = prog[pc]; pc = pc + 1;
            s1 = prog[pc]; pc = pc + 1;
            vars[d] = vars[s1];
        } else if (op == I_ADDI) {
            d = prog[pc]; pc = pc + 1;
            s1 = prog[pc]; pc = pc + 1;
            imm = prog[pc]; pc = pc + 1;
            vars[d] = vars[s1] + imm;
        } else if (op == I_HLT) {
            return;
        }
    }
}

int main(void) {
    init_ops();

    // Program 1: compute sum 1..10
    // a = counter = 1, b = sum = 0, c = limit = 11, d = temp for comparison
    reset();
    emit(I_SET); emit(0); emit(1);    // a = 1
    emit(I_SET); emit(1); emit(0);    // b = 0
    emit(I_SET); emit(2); emit(11);   // c = 11
    // Loop at pc=9
    emit(I_LT); emit(3); emit(0); emit(2);  // 9: d = (a < c)
    emit(I_JZ); emit(3); emit(24);          // 13: if d==0, jump to 24 (done)
    emit(I_ADD); emit(1); emit(1); emit(0);  // 16: b = b + a
    emit(I_ADDI); emit(0); emit(0); emit(1); // 20: a = a + 1
    emit(I_JMP); emit(9);                    // 24: jump to loop -- wait that's 24 too
    // Let me count carefully:
    // 0:SET 1:0 2:1       -> a=1
    // 3:SET 4:1 5:0       -> b=0
    // 6:SET 7:2 8:11      -> c=11
    // 9:LT 10:3 11:0 12:2   -> d = a < c
    // 13:JZ 14:3 15:??      -> if !d, jump to end
    // 16:ADD 17:1 18:1 19:0  -> b += a
    // 20:ADDI 21:0 22:0 23:1 -> a++
    // 24:JMP 25:9            -> loop
    // 26: PRT ...
    // JZ target should be 26
    reset();
    emit(I_SET); emit(0); emit(1);
    emit(I_SET); emit(1); emit(0);
    emit(I_SET); emit(2); emit(11);
    emit(I_LT); emit(3); emit(0); emit(2);
    emit(I_JZ); emit(3); emit(26);
    emit(I_ADD); emit(1); emit(1); emit(0);
    emit(I_ADDI); emit(0); emit(0); emit(1);
    emit(I_JMP); emit(9);
    // pc=26: done
    emit(I_PRT); emit(1);
    emit(I_HLT);
    execute();
    printf("sum 1..10 done\n"); // EXPECT: v1 = 55
    // EXPECT: sum 1..10 done

    // Program 2: compute fibonacci(10)
    // a = fib(n-2) = 0, b = fib(n-1) = 1, c = temp, d = counter = 2, e = limit = 10, f = cmp
    reset();
    emit(I_SET); emit(0); emit(0);    // a = 0
    emit(I_SET); emit(1); emit(1);    // b = 1
    emit(I_SET); emit(3); emit(2);    // d = counter = 2
    emit(I_SET); emit(4); emit(11);   // e = limit = 11
    // Loop:
    // 0:SET 1:0 2:0 = 3
    // 3:SET 4:1 5:1 = 3
    // 6:SET 7:3 8:2 = 3
    // 9:SET 10:4 11:11 = 3 -> pc=12
    // 12:LT 13:5 14:3 15:4 -> f = (d < e)
    // 16:JZ 17:5 18:?? -> if f==0 jump to end
    // 19:ADD 20:2 21:0 22:1 -> c = a + b
    // 23:SETI 24:0 25:1 -> a = b
    // 26:SETI 27:1 28:2 -> b = c
    // 29:ADDI 30:3 31:3 32:1 -> d++
    // 33:JMP 34:12 -> loop
    // 35: print
    emit(I_LT); emit(5); emit(3); emit(4);   // f = d < e
    emit(I_JZ); emit(5); emit(35);
    emit(I_ADD); emit(2); emit(0); emit(1);   // c = a + b
    emit(I_SETI); emit(0); emit(1);           // a = b
    emit(I_SETI); emit(1); emit(2);           // b = c
    emit(I_ADDI); emit(3); emit(3); emit(1);  // d++
    emit(I_JMP); emit(12);
    // pc=35:
    emit(I_PRT); emit(1);  // print fib(10)
    emit(I_HLT);
    execute();
    printf("fib(10) done\n"); // EXPECT: v1 = 55
    // EXPECT: fib(10) done

    // Program 3: compute GCD(48, 18) using Euclidean algorithm
    // a = 48, b = 18, c = temp (a mod b), d = zero-check
    reset();
    emit(I_SET); emit(0); emit(48);   // a = 48
    emit(I_SET); emit(1); emit(18);   // b = 18
    emit(I_SET); emit(3); emit(0);    // d = 0 (for comparison)
    // 0:SET 1:0 2:48 -> pc=3
    // 3:SET 4:1 5:18 -> pc=6
    // 6:SET 7:3 8:0  -> pc=9
    // Loop at 9:
    // 9:EQ 10:4 11:1 12:3 -> e = (b == 0)
    // but we use var index 4 for e... let me use different indices
    // a=0, b=1, c=2, zero=3, cmp=4
    emit(I_EQ); emit(4); emit(1); emit(3);   // 9: cmp = (b == 0)
    // 13:JZ skip, if cmp != 0 then b==0, done
    // Wait: if b==0 then cmp=1, and JZ would NOT jump (since cmp!=0)
    // We want: if b != 0 continue loop, else done
    // So: if cmp != 0 (b==0), jump to done
    // JZ jumps if var==0. We want jump if cmp!=0. So invert:
    // Or: check if b != 0 directly
    // Better: EQ sets cmp=1 if b==0. We want to stop if cmp==1.
    // Use: if cmp != 0, jump to end. But we only have JZ (jump if zero).
    // So: compute NOT of cmp: cmp = (b == 0) => notcmp = 1 - cmp? We don't have that.
    // Alternative: use GT. If b > 0, continue. If b == 0, stop.
    // GT: var = (b > zero) = (b > 0)
    emit(I_GT); emit(4); emit(1); emit(3);   // cmp = (b > 0)
    // wait, I already emitted EQ. Let me restart this program.
    reset();
    emit(I_SET); emit(0); emit(48);   // 0,1,2: a = 48
    emit(I_SET); emit(1); emit(18);   // 3,4,5: b = 18
    emit(I_SET); emit(3); emit(0);    // 6,7,8: zero = 0
    // Loop at 9:
    emit(I_GT); emit(4); emit(1); emit(3);   // 9,10,11,12: cmp = (b > 0)
    emit(I_JZ); emit(4); emit(26);           // 13,14,15: if b<=0 jump to done (26)
    emit(I_MOD); emit(2); emit(0); emit(1);  // 16,17,18,19: c = a mod b
    emit(I_SETI); emit(0); emit(1);          // 20,21,22: a = b
    emit(I_SETI); emit(1); emit(2);          // 23,24,25: b = c
    emit(I_JMP); emit(9);                    // 26,27: jump to 9 -- but 26 is our done target!
    // Fix: done target should be 28
    reset();
    emit(I_SET); emit(0); emit(48);   // 0,1,2
    emit(I_SET); emit(1); emit(18);   // 3,4,5
    emit(I_SET); emit(3); emit(0);    // 6,7,8
    emit(I_GT); emit(4); emit(1); emit(3);   // 9,10,11,12
    emit(I_JZ); emit(4); emit(28);           // 13,14,15
    emit(I_MOD); emit(2); emit(0); emit(1);  // 16,17,18,19
    emit(I_SETI); emit(0); emit(1);          // 20,21,22
    emit(I_SETI); emit(1); emit(2);          // 23,24,25
    emit(I_JMP); emit(9);                    // 26,27
    // pc=28: done, result in a
    emit(I_PRT); emit(0);
    emit(I_HLT);
    execute();
    printf("gcd(48,18) done\n"); // EXPECT: v0 = 6
    // EXPECT: gcd(48,18) done

    // Program 4: compute power 2^8 = 256
    // a = base = 2, b = exp = 8, c = result = 1, d = zero, e = cmp
    reset();
    emit(I_SET); emit(0); emit(2);    // a = 2
    emit(I_SET); emit(1); emit(8);    // b = 8 (exponent)
    emit(I_SET); emit(2); emit(1);    // c = 1 (result)
    emit(I_SET); emit(3); emit(0);    // d = 0
    // Loop at 12:
    emit(I_GT); emit(4); emit(1); emit(3);   // 12,13,14,15: cmp = (b > 0)
    emit(I_JZ); emit(4); emit(29);           // 16,17,18: if b<=0, done
    emit(I_MUL); emit(2); emit(2); emit(0);  // 19,20,21,22: c *= a
    emit(I_ADDI); emit(1); emit(1); emit(-1); // 23,24,25,26: b--
    emit(I_JMP); emit(12);                   // 27,28
    // pc=29: done
    emit(I_PRT); emit(2);
    emit(I_HLT);
    execute();
    printf("2^8 done\n"); // EXPECT: v2 = 256
    // EXPECT: 2^8 done

    printf("all interpreter tests passed\n"); // EXPECT: all interpreter tests passed
    return 0;
}
