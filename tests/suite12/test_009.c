int printf(const char *fmt, ...);

// Instruction selector
// Translates IR operations into target machine instructions
// Simulates a simple tiling/selection pass for a stack machine

int IR_CONST = 1;
int IR_ADD = 2;
int IR_SUB = 3;
int IR_MUL = 4;
int IR_LOAD = 5;
int IR_STORE = 6;
int IR_RET = 7;
int IR_CMP = 8;
int IR_BR = 9;
int IR_MOV = 10;

int MC_LOADI = 1;
int MC_ADD = 2;
int MC_SUB = 3;
int MC_MUL = 4;
int MC_LDR = 5;
int MC_STR = 6;
int MC_RET = 7;
int MC_CMP = 8;
int MC_BEQ = 9;
int MC_MOV = 10;
int MC_ADDI = 11;
int MC_SUBI = 12;
int MC_LSL = 13;

struct IRInst {
    int opcode;
    int dest;
    int src1;
    int src2;
    int imm;
};

struct MCInst {
    int opcode;
    int rd;
    int rs1;
    int rs2;
    int imm;
};

struct IRInst ir_code[64];
int ir_count;

struct MCInst mc_code[128];
int mc_count;

void emit_ir(int op, int d, int s1, int s2, int imm) {
    ir_code[ir_count].opcode = op;
    ir_code[ir_count].dest = d;
    ir_code[ir_count].src1 = s1;
    ir_code[ir_count].src2 = s2;
    ir_code[ir_count].imm = imm;
    ir_count = ir_count + 1;
}

void emit_mc(int op, int rd, int rs1, int rs2, int imm) {
    mc_code[mc_count].opcode = op;
    mc_code[mc_count].rd = rd;
    mc_code[mc_count].rs1 = rs1;
    mc_code[mc_count].rs2 = rs2;
    mc_code[mc_count].imm = imm;
    mc_count = mc_count + 1;
}

int is_power_of_2(int n) {
    if (n <= 0) return 0;
    return (n & (n - 1)) == 0;
}

int log2_of(int n) {
    int r = 0;
    while (n > 1) { n = n / 2; r = r + 1; }
    return r;
}

// Check if value fits in immediate field (12 bits)
int fits_imm12(int val) {
    return val >= 0 && val < 4096;
}

void select_instruction(int idx) {
    struct IRInst ir = ir_code[idx];

    if (ir.opcode == IR_CONST) {
        emit_mc(MC_LOADI, ir.dest, -1, -1, ir.imm);
        return;
    }

    if (ir.opcode == IR_ADD) {
        // Check for add-immediate pattern
        if (ir.src2 == -1 && fits_imm12(ir.imm)) {
            emit_mc(MC_ADDI, ir.dest, ir.src1, -1, ir.imm);
            return;
        }
        emit_mc(MC_ADD, ir.dest, ir.src1, ir.src2, 0);
        return;
    }

    if (ir.opcode == IR_SUB) {
        if (ir.src2 == -1 && fits_imm12(ir.imm)) {
            emit_mc(MC_SUBI, ir.dest, ir.src1, -1, ir.imm);
            return;
        }
        emit_mc(MC_SUB, ir.dest, ir.src1, ir.src2, 0);
        return;
    }

    if (ir.opcode == IR_MUL) {
        // Strength reduce: mul by power of 2 -> left shift
        if (ir.src2 == -1 && is_power_of_2(ir.imm)) {
            int shift = log2_of(ir.imm);
            emit_mc(MC_LSL, ir.dest, ir.src1, -1, shift);
            return;
        }
        emit_mc(MC_MUL, ir.dest, ir.src1, ir.src2, 0);
        return;
    }

    if (ir.opcode == IR_LOAD) {
        emit_mc(MC_LDR, ir.dest, ir.src1, -1, ir.imm);
        return;
    }

    if (ir.opcode == IR_STORE) {
        emit_mc(MC_STR, ir.src1, ir.src2, -1, ir.imm);
        return;
    }

    if (ir.opcode == IR_RET) {
        if (ir.src1 >= 0) {
            emit_mc(MC_MOV, 0, ir.src1, -1, 0);
        }
        emit_mc(MC_RET, -1, -1, -1, 0);
        return;
    }

    if (ir.opcode == IR_CMP) {
        emit_mc(MC_CMP, -1, ir.src1, ir.src2, 0);
        return;
    }

    if (ir.opcode == IR_BR) {
        emit_mc(MC_BEQ, -1, -1, -1, ir.imm);
        return;
    }

    if (ir.opcode == IR_MOV) {
        emit_mc(MC_MOV, ir.dest, ir.src1, -1, 0);
        return;
    }
}

