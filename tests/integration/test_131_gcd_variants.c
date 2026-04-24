int printf(const char *fmt, ...);
// EXPECT: gcd(12,8) = 4\ngcd(48,18) = 6\ngcd(100,75) = 25\ngcd(17,13) = 1\ngcd(0,5) = 5\nsubtract match: 1 1 1 \nrecursive match: 1 1 1 \nlcm(4,6) = 12\nlcm(12,18) = 36\nlcm(7,13) = 91\nGCD table:\n2 1 2 1 2 \n1 3 1 1 3 \n2 1 4 1 2 \n1 1 1 5 1 \n2 3 2 1 6 \ncoprime with 12: 1 5 7 11 \ntotient: 1 1 2 2 4 2 6 4 6 4 10 4 
// Test: GCD variants and related algorithms

int gcd_euclid(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int gcd_subtract(int a, int b) {
    while (a != b) {
        if (a > b) {
            a = a - b;
        } else {
            b = b - a;
        }
    }
    return a;
}

int gcd_recursive(int a, int b) {
    if (b == 0) return a;
    return gcd_recursive(b, a % b);
}

int lcm(int a, int b) {
    return (a / gcd_euclid(a, b)) * b;
}

int main(void) {
    int i, j;

    /* Basic GCD */
    printf("gcd(12,8) = %d\n", gcd_euclid(12, 8));
    printf("gcd(48,18) = %d\n", gcd_euclid(48, 18));
    printf("gcd(100,75) = %d\n", gcd_euclid(100, 75));
    printf("gcd(17,13) = %d\n", gcd_euclid(17, 13));
    printf("gcd(0,5) = %d\n", gcd_euclid(0, 5));

    /* Verify subtraction method matches */
    printf("subtract match: ");
    printf("%d ", gcd_subtract(12, 8) == gcd_euclid(12, 8));
    printf("%d ", gcd_subtract(48, 18) == gcd_euclid(48, 18));
    printf("%d ", gcd_subtract(100, 75) == gcd_euclid(100, 75));
    printf("\n");

    /* Verify recursive matches */
    printf("recursive match: ");
    printf("%d ", gcd_recursive(12, 8) == gcd_euclid(12, 8));
    printf("%d ", gcd_recursive(48, 18) == gcd_euclid(48, 18));
    printf("%d ", gcd_recursive(100, 75) == gcd_euclid(100, 75));
    printf("\n");

    /* LCM */
    printf("lcm(4,6) = %d\n", lcm(4, 6));
    printf("lcm(12,18) = %d\n", lcm(12, 18));
    printf("lcm(7,13) = %d\n", lcm(7, 13));

    /* GCD table for small values */
    printf("GCD table:\n");
    for (i = 2; i <= 6; i++) {
        for (j = 2; j <= 6; j++) {
            printf("%d ", gcd_euclid(i, j));
        }
        printf("\n");
    }

    /* Coprime pairs (GCD=1) in 1..10 */
    printf("coprime with 12: ");
    for (i = 1; i <= 12; i++) {
        if (gcd_euclid(i, 12) == 1) {
            printf("%d ", i);
        }
    }
    printf("\n");

    /* Euler totient for small values */
    printf("totient: ");
    for (i = 1; i <= 12; i++) {
        int count;
        count = 0;
        for (j = 1; j <= i; j++) {
            if (gcd_euclid(j, i) == 1) {
                count++;
            }
        }
        printf("%d ", count);
    }
    printf("\n");

    return 0;
}
