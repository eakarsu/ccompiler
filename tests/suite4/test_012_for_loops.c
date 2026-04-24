int printf(const char *fmt, ...);

int sum_range(int lo, int hi) {
    int s = 0;
    int i;
    for (i = lo; i <= hi; i = i + 1) {
        s = s + i;
    }
    return s;
}

int factorial(int n) {
    int result = 1;
    int i;
    for (i = 2; i <= n; i = i + 1) {
        result = result * i;
    }
    return result;
}

int power(int base, int exp) {
    int result = 1;
    int i;
    for (i = 0; i < exp; i = i + 1) {
        result = result * base;
    }
    return result;
}

int count_divisors(int n) {
    int count = 0;
    int i;
    for (i = 1; i <= n; i = i + 1) {
        if (n % i == 0)
            count = count + 1;
    }
    return count;
}

int main(void) {
    // Test 1: simple counting for loop
    int sum = 0;
    int i;
    for (i = 1; i <= 10; i = i + 1) {
        sum = sum + i;
    }
    // EXPECT: sum_1_to_10: 55
    printf("sum_1_to_10: %d\n", sum);

    // Test 2: for loop counting down
    int countdown = 0;
    for (i = 10; i > 0; i = i - 1) {
        countdown = countdown + 1;
    }
    // EXPECT: countdown: 10
    printf("countdown: %d\n", countdown);

    // Test 3: for loop with step of 2
    int even_sum = 0;
    for (i = 0; i <= 10; i = i + 2) {
        even_sum = even_sum + i;
    }
    // 0+2+4+6+8+10 = 30
    // EXPECT: even_sum: 30
    printf("even_sum: %d\n", even_sum);

    // Test 4: for loop with step of 3
    int count3 = 0;
    for (i = 0; i < 30; i = i + 3) {
        count3 = count3 + 1;
    }
    // 0,3,6,9,12,15,18,21,24,27 = 10 iterations
    // EXPECT: count_step3: 10
    printf("count_step3: %d\n", count3);

    // Test 5: nested for loops - multiplication table sum
    int table_sum = 0;
    int j;
    for (i = 1; i <= 5; i = i + 1) {
        for (j = 1; j <= 5; j = j + 1) {
            table_sum = table_sum + i * j;
        }
    }
    // sum of i*j for i,j in [1,5]: (1+2+3+4+5)*(1+2+3+4+5) = 15*15 = 225
    // EXPECT: table_sum: 225
    printf("table_sum: %d\n", table_sum);

    // Test 6: nested loops counting pairs
    int pairs = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = i + 1; j < 6; j = j + 1) {
            pairs = pairs + 1;
        }
    }
    // C(6,2) = 15
    // EXPECT: pairs: 15
    printf("pairs: %d\n", pairs);

    // Test 7: for loop that never executes
    int never = 0;
    for (i = 10; i < 5; i = i + 1) {
        never = 1;
    }
    // EXPECT: never_ran: 0
    printf("never_ran: %d\n", never);

    // Test 8: for loop executes exactly once
    int once = 0;
    for (i = 0; i < 1; i = i + 1) {
        once = once + 1;
    }
    // EXPECT: once: 1
    printf("once: %d\n", once);

    // Test 9: sum_range function
    // EXPECT: sum_range: 55
    printf("sum_range: %d\n", sum_range(1, 10));
    // EXPECT: sum_range2: 45
    printf("sum_range2: %d\n", sum_range(5, 10));

    // Test 10: factorial function
    // EXPECT: fact5: 120
    printf("fact5: %d\n", factorial(5));
    // EXPECT: fact7: 5040
    printf("fact7: %d\n", factorial(7));

    // Test 11: power function
    // EXPECT: pow2_10: 1024
    printf("pow2_10: %d\n", power(2, 10));
    // EXPECT: pow3_4: 81
    printf("pow3_4: %d\n", power(3, 4));

    // Test 12: count_divisors
    // 12: 1,2,3,4,6,12 = 6 divisors
    // EXPECT: divs12: 6
    printf("divs12: %d\n", count_divisors(12));

    // Test 13: triple nested for
    int triple = 0;
    int k;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            for (k = 0; k < 3; k = k + 1) {
                triple = triple + 1;
            }
        }
    }
    // 3*3*3 = 27
    // EXPECT: triple: 27
    printf("triple: %d\n", triple);

    // Test 14: for loop building a running product
    int prod = 1;
    for (i = 1; i <= 6; i = i + 1) {
        prod = prod * i;
    }
    // 6! = 720
    // EXPECT: prod: 720
    printf("prod: %d\n", prod);

    // Test 15: nested for computing sum of squares
    int sq_sum = 0;
    for (i = 1; i <= 5; i = i + 1) {
        sq_sum = sq_sum + i * i;
    }
    // 1+4+9+16+25 = 55
    // EXPECT: sq_sum: 55
    printf("sq_sum: %d\n", sq_sum);

    // Test 16: for loop with multiple updates
    int a = 0;
    int b = 100;
    for (i = 0; i < 10; i = i + 1) {
        a = a + 1;
        b = b - 1;
    }
    // EXPECT: multi_update: 10 90
    printf("multi_update: %d %d\n", a, b);

    return 0;
}
