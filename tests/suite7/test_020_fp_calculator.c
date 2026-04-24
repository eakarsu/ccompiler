int printf(const char *fmt, ...);

typedef int (*CalcOp)(int, int);

int calc_add(int a, int b) { return a + b; }
int calc_sub(int a, int b) { return a - b; }
int calc_mul(int a, int b) { return a * b; }
int calc_div(int a, int b) { return a / b; }
int calc_mod(int a, int b) { return a % b; }
int calc_min(int a, int b) { return a < b ? a : b; }
int calc_max(int a, int b) { return a > b ? a : b; }
int calc_pow(int a, int b) {
    int result;
    int i;
    result = 1;
    for (i = 0; i < b; i++) {
        result = result * a;
    }
    return result;
}

struct OpEntry {
    char symbol;
    CalcOp func;
};

struct Calculator {
    struct OpEntry ops[8];
    int op_count;
    int memory;
    int last_result;
};

void calc_init(struct Calculator *c) {
    c->op_count = 0;
    c->memory = 0;
    c->last_result = 0;
}

void calc_register(struct Calculator *c, char sym, CalcOp func) {
    if (c->op_count < 8) {
        c->ops[c->op_count].symbol = sym;
        c->ops[c->op_count].func = func;
        c->op_count = c->op_count + 1;
    }
}

CalcOp calc_lookup(struct Calculator *c, char sym) {
    int i;
    for (i = 0; i < c->op_count; i++) {
        if (c->ops[i].symbol == sym) {
            return c->ops[i].func;
        }
    }
    return 0;
}

int calc_eval(struct Calculator *c, int a, char op, int b) {
    CalcOp func;
    func = calc_lookup(c, op);
    if (func) {
        c->last_result = func(a, b);
        return c->last_result;
    }
    return 0;
}

void calc_store(struct Calculator *c) {
    c->memory = c->last_result;
}

int calc_recall(struct Calculator *c) {
    return c->memory;
}

int calc_chain(struct Calculator *c, int init, char ops[], int vals[], int n) {
    int result;
    int i;
    result = init;
    for (i = 0; i < n; i++) {
        result = calc_eval(c, result, ops[i], vals[i]);
    }
    return result;
}

int calc_eval_rpn(struct Calculator *c, int values[], char ops[], int n_vals, int n_ops) {
    int stack[16];
    int sp;
    int i;
    int j;
    int a;
    int b;
    sp = 0;
    j = 0;
    for (i = 0; i < n_vals; i++) {
        stack[sp] = values[i];
        sp = sp + 1;
        if (i > 0 && j < n_ops) {
            b = stack[sp - 1];
            a = stack[sp - 2];
            sp = sp - 2;
            stack[sp] = calc_eval(c, a, ops[j], b);
            sp = sp + 1;
            j = j + 1;
        }
    }
    if (sp > 0) return stack[sp - 1];
    return 0;
}

int calc_reduce(struct Calculator *c, int arr[], int n, char op) {
    int result;
    int i;
    result = arr[0];
    for (i = 1; i < n; i++) {
        result = calc_eval(c, result, op, arr[i]);
    }
    return result;
}

int main(void) {
    struct Calculator calc;
    int val;
    char ops[5];
    int vals[5];
    int arr[5];

    calc_init(&calc);
    calc_register(&calc, '+', calc_add);
    calc_register(&calc, '-', calc_sub);
    calc_register(&calc, '*', calc_mul);
    calc_register(&calc, '/', calc_div);
    calc_register(&calc, '%', calc_mod);
    calc_register(&calc, '<', calc_min);
    calc_register(&calc, '>', calc_max);
    calc_register(&calc, '^', calc_pow);
    // EXPECT: ops registered: 8
    printf("ops registered: %d\n", calc.op_count);

    val = calc_eval(&calc, 10, '+', 5);
    // EXPECT: 10+5 = 15
    printf("10+5 = %d\n", val);

    val = calc_eval(&calc, 10, '-', 3);
    // EXPECT: 10-3 = 7
    printf("10-3 = %d\n", val);

    val = calc_eval(&calc, 6, '*', 7);
    // EXPECT: 6*7 = 42
    printf("6*7 = %d\n", val);

    val = calc_eval(&calc, 20, '/', 4);
    // EXPECT: 20/4 = 5
    printf("20/4 = %d\n", val);

    val = calc_eval(&calc, 17, '%', 5);
    // EXPECT: 17%5 = 2
    printf("17%%5 = %d\n", val);

    val = calc_eval(&calc, 2, '^', 8);
    // EXPECT: 2^8 = 256
    printf("2^8 = %d\n", val);

    val = calc_eval(&calc, 3, '<', 7);
    // EXPECT: min(3,7) = 3
    printf("min(3,7) = %d\n", val);

    val = calc_eval(&calc, 3, '>', 7);
    // EXPECT: max(3,7) = 7
    printf("max(3,7) = %d\n", val);

    calc_store(&calc);
    // EXPECT: memory: 7
    printf("memory: %d\n", calc_recall(&calc));

    ops[0] = '+'; ops[1] = '*'; ops[2] = '-';
    vals[0] = 3; vals[1] = 4; vals[2] = 10;
    val = calc_chain(&calc, 5, ops, vals, 3);
    // EXPECT: chain: 22
    printf("chain: %d\n", val);

    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    val = calc_reduce(&calc, arr, 5, '+');
    // EXPECT: sum: 15
    printf("sum: %d\n", val);

    val = calc_reduce(&calc, arr, 5, '*');
    // EXPECT: product: 120
    printf("product: %d\n", val);

    val = calc_reduce(&calc, arr, 5, '>');
    // EXPECT: max all: 5
    printf("max all: %d\n", val);

    val = calc_reduce(&calc, arr, 5, '<');
    // EXPECT: min all: 1
    printf("min all: %d\n", val);

    // EXPECT: last: 1
    printf("last: %d\n", calc.last_result);

    return 0;
}
