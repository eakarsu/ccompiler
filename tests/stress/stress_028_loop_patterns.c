int printf(const char *fmt, ...);

// Complex loop patterns: infinite loop with break, multiple iterators,
// countdown, fibonacci, nested break, string building, complex step.

int test_infinite_break(void) {
    int i = 0;
    int sum = 0;
    for (;;) {
        sum += i;
        i++;
        if (i > 100) break;
    }
    // sum = 0+1+2+...+100 = 5050
    return sum;
}

int test_while_true_break(void) {
    int n = 1;
    int steps = 0;
    while (1) {
        if (n == 1) break;
        steps++;
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
    }
    // n starts at 1, breaks immediately
    // Let's use n=27 (famous Collatz sequence, 111 steps)
    return steps;
}

int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        steps++;
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
    }
    return steps;
}

int test_two_iterators(void) {
    // Two variables moving in opposite directions
    int i, j;
    int count = 0;
    for (i = 0, j = 99; i < j; i++, j--) {
        count++;
    }
    // i and j meet at i=50,j=49 (since we need i<j)
    // count = 50
    return count;
}

int test_countdown(void) {
    int sum = 0;
    int i;
    for (i = 50; i > 0; i--) {
        sum += i;
    }
    // 50+49+...+1 = 1275
    return sum;
}

int test_fibonacci_loop(void) {
    int a = 0, b = 1;
    int i;
    for (i = 0; i < 20; i++) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    // fib(20)=6765, fib(21)=10946
    // After 20 iterations: a=fib(20)=6765, b=fib(21)=10946
    return a;
}

int test_nested_inner_break(void) {
    int total = 0;
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 100; j++) {
            if (j >= i + 1) break;
            total++;
        }
    }
    // i=0: j breaks at 1, 1 iter
    // i=1: j breaks at 2, 2 iter
    // ...
    // i=9: j breaks at 10, 10 iter
    // total = 1+2+...+10 = 55
    return total;
}

int test_string_build(void) {
    char buf[100];
    int pos = 0;
    int i;

    // Build string "0123456789"
    for (i = 0; i < 10; i++) {
        buf[pos++] = '0' + i;
    }
    buf[pos] = '\0';

    // Verify by summing char values
    int sum = 0;
    for (i = 0; buf[i]; i++) {
        sum += buf[i] - '0';
    }
    // 0+1+2+...+9 = 45
    return sum;
}

int test_complex_step(void) {
    int sum = 0;
    int i;
    // Step doubles each iteration
    for (i = 1; i < 1000; i *= 2) {
        sum += i;
    }
    // 1+2+4+8+16+32+64+128+256+512 = 1023
    return sum;
}

int test_do_while(void) {
    int n = 1;
    int count = 0;
    do {
        count++;
        n *= 3;
    } while (n < 10000);
    // n: 3, 9, 27, 81, 243, 729, 2187, 6561, 19683 (>=10000, stop)
    // count = 9
    return count;
}

int test_for_with_continue(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 100; i++) {
        if (i % 5 == 0) continue;
        if (i % 3 == 0) continue;
        sum += i;
    }
    // Sum all i in [0,99] not divisible by 3 or 5
    // Total sum = 4950
    // Sum div by 3: 0+3+6+...+99 = 3*(0+1+...+33) = 3*561 = 1683
    // Sum div by 5 (not div by 3): 5+10+20+25+35+40+50+55+65+70+80+85+95 = 635
    // Wait, let me just compute: sum div by 5 = 0+5+10+...+95 = 5*(0+1+...+19) = 5*190 = 950
    // Sum div by 15 (both): 0+15+30+45+60+75+90 = 315
    // By inclusion-exclusion: div by 3 or 5 = 1683+950-315 = 2318
    // Not div by 3 or 5 = 4950 - 2318 = 2632
    return sum;
}

