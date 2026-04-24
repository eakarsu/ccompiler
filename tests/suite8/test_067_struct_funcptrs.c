int printf(const char *fmt, ...);

typedef int (*BinOp)(int, int);
typedef int (*UnaryOp)(int);
typedef void (*PrintFn)(int);

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int divide(int a, int b) { if (b == 0) return 0; return a / b; }
int modulo(int a, int b) { if (b == 0) return 0; return a - (a / b) * b; }

int negate(int x) { return -x; }
int double_it(int x) { return x * 2; }
int square(int x) { return x * x; }
int abs_val(int x) { if (x < 0) return -x; return x; }

void print_dec(int x) {
    printf("  dec: %d\n", x);
}

struct Calculator {
    BinOp operations[5];
    UnaryOp transforms[4];
    PrintFn printer;
    int num_ops;
    int num_transforms;
};

void init_calculator(struct Calculator *c) {
    c->operations[0] = add;
    c->operations[1] = sub;
    c->operations[2] = mul;
    c->operations[3] = divide;
    c->operations[4] = modulo;
    c->transforms[0] = negate;
    c->transforms[1] = double_it;
    c->transforms[2] = square;
    c->transforms[3] = abs_val;
    c->printer = print_dec;
    c->num_ops = 5;
    c->num_transforms = 4;
}

int apply_binop(struct Calculator *c, int op_idx, int a, int b) {
    return c->operations[op_idx](a, b);
}

int apply_unary(struct Calculator *c, int tr_idx, int val) {
    return c->transforms[tr_idx](val);
}

int chain_transforms(struct Calculator *c, int val, int *indices, int count) {
    int result = val;
    int i;
    for (i = 0; i < count; i = i + 1) {
        result = c->transforms[indices[i]](result);
    }
    return result;
}

struct OpTable {
    char *name;
    BinOp fn;
};

int main() {
    struct Calculator calc;
    init_calculator(&calc);

    // EXPECT: add(10,3) = 13
    printf("add(10,3) = %d\n", apply_binop(&calc, 0, 10, 3));
    // EXPECT: sub(10,3) = 7
    printf("sub(10,3) = %d\n", apply_binop(&calc, 1, 10, 3));
    // EXPECT: mul(10,3) = 30
    printf("mul(10,3) = %d\n", apply_binop(&calc, 2, 10, 3));
    // EXPECT: div(10,3) = 3
    printf("div(10,3) = %d\n", apply_binop(&calc, 3, 10, 3));
    // EXPECT: mod(10,3) = 1
    printf("mod(10,3) = %d\n", apply_binop(&calc, 4, 10, 3));

    // EXPECT: negate(7) = -7
    printf("negate(7) = %d\n", apply_unary(&calc, 0, 7));
    // EXPECT: double(7) = 14
    printf("double(7) = %d\n", apply_unary(&calc, 1, 7));
    // EXPECT: square(7) = 49
    printf("square(7) = %d\n", apply_unary(&calc, 2, 7));
    // EXPECT: abs(-42) = 42
    printf("abs(-42) = %d\n", apply_unary(&calc, 3, -42));

    int chain1[2];
    chain1[0] = 0;
    chain1[1] = 2;
    int result1 = chain_transforms(&calc, 3, chain1, 2);
    // EXPECT: negate then square(3) = 9
    printf("negate then square(3) = %d\n", result1);

    int chain2[3];
    chain2[0] = 1;
    chain2[1] = 0;
    chain2[2] = 3;
    int result2 = chain_transforms(&calc, 5, chain2, 3);
    // EXPECT: double,negate,abs(5) = 10
    printf("double,negate,abs(5) = %d\n", result2);

    // EXPECT: Printing via func ptr:
    printf("Printing via func ptr:\n");
    // EXPECT:   dec: 42
    calc.printer(42);
    // EXPECT:   dec: -17
    calc.printer(-17);

    struct OpTable table[3];
    table[0].name = "ADD";
    table[0].fn = add;
    table[1].name = "SUB";
    table[1].fn = sub;
    table[2].name = "MUL";
    table[2].fn = mul;

    // EXPECT: Table operations:
    printf("Table operations:\n");
    int i;
    for (i = 0; i < 3; i = i + 1) {
        int r = table[i].fn(20, 4);
        // EXPECT:   ADD: 24
        // EXPECT:   SUB: 16
        // EXPECT:   MUL: 80
        printf("  %s: %d\n", table[i].name, r);
    }

    // EXPECT: All binops on (15,4):
    printf("All binops on (15,4):\n");
    for (i = 0; i < calc.num_ops; i = i + 1) {
        int r = calc.operations[i](15, 4);
        // EXPECT:   op0: 19
        // EXPECT:   op1: 11
        // EXPECT:   op2: 60
        // EXPECT:   op3: 3
        // EXPECT:   op4: 3
        printf("  op%d: %d\n", i, r);
    }

    return 0;
}
