int printf(const char *fmt, ...);
// EXPECT: mul table:\n  1  2  3  4  5\n  2  4  6  8 10\n  3  6  9 12 15\n  4  8 12 16 20\n  5 10 15 20 25\npower tower = 65536\ntriangle:\n1 = 1\n1 2 = 3\n1 2 3 = 6\n1 2 3 4 = 10\n1 2 3 4 5 = 15\nconverge 99999: 45 9 \nfirst div by 7 in rows: 14 21 35 42 56 \nconverge: 1/100 2/67 4/45 8/30 16/20 \ndo-while sum = 55\nfib-step: 1 1 2 3 5 8 13 21 
// Test: loop-based arithmetic patterns

int main(void) {
    int i, j;
    int acc;
    int n;

    /* Nested loop multiplication table */
    printf("mul table:\n");
    for (i = 1; i <= 5; i++) {
        for (j = 1; j <= 5; j++) {
            printf("%3d", i * j);
        }
        printf("\n");
    }

    /* Power tower: compute 2^2^2^... iteratively */
    acc = 2;
    for (i = 0; i < 3; i++) {
        int base, exp, result, k;
        base = 2;
        exp = acc;
        result = 1;
        for (k = 0; k < exp; k++) {
            result *= base;
        }
        acc = result;
    }
    printf("power tower = %d\n", acc);

    /* Triangular pattern with sums */
    printf("triangle:\n");
    for (i = 1; i <= 5; i++) {
        int row_sum;
        row_sum = 0;
        for (j = 1; j <= i; j++) {
            printf("%d ", j);
            row_sum += j;
        }
        printf("= %d\n", row_sum);
    }

    /* Digit sum convergence */
    n = 99999;
    printf("converge %d: ", n);
    while (n >= 10) {
        int sum;
        int tmp;
        sum = 0;
        tmp = n;
        while (tmp > 0) {
            sum += tmp % 10;
            tmp /= 10;
        }
        n = sum;
        printf("%d ", n);
    }
    printf("\n");

    /* Nested break pattern */
    printf("first div by 7 in rows: ");
    for (i = 1; i <= 5; i++) {
        for (j = i * 10; j < i * 10 + 10; j++) {
            if (j % 7 == 0) {
                printf("%d ", j);
                break;
            }
        }
    }
    printf("\n");

    /* While loop with multiple conditions */
    i = 1;
    j = 100;
    printf("converge: ");
    while (i < j) {
        printf("%d/%d ", i, j);
        i = i * 2;
        j = j - j / 3;
    }
    printf("\n");

    /* Do-while computing */
    n = 1;
    acc = 0;
    do {
        acc += n;
        n++;
    } while (n <= 10);
    printf("do-while sum = %d\n", acc);

    /* For loop with complex update */
    printf("fib-step: ");
    {
        int a, b, temp;
        a = 1;
        b = 1;
        for (i = 0; i < 8; i++) {
            printf("%d ", a);
            temp = a + b;
            a = b;
            b = temp;
        }
    }
    printf("\n");

    return 0;
}
