int printf(const char *fmt, ...);

// Register-based expression machine
// 8 registers (R0-R7), instructions operate on registers
// Opcodes: LOADI, MOV, ADD, SUB, MUL, DIV, PRNT, HLT, CMP, BEQ, BNE

int regs[8];
int code[256];
int code_len;
int flag_eq; // comparison flag

int LOADI;  // LOADI Rd, imm  -> regs[Rd] = imm
int MOV;    // MOV Rd, Rs     -> regs[Rd] = regs[Rs]
int ADDR;   // ADD Rd, Rs, Rt -> regs[Rd] = regs[Rs] + regs[Rt]
int SUBR;   // SUB Rd, Rs, Rt -> regs[Rd] = regs[Rs] - regs[Rt]
int MULR;   // MUL Rd, Rs, Rt -> regs[Rd] = regs[Rs] * regs[Rt]
int DIVR;   // DIV Rd, Rs, Rt -> regs[Rd] = regs[Rs] / regs[Rt]
int PRNT;   // PRNT Rs        -> print regs[Rs]
int HLT;    // HLT            -> halt
int CMP;    // CMP Rs, Rt     -> set flag_eq = (Rs == Rt)
int BEQ;    // BEQ addr       -> if flag_eq, jump to addr
int BNE;    // BNE addr       -> if !flag_eq, jump to addr
int ADDI;   // ADDI Rd, Rs, imm -> regs[Rd] = regs[Rs] + imm

void init_ops(void) {
    LOADI = 1;
    MOV   = 2;
    ADDR  = 3;
    SUBR  = 4;
    MULR  = 5;
    DIVR  = 6;
    PRNT  = 7;
    HLT   = 8;
    CMP   = 9;
    BEQ   = 10;
    BNE   = 11;
    ADDI  = 12;
}

void reset(void) {
    int i;
    code_len = 0;
    flag_eq = 0;
    for (i = 0; i < 8; i = i + 1) regs[i] = 0;
}

void emit(int v) {
    code[code_len] = v;
    code_len = code_len + 1;
}

void run(void) {
    int pc;
    int op;
    int rd;
    int rs;
    int rt;
    int imm;

    pc = 0;
    while (pc < code_len) {
        op = code[pc];
        pc = pc + 1;
        if (op == LOADI) {
            rd = code[pc]; pc = pc + 1;
            imm = code[pc]; pc = pc + 1;
            regs[rd] = imm;
        } else if (op == MOV) {
            rd = code[pc]; pc = pc + 1;
            rs = code[pc]; pc = pc + 1;
            regs[rd] = regs[rs];
        } else if (op == ADDR) {
            rd = code[pc]; pc = pc + 1;
            rs = code[pc]; pc = pc + 1;
            rt = code[pc]; pc = pc + 1;
            regs[rd] = regs[rs] + regs[rt];
        } else if (op == SUBR) {
            rd = code[pc]; pc = pc + 1;
            rs = code[pc]; pc = pc + 1;
            rt = code[pc]; pc = pc + 1;
            regs[rd] = regs[rs] - regs[rt];
        } else if (op == MULR) {
            rd = code[pc]; pc = pc + 1;
            rs = code[pc]; pc = pc + 1;
            rt = code[pc]; pc = pc + 1;
            regs[rd] = regs[rs] * regs[rt];
        } else if (op == DIVR) {
            rd = code[pc]; pc = pc + 1;
            rs = code[pc]; pc = pc + 1;
            rt = code[pc]; pc = pc + 1;
            regs[rd] = regs[rs] / regs[rt];
        } else if (op == PRNT) {
            rs = code[pc]; pc = pc + 1;
            printf("R%d = %d\n", rs, regs[rs]);
        } else if (op == CMP) {
            rs = code[pc]; pc = pc + 1;
            rt = code[pc]; pc = pc + 1;
            flag_eq = (regs[rs] == regs[rt]);
        } else if (op == BEQ) {
            imm = code[pc]; pc = pc + 1;
            if (flag_eq) pc = imm;
        } else if (op == BNE) {
            imm = code[pc]; pc = pc + 1;
            if (!flag_eq) pc = imm;
        } else if (op == ADDI) {
            rd = code[pc]; pc = pc + 1;
            rs = code[pc]; pc = pc + 1;
            imm = code[pc]; pc = pc + 1;
            regs[rd] = regs[rs] + imm;
        } else if (op == HLT) {
            return;
        }
    }
}

