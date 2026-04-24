int printf(const char *fmt, ...);

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div_op(int a, int b) {
    if (b == 0) return 0;
    return a / b;
}
int mod_op(int a, int b) {
    if (b == 0) return 0;
    return a % b;
}

typedef int (*BinOp)(int, int);

struct Calculator {
    int result;
    int op_count;
    int last_op;
};

void calc_init(struct Calculator *c) {
    c->result = 0;
    c->op_count = 0;
    c->last_op = -1;
}

void calc_apply(struct Calculator *c, BinOp ops[], int op, int value) {
    c->result = ops[op](c->result, value);
    c->op_count = c->op_count + 1;
    c->last_op = op;
}

int eval_expression(int *values, int *ops, int len, BinOp dispatch[]) {
    int result = values[0];
    int i;
    for (i = 0; i < len; i = i + 1) {
        result = dispatch[ops[i]](result, values[i + 1]);
    }
    return result;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int max_val(int a, int b) {
    if (a > b) return a;
    return b;
}

int min_val(int a, int b) {
    if (a < b) return a;
    return b;
}

int main(void) {
    BinOp ops[5];
    ops[0] = add;
    ops[1] = sub;
    ops[2] = mul;
    ops[3] = div_op;
    ops[4] = mod_op;

    // Test basic operations via dispatch table
    // EXPECT: add(3,4) = 7
    printf("add(3,4) = %d\n", ops[0](3, 4));
    // EXPECT: sub(10,3) = 7
    printf("sub(10,3) = %d\n", ops[1](10, 3));
    // EXPECT: mul(5,6) = 30
    printf("mul(5,6) = %d\n", ops[2](5, 6));
    // EXPECT: div(20,4) = 5
    printf("div(20,4) = %d\n", ops[3](20, 4));
    // EXPECT: mod(17,5) = 2
    printf("mod(17,5) = %d\n", ops[4](17, 5));

    // Test calculator struct
    struct Calculator calc;
    calc_init(&calc);
    // EXPECT: Initial result: 0
    printf("Initial result: %d\n", calc.result);

    calc_apply(&calc, ops, 0, 10);  // 0 + 10 = 10
    // EXPECT: After +10: 10
    printf("After +10: %d\n", calc.result);

    calc_apply(&calc, ops, 2, 3);   // 10 * 3 = 30
    // EXPECT: After *3: 30
    printf("After *3: %d\n", calc.result);

    calc_apply(&calc, ops, 1, 5);   // 30 - 5 = 25
    // EXPECT: After -5: 25
    printf("After -5: %d\n", calc.result);

    calc_apply(&calc, ops, 3, 5);   // 25 / 5 = 5
    // EXPECT: After /5: 5
    printf("After /5: %d\n", calc.result);

    // EXPECT: Op count: 4
    printf("Op count: %d\n", calc.op_count);
    // EXPECT: Last op: 3
    printf("Last op: %d\n", calc.last_op);

    // Evaluate expression: 2 + 3 * 4 - 1 (left to right, no precedence)
    // (2+3)=5, (5*4)=20, (20-1)=19
    int values[4];
    values[0] = 2; values[1] = 3; values[2] = 4; values[3] = 1;
    int operators[3];
    operators[0] = 0; operators[1] = 2; operators[2] = 1;
    int result = eval_expression(values, operators, 3, ops);
    // EXPECT: Expression result: 19
    printf("Expression result: %d\n", result);

    // Another expression: 100 / 10 + 5 * 2
    // (100/10)=10, (10+5)=15, (15*2)=30
    int v2[4];
    v2[0] = 100; v2[1] = 10; v2[2] = 5; v2[3] = 2;
    int o2[3];
    o2[0] = 3; o2[1] = 0; o2[2] = 2;
    result = eval_expression(v2, o2, 3, ops);
    // EXPECT: Expression2 result: 30
    printf("Expression2 result: %d\n", result);

    // Division by zero safety
    // EXPECT: div(5,0) = 0
    printf("div(5,0) = %d\n", ops[3](5, 0));

    // Chained operations using function pointers
    BinOp chain[4];
    chain[0] = add;
    chain[1] = mul;
    chain[2] = sub;
    chain[3] = add;
    int acc = 1;
    int operands[4];
    operands[0] = 9; operands[1] = 5; operands[2] = 20; operands[3] = 7;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        acc = chain[i](acc, operands[i]);
    }
    // (1+9)=10, (10*5)=50, (50-20)=30, (30+7)=37
    // EXPECT: Chained result: 37
    printf("Chained result: %d\n", acc);

    // Test helper functions
    // EXPECT: abs(-5) = 5
    printf("abs(-5) = %d\n", abs_val(-5));
    // EXPECT: abs(3) = 3
    printf("abs(3) = %d\n", abs_val(3));
    // EXPECT: max(7,12) = 12
    printf("max(7,12) = %d\n", max_val(7, 12));
    // EXPECT: min(7,12) = 7
    printf("min(7,12) = %d\n", min_val(7, 12));

    // Build a calculation table
    int table[5];
    for (i = 0; i < 5; i = i + 1) {
        table[i] = ops[i](20, 3);
    }
    // 20+3=23, 20-3=17, 20*3=60, 20/3=6, 20%3=2
    // EXPECT: table[0] = 23
    printf("table[0] = %d\n", table[0]);
    // EXPECT: table[1] = 17
    printf("table[1] = %d\n", table[1]);
    // EXPECT: table[2] = 60
    printf("table[2] = %d\n", table[2]);
    // EXPECT: table[3] = 6
    printf("table[3] = %d\n", table[3]);
    // EXPECT: table[4] = 2
    printf("table[4] = %d\n", table[4]);

    return 0;
}
