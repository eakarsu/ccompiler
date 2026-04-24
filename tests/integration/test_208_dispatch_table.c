int printf(const char *fmt, ...);
// EXPECT: --- dispatch 20,6 ---\nop[0](20,6)=26\nop[1](20,6)=14\nop[2](20,6)=120\nop[3](20,6)=3\nop[4](20,6)=2\nop[5](20,6)=20\nop[6](20,6)=6\nop[7](20,6)=13\n--- dispatch 15,4 ---\nadd=19\nsub=11\nmul=60\ndiv=3\nchain result=117\nsum=44\nmax=15
// Test 208: Function pointer arrays (dispatch table)

int op_add(int a, int b) { return a + b; }
int op_sub(int a, int b) { return a - b; }
int op_mul(int a, int b) { return a * b; }
int op_div(int a, int b) { if (b == 0) return 0; return a / b; }
int op_mod(int a, int b) { if (b == 0) return 0; return a - (a / b) * b; }
int op_max(int a, int b) { return (a > b) ? a : b; }
int op_min(int a, int b) { return (a < b) ? a : b; }
int op_avg(int a, int b) { return (a + b) / 2; }

typedef int (*operation)(int, int);

int dispatch(operation *table, int op, int a, int b) {
    if (op < 0 || op >= 8) return 0;
    return table[op](a, b);
}

void print_table_results(operation *table, int n, int a, int b) {
    int i = 0;
    while (i < n) {
        printf("op[%d](%d,%d)=%d\n", i, a, b, table[i](a, b));
        i = i + 1;
    }
}

int chain_operations(operation *table, int *ops, int n_ops, int initial, int operand) {
    int result = initial;
    int i = 0;
    while (i < n_ops) {
        result = table[ops[i]](result, operand);
        i = i + 1;
    }
    return result;
}

int main(void) {
    operation table[8];
    table[0] = op_add;
    table[1] = op_sub;
    table[2] = op_mul;
    table[3] = op_div;
    table[4] = op_mod;
    table[5] = op_max;
    table[6] = op_min;
    table[7] = op_avg;

    printf("--- dispatch 20,6 ---\n");
    print_table_results(table, 8, 20, 6);

    printf("--- dispatch 15,4 ---\n");
    printf("add=%d\n", dispatch(table, 0, 15, 4));
    printf("sub=%d\n", dispatch(table, 1, 15, 4));
    printf("mul=%d\n", dispatch(table, 2, 15, 4));
    printf("div=%d\n", dispatch(table, 3, 15, 4));

    int ops[5];
    ops[0] = 0; ops[1] = 2; ops[2] = 1; ops[3] = 0; ops[4] = 2;
    int result = chain_operations(table, ops, 5, 10, 3);
    printf("chain result=%d\n", result);

    int data[6];
    data[0] = 5; data[1] = 12; data[2] = 3; data[3] = 8; data[4] = 15; data[5] = 1;

    int sum = 0;
    int i = 0;
    while (i < 6) {
        sum = table[0](sum, data[i]);
        i = i + 1;
    }
    printf("sum=%d\n", sum);

    int mx = data[0];
    i = 1;
    while (i < 6) {
        mx = table[5](mx, data[i]);
        i = i + 1;
    }
    printf("max=%d\n", mx);

    return 0;
}
