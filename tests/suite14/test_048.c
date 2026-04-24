int printf(const char *fmt, ...);

// RPN calculator with memory registers and undo stack

typedef struct {
    int stack[32];
    int sp;
    int memory[10];
    int undo_stack[64];
    int undo_sp_stack[64];
    int undo_top;
} RPNCalc;

void rpn_init(RPNCalc *c) {
    c->sp = 0;
    c->undo_top = 0;
    int i = 0;
    while (i < 32) { c->stack[i] = 0; i = i + 1; }
    i = 0;
    while (i < 10) { c->memory[i] = 0; i = i + 1; }
    i = 0;
    while (i < 64) {
        c->undo_stack[i] = 0;
        c->undo_sp_stack[i] = 0;
        i = i + 1;
    }
}

void rpn_save_state(RPNCalc *c) {
    // Save sp and top of stack for undo
    c->undo_sp_stack[c->undo_top] = c->sp;
    if (c->sp > 0) {
        c->undo_stack[c->undo_top] = c->stack[c->sp - 1];
    } else {
        c->undo_stack[c->undo_top] = 0;
    }
    c->undo_top = c->undo_top + 1;
}

void rpn_push(RPNCalc *c, int val) {
    rpn_save_state(c);
    c->stack[c->sp] = val;
    c->sp = c->sp + 1;
}

int rpn_pop(RPNCalc *c) {
    rpn_save_state(c);
    c->sp = c->sp - 1;
    return c->stack[c->sp];
}

int rpn_peek(RPNCalc *c) {
    return c->stack[c->sp - 1];
}

void rpn_add(RPNCalc *c) {
    int b = rpn_pop(c);
    int a = rpn_pop(c);
    rpn_push(c, a + b);
}

void rpn_sub(RPNCalc *c) {
    int b = rpn_pop(c);
    int a = rpn_pop(c);
    rpn_push(c, a - b);
}

void rpn_mul(RPNCalc *c) {
    int b = rpn_pop(c);
    int a = rpn_pop(c);
    rpn_push(c, a * b);
}

void rpn_div(RPNCalc *c) {
    int b = rpn_pop(c);
    int a = rpn_pop(c);
    rpn_push(c, a / b);
}

void rpn_dup(RPNCalc *c) {
    int a = rpn_peek(c);
    rpn_push(c, a);
}

void rpn_swap(RPNCalc *c) {
    rpn_save_state(c);
    int tmp = c->stack[c->sp - 1];
    c->stack[c->sp - 1] = c->stack[c->sp - 2];
    c->stack[c->sp - 2] = tmp;
}

void rpn_store(RPNCalc *c, int reg) {
    c->memory[reg] = rpn_peek(c);
}

void rpn_recall(RPNCalc *c, int reg) {
    rpn_push(c, c->memory[reg]);
}

void rpn_negate(RPNCalc *c) {
    rpn_save_state(c);
    c->stack[c->sp - 1] = -c->stack[c->sp - 1];
}

void rpn_abs(RPNCalc *c) {
    rpn_save_state(c);
    int val = c->stack[c->sp - 1];
    if (val < 0) c->stack[c->sp - 1] = -val;
}

void rpn_min(RPNCalc *c) {
    int b = rpn_pop(c);
    int a = rpn_pop(c);
    if (a < b) rpn_push(c, a);
    else rpn_push(c, b);
}

void rpn_max(RPNCalc *c) {
    int b = rpn_pop(c);
    int a = rpn_pop(c);
    if (a > b) rpn_push(c, a);
    else rpn_push(c, b);
}

