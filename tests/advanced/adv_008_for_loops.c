int printf(const char *fmt, ...);
// EXPECT: === For Loop Tests ===
// EXPECT: sum_1_to_10: 55
// EXPECT: sum_even: 30
// EXPECT: countdown: 10 9 8 7 6 5 4 3 2 1
// EXPECT: factorial_10: 3628800
// EXPECT: power: 2^10=1024
// EXPECT: nested_2d:(0,0)(0,1)(0,2)(1,0)(1,1)(1,2)(2,0)(2,1)(2,2)
// EXPECT: nested_3d: 27
// EXPECT: multiply_table: 1 2 3 4 5 2 4 6 8 10 3 6 9 12 15 4 8 12 16 20 5 10 15 20 25
// EXPECT: break_find: found 7 at index 3
// EXPECT: continue_skip: 1 3 5 7 9
// EXPECT: break_nested: broke at i=1 j=2
// EXPECT: empty_init: sum=10
// EXPECT: empty_cond: counted to 5
// EXPECT: empty_inc: sum=10
// EXPECT: complex_inc: 0 2 4 6 8
// EXPECT: complex_inc2: 1 2 4 8 16
// EXPECT: comma_expr: i=5 j=0
// EXPECT: reverse: 5 4 3 2 1
// EXPECT: loop_var_scope: outer_i=5
// EXPECT: triangular: 1 3 6 10 15
// EXPECT: gcd: gcd(48,18)=6
// EXPECT: prime_check: 2P 3P 4. 5P 6. 7P 8. 9. 10. 11P 12. 13P 14. 15. 16. 17P 18. 19P 20.
// EXPECT: bubble_sort: 1 2 3 4 5 6 7 8 9 10
// EXPECT: matrix_sum: 45
// EXPECT: fibonacci: 0 1 1 2 3 5 8 13 21 34
// EXPECT: collatz_steps: collatz(27)=111 steps
// EXPECT: nested_break_continue: 0:1 0:3 1:1 1:3 2:1 2:3
// sum_1_to_10: 55
// sum_even: 30
// countdown: 10 9 8 7 6 5 4 3 2 1
// factorial_10: 3628800
// power: 2^10=1024
// nested_2d:(0,0)(0,1)(0,2)(1,0)(1,1)(1,2)(2,0)(2,1)(2,2)
// nested_3d: 27
// multiply_table: 1 2 3 4 5 2 4 6 8 10 3 6 9 12 15 4 8 12 16 20 5 10 15 20 25
// break_find: found 7 at index 3
// continue_skip: 1 3 5 7 9
// break_nested: broke at i=1 j=2
// empty_init: sum=10
// empty_cond: counted to 5
// empty_inc: sum=10
// complex_inc: 0 2 4 6 8
// complex_inc2: 1 2 4 8 16
// comma_expr: i=5 j=0
// reverse: 5 4 3 2 1
// loop_var_scope: outer_i=5
// triangular: 1 3 6 10 15
// gcd: gcd(48,18)=6
// prime_check: 2P 3P 4. 5P 6. 7P 8. 9. 10. 11P 12. 13P 14. 15. 16. 17P 18. 19P 20.
// bubble_sort: 1 2 3 4 5 6 7 8 9 10
// matrix_sum: 45
// fibonacci: 0 1 1 2 3 5 8 13 21 34
// collatz_steps: collatz(27)=111 steps
// nested_break_continue: 0:1 0:3 1:1 1:3 2:1 2:3

int main(void) {
    int i, j, k, r, sum;

    printf("=== For Loop Tests ===\n");

    // Sum 1 to 10
    sum = 0;
    for (i = 1; i <= 10; i++) {
        sum = sum + i;
    }
    printf("sum_1_to_10: %d\n", sum);

    // Sum of even numbers 2..10
    sum = 0;
    for (i = 2; i <= 10; i = i + 2) {
        sum = sum + i;
    }
    printf("sum_even: %d\n", sum);

    // Countdown
    printf("countdown:");
    for (i = 10; i >= 1; i--) {
        printf(" %d", i);
    }
    printf("\n");

    // Factorial
    r = 1;
    for (i = 1; i <= 10; i++) {
        r = r * i;
    }
    printf("factorial_10: %d\n", r);

    // Power of 2
    r = 1;
    for (i = 0; i < 10; i++) {
        r = r * 2;
    }
    printf("power: 2^10=%d\n", r);

    // Nested 2D loops
    printf("nested_2d:");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("(%d,%d)", i, j);
        }
    }
    printf("\n");

    // Nested 3D loop counting
    int count = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                count++;
            }
        }
    }
    printf("nested_3d: %d\n", count);

    // Multiplication table 5x5
    printf("multiply_table:");
    for (i = 1; i <= 5; i++) {
        for (j = 1; j <= 5; j++) {
            printf(" %d", i * j);
        }
    }
    printf("\n");

    // Break: find element
    int arr[6];
    arr[0] = 3; arr[1] = 1; arr[2] = 4; arr[3] = 7; arr[4] = 2; arr[5] = 8;
    r = -1;
    for (i = 0; i < 6; i++) {
        if (arr[i] == 7) {
            r = i;
            break;
        }
    }
    printf("break_find: found 7 at index %d\n", r);

    // Continue: skip even numbers
    printf("continue_skip:");
    for (i = 0; i < 10; i++) {
        if (i % 2 == 0) continue;
        printf(" %d", i);
    }
    printf("\n");

    // Break in nested loops (only breaks inner)
    int bi = -1;
    int bj = -1;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (i * 5 + j >= 7) {
                bi = i;
                bj = j;
                goto found;
            }
        }
    }
