int printf(const char *fmt, ...);
// EXPECT: Program 1 (load, add, print):\n15\n18\nfinal=18\nProgram 2 (multiply chain):\n6\n12\n11\nfinal=11\nProgram 3 (negate):\n-42\nfinal=-42\nProgram 4 (countdown with jump):\n5\n4\n3\n2\n1\nfinal=0\nDispatch table test:\nop0: 11\nop1: 9\nop2: 20\nop3: 5\nop4: 1\nop5: -10\nop6: 100\nop7: 0
int execute(int *prog, int len) {
    int pc = 0;
    int acc = 0;
    int running = 1;
    while (running && pc < len) {
        int instr = prog[pc];
        switch (instr) {
            case 0:
                running = 0;
                break;
            case 1:
                pc++;
                acc = prog[pc];
                break;
            case 2:
                pc++;
                acc = acc + prog[pc];
                break;
            case 3:
                pc++;
                acc = acc - prog[pc];
                break;
            case 4:
                pc++;
                acc = acc * prog[pc];
                break;
            case 5:
                printf("%d\n", acc);
                break;
            case 6:
                pc++;
                if (acc > 0) {
                    pc = prog[pc] - 1;
                }
                break;
            case 7:
                acc = -acc;
                break;
            default:
                running = 0;
                break;
        }
        pc++;
    }
    return acc;
}

int main(void) {
    printf("Program 1 (load, add, print):\n");
    int prog1[10];
    prog1[0] = 1; prog1[1] = 10;
    prog1[2] = 2; prog1[3] = 5;
    prog1[4] = 5;
    prog1[5] = 2; prog1[6] = 3;
    prog1[7] = 5;
    prog1[8] = 0;
    int r = execute(prog1, 9);
    printf("final=%d\n", r);

    printf("Program 2 (multiply chain):\n");
    int prog2[12];
    prog2[0] = 1; prog2[1] = 2;
    prog2[2] = 4; prog2[3] = 3;
    prog2[4] = 5;
    prog2[5] = 4; prog2[6] = 2;
    prog2[7] = 5;
    prog2[8] = 3; prog2[9] = 1;
    prog2[10] = 5;
    prog2[11] = 0;
    r = execute(prog2, 12);
    printf("final=%d\n", r);

    printf("Program 3 (negate):\n");
    int prog3[6];
    prog3[0] = 1; prog3[1] = 42;
    prog3[2] = 7;
    prog3[3] = 5;
    prog3[4] = 0;
    r = execute(prog3, 5);
    printf("final=%d\n", r);

    printf("Program 4 (countdown with jump):\n");
    int prog4[12];
    prog4[0] = 1; prog4[1] = 5;
    prog4[2] = 5;
    prog4[3] = 3; prog4[4] = 1;
    prog4[5] = 6; prog4[6] = 2;
    prog4[7] = 0;
    r = execute(prog4, 8);
    printf("final=%d\n", r);

    printf("Dispatch table test:\n");
    int ops[8];
    ops[0] = 0; ops[1] = 1; ops[2] = 2; ops[3] = 3;
    ops[4] = 4; ops[5] = 5; ops[6] = 6; ops[7] = 7;
    int i;
    for (i = 0; i < 8; i++) {
        int val = 10;
        switch (ops[i]) {
            case 0: val = val + 1; break;
            case 1: val = val - 1; break;
            case 2: val = val * 2; break;
            case 3: val = val / 2; break;
            case 4: val = val % 3; break;
            case 5: val = -val; break;
            case 6: val = val * val; break;
            case 7: val = 0; break;
        }
        printf("op%d: %d\n", i, val);
    }

    return 0;
}
