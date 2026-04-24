int printf(const char *fmt, ...);

enum StackOp {
    SM_PUSH,
    SM_POP,
    SM_ADD,
    SM_SUB,
    SM_MUL,
    SM_DIV,
    SM_MOD,
    SM_DUP,
    SM_SWAP,
    SM_OVER,
    SM_ROT,
    SM_PRINT,
    SM_HALT,
    SM_JMPZ,
    SM_JMP,
    SM_NEG
};

struct SMInst {
    int op;
    int operand;
};

struct StackMachine {
    int stack[64];
    int sp;
    int output[32];
    int out_count;
    int pc;
    int halted;
    int steps;
};

void sm_init(struct StackMachine *sm) {
    int i;
    for (i = 0; i < 64; i++) sm->stack[i] = 0;
    for (i = 0; i < 32; i++) sm->output[i] = 0;
    sm->sp = -1;
    sm->out_count = 0;
    sm->pc = 0;
    sm->halted = 0;
    sm->steps = 0;
}

void sm_push(struct StackMachine *sm, int val) {
    sm->sp = sm->sp + 1;
    sm->stack[sm->sp] = val;
}

int sm_pop(struct StackMachine *sm) {
    int val = sm->stack[sm->sp];
    sm->sp = sm->sp - 1;
    return val;
}

int sm_peek(struct StackMachine *sm) {
    return sm->stack[sm->sp];
}

void sm_run(struct StackMachine *sm, struct SMInst *prog, int plen) {
    int a, b, c;
    while (!sm->halted && sm->pc < plen && sm->steps < 5000) {
        struct SMInst *inst = &prog[sm->pc];
        sm->steps = sm->steps + 1;

        switch (inst->op) {
            case SM_PUSH:
                sm_push(sm, inst->operand);
                sm->pc = sm->pc + 1;
                break;
            case SM_POP:
                sm_pop(sm);
                sm->pc = sm->pc + 1;
                break;
            case SM_ADD:
                b = sm_pop(sm);
                a = sm_pop(sm);
                sm_push(sm, a + b);
                sm->pc = sm->pc + 1;
                break;
            case SM_SUB:
                b = sm_pop(sm);
                a = sm_pop(sm);
                sm_push(sm, a - b);
                sm->pc = sm->pc + 1;
                break;
            case SM_MUL:
                b = sm_pop(sm);
                a = sm_pop(sm);
                sm_push(sm, a * b);
                sm->pc = sm->pc + 1;
                break;
            case SM_DIV:
                b = sm_pop(sm);
                a = sm_pop(sm);
                if (b != 0) sm_push(sm, a / b);
                else sm_push(sm, 0);
                sm->pc = sm->pc + 1;
                break;
            case SM_MOD:
                b = sm_pop(sm);
                a = sm_pop(sm);
                if (b != 0) sm_push(sm, a % b);
                else sm_push(sm, 0);
                sm->pc = sm->pc + 1;
                break;
            case SM_DUP:
                a = sm_peek(sm);
                sm_push(sm, a);
                sm->pc = sm->pc + 1;
                break;
            case SM_SWAP:
                b = sm_pop(sm);
                a = sm_pop(sm);
                sm_push(sm, b);
                sm_push(sm, a);
                sm->pc = sm->pc + 1;
                break;
            case SM_OVER:
                a = sm->stack[sm->sp - 1];
                sm_push(sm, a);
                sm->pc = sm->pc + 1;
                break;
            case SM_ROT:
                c = sm_pop(sm);
                b = sm_pop(sm);
                a = sm_pop(sm);
                sm_push(sm, b);
                sm_push(sm, c);
                sm_push(sm, a);
                sm->pc = sm->pc + 1;
                break;
            case SM_PRINT:
                a = sm_pop(sm);
                if (sm->out_count < 32) {
                    sm->output[sm->out_count] = a;
                    sm->out_count = sm->out_count + 1;
                }
                sm->pc = sm->pc + 1;
                break;
            case SM_NEG:
                a = sm_pop(sm);
                sm_push(sm, -a);
                sm->pc = sm->pc + 1;
                break;
            case SM_JMPZ:
                a = sm_pop(sm);
                if (a == 0) sm->pc = inst->operand;
                else sm->pc = sm->pc + 1;
                break;
            case SM_JMP:
                sm->pc = inst->operand;
                break;
            case SM_HALT:
                sm->halted = 1;
                break;
        }
    }
}

void set_smi(struct SMInst *inst, int op, int operand) {
    inst->op = op;
    inst->operand = operand;
}

