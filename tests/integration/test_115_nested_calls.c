int printf(const char *fmt, ...);
// EXPECT: add(1, add(2,3)) = 6\nadd(add(1,2), add(3,4)) = 10\nmul(add(2,3), sub(10,4)) = 30\ndeep nest = 33\nsquare(add(3,4)) = 49\nmax of maxes = 7\nmin of mins = 2\nmax3 = 15\nabs(5-12) = 7\ncomplex = 35\nmax > 3: yes\nsum of squares via calls = 55
// Test: nested function calls as arguments

int add(int a, int b) {
    return a + b;
}

int mul(int a, int b) {
    return a * b;
}

int sub(int a, int b) {
    return a - b;
}

int square(int x) {
    return x * x;
}

int max2(int a, int b) {
    if (a > b) return a;
    return b;
}

int min2(int a, int b) {
    if (a < b) return a;
    return b;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int main(void) {
    int r;

    /* Simple nesting */
    r = add(1, add(2, 3));
    printf("add(1, add(2,3)) = %d\n", r);

    /* Double nesting */
    r = add(add(1, 2), add(3, 4));
    printf("add(add(1,2), add(3,4)) = %d\n", r);

    /* Mixed operations */
    r = mul(add(2, 3), sub(10, 4));
    printf("mul(add(2,3), sub(10,4)) = %d\n", r);

    /* Triple nesting */
    r = add(mul(2, 3), add(mul(4, 5), sub(10, 3)));
    printf("deep nest = %d\n", r);

    /* Square of sum */
    r = square(add(3, 4));
    printf("square(add(3,4)) = %d\n", r);

    /* Nested max/min */
    r = max2(max2(3, 7), max2(5, 2));
    printf("max of maxes = %d\n", r);

    r = min2(min2(3, 7), min2(5, 2));
    printf("min of mins = %d\n", r);

    /* Max of three using nested */
    r = max2(max2(15, 8), 12);
    printf("max3 = %d\n", r);

    /* Abs of subtraction */
    r = abs_val(sub(5, 12));
    printf("abs(5-12) = %d\n", r);

    /* Complex expression with many calls */
    r = add(mul(2, square(3)), sub(mul(4, 5), add(1, 2)));
    printf("complex = %d\n", r);

    /* Call result as condition */
    if (max2(add(1, 2), sub(5, 1)) > 3) {
        printf("max > 3: yes\n");
    } else {
        printf("max > 3: no\n");
    }

    /* Nested calls in loop */
    {
        int i, sum;
        sum = 0;
        for (i = 1; i <= 5; i++) {
            sum = add(sum, mul(i, i));
        }
        printf("sum of squares via calls = %d\n", sum);
    }

    return 0;
}
