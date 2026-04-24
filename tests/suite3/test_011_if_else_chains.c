int printf(const char *fmt, ...);

/* test_011_if_else_chains.c
   Tests: nested if/else, if/else if/else chains, complex conditions,
   deeply nested (5+ levels), short-circuit evaluation.
*/

int classify_digit(int n) {
    if (n < 0) {
        return -1;
    } else if (n == 0) {
        return 0;
    } else if (n < 10) {
        return 1;
    } else if (n < 100) {
        return 2;
    } else {
        return 3;
    }
}

int sign(int n) {
    if (n > 0) return 1;
    else if (n < 0) return -1;
    else return 0;
}

int max3(int a, int b, int c) {
    if (a >= b && a >= c) return a;
    else if (b >= a && b >= c) return b;
    else return c;
}

int in_range(int x, int lo, int hi) {
    if (x >= lo && x <= hi) return 1;
    else return 0;
}

int deep_nested(int a, int b, int c, int d, int e) {
    /* 5-level nesting */
    if (a > 0) {
        if (b > 0) {
            if (c > 0) {
                if (d > 0) {
                    if (e > 0) {
                        return 5;
                    } else {
                        return 4;
                    }
                } else {
                    return 3;
                }
            } else {
                return 2;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int complex_cond(int x, int y, int z) {
    /* complex && and || */
    if ((x > 0 && y > 0) || z == 99) {
        if (x + y > 10 || z == 99) {
            return 1;
        } else {
            return 2;
        }
    } else if (x < 0 || y < 0) {
        return 3;
    } else {
        return 4;
    }
}

int short_circuit(int *flag, int val) {
    /* && short-circuits: if val==0, flag should not be set */
    if (val != 0 && (*flag = 1)) {
        return 1;
    }
    return 0;
}

int grade(int score) {
    if (score >= 90) return 4; /* A */
    else if (score >= 80) return 3; /* B */
    else if (score >= 70) return 2; /* C */
    else if (score >= 60) return 1; /* D */
    else return 0;                  /* F */
}

int fizzbuzz_val(int n) {
    if (n % 15 == 0) return 3;
    else if (n % 3 == 0) return 1;
    else if (n % 5 == 0) return 2;
    else return 0;
}

int main(void) {
    /* classify_digit */
    printf("%d\n", classify_digit(-5));  // EXPECT: -1
    printf("%d\n", classify_digit(0));   // EXPECT: 0
    printf("%d\n", classify_digit(7));   // EXPECT: 1
    printf("%d\n", classify_digit(42));  // EXPECT: 2
    printf("%d\n", classify_digit(100)); // EXPECT: 3

    /* sign */
    printf("%d\n", sign(-10)); // EXPECT: -1
    printf("%d\n", sign(0));   // EXPECT: 0
    printf("%d\n", sign(5));   // EXPECT: 1

    /* max3 */
    printf("%d\n", max3(1, 2, 3)); // EXPECT: 3
    printf("%d\n", max3(5, 3, 4)); // EXPECT: 5
    printf("%d\n", max3(2, 7, 7)); // EXPECT: 7

    /* in_range */
    printf("%d\n", in_range(5, 1, 10));  // EXPECT: 1
    printf("%d\n", in_range(0, 1, 10));  // EXPECT: 0
    printf("%d\n", in_range(10, 1, 10)); // EXPECT: 1

    /* deep_nested */
    printf("%d\n", deep_nested(1, 1, 1, 1, 1));  // EXPECT: 5
    printf("%d\n", deep_nested(1, 1, 1, 1, -1)); // EXPECT: 4
    printf("%d\n", deep_nested(1, 1, 1, -1, 0)); // EXPECT: 3
    printf("%d\n", deep_nested(1, 1, -1, 0, 0)); // EXPECT: 2
    printf("%d\n", deep_nested(1, -1, 0, 0, 0)); // EXPECT: 1
    printf("%d\n", deep_nested(-1, 0, 0, 0, 0)); // EXPECT: 0

    /* complex_cond */
    printf("%d\n", complex_cond(6, 7, 0));  // EXPECT: 1
    printf("%d\n", complex_cond(1, 2, 0));  // EXPECT: 2
    printf("%d\n", complex_cond(-1, 2, 0)); // EXPECT: 3
    printf("%d\n", complex_cond(0, 0, 0));  // EXPECT: 4

    /* short_circuit: val==0 => flag not set */
    int flag = 0;
    short_circuit(&flag, 0);
    printf("%d\n", flag); // EXPECT: 0
    short_circuit(&flag, 1);
    printf("%d\n", flag); // EXPECT: 1

    /* grade */
    printf("%d\n", grade(95)); // EXPECT: 4
    printf("%d\n", grade(85)); // EXPECT: 3
    printf("%d\n", grade(75)); // EXPECT: 2
    printf("%d\n", grade(65)); // EXPECT: 1
    printf("%d\n", grade(55)); // EXPECT: 0

    /* fizzbuzz_val */
    printf("%d\n", fizzbuzz_val(15)); // EXPECT: 3
    printf("%d\n", fizzbuzz_val(9));  // EXPECT: 1
    printf("%d\n", fizzbuzz_val(10)); // EXPECT: 2
    printf("%d\n", fizzbuzz_val(7));  // EXPECT: 0

    return 0;
}
