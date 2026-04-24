int printf(const char *fmt, ...);
// EXPECT: Calculator history:\n  10 + 3 = 13\n  10 - 3 = 7\n  10 * 3 = 30\n  10 / 3 = 3\n  10 % 3 = 1\n  2 ^ 10 = 1024\n  100 / 0 = ERROR\n  7 % 0 = ERROR\n  15 * 4 = 60\n  99 / 11 = 9\nValid: 8, Sum of results: 1147

enum Operation {
    OP_ADD = 0,
    OP_SUB = 1,
    OP_MUL = 2,
    OP_DIV = 3,
    OP_MOD = 4,
    OP_POW = 5
};

struct CalcEntry {
    int operand1;
    int operand2;
    int op;
    int result;
    int valid;
};

int power(int base, int exp) {
    int result = 1;
    int i;
    for (i = 0; i < exp; i++) {
        result = result * base;
    }
    return result;
}

struct CalcEntry calc_compute(int a, int b, int op) {
    struct CalcEntry e;
    e.operand1 = a;
    e.operand2 = b;
    e.op = op;
    e.valid = 1;
    if (op == OP_ADD) e.result = a + b;
    else if (op == OP_SUB) e.result = a - b;
    else if (op == OP_MUL) e.result = a * b;
    else if (op == OP_DIV) {
        if (b == 0) { e.valid = 0; e.result = 0; }
        else e.result = a / b;
    }
    else if (op == OP_MOD) {
        if (b == 0) { e.valid = 0; e.result = 0; }
        else e.result = a % b;
    }
    else if (op == OP_POW) e.result = power(a, b);
    else { e.valid = 0; e.result = 0; }
    return e;
}

char *op_symbol(int op) {
    if (op == OP_ADD) return "+";
    if (op == OP_SUB) return "-";
    if (op == OP_MUL) return "*";
    if (op == OP_DIV) return "/";
    if (op == OP_MOD) return "%";
    if (op == OP_POW) return "^";
    return "?";
}

void print_entry(struct CalcEntry *e) {
    if (e->valid) {
        printf("%d %s %d = %d\n", e->operand1, op_symbol(e->op), e->operand2, e->result);
    } else {
        printf("%d %s %d = ERROR\n", e->operand1, op_symbol(e->op), e->operand2);
    }
}

int main(void) {
    struct CalcEntry history[20];
    int hcount = 0;

    history[hcount] = calc_compute(10, 3, OP_ADD); hcount++;
    history[hcount] = calc_compute(10, 3, OP_SUB); hcount++;
    history[hcount] = calc_compute(10, 3, OP_MUL); hcount++;
    history[hcount] = calc_compute(10, 3, OP_DIV); hcount++;
    history[hcount] = calc_compute(10, 3, OP_MOD); hcount++;
    history[hcount] = calc_compute(2, 10, OP_POW); hcount++;
    history[hcount] = calc_compute(100, 0, OP_DIV); hcount++;
    history[hcount] = calc_compute(7, 0, OP_MOD); hcount++;
    history[hcount] = calc_compute(15, 4, OP_MUL); hcount++;
    history[hcount] = calc_compute(99, 11, OP_DIV); hcount++;

    printf("Calculator history:\n");
    int i;
    for (i = 0; i < hcount; i++) {
        printf("  ");
        print_entry(&history[i]);
    }

    int valid_count = 0;
    int result_sum = 0;
    for (i = 0; i < hcount; i++) {
        if (history[i].valid) {
            valid_count++;
            result_sum = result_sum + history[i].result;
        }
    }
    printf("Valid: %d, Sum of results: %d\n", valid_count, result_sum);

    return 0;
}
