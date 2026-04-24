int printf(const char *fmt, ...);

// Perfect numbers and amicable pairs

// Sum of proper divisors of n
int sum_proper_divisors(int n) {
    if (n <= 1) return 0;
    int sum = 1;
    int i;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum = sum + i;
            if (i != n / i) {
                sum = sum + n / i;
            }
        }
    }
    return sum;
}

// Check if n is a perfect number (sum of proper divisors == n)
int is_perfect(int n) {
    if (n <= 1) return 0;
    return sum_proper_divisors(n) == n;
}

// Check if n is abundant (sum of proper divisors > n)
int is_abundant(int n) {
    if (n <= 1) return 0;
    return sum_proper_divisors(n) > n;
}

// Check if n is deficient (sum of proper divisors < n)
int is_deficient(int n) {
    if (n <= 1) return 0;
    return sum_proper_divisors(n) < n;
}

// Check if (a, b) form an amicable pair
int is_amicable_pair(int a, int b) {
    if (a == b) return 0;
    return sum_proper_divisors(a) == b && sum_proper_divisors(b) == a;
}

// Find amicable pairs up to limit, store in result arrays
int find_amicable_pairs(int limit, int *first, int *second) {
    int count = 0;
    int n;
    for (n = 2; n < limit; n++) {
        int s = sum_proper_divisors(n);
        if (s > n && s < limit) {
            if (sum_proper_divisors(s) == n) {
                first[count] = n;
                second[count] = s;
                count = count + 1;
            }
        }
    }
    return count;
}

// Count abundant numbers up to n
int count_abundant(int n) {
    int count = 0;
    int i;
    for (i = 1; i <= n; i++) {
        if (is_abundant(i)) {
            count = count + 1;
        }
    }
    return count;
}

// Count deficient numbers up to n
int count_deficient(int n) {
    int count = 0;
    int i;
    for (i = 1; i <= n; i++) {
        if (is_deficient(i)) {
            count = count + 1;
        }
    }
    return count;
}

// Aliquot sum chain length (stop if cycle or exceeds limit)
int aliquot_length(int n, int limit) {
    int len = 0;
    int current = n;
    while (current > 0 && len < limit) {
        current = sum_proper_divisors(current);
        len = len + 1;
        if (current == n) return len;  // cycled back
        if (current == 0) return len;
    }
    return len;
}

// Classify a number
void classify(int n) {
    int s = sum_proper_divisors(n);
    if (s == n) {
        printf("%d is perfect (sigma=%d)\n", n, s);
    } else if (s > n) {
        printf("%d is abundant (sigma=%d)\n", n, s);
    } else {
        printf("%d is deficient (sigma=%d)\n", n, s);
    }
}

int main(void) {
    // Perfect numbers
    printf("is_perfect(6): %d\n", is_perfect(6));
    // EXPECT: is_perfect(6): 1

    printf("is_perfect(28): %d\n", is_perfect(28));
    // EXPECT: is_perfect(28): 1

    printf("is_perfect(496): %d\n", is_perfect(496));
    // EXPECT: is_perfect(496): 1

    printf("is_perfect(12): %d\n", is_perfect(12));
    // EXPECT: is_perfect(12): 0

    // Sum of proper divisors
    printf("spd(220): %d\n", sum_proper_divisors(220));
    // EXPECT: spd(220): 284

    printf("spd(284): %d\n", sum_proper_divisors(284));
    // EXPECT: spd(284): 220

    // Amicable pair check
    printf("amicable(220,284): %d\n", is_amicable_pair(220, 284));
    // EXPECT: amicable(220,284): 1

    printf("amicable(220,221): %d\n", is_amicable_pair(220, 221));
    // EXPECT: amicable(220,221): 0

    // Find amicable pairs up to 2000
    int first[10];
    int second[10];
    int acount = find_amicable_pairs(2000, first, second);
    printf("Amicable pairs < 2000: %d\n", acount);
    // EXPECT: Amicable pairs < 2000: 2

    if (acount > 0) {
        printf("Pair: (%d, %d)\n", first[0], second[0]);
        // EXPECT: Pair: (220, 284)
    }

    // Classification
    classify(6);
    // EXPECT: 6 is perfect (sigma=6)

    classify(12);
    // EXPECT: 12 is abundant (sigma=16)

    classify(10);
    // EXPECT: 10 is deficient (sigma=8)

    // Counts up to 100
    int ab = count_abundant(100);
    printf("Abundant nums <= 100: %d\n", ab);
    // EXPECT: Abundant nums <= 100: 22

    int def = count_deficient(100);
    printf("Deficient nums <= 100: %d\n", def);
    // EXPECT: Deficient nums <= 100: 75

    // Aliquot chain lengths
    int al6 = aliquot_length(6, 20);
    printf("aliquot(6) length: %d\n", al6);
    // EXPECT: aliquot(6) length: 1

    int al28 = aliquot_length(28, 20);
    printf("aliquot(28) length: %d\n", al28);
    // EXPECT: aliquot(28) length: 1

    // Aliquot chain for 12: 12->16->15->9->4->3->1->0, length=7 before cycling/hitting 0
    int al12 = aliquot_length(12, 20);
    printf("aliquot(12) length: %d\n", al12);
    // EXPECT: aliquot(12) length: 7

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