found:
    printf("break_nested: broke at i=%d j=%d\n", bi, bj);

    // Empty init part
    i = 0;
    sum = 0;
    for (; i < 5; i++) {
        sum = sum + i;
    }
    printf("empty_init: sum=%d\n", sum);

    // Empty condition (break manually)
    i = 0;
    for (;;) {
        if (i >= 5) break;
        i++;
    }
    printf("empty_cond: counted to %d\n", i);

    // Empty increment
    sum = 0;
    for (i = 0; i < 5;) {
        sum = sum + i;
        i++;
    }
    printf("empty_inc: sum=%d\n", sum);

    // Complex increment expressions
    printf("complex_inc:");
    for (i = 0; i < 10; i = i + 2) {
        printf(" %d", i);
    }
    printf("\n");

    printf("complex_inc2:");
    for (i = 1; i <= 16; i = i * 2) {
        printf(" %d", i);
    }
    printf("\n");

    // Comma expressions in for
    for (i = 0, j = 5; i < 5; i++, j--) {
        // runs 5 times
    }
    printf("comma_expr: i=%d j=%d\n", i, j);

    // Reverse iteration
    printf("reverse:");
    for (i = 5; i >= 1; i--) {
        printf(" %d", i);
    }
    printf("\n");

    // Loop variable scope
    for (i = 0; i < 5; i++) {
        // inner scope
    }
    printf("loop_var_scope: outer_i=%d\n", i);

    // Triangular numbers
    printf("triangular:");
    sum = 0;
    for (i = 1; i <= 5; i++) {
        sum = sum + i;
        printf(" %d", sum);
    }
    printf("\n");

    // GCD via Euclidean algorithm
    int a = 48;
    int b = 18;
    int ta = a;
    int tb = b;
    for (;;) {
        if (tb == 0) break;
        int temp = tb;
        tb = ta % tb;
        ta = temp;
    }
    printf("gcd: gcd(%d,%d)=%d\n", a, b, ta);

    // Prime checking for 2..20
    printf("prime_check:");
    for (i = 2; i <= 20; i++) {
        int is_prime = 1;
        for (j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            printf(" %dP", i);
        } else {
            printf(" %d.", i);
        }
    }
    printf("\n");

    // Bubble sort
    int sa[10];
    sa[0] = 5; sa[1] = 3; sa[2] = 8; sa[3] = 1; sa[4] = 9;
    sa[5] = 2; sa[6] = 7; sa[7] = 4; sa[8] = 10; sa[9] = 6;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 9 - i; j++) {
            if (sa[j] > sa[j + 1]) {
                int temp = sa[j];
                sa[j] = sa[j + 1];
                sa[j + 1] = temp;
            }
        }
    }
    printf("bubble_sort:");
    for (i = 0; i < 10; i++) {
        printf(" %d", sa[i]);
    }
    printf("\n");

    // Matrix sum (flattened 3x3 as 1D)
    int mat[9];
    for (i = 0; i < 9; i++) {
        mat[i] = i + 1;
    }
    sum = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            sum = sum + mat[i * 3 + j];
        }
    }
    printf("matrix_sum: %d\n", sum);

    // Fibonacci first 10
    printf("fibonacci:");
    int fa = 0;
    int fb = 1;
    for (i = 0; i < 10; i++) {
        printf(" %d", fa);
        int temp = fa + fb;
        fa = fb;
        fb = temp;
    }
    printf("\n");

    // Collatz conjecture steps
    int n = 27;
    int steps = 0;
    int cn = n;
    for (;;) {
        if (cn == 1) break;
        if (cn % 2 == 0) {
            cn = cn / 2;
        } else {
            cn = 3 * cn + 1;
        }
        steps++;
    }
    printf("collatz_steps: collatz(%d)=%d steps\n", n, steps);

    // Nested loops with break and continue combined
    printf("nested_break_continue:");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 5; j++) {
            if (j % 2 == 0) continue;
            if (j > 3) break;
            printf(" %d:%d", i, j);
        }
    }
    printf("\n");

    return 0;
}