int main() {
    struct StackMachine sm;
    struct SMInst prog[32];
    int i;

    /* Test 1: Basic arithmetic: (3+4)*5 = 35 */
    sm_init(&sm);
    set_smi(&prog[0], SM_PUSH, 3);
    set_smi(&prog[1], SM_PUSH, 4);
    set_smi(&prog[2], SM_ADD, 0);
    set_smi(&prog[3], SM_PUSH, 5);
    set_smi(&prog[4], SM_MUL, 0);
    set_smi(&prog[5], SM_PRINT, 0);
    set_smi(&prog[6], SM_HALT, 0);
    sm_run(&sm, prog, 7);
    // EXPECT: Test 1: (3+4)*5 = 35
    printf("Test 1: (3+4)*5 = %d\n", sm.output[0]);

    /* Test 2: DUP and multiply: 7 DUP MUL = 49 */
    sm_init(&sm);
    set_smi(&prog[0], SM_PUSH, 7);
    set_smi(&prog[1], SM_DUP, 0);
    set_smi(&prog[2], SM_MUL, 0);
    set_smi(&prog[3], SM_PRINT, 0);
    set_smi(&prog[4], SM_HALT, 0);
    sm_run(&sm, prog, 5);
    // EXPECT: Test 2: 7*7 = 49
    printf("Test 2: 7*7 = %d\n", sm.output[0]);

    /* Test 3: SWAP test: push 10, push 20, SWAP, print both */
    sm_init(&sm);
    set_smi(&prog[0], SM_PUSH, 10);
    set_smi(&prog[1], SM_PUSH, 20);
    set_smi(&prog[2], SM_SWAP, 0);
    set_smi(&prog[3], SM_PRINT, 0);
    set_smi(&prog[4], SM_PRINT, 0);
    set_smi(&prog[5], SM_HALT, 0);
    sm_run(&sm, prog, 6);
    // EXPECT: Test 3: swap top=10 next=20
    printf("Test 3: swap top=%d next=%d\n", sm.output[0], sm.output[1]);

    /* Test 4: Countdown loop using JMPZ */
    /* push 5, [dup, print, push 1, sub, dup, jmpz end, jmp loop] */
    sm_init(&sm);
    set_smi(&prog[0], SM_PUSH, 5);         /* push 5 */
    /* loop (pc=1): */
    set_smi(&prog[1], SM_DUP, 0);          /* dup */
    set_smi(&prog[2], SM_PRINT, 0);        /* print */
    set_smi(&prog[3], SM_PUSH, 1);         /* push 1 */
    set_smi(&prog[4], SM_SUB, 0);          /* sub */
    set_smi(&prog[5], SM_DUP, 0);          /* dup */
    set_smi(&prog[6], SM_JMPZ, 8);         /* if 0, goto halt */
    set_smi(&prog[7], SM_JMP, 1);          /* goto loop */
    set_smi(&prog[8], SM_POP, 0);          /* pop leftover zero */
    set_smi(&prog[9], SM_HALT, 0);
    sm_run(&sm, prog, 10);

    // EXPECT: Countdown:
    printf("Countdown:\n");
    for (i = 0; i < sm.out_count; i++) {
        // EXPECT: 5
        // EXPECT: 4
        // EXPECT: 3
        // EXPECT: 2
        // EXPECT: 1
        printf("%d\n", sm.output[i]);
    }

    /* Test 5: Complex expression: (100 - 37) % 10 */
    sm_init(&sm);
    set_smi(&prog[0], SM_PUSH, 100);
    set_smi(&prog[1], SM_PUSH, 37);
    set_smi(&prog[2], SM_SUB, 0);
    set_smi(&prog[3], SM_PUSH, 10);
    set_smi(&prog[4], SM_MOD, 0);
    set_smi(&prog[5], SM_PRINT, 0);
    set_smi(&prog[6], SM_HALT, 0);
    sm_run(&sm, prog, 7);
    // EXPECT: Test 5: (100-37)%%10 = 3
    printf("Test 5: (100-37)%%%%10 = %d\n", sm.output[0]);

    /* Test 6: OVER test */
    sm_init(&sm);
    set_smi(&prog[0], SM_PUSH, 1);
    set_smi(&prog[1], SM_PUSH, 2);
    set_smi(&prog[2], SM_OVER, 0);
    set_smi(&prog[3], SM_PRINT, 0);   /* should print 1 (copied from under 2) */
    set_smi(&prog[4], SM_PRINT, 0);   /* should print 2 */
    set_smi(&prog[5], SM_PRINT, 0);   /* should print 1 */
    set_smi(&prog[6], SM_HALT, 0);
    sm_run(&sm, prog, 7);
    // EXPECT: Test 6: over=1 2 1 => 1 2 1
    printf("Test 6: over=%d %d %d => 1 2 1\n", sm.output[0], sm.output[1], sm.output[2]);

    // EXPECT: All stack machine tests done
    printf("All stack machine tests done\n");
    return 0;
}