int main(void) {
    RPNCalc c;
    int result;

    // Test 1: Basic addition
    rpn_init(&c);
    rpn_push(&c, 3);
    rpn_push(&c, 4);
    rpn_add(&c);
    printf("t1: %d\n", rpn_peek(&c));
    // EXPECT: t1: 7

    // Test 2: Chain operations: (5 + 3) * 2 = 16
    rpn_init(&c);
    rpn_push(&c, 5);
    rpn_push(&c, 3);
    rpn_add(&c);
    rpn_push(&c, 2);
    rpn_mul(&c);
    printf("t2: %d\n", rpn_peek(&c));
    // EXPECT: t2: 16

    // Test 3: Subtraction and division: (20 - 8) / 3 = 4
    rpn_init(&c);
    rpn_push(&c, 20);
    rpn_push(&c, 8);
    rpn_sub(&c);
    rpn_push(&c, 3);
    rpn_div(&c);
    printf("t3: %d\n", rpn_peek(&c));
    // EXPECT: t3: 4

    // Test 4: Dup and multiply (square): 7 dup * = 49
    rpn_init(&c);
    rpn_push(&c, 7);
    rpn_dup(&c);
    rpn_mul(&c);
    printf("t4: %d\n", rpn_peek(&c));
    // EXPECT: t4: 49

    // Test 5: Swap: push 3, push 7, swap, sub => 7-3=4
    rpn_init(&c);
    rpn_push(&c, 3);
    rpn_push(&c, 7);
    rpn_swap(&c);
    rpn_sub(&c);
    printf("t5: %d\n", rpn_peek(&c));
    // EXPECT: t5: 4

    // Test 6: Memory store and recall
    rpn_init(&c);
    rpn_push(&c, 42);
    rpn_store(&c, 0);
    rpn_pop(&c);
    rpn_push(&c, 100);
    rpn_recall(&c, 0);
    printf("t6: %d %d\n", c.stack[c.sp - 2], rpn_peek(&c));
    // EXPECT: t6: 100 42

    // Test 7: Negate
    rpn_init(&c);
    rpn_push(&c, 7);
    rpn_negate(&c);
    printf("t7: %d\n", rpn_peek(&c));
    // EXPECT: t7: -7

    // Test 8: Abs
    rpn_init(&c);
    rpn_push(&c, -13);
    rpn_abs(&c);
    printf("t8: %d\n", rpn_peek(&c));
    // EXPECT: t8: 13

    // Test 9: Min and Max
    rpn_init(&c);
    rpn_push(&c, 5);
    rpn_push(&c, 9);
    rpn_min(&c);
    printf("t9: min=%d\n", rpn_peek(&c));
    // EXPECT: t9: min=5

    rpn_init(&c);
    rpn_push(&c, 5);
    rpn_push(&c, 9);
    rpn_max(&c);
    printf("t9: max=%d\n", rpn_peek(&c));
    // EXPECT: t9: max=9

    // Test 10: Complex expression: compute 2^8 using dup and mul
    // 2 dup * = 4, dup * = 16, dup * = 256
    rpn_init(&c);
    rpn_push(&c, 2);
    rpn_dup(&c); rpn_mul(&c); // 4
    rpn_dup(&c); rpn_mul(&c); // 16
    rpn_dup(&c); rpn_mul(&c); // 256
    printf("t10: %d\n", rpn_peek(&c));
    // EXPECT: t10: 256

    // Test 11: Multiple memory registers
    rpn_init(&c);
    rpn_push(&c, 10); rpn_store(&c, 0); rpn_pop(&c);
    rpn_push(&c, 20); rpn_store(&c, 1); rpn_pop(&c);
    rpn_push(&c, 30); rpn_store(&c, 2); rpn_pop(&c);
    rpn_recall(&c, 0);
    rpn_recall(&c, 1);
    rpn_recall(&c, 2);
    rpn_add(&c);
    rpn_add(&c);
    printf("t11: %d\n", rpn_peek(&c));
    // EXPECT: t11: 60

    // Test 12: Stack depth
    rpn_init(&c);
    rpn_push(&c, 1);
    rpn_push(&c, 2);
    rpn_push(&c, 3);
    printf("t12: sp=%d\n", c.sp);
    // EXPECT: t12: sp=3

    printf("rpn calculator done\n");
    // EXPECT: rpn calculator done

    return 0;
}
