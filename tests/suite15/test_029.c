int printf(const char *fmt, ...);

// Three-address code generation
// Translates simple expression trees into three-address instructions

enum TACOp {
    TAC_ASSIGN,     // t = val
    TAC_ADD,        // t = a + b
    TAC_SUB,        // t = a - b
    TAC_MUL,        // t = a * b
    TAC_DIV,        // t = a / b
    TAC_NEG,        // t = -a
    TAC_LABEL,      // label:
    TAC_GOTO,       // goto label
    TAC_IF_ZERO,    // if a == 0 goto label
    TAC_RETURN,     // return a
    TAC_PARAM,      // param a
    TAC_CALL        // t = call func, nargs
};

struct TACInst {
    int op;
    int dest;       // temp register
    int arg1;       // first operand (temp or const)
    int arg2;       // second operand (temp, const, or label)
    int is_const1;  // arg1 is immediate constant
    int is_const2;  // arg2 is immediate constant
};

struct TACProgram {
    struct TACInst code[128];
    int count;
    int next_temp;
    int next_label;
};

void tac_init(struct TACProgram *p) {
    p->count = 0;
    p->next_temp = 0;
    p->next_label = 0;
}

int new_temp(struct TACProgram *p) {
    int t = p->next_temp;
    p->next_temp = p->next_temp + 1;
    return t;
}

int new_label(struct TACProgram *p) {
    int l = p->next_label;
    p->next_label = p->next_label + 1;
    return l;
}

int emit_const_assign(struct TACProgram *p, int val) {
    int t = new_temp(p);
    int idx = p->count;
    p->code[idx].op = TAC_ASSIGN;
    p->code[idx].dest = t;
    p->code[idx].arg1 = val;
    p->code[idx].arg2 = 0;
    p->code[idx].is_const1 = 1;
    p->code[idx].is_const2 = 0;
    p->count = p->count + 1;
    return t;
}

int emit_binop(struct TACProgram *p, int op, int a, int b) {
    int t = new_temp(p);
    int idx = p->count;
    p->code[idx].op = op;
    p->code[idx].dest = t;
    p->code[idx].arg1 = a;
    p->code[idx].arg2 = b;
    p->code[idx].is_const1 = 0;
    p->code[idx].is_const2 = 0;
    p->count = p->count + 1;
    return t;
}

void emit_label(struct TACProgram *p, int label) {
    int idx = p->count;
    p->code[idx].op = TAC_LABEL;
    p->code[idx].dest = label;
    p->code[idx].arg1 = 0;
    p->code[idx].arg2 = 0;
    p->code[idx].is_const1 = 0;
    p->code[idx].is_const2 = 0;
    p->count = p->count + 1;
}

void emit_goto(struct TACProgram *p, int label) {
    int idx = p->count;
    p->code[idx].op = TAC_GOTO;
    p->code[idx].dest = 0;
    p->code[idx].arg1 = label;
    p->code[idx].arg2 = 0;
    p->code[idx].is_const1 = 0;
    p->code[idx].is_const2 = 0;
    p->count = p->count + 1;
}

void emit_if_zero(struct TACProgram *p, int temp, int label) {
    int idx = p->count;
    p->code[idx].op = TAC_IF_ZERO;
    p->code[idx].dest = 0;
    p->code[idx].arg1 = temp;
    p->code[idx].arg2 = label;
    p->code[idx].is_const1 = 0;
    p->code[idx].is_const2 = 0;
    p->count = p->count + 1;
}

void emit_return(struct TACProgram *p, int temp) {
    int idx = p->count;
    p->code[idx].op = TAC_RETURN;
    p->code[idx].dest = 0;
    p->code[idx].arg1 = temp;
    p->code[idx].arg2 = 0;
    p->code[idx].is_const1 = 0;
    p->code[idx].is_const2 = 0;
    p->count = p->count + 1;
}

