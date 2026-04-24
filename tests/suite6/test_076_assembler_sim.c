int printf(const char *fmt, ...);

int OP_NOP = 0;
int OP_LOAD = 1;
int OP_ADD = 2;
int OP_SUB = 3;
int OP_MUL = 4;
int OP_STORE = 5;
int OP_PRINT = 6;
int OP_HALT = 7;
int OP_JMP = 8;
int OP_JZ = 9;
int OP_CMP = 10;
int OP_MOV = 11;

int code[128];
int code_len;

int regs[8];
int memory[64];
int zflag;

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int lookup_opcode(char *mnemonic) {
    if (str_eq(mnemonic, "nop")) return OP_NOP;
    if (str_eq(mnemonic, "load")) return OP_LOAD;
    if (str_eq(mnemonic, "add")) return OP_ADD;
    if (str_eq(mnemonic, "sub")) return OP_SUB;
    if (str_eq(mnemonic, "mul")) return OP_MUL;
    if (str_eq(mnemonic, "store")) return OP_STORE;
    if (str_eq(mnemonic, "print")) return OP_PRINT;
    if (str_eq(mnemonic, "halt")) return OP_HALT;
    if (str_eq(mnemonic, "jmp")) return OP_JMP;
    if (str_eq(mnemonic, "jz")) return OP_JZ;
    if (str_eq(mnemonic, "cmp")) return OP_CMP;
    if (str_eq(mnemonic, "mov")) return OP_MOV;
    return -1;
}

char *opcode_name(int op) {
    if (op == 0) return "nop";
    if (op == 1) return "load";
    if (op == 2) return "add";
    if (op == 3) return "sub";
    if (op == 4) return "mul";
    if (op == 5) return "store";
    if (op == 6) return "print";
    if (op == 7) return "halt";
    if (op == 8) return "jmp";
    if (op == 9) return "jz";
    if (op == 10) return "cmp";
    if (op == 11) return "mov";
    return "???";
}

void assemble3(char *mnemonic, int a, int b) {
    int op = lookup_opcode(mnemonic);
    code[code_len] = op;
    code[code_len + 1] = a;
    code[code_len + 2] = b;
    code_len = code_len + 3;
}

void reset(void) {
    code_len = 0;
    zflag = 0;
    int i = 0;
    while (i < 8) { regs[i] = 0; i = i + 1; }
    i = 0;
    while (i < 64) { memory[i] = 0; i = i + 1; }
}

int execute(void) {
    int pc = 0;
    int steps = 0;
    while (pc < code_len && steps < 500) {
        int op = code[pc];
        int a = code[pc + 1];
        int b = code[pc + 2];
        steps = steps + 1;

        if (op == OP_NOP) {
            pc = pc + 3;
        } else if (op == OP_LOAD) {
            regs[a] = b;
            pc = pc + 3;
        } else if (op == OP_ADD) {
            regs[a] = regs[a] + regs[b];
            pc = pc + 3;
        } else if (op == OP_SUB) {
            regs[a] = regs[a] - regs[b];
            pc = pc + 3;
        } else if (op == OP_MUL) {
            regs[a] = regs[a] * regs[b];
            pc = pc + 3;
        } else if (op == OP_STORE) {
            memory[a] = regs[b];
            pc = pc + 3;
        } else if (op == OP_PRINT) {
            printf("r%d=%d\n", a, regs[a]);
            pc = pc + 3;
        } else if (op == OP_HALT) {
            return steps;
        } else if (op == OP_JMP) {
            pc = a * 3;
        } else if (op == OP_JZ) {
            if (zflag) pc = a * 3; else pc = pc + 3;
        } else if (op == OP_CMP) {
            int diff = regs[a] - regs[b];
            zflag = (diff == 0);
            pc = pc + 3;
        } else if (op == OP_MOV) {
            regs[a] = regs[b];
            pc = pc + 3;
        } else {
            pc = pc + 3;
        }
    }
    return steps;
}

int main(void) {
    int op;

    op = lookup_opcode("load");
    // EXPECT: load=1
    printf("load=%d\n", op);

    op = lookup_opcode("add");
    // EXPECT: add=2
    printf("add=%d\n", op);

    op = lookup_opcode("halt");
    // EXPECT: halt=7
    printf("halt=%d\n", op);

    op = lookup_opcode("xyz");
    // EXPECT: unknown=-1
    printf("unknown=%d\n", op);

    // EXPECT: name_3=sub
    printf("name_3=%s\n", opcode_name(3));

    reset();
    assemble3("load", 0, 10);
    assemble3("load", 1, 20);
    assemble3("add", 0, 1);
    assemble3("print", 0, 0);
    assemble3("halt", 0, 0);
    // EXPECT: r0=30
    execute();

    reset();
    assemble3("load", 0, 50);
    assemble3("load", 1, 8);
    assemble3("sub", 0, 1);
    assemble3("print", 0, 0);
    assemble3("halt", 0, 0);
    // EXPECT: r0=42
    execute();

    reset();
    assemble3("load", 0, 6);
    assemble3("load", 1, 7);
    assemble3("mul", 0, 1);
    assemble3("print", 0, 0);
    assemble3("halt", 0, 0);
    // EXPECT: r0=42
    execute();

    reset();
    assemble3("load", 0, 99);
    assemble3("store", 5, 0);
    assemble3("halt", 0, 0);
    execute();
    // EXPECT: mem5=99
    printf("mem5=%d\n", memory[5]);

    reset();
    assemble3("load", 0, 5);
    assemble3("mov", 1, 0);
    assemble3("print", 1, 0);
    assemble3("halt", 0, 0);
    // EXPECT: r1=5
    execute();

    // EXPECT: code_size=12
    printf("code_size=%d\n", code_len);

    reset();
    assemble3("load", 0, 1);
    assemble3("load", 1, 10);
    assemble3("load", 2, 0);
    /* instr 3: cmp r2, r1 */
    assemble3("cmp", 2, 1);
    /* instr 4: jz to 7 (halt) */
    assemble3("jz", 7, 0);
    /* instr 5: add r0, r0 (dup value) */
    assemble3("add", 2, 0);
    /* instr 6: jmp to 3 */
    assemble3("jmp", 3, 0);
    /* instr 7: print and halt */
    assemble3("print", 2, 0);
    assemble3("halt", 0, 0);
    /* r0=1, loop 10 times adding 1 each time to r2: r2 = 10 */
    // EXPECT: r2=10
    execute();

    reset();
    assemble3("nop", 0, 0);
    assemble3("load", 0, 77);
    assemble3("print", 0, 0);
    assemble3("halt", 0, 0);
    // EXPECT: r0=77
    int steps = execute();
    // EXPECT: steps=4
    printf("steps=%d\n", steps);

    reset();
    assemble3("load", 0, 3);
    assemble3("load", 1, 3);
    assemble3("cmp", 0, 1);
    assemble3("halt", 0, 0);
    execute();
    // EXPECT: zflag=1
    printf("zflag=%d\n", zflag);

    return 0;
}
