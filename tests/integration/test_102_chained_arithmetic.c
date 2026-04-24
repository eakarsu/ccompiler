int printf(const char *fmt, ...);
// EXPECT: sum = 75\nmixed = 325\nnested = 25\nchain(5) = 34\nchain(10) = 46\ntriple = 30\nproduct = 720\nalternating = 10\nsum of squares = 385
// Test: chained arithmetic with temporaries

int compute_chain(int x) {
    int t1, t2, t3, t4, t5;
    t1 = x + 10;
    t2 = t1 * 3;
    t3 = t2 - 7;
    t4 = t3 / 2;
    t5 = t4 + t1;
    return t5;
}

int triple_add(int a, int b, int c) {
    int s1, s2;
    s1 = a + b;
    s2 = s1 + c;
    return s2;
}

int main(void) {
    int a, b, c, d, e;
    int r1, r2, r3, r4, r5, r6, r7, r8;

    a = 5;
    b = 10;
    c = 15;
    d = 20;
    e = 25;

    /* Chained additions */
    r1 = a + b + c + d + e;
    printf("sum = %d\n", r1);

    /* Mixed chain */
    r2 = a * b + c * d - e;
    printf("mixed = %d\n", r2);

    /* Deeply nested temps */
    r3 = ((a + b) * (c - d)) + ((e * a) - (b + c));
    printf("nested = %d\n", r3);

    /* Function with chain */
    r4 = compute_chain(5);
    printf("chain(5) = %d\n", r4);
    r5 = compute_chain(10);
    printf("chain(10) = %d\n", r5);

    /* Triple add chains */
    r6 = triple_add(a, b, c);
    printf("triple = %d\n", r6);

    /* Chain of multiplies */
    r7 = 2 * 3 * 4 * 5 * 6;
    printf("product = %d\n", r7);

    /* Alternating ops */
    r8 = a + b - c + d - e + a - b + c - d + e;
    printf("alternating = %d\n", r8);

    /* Accumulator pattern */
    {
        int acc, i;
        acc = 0;
        for (i = 1; i <= 10; i = i + 1) {
            acc = acc + i * i;
        }
        printf("sum of squares = %d\n", acc);
    }

    return 0;
}
