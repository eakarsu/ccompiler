int printf(const char *fmt, ...);

// Perfect numbers, abundant numbers, deficient numbers

int sum_proper_divisors(int n) {
    int sum;
    int i;
    if (n <= 1) return 0;
    sum = 1;
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

int is_perfect(int n) {
    if (n <= 1) return 0;
    return (sum_proper_divisors(n) == n) ? 1 : 0;
}

int is_abundant(int n) {
    if (n <= 1) return 0;
    return (sum_proper_divisors(n) > n) ? 1 : 0;
}

int is_deficient(int n) {
    if (n <= 1) return 0;
    return (sum_proper_divisors(n) < n) ? 1 : 0;
}

int abundance(int n) {
    return sum_proper_divisors(n) - n;
}

void find_perfect_numbers(int limit) {
    int i;
    int first;
    first = 1;
    printf("Perfect numbers up to %d:", limit);
    for (i = 2; i <= limit; i++) {
        if (is_perfect(i)) {
            printf(" %d", i);
            first = 0;
        }
    }
    printf("\n");
}

int count_abundant(int limit) {
    int count;
    int i;
    count = 0;
    for (i = 2; i <= limit; i++) {
        if (is_abundant(i)) {
            count = count + 1;
        }
    }
    return count;
}

int count_deficient(int limit) {
    int count;
    int i;
    count = 0;
    for (i = 2; i <= limit; i++) {
        if (is_deficient(i)) {
            count = count + 1;
        }
    }
    return count;
}

int smallest_abundant(void) {
    int i;
    for (i = 2; i < 10000; i++) {
        if (is_abundant(i)) return i;
    }
    return -1;
}

void print_divisors(int n) {
    int i;
    int first;
    first = 1;
    for (i = 1; i <= n; i++) {
        if (n % i == 0) {
            if (first) {
                printf("%d", i);
                first = 0;
            } else {
                printf(" %d", i);
            }
        }
    }
    printf("\n");
}

int is_multiply_perfect(int n, int k) {
    int s;
    s = sum_proper_divisors(n) + n;
    return (s == k * n) ? 1 : 0;
}

int aliquot_sequence_length(int n, int max_steps) {
    int step;
    int next;
    step = 0;
    while (n > 0 && step < max_steps) {
        next = sum_proper_divisors(n);
        step = step + 1;
        if (next == n) return step;
        if (next == 0) return step;
        n = next;
    }
    return step;
}

int main() {
    int spd;
    int ab;
    int ca;
    int cd;
    int sa;
    int mp;
    int al;

    // EXPECT: Perfect numbers up to 10000: 6 28 496 8128
    find_perfect_numbers(10000);

    spd = sum_proper_divisors(28);
    // EXPECT: Sum of proper divisors of 28: 28
    printf("Sum of proper divisors of 28: %d\n", spd);

    spd = sum_proper_divisors(12);
    // EXPECT: Sum of proper divisors of 12: 16
    printf("Sum of proper divisors of 12: %d\n", spd);

    // EXPECT: Divisors of 28: 1 2 4 7 14 28
    printf("Divisors of 28: ");
    print_divisors(28);

    // EXPECT: 6 is perfect: 1
    printf("6 is perfect: %d\n", is_perfect(6));
    // EXPECT: 10 is perfect: 0
    printf("10 is perfect: %d\n", is_perfect(10));

    // EXPECT: 12 is abundant: 1
    printf("12 is abundant: %d\n", is_abundant(12));
    // EXPECT: 8 is abundant: 0
    printf("8 is abundant: %d\n", is_abundant(8));

    // EXPECT: 8 is deficient: 1
    printf("8 is deficient: %d\n", is_deficient(8));
    // EXPECT: 12 is deficient: 0
    printf("12 is deficient: %d\n", is_deficient(12));

    ab = abundance(12);
    // EXPECT: Abundance of 12: 4
    printf("Abundance of 12: %d\n", ab);
    ab = abundance(8);
    // EXPECT: Abundance of 8: -1
    printf("Abundance of 8: %d\n", ab);

    ca = count_abundant(100);
    // EXPECT: Abundant numbers up to 100: 22
    printf("Abundant numbers up to 100: %d\n", ca);

    cd = count_deficient(100);
    // EXPECT: Deficient numbers up to 100: 75
    printf("Deficient numbers up to 100: %d\n", cd);

    sa = smallest_abundant();
    // EXPECT: Smallest abundant number: 12
    printf("Smallest abundant number: %d\n", sa);

    mp = is_multiply_perfect(120, 3);
    // EXPECT: 120 is 3-perfect: 1
    printf("120 is 3-perfect: %d\n", mp);

    al = aliquot_sequence_length(6, 20);
    // EXPECT: Aliquot length of 6: 1
    printf("Aliquot length of 6: %d\n", al);

    return 0;
}
