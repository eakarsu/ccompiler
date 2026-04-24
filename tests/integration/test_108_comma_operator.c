int printf(const char *fmt, ...);
// EXPECT: (1,2,3) = 3\ncomma side effects: x=5 y=10 r=15\nfor comma: 0/10 1/20 2/30 3/40 4/50 \nchain: a=6 b=16\nnested: a=5 b=6 c=11\nwhile comma: i=5 a=6\nfunc comma: x=18 r=18\narr: 0 3 6 9 12 \nseq: a=3 b=1 c=2
// Test: comma operator in expressions

int side_effect_add(int *p, int val) {
    *p = *p + val;
    return *p;
}

int main(void) {
    int a, b, c, r;
    int x, y;
    int i;
    int arr[5];

    /* Basic comma: evaluates left, discards, returns right */
    a = (1, 2, 3);
    printf("(1,2,3) = %d\n", a);

    /* Comma with side effects */
    x = 0;
    y = 0;
    r = (x = 5, y = 10, x + y);
    printf("comma side effects: x=%d y=%d r=%d\n", x, y, r);

    /* Comma in for loop */
    printf("for comma: ");
    for (i = 0, a = 10; i < 5; i++, a += 10) {
        printf("%d/%d ", i, a);
    }
    printf("\n");

    /* Comma separating computations */
    a = 1;
    b = (a = a + 1, a = a * 3, a + 10);
    printf("chain: a=%d b=%d\n", a, b);

    /* Nested comma expressions */
    a = 0;
    b = 0;
    c = ((a = 5, b = a + 1), a + b);
    printf("nested: a=%d b=%d c=%d\n", a, b, c);

    /* Comma in while condition (not typical but valid) */
    i = 0;
    a = 0;
    while (a += 1, i < 5) {
        i++;
    }
    printf("while comma: i=%d a=%d\n", i, a);

    /* Comma with function calls */
    x = 10;
    r = (side_effect_add(&x, 5), side_effect_add(&x, 3));
    printf("func comma: x=%d r=%d\n", x, r);

    /* Comma in array init loop */
    for (i = 0; i < 5; i++) {
        arr[i] = (i * 2, i * 3);
    }
    printf("arr: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    /* Multiple comma expressions in sequence */
    a = 0;
    b = 0;
    c = 0;
    a = (b = 1, c = 2, b + c);
    printf("seq: a=%d b=%d c=%d\n", a, b, c);

    return 0;
}