int test_triple_nested(void) {
    int count = 0;
    int i, j, k;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            for (k = 0; k < 10; k++) {
                if (i + j + k == 10) count++;
            }
        }
    }
    // Number of solutions to i+j+k=10 with 0<=i,j,k<=9
    // This is a stars and bars with upper bound.
    // Without upper bound: C(12,2)=66
    // Subtract cases where any var >= 10:
    //   i>=10: i'=i-10, i'+j+k=0, C(2,2)=1, times 3 vars = 3
    // No case where two vars >= 10 (sum would be >= 20)
    // Answer: 66 - 3 = 63
    return count;
}

int test_while_with_multiple_conditions(void) {
    int a = 0, b = 100;
    int steps = 0;
    while (a < b && steps < 1000) {
        a += 3;
        b -= 2;
        steps++;
    }
    // After n steps: a=3n, b=100-2n
    // 3n >= 100-2n => 5n >= 100 => n >= 20
    // At n=20: a=60, b=60, a<b is false => stop
    return steps;
}

int test_loop_with_early_return(int target) {
    int i;
    for (i = 2; i * i <= target; i++) {
        if (target % i == 0) return 0;  // composite
    }
    if (target < 2) return 0;
    return 1;  // prime
}

int count_primes(int limit) {
    int count = 0;
    int n;
    for (n = 2; n <= limit; n++) {
        if (test_loop_with_early_return(n)) count++;
    }
    return count;
}

int test_accumulate_pattern(void) {
    int arr[20];
    int i;
    for (i = 0; i < 20; i++) arr[i] = i + 1;

    // Running sum (prefix sum)
    int prefix[20];
    prefix[0] = arr[0];
    for (i = 1; i < 20; i++) {
        prefix[i] = prefix[i - 1] + arr[i];
    }
    // prefix[19] = 1+2+...+20 = 210
    // prefix[9] = 1+2+...+10 = 55
    return prefix[19] * 1000 + prefix[9];
}

int test_bubble_sort_loop(void) {
    int arr[] = {9, 3, 7, 1, 5, 8, 2, 6, 4, 0};
    int n = 10;
    int i, j, tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
    // Should be sorted: 0,1,2,...,9
    return arr[0] * 1000 + arr[5] * 100 + arr[9];
}

int test_gcd_loop(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int main(void) {
    printf("%d\n", test_infinite_break());
    // EXPECT: 5050

    printf("%d\n", collatz_steps(27));
    // EXPECT: 111

    printf("%d\n", test_two_iterators());
    // EXPECT: 50

    printf("%d\n", test_countdown());
    // EXPECT: 1275

    printf("%d\n", test_fibonacci_loop());
    // EXPECT: 6765

    printf("%d\n", test_nested_inner_break());
    // EXPECT: 55

    printf("%d\n", test_string_build());
    // EXPECT: 45

    printf("%d\n", test_complex_step());
    // EXPECT: 1023

    printf("%d\n", test_do_while());
    // EXPECT: 9

    printf("%d\n", test_for_with_continue());
    // EXPECT: 2632

    printf("%d\n", test_triple_nested());
    // EXPECT: 63

    printf("%d\n", test_while_with_multiple_conditions());
    // EXPECT: 20

    // Primes up to 100: there are 25
    printf("%d\n", count_primes(100));
    // EXPECT: 25

    printf("%d\n", test_accumulate_pattern());
    // EXPECT: 210055

    // Bubble sort: arr[0]=0, arr[5]=5, arr[9]=9 => 0*1000+5*100+9 = 509
    printf("%d\n", test_bubble_sort_loop());
    // EXPECT: 509

    // GCD(48, 36) = 12
    printf("%d\n", test_gcd_loop(48, 36));
    // EXPECT: 12

    // GCD(100, 75) = 25
    printf("%d\n", test_gcd_loop(100, 75));
    // EXPECT: 25

    printf("ALL PASSED\n");
    // EXPECT: ALL PASSED
    return 0;
}
