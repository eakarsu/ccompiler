int printf(const char *fmt, ...);

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int a = 0;
    int b = 1;
    int i;
    for (i = 2; i <= n; i = i + 1) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

int factorial(int n) {
    int result = 1;
    int i;
    for (i = 2; i <= n; i = i + 1) {
        result = result * i;
    }
    return result;
}

int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    int i = 3;
    while (i * i <= n) {
        if (n % i == 0) return 0;
        i = i + 2;
    }
    return 1;
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int int_sqrt(int n) {
    if (n < 0) return -1;
    int guess = 1;
    while (guess * guess <= n) {
        guess = guess + 1;
    }
    return guess - 1;
}

int main(void) {
    // Test 1: Fibonacci sequence
    // EXPECT: fib0: 0
    printf("fib0: %d\n", fibonacci(0));
    // EXPECT: fib1: 1
    printf("fib1: %d\n", fibonacci(1));
    // EXPECT: fib10: 55
    printf("fib10: %d\n", fibonacci(10));
    // EXPECT: fib15: 610
    printf("fib15: %d\n", fibonacci(15));

    // Test 2: Factorial
    // EXPECT: fact0: 1
    printf("fact0: %d\n", factorial(0));
    // EXPECT: fact5: 120
    printf("fact5: %d\n", factorial(5));
    // EXPECT: fact8: 40320
    printf("fact8: %d\n", factorial(8));

    // Test 3: accumulation pattern - sum of squares
    int sum_sq = 0;
    int i;
    for (i = 1; i <= 10; i = i + 1) {
        sum_sq = sum_sq + i * i;
    }
    // 1+4+9+16+25+36+49+64+81+100 = 385
    // EXPECT: sum_sq: 385
    printf("sum_sq: %d\n", sum_sq);

    // Test 4: accumulation - sum of cubes
    int sum_cu = 0;
    for (i = 1; i <= 5; i = i + 1) {
        sum_cu = sum_cu + i * i * i;
    }
    // 1+8+27+64+125 = 225
    // EXPECT: sum_cu: 225
    printf("sum_cu: %d\n", sum_cu);

    // Test 5: counting pattern - count primes up to 30
    int prime_count = 0;
    for (i = 2; i <= 30; i = i + 1) {
        if (is_prime(i)) {
            prime_count = prime_count + 1;
        }
    }
    // Primes up to 30: 2,3,5,7,11,13,17,19,23,29 = 10
    // EXPECT: primes_30: 10
    printf("primes_30: %d\n", prime_count);

    // Test 6: searching pattern - find largest prime <= 50
    int largest_prime = 0;
    for (i = 50; i >= 2; i = i - 1) {
        if (is_prime(i)) {
            largest_prime = i;
            break;
        }
    }
    // EXPECT: largest_prime: 47
    printf("largest_prime: %d\n", largest_prime);

    // Test 7: minimum and maximum in array
    int data[10];
    data[0] = 34; data[1] = 12; data[2] = 56; data[3] = 78; data[4] = 23;
    data[5] = 45; data[6] = 89; data[7] = 1;  data[8] = 67; data[9] = 43;
    int min_val = data[0];
    int max_val = data[0];
    for (i = 1; i < 10; i = i + 1) {
        if (data[i] < min_val) min_val = data[i];
        if (data[i] > max_val) max_val = data[i];
    }
    // EXPECT: min_val: 1
    printf("min_val: %d\n", min_val);
    // EXPECT: max_val: 89
    printf("max_val: %d\n", max_val);

    // Test 8: bubble sort
    int arr[6];
    arr[0] = 5; arr[1] = 3; arr[2] = 8; arr[3] = 1; arr[4] = 9; arr[5] = 2;
    int j;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 5 - i; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
    // EXPECT: sorted: 1 2 3 5 8 9
    printf("sorted: %d %d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);

    // Test 9: selection sort on different data
    int arr2[5];
    arr2[0] = 42; arr2[1] = 17; arr2[2] = 93; arr2[3] = 5; arr2[4] = 28;
    for (i = 0; i < 5; i = i + 1) {
        int min_idx = i;
        for (j = i + 1; j < 5; j = j + 1) {
            if (arr2[j] < arr2[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int tmp = arr2[i];
            arr2[i] = arr2[min_idx];
            arr2[min_idx] = tmp;
        }
    }
    // EXPECT: sel_sort: 5 17 28 42 93
    printf("sel_sort: %d %d %d %d %d\n", arr2[0], arr2[1], arr2[2], arr2[3], arr2[4]);

    // Test 10: linear search
    int target = 67;
    int found_idx = -1;
    for (i = 0; i < 10; i = i + 1) {
        if (data[i] == target) {
            found_idx = i;
            break;
        }
    }
    // EXPECT: search_67: 8
    printf("search_67: %d\n", found_idx);

    // Test 11: GCD
    // EXPECT: gcd_48_18: 6
    printf("gcd_48_18: %d\n", gcd(48, 18));

    // Test 12: LCM via GCD
    int a_val = 12;
    int b_val = 8;
    int lcm = (a_val / gcd(a_val, b_val)) * b_val;
    // gcd(12,8)=4, lcm = (12/4)*8 = 24
    // EXPECT: lcm_12_8: 24
    printf("lcm_12_8: %d\n", lcm);

    // Test 13: integer square root
    // EXPECT: sqrt_100: 10
    printf("sqrt_100: %d\n", int_sqrt(100));
    // EXPECT: sqrt_50: 7
    printf("sqrt_50: %d\n", int_sqrt(50));

    // Test 14: running average (integer)
    int running = 0;
    int values[5];
    values[0] = 10; values[1] = 20; values[2] = 30; values[3] = 40; values[4] = 50;
    for (i = 0; i < 5; i = i + 1) {
        running = running + values[i];
    }
    int avg = running / 5;
    // (10+20+30+40+50)/5 = 30
    // EXPECT: avg: 30
    printf("avg: %d\n", avg);

    // Test 15: power by repeated squaring
    int base = 3;
    int exp = 7;
    int result = 1;
    int b = base;
    int e = exp;
    while (e > 0) {
        if (e % 2 == 1) {
            result = result * b;
        }
        b = b * b;
        e = e / 2;
    }
    // 3^7 = 2187
    // EXPECT: fast_pow: 2187
    printf("fast_pow: %d\n", result);

    // Test 16: prefix sum array
    int prefix[6];
    prefix[0] = 0;
    int src[5];
    src[0] = 3; src[1] = 1; src[2] = 4; src[3] = 1; src[4] = 5;
    for (i = 0; i < 5; i = i + 1) {
        prefix[i + 1] = prefix[i] + src[i];
    }
    // prefix: 0,3,4,8,9,14
    // EXPECT: prefix_sum: 0 3 4 8 9 14
    printf("prefix_sum: %d %d %d %d %d %d\n",
           prefix[0], prefix[1], prefix[2], prefix[3], prefix[4], prefix[5]);

    return 0;
}
