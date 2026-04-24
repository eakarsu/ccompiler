int printf(const char *fmt, ...);
// EXPECT: post++: r=5 a=6\npre++: r=6 b=6\npost--: r=5 c=4\npre--: r=4 d=4\n3x post++ = 3\n3x pre-- = 7\ncounting: 0 1 2 3 4 \ncountdown: 5 4 3 2 1 \narr: 1 11 19 30 40\npre++ in expr: a=6 b=12\npost++ in expr: a=6 b=10\n-3++ = -2\n++ = -1\n++ = 0
// Test: pre/post increment/decrement (++, --)

int main(void) {
    int a, b, c, d;
    int r1, r2, r3, r4;
    int arr[5];
    int i;

    /* Post-increment */
    a = 5;
    r1 = a++;
    printf("post++: r=%d a=%d\n", r1, a);

    /* Pre-increment */
    b = 5;
    r2 = ++b;
    printf("pre++: r=%d b=%d\n", r2, b);

    /* Post-decrement */
    c = 5;
    r3 = c--;
    printf("post--: r=%d c=%d\n", r3, c);

    /* Pre-decrement */
    d = 5;
    r4 = --d;
    printf("pre--: r=%d d=%d\n", r4, d);

    /* Multiple increments */
    a = 0;
    a++;
    a++;
    a++;
    printf("3x post++ = %d\n", a);

    a = 10;
    --a;
    --a;
    --a;
    printf("3x pre-- = %d\n", a);

    /* In loop */
    printf("counting: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", i);
    }
    printf("\n");

    /* Decrement loop */
    printf("countdown: ");
    for (i = 5; i > 0; i--) {
        printf("%d ", i);
    }
    printf("\n");

    /* Increment on array elements */
    for (i = 0; i < 5; i++) {
        arr[i] = i * 10;
    }
    arr[0]++;
    arr[1]++;
    arr[2]--;
    printf("arr: %d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);

    /* Pre-inc in expression */
    a = 5;
    b = ++a * 2;
    printf("pre++ in expr: a=%d b=%d\n", a, b);

    /* Post-inc in expression */
    a = 5;
    b = a++ * 2;
    printf("post++ in expr: a=%d b=%d\n", a, b);

    /* Negative values */
    a = -3;
    a++;
    printf("-3++ = %d\n", a);
    a++;
    printf("++ = %d\n", a);
    a++;
    printf("++ = %d\n", a);

    return 0;
}
