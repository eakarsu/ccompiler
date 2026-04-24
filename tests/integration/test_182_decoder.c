int printf(const char *fmt, ...);
// EXPECT: Opcode decode:\ninstr=0x000 opcode=0 decoded=0\ninstr=0x351 opcode=1 decoded=53\ninstr=0x4A2 opcode=2 decoded=74\ninstr=0x073 opcode=3 decoded=7\ninstr=0xB04 opcode=4 decoded=11\ninstr=0xFF5 opcode=5 decoded=255\ninstr=0x126 opcode=6 decoded=-1\ninstr=0x0FF opcode=15 decoded=-1\nInstruction execution:\nop=1 args=(5,3) result=8\nop=2 args=(2,3) result=-1\nop=3 args=(3,2) result=6\nop=4 args=(3,2) result=1\nop=5 args=(6,3) result=2\nop=6 args=(7,3) result=7\nop=7 args=(5,3) result=6\nop=0 args=(0,0) result=0\nop=8 args=(2,2) result=-999\nop=1 args=(4,4) result=8\nRegister machine:\nr0=0 r1=1 r2=0 r3=0\nr0=0 r1=1 r2=2 r3=0\nr0=0 r1=3 r2=2 r3=0\nr0=4 r1=3 r2=2 r3=0\nr0=4 r1=7 r2=2 r3=0\nr0=12 r1=7 r2=2 r3=0
int decode_opcode(int instr) {
    int opcode = instr & 0xF;
    int result;
    switch (opcode) {
        case 0: result = 0; break;
        case 1: result = (instr >> 4) & 0xFF; break;
        case 2: result = (instr >> 4) & 0xFF; break;
        case 3: result = (instr >> 4) & 0xF; break;
        case 4: result = (instr >> 8) & 0xF; break;
        case 5: result = instr >> 4; break;
        default: result = -1; break;
    }
    return result;
}

int decode_instruction(int instr) {
    int opcode = instr & 0xF;
    int arg1 = (instr >> 4) & 0xF;
    int arg2 = (instr >> 8) & 0xF;
    int result = 0;

    switch (opcode) {
        case 0:
            result = 0;
            break;
        case 1:
            result = arg1 + arg2;
            break;
        case 2:
            result = arg1 - arg2;
            break;
        case 3:
            result = arg1 * arg2;
            break;
        case 4:
            if (arg2 != 0) result = arg1 / arg2;
            else result = -1;
            break;
        case 5:
            result = arg1 & arg2;
            break;
        case 6:
            result = arg1 | arg2;
            break;
        case 7:
            result = arg1 ^ arg2;
            break;
        default:
            result = -999;
            break;
    }
    return result;
}

int main(void) {
    printf("Opcode decode:\n");
    int instrs[8];
    instrs[0] = 0x000;
    instrs[1] = 0x351;
    instrs[2] = 0x4A2;
    instrs[3] = 0x073;
    instrs[4] = 0xB04;
    instrs[5] = 0xFF5;
    instrs[6] = 0x126;
    instrs[7] = 0x0FF;
    int i;
    for (i = 0; i < 8; i++) {
        printf("instr=0x%03X opcode=%d decoded=%d\n",
               instrs[i], instrs[i] & 0xF, decode_opcode(instrs[i]));
    }

    printf("Instruction execution:\n");
    int prog[10];
    prog[0] = 0x351;
    prog[1] = 0x322;
    prog[2] = 0x233;
    prog[3] = 0x234;
    prog[4] = 0x365;
    prog[5] = 0x376;
    prog[6] = 0x357;
    prog[7] = 0x000;
    prog[8] = 0x228;
    prog[9] = 0x441;
    for (i = 0; i < 10; i++) {
        int op = prog[i] & 0xF;
        int a1 = (prog[i] >> 4) & 0xF;
        int a2 = (prog[i] >> 8) & 0xF;
        int res = decode_instruction(prog[i]);
        printf("op=%d args=(%d,%d) result=%d\n", op, a1, a2, res);
    }

    printf("Register machine:\n");
    int regs[4];
    regs[0] = 0; regs[1] = 0; regs[2] = 0; regs[3] = 0;
    int rprog[6];
    rprog[0] = 0x011;
    rprog[1] = 0x121;
    rprog[2] = 0x013;
    rprog[3] = 0x101;
    rprog[4] = 0x013;
    rprog[5] = 0x101;
    for (i = 0; i < 6; i++) {
        int op = rprog[i] & 0xF;
        int dst = (rprog[i] >> 4) & 0xF;
        int src = (rprog[i] >> 8) & 0xF;
        switch (op) {
            case 1: regs[dst] = regs[dst] + regs[src] + 1; break;
            case 2: regs[dst] = regs[dst] - regs[src]; break;
            case 3: regs[dst] = regs[dst] * 2 + 1; break;
        }
        printf("r0=%d r1=%d r2=%d r3=%d\n", regs[0], regs[1], regs[2], regs[3]);
    }

    return 0;
}
