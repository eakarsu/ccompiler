int printf(const char *fmt, ...);

// Amicable numbers and related concepts

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

int are_amicable(int a, int b) {
    if (a == b) return 0;
    if (sum_proper_divisors(a) == b && sum_proper_divisors(b) == a) {
        return 1;
    }
    return 0;
}

void find_amicable_pairs(int limit) {
    int i;
    int j;
    int count;
    count = 0;
    for (i = 2; i < limit; i++) {
        j = sum_proper_divisors(i);
        if (j > i && j < limit) {
            if (sum_proper_divisors(j) == i) {
                printf("Amicable pair: (%d, %d)\n", i, j);
                count = count + 1;
            }
        }
    }
    printf("Total amicable pairs found: %d\n", count);
}

int sum_amicable_under(int limit) {
    int sum;
    int i;
    int j;
    sum = 0;
    for (i = 2; i < limit; i++) {
        j = sum_proper_divisors(i);
        if (j != i && j < limit) {
            if (sum_proper_divisors(j) == i) {
                sum = sum + i;
            }
        }
    }
    return sum;
}

int is_sociable(int n, int order) {
    int current;
    int step;
    current = n;
    for (step = 0; step < order; step++) {
        current = sum_proper_divisors(current);
        if (current <= 0) return 0;
    }
    return (current == n) ? 1 : 0;
}

int classify_number(int n) {
    int s;
    s = sum_proper_divisors(n);
    if (s == n) return 0;
    if (s > n) return 1;
    return -1;
}

void print_aliquot_chain(int start, int max_steps) {
    int n;
    int step;
    n = start;
    printf("%d", n);
    for (step = 0; step < max_steps; step++) {
        n = sum_proper_divisors(n);
        printf(" -> %d", n);
        if (n == 0 || n == start) break;
    }
    printf("\n");
}

int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int is_coprime(int a, int b) {
    return (gcd(a, b) == 1) ? 1 : 0;
}

int digital_root(int n) {
    while (n >= 10) {
        int sum;
        sum = 0;
        while (n > 0) {
            sum = sum + n % 10;
            n = n / 10;
        }
        n = sum;
    }
    return n;
}

int digit_sum(int n) {
    int sum;
    sum = 0;
    while (n > 0) {
        sum = sum + n % 10;
        n = n / 10;
    }
    return sum;
}

int main() {
    int sa;
    int c;
    int dr;
    int ds;

    // EXPECT: Amicable pair check (220, 284): 1
    printf("Amicable pair check (220, 284): %d\n", are_amicable(220, 284));
    // EXPECT: Amicable pair check (220, 280): 0
    printf("Amicable pair check (220, 280): %d\n", are_amicable(220, 280));
    // EXPECT: Amicable pair check (1184, 1210): 1
    printf("Amicable pair check (1184, 1210): %d\n", are_amicable(1184, 1210));

    // EXPECT: sigma(220) = 284
    printf("sigma(220) = %d\n", sum_proper_divisors(220));
    // EXPECT: sigma(284) = 220
    printf("sigma(284) = %d\n", sum_proper_divisors(284));

    // EXPECT: Amicable pair: (220, 284)
    // EXPECT: Amicable pair: (1184, 1210)
    // EXPECT: Amicable pair: (2620, 2924)
    // EXPECT: Amicable pair: (5020, 5564)
    // EXPECT: Amicable pair: (6232, 6368)
    // EXPECT: Total amicable pairs found: 5
    find_amicable_pairs(10000);

    sa = sum_amicable_under(10000);
    // EXPECT: Sum of amicable numbers under 10000: 31626
    printf("Sum of amicable numbers under 10000: %d\n", sa);

    // EXPECT: 6 is sociable of order 1: 1
    printf("6 is sociable of order 1: %d\n", is_sociable(6, 1));
    // EXPECT: 220 is sociable of order 2: 1
    printf("220 is sociable of order 2: %d\n", is_sociable(220, 2));

    c = classify_number(6);
    // EXPECT: Classify 6: 0
    printf("Classify 6: %d\n", c);
    c = classify_number(12);
    // EXPECT: Classify 12: 1
    printf("Classify 12: %d\n", c);
    c = classify_number(8);
    // EXPECT: Classify 8: -1
    printf("Classify 8: %d\n", c);

    // EXPECT: Aliquot chain from 220: 220 -> 284 -> 220
    printf("Aliquot chain from 220: ");
    print_aliquot_chain(220, 10);

    // EXPECT: Aliquot chain from 12: 12 -> 16 -> 15 -> 9 -> 4 -> 3 -> 1 -> 0
    printf("Aliquot chain from 12: ");
    print_aliquot_chain(12, 10);

    // EXPECT: Coprime(220, 284): 0
    printf("Coprime(220, 284): %d\n", is_coprime(220, 284));
    // EXPECT: Coprime(7, 13): 1
    printf("Coprime(7, 13): %d\n", is_coprime(7, 13));

    dr = digital_root(220);
    // EXPECT: Digital root of 220: 4
    printf("Digital root of 220: %d\n", dr);

    ds = digit_sum(284);
    // EXPECT: Digit sum of 284: 14
    printf("Digit sum of 284: %d\n", ds);

    return 0;
}