void dump_tac(struct TACProgram *p) {
    int i;
    for (i = 0; i < p->count; i++) {
        struct TACInst *c = &p->code[i];
        if (c->op == TAC_ASSIGN) {
            printf("  t%d = %d\n", c->dest, c->arg1);
        } else if (c->op == TAC_ADD) {
            printf("  t%d = t%d + t%d\n", c->dest, c->arg1, c->arg2);
        } else if (c->op == TAC_SUB) {
            printf("  t%d = t%d - t%d\n", c->dest, c->arg1, c->arg2);
        } else if (c->op == TAC_MUL) {
            printf("  t%d = t%d * t%d\n", c->dest, c->arg1, c->arg2);
        } else if (c->op == TAC_DIV) {
            printf("  t%d = t%d / t%d\n", c->dest, c->arg1, c->arg2);
        } else if (c->op == TAC_LABEL) {
            printf("L%d:\n", c->dest);
        } else if (c->op == TAC_GOTO) {
            printf("  goto L%d\n", c->arg1);
        } else if (c->op == TAC_IF_ZERO) {
            printf("  if t%d == 0 goto L%d\n", c->arg1, c->arg2);
        } else if (c->op == TAC_RETURN) {
            printf("  return t%d\n", c->arg1);
        }
    }
}

// Simulate execution of TAC to verify correctness
int simulate_tac(struct TACProgram *p) {
    int regs[32];
    int i;
    for (i = 0; i < 32; i++) regs[i] = 0;

    i = 0;
    int steps = 0;
    while (i < p->count && steps < 200) {
        struct TACInst *c = &p->code[i];
        steps = steps + 1;

        if (c->op == TAC_ASSIGN) {
            regs[c->dest] = c->arg1;
            i = i + 1;
        } else if (c->op == TAC_ADD) {
            regs[c->dest] = regs[c->arg1] + regs[c->arg2];
            i = i + 1;
        } else if (c->op == TAC_SUB) {
            regs[c->dest] = regs[c->arg1] - regs[c->arg2];
            i = i + 1;
        } else if (c->op == TAC_MUL) {
            regs[c->dest] = regs[c->arg1] * regs[c->arg2];
            i = i + 1;
        } else if (c->op == TAC_DIV) {
            regs[c->dest] = regs[c->arg1] / regs[c->arg2];
            i = i + 1;
        } else if (c->op == TAC_LABEL) {
            i = i + 1;
        } else if (c->op == TAC_GOTO) {
            // Find label
            int target = c->arg1;
            int j;
            for (j = 0; j < p->count; j++) {
                if (p->code[j].op == TAC_LABEL && p->code[j].dest == target) {
                    i = j;
                    break;
                }
            }
        } else if (c->op == TAC_IF_ZERO) {
            if (regs[c->arg1] == 0) {
                int target = c->arg2;
                int j;
                for (j = 0; j < p->count; j++) {
                    if (p->code[j].op == TAC_LABEL && p->code[j].dest == target) {
                        i = j;
                        break;
                    }
                }
            } else {
                i = i + 1;
            }
        } else if (c->op == TAC_RETURN) {
            return regs[c->arg1];
        } else {
            i = i + 1;
        }
    }
    return -1;
}

