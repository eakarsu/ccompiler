int printf(const char *fmt, ...);
// EXPECT: 100-30-20-10 = 40\n1000/10/5/2 = 10\n1000/(10/(5/2)) = 200\nshort-circuit AND: 0\nshort-circuit OR: 1\nside effects sum: r=30 g_val=101\nskipped call: g_val=0\nskipped call OR: g_val=0\nternary skip: 42\nchain AND: 1\nchain OR: 1\nseq inc: 5 6\nnested ternary: 200\nnested side: r=30 g_val=101
// Test: expression evaluation order

int g_val = 0;

int side_a(void) {
    g_val = g_val + 1;
    return 10;
}

int side_b(void) {
    g_val = g_val + 100;
    return 20;
}

int identity(int x) {
    return x;
}

int main(void) {
    int a, b, c, d;
    int r;
    int arr[10];
    int i;

    /* Left-to-right associativity of subtraction */
    r = 100 - 30 - 20 - 10;
    printf("100-30-20-10 = %d\n", r);

    /* Left-to-right associativity of division */
    r = 1000 / 10 / 5 / 2;
    printf("1000/10/5/2 = %d\n", r);

    /* Verify with parentheses */
    r = 1000 / (10 / (5 / 2));
    printf("1000/(10/(5/2)) = %d\n", r);

    /* Short-circuit AND */
    a = 0;
    b = 1;
    r = (a != 0) && (100 / a > 0);
    printf("short-circuit AND: %d\n", r);

    /* Short-circuit OR */
    a = 1;
    r = (a == 1) || (100 / 0 > 0);
    printf("short-circuit OR: %d\n", r);

    /* Side effects with function calls */
    g_val = 0;
    r = side_a() + side_b();
    printf("side effects sum: r=%d g_val=%d\n", r, g_val);

    /* Logical short-circuit with side effects */
    g_val = 0;
    r = (1 == 0) && (side_a() > 0);
    printf("skipped call: g_val=%d\n", g_val);

    g_val = 0;
    r = (1 == 1) || (side_a() > 0);
    printf("skipped call OR: g_val=%d\n", g_val);

    /* Conditional evaluation in ternary */
    a = 1;
    r = a ? 42 : 100 / 0;
    printf("ternary skip: %d\n", r);

    /* Complex short-circuit chain */
    a = 5; b = 0; c = 3;
    r = (a > 0) && (b == 0) && (c < 10);
    printf("chain AND: %d\n", r);

    r = (a < 0) || (b > 0) || (c == 3);
    printf("chain OR: %d\n", r);

    /* Increment in expressions - well defined cases */
    a = 5;
    arr[0] = a;
    a++;
    arr[1] = a;
    printf("seq inc: %d %d\n", arr[0], arr[1]);

    /* Nested ternary evaluation */
    a = 2;
    r = (a > 3) ? 100 : (a > 1) ? 200 : 300;
    printf("nested ternary: %d\n", r);

    /* Multiple function calls */
    g_val = 0;
    r = identity(side_a()) + identity(side_b());
    printf("nested side: r=%d g_val=%d\n", r, g_val);

    return 0;
}