int main(void) {
    init_ops();

    // Test 1: Simple arithmetic (3+4)*5 = 35
    reset();
    emit(LOADI); emit(0); emit(3);   // R0 = 3
    emit(LOADI); emit(1); emit(4);   // R1 = 4
    emit(ADDR);  emit(2); emit(0); emit(1); // R2 = R0 + R1 = 7
    emit(LOADI); emit(3); emit(5);   // R3 = 5
    emit(MULR);  emit(4); emit(2); emit(3); // R4 = R2 * R3 = 35
    emit(PRNT);  emit(4);
    emit(HLT);
    run();
    printf("test1 done\n"); // EXPECT: R4 = 35
    // EXPECT: test1 done

    // Test 2: Quadratic formula discriminant b^2 - 4ac
    // a=1, b=5, c=6 -> 25 - 24 = 1
    reset();
    emit(LOADI); emit(0); emit(1);   // R0 = a = 1
    emit(LOADI); emit(1); emit(5);   // R1 = b = 5
    emit(LOADI); emit(2); emit(6);   // R2 = c = 6
    emit(MULR);  emit(3); emit(1); emit(1); // R3 = b*b = 25
    emit(LOADI); emit(4); emit(4);   // R4 = 4
    emit(MULR);  emit(5); emit(4); emit(0); // R5 = 4*a = 4
    emit(MULR);  emit(5); emit(5); emit(2); // R5 = 4a*c = 24
    emit(SUBR);  emit(6); emit(3); emit(5); // R6 = b^2 - 4ac = 1
    emit(PRNT);  emit(6);
    emit(HLT);
    run();
    printf("test2 done\n"); // EXPECT: R6 = 1
    // EXPECT: test2 done

    // Test 3: Loop - compute factorial of 5 = 120
    // R0 = n = 5, R1 = result = 1, R2 = 1 (constant)
    // Loop: if R0 == 0, done. result *= n. n -= 1.
    reset();
    emit(LOADI); emit(0); emit(5);   // 0-2: R0 = 5 (n)
    emit(LOADI); emit(1); emit(1);   // 3-5: R1 = 1 (result)
    emit(LOADI); emit(2); emit(0);   // 6-8: R2 = 0 (zero for comparison)
    // pc=9: loop start
    emit(CMP);   emit(0); emit(2);   // 9-11: compare R0 with R2 (0)
    emit(BEQ);   emit(25);           // 12-13: if R0==0, jump to done
    emit(MULR);  emit(1); emit(1); emit(0); // 14-17: R1 = R1 * R0
    emit(ADDI);  emit(0); emit(0); emit(-1); // 18-21: R0 = R0 - 1
    emit(LOADI); emit(7); emit(9);   // 22-24: R7 = loop addr (hack to jump)
    // Actually just use a direct jump approach
    // Let me recount and go back to loop start
    reset();
    emit(LOADI); emit(0); emit(5);   // 0,1,2: R0 = n = 5
    emit(LOADI); emit(1); emit(1);   // 3,4,5: R1 = result = 1
    emit(LOADI); emit(2); emit(0);   // 6,7,8: R2 = 0
    // Loop at pc=9
    emit(CMP);   emit(0); emit(2);   // 9,10,11: R0 == 0?
    emit(BEQ);   emit(22);           // 12,13: if yes, jump to 22 (print)
    emit(MULR);  emit(1); emit(1); emit(0); // 14,15,16,17: R1 *= R0
    emit(ADDI);  emit(0); emit(0); emit(-1); // 18,19,20,21: R0--
    // Need to jump back to 9
    // We don't have a JMP instruction... use CMP+BNE trick
    // R2 is 0, R2 == R2 always true -> BEQ back? No, we just set flag from CMP R0,R2
    // Actually after ADDI, we need to go back. Let me add a JMP opcode...
    // Or reuse: compare something that's always not-equal
    // R1 is always >= 1 here, R2 is 0, so R1 != R2 -> BNE will go back
    emit(CMP);   emit(1); emit(2);   // 22,23,24: R1 != R2? (R1 is never 0)
    // Wait, but my BEQ target was 22, which now is CMP. Let me fix.
    reset();
    emit(LOADI); emit(0); emit(5);   // 0,1,2
    emit(LOADI); emit(1); emit(1);   // 3,4,5
    emit(LOADI); emit(2); emit(0);   // 6,7,8
    // pc=9: loop
    emit(CMP);   emit(0); emit(2);   // 9,10,11
    emit(BEQ);   emit(25);           // 12,13: done -> jump to 25
    emit(MULR);  emit(1); emit(1); emit(0); // 14,15,16,17
    emit(ADDI);  emit(0); emit(0); emit(-1); // 18,19,20,21
    emit(CMP);   emit(1); emit(2);   // 22,23,24: always not equal
    emit(BNE);   emit(9);            // 25,26: jump back to 9
    // done at 27... but BEQ target is 25 which is BNE...
    // BNE at 25 with operand at 26 = 9. If BEQ jumps to 25, it executes BNE.
    // At that point flag_eq is 1 (from CMP R0,R2 which was equal), so BNE won't jump. pc becomes 27.
    emit(PRNT);  emit(1);            // 27,28
    emit(HLT);                        // 29
    run();
    printf("test3 done\n"); // EXPECT: R1 = 120
    // EXPECT: test3 done

    // Test 4: Compute sum of squares 1^2 + 2^2 + 3^2 + 4^2 = 30
    reset();
    emit(LOADI); emit(0); emit(1);   // R0 = i = 1
    emit(LOADI); emit(1); emit(0);   // R1 = sum = 0
    emit(LOADI); emit(2); emit(5);   // R2 = limit = 5
    // Loop at 9
    emit(CMP);   emit(0); emit(2);   // 9,10,11: i == 5?
    emit(BEQ);   emit(27);           // 12,13: done
    emit(MULR);  emit(3); emit(0); emit(0); // 14,15,16,17: R3 = i*i
    emit(ADDR);  emit(1); emit(1); emit(3); // 18,19,20,21: R1 += R3
    emit(ADDI);  emit(0); emit(0); emit(1); // 22,23,24,25: R0++
    // Unconditional jump: compare R2 with R2 (always equal), BEQ back
    // Actually easier: just always compare and BNE won't work. Use CMP R2,R2 + BEQ
    // But that sets flag_eq=1 from CMP at top of loop...
    // Simpler: just duplicate the CMP and let it fall through
    // At pc=26 we need a jump back to 9. Let me use BNE trick:
    // After ADDI, R0 is incremented. As long as R0 != R2, loop.
    // Actually CMP R0,R2 at the top handles that. So we need unconditional jump.
    // Let me just compare two same regs:
    emit(CMP);   emit(3); emit(3);   // 26,27,28: always equal -- wait target is 27
    // Let me recount
    reset();
    emit(LOADI); emit(0); emit(1);   // 0,1,2
    emit(LOADI); emit(1); emit(0);   // 3,4,5
    emit(LOADI); emit(2); emit(5);   // 6,7,8
    emit(CMP);   emit(0); emit(2);   // 9,10,11
    emit(BEQ);   emit(26);           // 12,13
    emit(MULR);  emit(3); emit(0); emit(0); // 14,15,16,17
    emit(ADDR);  emit(1); emit(1); emit(3); // 18,19,20,21
    emit(ADDI);  emit(0); emit(0); emit(1); // 22,23,24,25
    // pc=26: need to jump back to 9
    // Use: CMP R3,R3 (always eq) then BEQ 9
    emit(CMP);   emit(3); emit(3);   // 26,27,28
    emit(BEQ);   emit(9);            // 29,30
    // BEQ target was 26, which is CMP R3,R3, which sets flag_eq=1, then BEQ 9 would loop forever
    // Need to fix: when we branch to "done", flag_eq was 1, so BEQ at 29 would go to 9. Infinite loop!
    // Solution: set BEQ target to 31 (after the BEQ 9)
    reset();
    emit(LOADI); emit(0); emit(1);   // 0,1,2
    emit(LOADI); emit(1); emit(0);   // 3,4,5
    emit(LOADI); emit(2); emit(5);   // 6,7,8
    emit(CMP);   emit(0); emit(2);   // 9,10,11
    emit(BEQ);   emit(31);           // 12,13: done -> 31
    emit(MULR);  emit(3); emit(0); emit(0); // 14,15,16,17
    emit(ADDR);  emit(1); emit(1); emit(3); // 18,19,20,21
    emit(ADDI);  emit(0); emit(0); emit(1); // 22,23,24,25
    emit(CMP);   emit(3); emit(3);   // 26,27,28
    emit(BEQ);   emit(9);            // 29,30
    emit(PRNT);  emit(1);            // 31,32
    emit(HLT);                        // 33
    run();
    printf("test4 done\n"); // EXPECT: R1 = 30
    // EXPECT: test4 done

    printf("all register machine tests passed\n"); // EXPECT: all register machine tests passed
    return 0;
}