void run_isel() {
    mc_count = 0;
    int i = 0;
    while (i < ir_count) {
        select_instruction(i);
        i = i + 1;
    }
}

char *mc_name(int op) {
    if (op == MC_LOADI) return "LOADI";
    if (op == MC_ADD) return "ADD";
    if (op == MC_SUB) return "SUB";
    if (op == MC_MUL) return "MUL";
    if (op == MC_LDR) return "LDR";
    if (op == MC_STR) return "STR";
    if (op == MC_RET) return "RET";
    if (op == MC_CMP) return "CMP";
    if (op == MC_BEQ) return "BEQ";
    if (op == MC_MOV) return "MOV";
    if (op == MC_ADDI) return "ADDI";
    if (op == MC_SUBI) return "SUBI";
    if (op == MC_LSL) return "LSL";
    return "???";
}

int main() {
    ir_count = 0;

    // r0 = 42
    emit_ir(IR_CONST, 0, -1, -1, 42);
    // r1 = 10
    emit_ir(IR_CONST, 1, -1, -1, 10);
    // r2 = r0 + r1
    emit_ir(IR_ADD, 2, 0, 1, 0);
    // r3 = r2 + 5 (immediate)
    emit_ir(IR_ADD, 3, 2, -1, 5);
    // r4 = r3 * 8 (power of 2, should become shift)
    emit_ir(IR_MUL, 4, 3, -1, 8);
    // r5 = r4 - 1 (immediate)
    emit_ir(IR_SUB, 5, 4, -1, 1);
    // store r5 at [r0 + 16]
    emit_ir(IR_STORE, -1, 5, 0, 16);
    // ret r5
    emit_ir(IR_RET, -1, 5, -1, 0);

    run_isel();

    printf("mc count: %d\n", mc_count);
    // EXPECT: mc count: 9

    int i = 0;
    while (i < mc_count) {
        printf("mc[%d]: %s", i, mc_name(mc_code[i].opcode));
        if (mc_code[i].rd >= 0) printf(" r%d", mc_code[i].rd);
        if (mc_code[i].rs1 >= 0) printf(" r%d", mc_code[i].rs1);
        if (mc_code[i].rs2 >= 0) printf(" r%d", mc_code[i].rs2);
        if (mc_code[i].imm != 0) printf(" #%d", mc_code[i].imm);
        printf("\n");
        i = i + 1;
    }
    // EXPECT: mc[0]: LOADI r0 #42
    // EXPECT: mc[1]: LOADI r1 #10
    // EXPECT: mc[2]: ADD r2 r0 r1
    // EXPECT: mc[3]: ADDI r3 r2 #5
    // EXPECT: mc[4]: LSL r4 r3 #3
    // EXPECT: mc[5]: SUBI r5 r4 #1
    // EXPECT: mc[6]: STR r5 r0 #16
    // EXPECT: mc[7]: MOV r0 r5
    // EXPECT: mc[8]: RET

    printf("power2 check 8: %d\n", is_power_of_2(8));
    // EXPECT: power2 check 8: 1
    printf("power2 check 7: %d\n", is_power_of_2(7));
    // EXPECT: power2 check 7: 0
    printf("log2(8): %d\n", log2_of(8));
    // EXPECT: log2(8): 3
    printf("log2(16): %d\n", log2_of(16));
    // EXPECT: log2(16): 4

    printf("isel done\n");
    // EXPECT: isel done

    return 0;
}
