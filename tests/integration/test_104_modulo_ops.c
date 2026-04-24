int printf(const char *fmt, ...);
// EXPECT: 17 % 5 = 2\n-17 % 5 = -2\n17 % -5 = 2\n-17 % -5 = -2\n100 % 1 = 0\n-100 % 1 = 0\n3 % 7 = 3\n-3 % 7 = -3\n5 % 5 = 0\n-5 % 5 = 0\n0:even 1:odd 2:even 3:odd 4:even 5:odd 6:even 7:odd 8:even 9:odd \n100%7 = 2\n100%7%3 = 2\n1000000 % 7 = 1\n1000000 % 13 = 1\nverify: 1\nverify neg: 1
// Test: modulo with positive/negative combos

int main(void) {
    int a, b;
    int r1, r2, r3, r4;
    int i;

    /* Basic modulo */
    a = 17;
    b = 5;
    r1 = a % b;
    printf("17 %% 5 = %d\n", r1);

    /* Negative dividend */
    r2 = (-17) % 5;
    printf("-17 %% 5 = %d\n", r2);

    /* Negative divisor */
    r3 = 17 % (-5);
    printf("17 %% -5 = %d\n", r3);

    /* Both negative */
    r4 = (-17) % (-5);
    printf("-17 %% -5 = %d\n", r4);

    /* Modulo by 1 always gives 0 */
    printf("100 %% 1 = %d\n", 100 % 1);
    printf("-100 %% 1 = %d\n", (-100) % 1);

    /* Modulo where dividend < divisor */
    printf("3 %% 7 = %d\n", 3 % 7);
    printf("-3 %% 7 = %d\n", (-3) % 7);

    /* Modulo with equal values */
    printf("5 %% 5 = %d\n", 5 % 5);
    printf("-5 %% 5 = %d\n", (-5) % 5);

    /* Use modulo to check even/odd */
    for (i = 0; i < 10; i = i + 1) {
        if (i % 2 == 0) {
            printf("%d:even ", i);
        } else {
            printf("%d:odd ", i);
        }
    }
    printf("\n");

    /* Modulo chain */
    printf("100%%7 = %d\n", 100 % 7);
    printf("100%%7%%3 = %d\n", 100 % 7 % 3);

    /* Large values */
    printf("1000000 %% 7 = %d\n", 1000000 % 7);
    printf("1000000 %% 13 = %d\n", 1000000 % 13);

    /* Verify: (a/b)*b + (a%b) == a */
    a = 123;
    b = 17;
    printf("verify: %d\n", (a / b) * b + (a % b) == a);

    a = -123;
    b = 17;
    printf("verify neg: %d\n", (a / b) * b + (a % b) == a);

    return 0;
}