int main() {
    struct TACProgram prog;

    // Test 1: Generate TAC for (3 + 4) * 2
    printf("=== TAC Test 1: (3+4)*2 ===\n");
    // EXPECT: === TAC Test 1: (3+4)*2 ===
    tac_init(&prog);
    int t0 = emit_const_assign(&prog, 3);
    int t1 = emit_const_assign(&prog, 4);
    int t2 = emit_binop(&prog, TAC_ADD, t0, t1);
    int t3 = emit_const_assign(&prog, 2);
    int t4 = emit_binop(&prog, TAC_MUL, t2, t3);
    emit_return(&prog, t4);

    dump_tac(&prog);
    // EXPECT:   t0 = 3
    // EXPECT:   t1 = 4
    // EXPECT:   t2 = t0 + t1
    // EXPECT:   t3 = 2
    // EXPECT:   t4 = t2 * t3
    // EXPECT:   return t4
    printf("result: %d\n", simulate_tac(&prog));
    // EXPECT: result: 14
    printf("instructions: %d\n", prog.count);
    // EXPECT: instructions: 6
    printf("temps used: %d\n", prog.next_temp);
    // EXPECT: temps used: 5

    // Test 2: Generate TAC for 100 / (5 - 3) + 8
    printf("=== TAC Test 2: 100/(5-3)+8 ===\n");
    // EXPECT: === TAC Test 2: 100/(5-3)+8 ===
    tac_init(&prog);
    t0 = emit_const_assign(&prog, 100);
    t1 = emit_const_assign(&prog, 5);
    t2 = emit_const_assign(&prog, 3);
    t3 = emit_binop(&prog, TAC_SUB, t1, t2);
    t4 = emit_binop(&prog, TAC_DIV, t0, t3);
    int t5 = emit_const_assign(&prog, 8);
    int t6 = emit_binop(&prog, TAC_ADD, t4, t5);
    emit_return(&prog, t6);

    printf("result: %d\n", simulate_tac(&prog));
    // EXPECT: result: 58
    printf("instructions: %d\n", prog.count);
    // EXPECT: instructions: 8

    // Test 3: Generate TAC with control flow (if x==0 goto)
    printf("=== TAC Test 3: Conditional ===\n");
    // EXPECT: === TAC Test 3: Conditional ===
    tac_init(&prog);
    // if (5-5 == 0) result = 100; else result = 200;
    t0 = emit_const_assign(&prog, 5);
    t1 = emit_const_assign(&prog, 5);
    t2 = emit_binop(&prog, TAC_SUB, t0, t1);  // t2 = 0
    int lbl_else = new_label(&prog);
    int lbl_end = new_label(&prog);
    emit_if_zero(&prog, t2, lbl_else);         // if t2==0 goto else (take the branch)
    // "then" branch: result = 200 (when NOT zero)
    t3 = emit_const_assign(&prog, 200);
    emit_goto(&prog, lbl_end);
    // "else" branch: result = 100 (when zero)
    emit_label(&prog, lbl_else);
    t4 = emit_const_assign(&prog, 100);
    emit_return(&prog, t4);                    // return 100
    emit_label(&prog, lbl_end);
    emit_return(&prog, t3);

    printf("result: %d\n", simulate_tac(&prog));
    // EXPECT: result: 100

    // Test 4: Unrolled sum 1+2+3+4+5
    printf("=== TAC Test 4: Unrolled sum ===\n");
    // EXPECT: === TAC Test 4: Unrolled sum ===
    tac_init(&prog);
    // sum = 1+2+3+4+5 via chain of adds
    int s0 = emit_const_assign(&prog, 1);   // t0 = 1
    int s1 = emit_const_assign(&prog, 2);   // t1 = 2
    int s2 = emit_binop(&prog, TAC_ADD, s0, s1); // t2 = 1+2 = 3
    int s3 = emit_const_assign(&prog, 3);   // t3 = 3
    int s4 = emit_binop(&prog, TAC_ADD, s2, s3); // t4 = 3+3 = 6
    int s5 = emit_const_assign(&prog, 4);   // t5 = 4
    int s6 = emit_binop(&prog, TAC_ADD, s4, s5); // t6 = 6+4 = 10
    int s7 = emit_const_assign(&prog, 5);   // t7 = 5
    int s8 = emit_binop(&prog, TAC_ADD, s6, s7); // t8 = 10+5 = 15
    emit_return(&prog, s8);

    printf("result: %d\n", simulate_tac(&prog));
    // EXPECT: result: 15
    printf("instructions: %d\n", prog.count);
    // EXPECT: instructions: 10

    // Test 5: Nested arithmetic
    printf("=== TAC Test 5: Nested ===\n");
    // EXPECT: === TAC Test 5: Nested ===
    tac_init(&prog);
    // ((2*3) + (4*5)) - ((6/3) + 1) = (6+20) - (2+1) = 26 - 3 = 23
    int a = emit_const_assign(&prog, 2);
    int b = emit_const_assign(&prog, 3);
    int c = emit_binop(&prog, TAC_MUL, a, b);
    int d = emit_const_assign(&prog, 4);
    int e = emit_const_assign(&prog, 5);
    int f = emit_binop(&prog, TAC_MUL, d, e);
    int g = emit_binop(&prog, TAC_ADD, c, f);
    int h = emit_const_assign(&prog, 6);
    int k = emit_const_assign(&prog, 3);
    int l = emit_binop(&prog, TAC_DIV, h, k);
    int m = emit_const_assign(&prog, 1);
    int n = emit_binop(&prog, TAC_ADD, l, m);
    int r = emit_binop(&prog, TAC_SUB, g, n);
    emit_return(&prog, r);

    printf("result: %d\n", simulate_tac(&prog));
    // EXPECT: result: 23
    printf("temps used: %d\n", prog.next_temp);
    // EXPECT: temps used: 13

    printf("done\n");
    // EXPECT: done
    return 0;
}
