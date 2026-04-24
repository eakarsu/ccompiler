int printf(const char *fmt, ...);
// EXPECT: 2+3 = 5\n10*20 = 200\n100/4 = 25\n17%5 = 2\n(2+3)*(4+5) = 45\n100-50+25-12 = 63\n1<<10 = 1024\n1024>>3 = 128\n5>3 = 1\n3>5 = 0\n5==5 = 1\n5!=5 = 0\nternary = 42\nx+0 = 7\nx*1 = 7\nx-0 = 7\nx/1 = 7\nx*0 = 0\nx-x = 0\n0/x = 0\ncomplex const = 16\narr: 100 110 120 130 140 150 160 170 180 190 \nx*2 = 26\nx*4 = 52\nx*8 = 104
// Test: constant folding scenarios

int main(void) {
    int r;
    int x;
    int arr[10];
    int i;

    /* Pure constant expressions */
    r = 2 + 3;
    printf("2+3 = %d\n", r);

    r = 10 * 20;
    printf("10*20 = %d\n", r);

    r = 100 / 4;
    printf("100/4 = %d\n", r);

    r = 17 % 5;
    printf("17%%5 = %d\n", r);

    /* Nested constant expressions */
    r = (2 + 3) * (4 + 5);
    printf("(2+3)*(4+5) = %d\n", r);

    r = 100 - 50 + 25 - 12;
    printf("100-50+25-12 = %d\n", r);

    /* Constants with shifts */
    r = 1 << 10;
    printf("1<<10 = %d\n", r);

    r = 1024 >> 3;
    printf("1024>>3 = %d\n", r);

    /* Constant boolean expressions */
    r = (5 > 3);
    printf("5>3 = %d\n", r);

    r = (3 > 5);
    printf("3>5 = %d\n", r);

    r = (5 == 5);
    printf("5==5 = %d\n", r);

    r = (5 != 5);
    printf("5!=5 = %d\n", r);

    /* Constant ternary */
    r = (1 > 0) ? 42 : 99;
    printf("ternary = %d\n", r);

    /* Identity operations that should fold */
    x = 7;
    r = x + 0;
    printf("x+0 = %d\n", r);
    r = x * 1;
    printf("x*1 = %d\n", r);
    r = x - 0;
    printf("x-0 = %d\n", r);
    r = x / 1;
    printf("x/1 = %d\n", r);

    /* Zero-producing operations */
    r = x * 0;
    printf("x*0 = %d\n", r);
    r = x - x;
    printf("x-x = %d\n", r);
    r = 0 / x;
    printf("0/x = %d\n", r);

    /* Complex constant expression */
    r = ((10 + 20) * 3 - 15) / 5 + 1;
    printf("complex const = %d\n", r);

    /* Array filled with constant expressions */
    for (i = 0; i < 10; i++) {
        arr[i] = 100 + i * 10;
    }
    printf("arr: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    /* Strength reduction candidates */
    x = 13;
    printf("x*2 = %d\n", x * 2);
    printf("x*4 = %d\n", x * 4);
    printf("x*8 = %d\n", x * 8);

    return 0;
}
