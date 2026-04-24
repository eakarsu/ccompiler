int printf(const char *fmt, ...);
// EXPECT: 10 += 5 => 15\n+= 20 => 35\n100 -= 30 => 70\n-= 50 => 20\n3 *= 4 => 12\n*= 5 => 60\n100 /= 5 => 20\n/= 4 => 5\n17 %= 5 => 2\n%= 2 => 0\narr[0] = 11\narr[1] = 6\narr[2] = 2\narr[3] = 2\narr[4] = 2\nchained = 5\n10 += -5 => 5\n-= -3 => 8\nsum 1..10 = 55
// Test: compound assignment operators (+=, -=, *=, /=, %=)

int main(void) {
    int a, b, c, d, e;
    int arr[5];
    int i;

    /* += operator */
    a = 10;
    a += 5;
    printf("10 += 5 => %d\n", a);
    a += 20;
    printf("+= 20 => %d\n", a);

    /* -= operator */
    b = 100;
    b -= 30;
    printf("100 -= 30 => %d\n", b);
    b -= 50;
    printf("-= 50 => %d\n", b);

    /* *= operator */
    c = 3;
    c *= 4;
    printf("3 *= 4 => %d\n", c);
    c *= 5;
    printf("*= 5 => %d\n", c);

    /* /= operator */
    d = 100;
    d /= 5;
    printf("100 /= 5 => %d\n", d);
    d /= 4;
    printf("/= 4 => %d\n", d);

    /* %= operator */
    e = 17;
    e %= 5;
    printf("17 %%= 5 => %d\n", e);
    e %= 2;
    printf("%%= 2 => %d\n", e);

    /* Compound assign on array elements */
    for (i = 0; i < 5; i = i + 1) {
        arr[i] = i + 1;
    }
    arr[0] += 10;
    arr[1] *= 3;
    arr[2] -= 1;
    arr[3] /= 2;
    arr[4] %= 3;
    for (i = 0; i < 5; i = i + 1) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    /* Chained compound assigns */
    a = 1;
    a += 2;
    a *= 3;
    a -= 4;
    a /= 1;
    printf("chained = %d\n", a);

    /* Compound with negative values */
    a = 10;
    a += -5;
    printf("10 += -5 => %d\n", a);
    a -= -3;
    printf("-= -3 => %d\n", a);

    /* Accumulator loop */
    a = 0;
    for (i = 1; i <= 10; i = i + 1) {
        a += i;
    }
    printf("sum 1..10 = %d\n", a);

    return 0;
}
