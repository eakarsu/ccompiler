int printf(const char *fmt, ...);

void skip_multiples() {
    int i;
    int j;
    int count = 0;
    for (i = 1; i <= 10; i = i + 1) {
        if (i % 3 == 0) {
            continue;
        }
        for (j = 1; j <= 10; j = j + 1) {
            if (j % 2 == 0) {
                continue;
            }
            count = count + 1;
        }
    }
    printf("skip multiples count: %d\n", count);
}

void continue_in_while() {
    int i = 0;
    int sum = 0;
    while (i < 50) {
        i = i + 1;
        if (i % 2 == 0) {
            continue;
        }
        if (i % 5 == 0) {
            continue;
        }
        sum = sum + i;
    }
    printf("odd non-mult5 sum: %d\n", sum);
}

void continue_in_do_while() {
    int n = 1;
    int product = 1;
    int skipped = 0;
    do {
        if (n % 4 == 0) {
            n = n + 1;
            skipped = skipped + 1;
            continue;
        }
        product = product * n;
        if (product > 1000) {
            product = product / n;
            break;
        }
        n = n + 1;
    } while (n <= 20);
    printf("do-while product: %d\n", product);
    printf("do-while skipped: %d\n", skipped);
}

void nested_continue_pattern() {
    int i;
    int j;
    int k;
    int count = 0;
    for (i = 0; i < 6; i = i + 1) {
        if (i == 3) continue;
        for (j = 0; j < 6; j = j + 1) {
            if (j == i) continue;
            for (k = 0; k < 6; k = k + 1) {
                if (k == i || k == j) continue;
                if (i + j + k == 8) {
                    count = count + 1;
                }
            }
        }
    }
    printf("triple no-repeat sum8: %d\n", count);
}

void continue_with_switch() {
    int i;
    int total = 0;
    for (i = 0; i < 20; i = i + 1) {
        switch (i % 4) {
            case 0:
                total = total + i;
                break;
            case 1:
                continue;
            case 2:
                total = total + i * 2;
                break;
            case 3:
                continue;
        }
        total = total + 1;
    }
    printf("continue with switch: %d\n", total);
}

void sieve_continue() {
    int primes[50];
    int i;
    int j;
    for (i = 0; i < 50; i = i + 1) {
        primes[i] = 1;
    }
    primes[0] = 0;
    primes[1] = 0;
    for (i = 2; i < 50; i = i + 1) {
        if (!primes[i]) continue;
        for (j = i * 2; j < 50; j = j + i) {
            primes[j] = 0;
        }
    }
    int count = 0;
    int sum = 0;
    for (i = 2; i < 50; i = i + 1) {
        if (!primes[i]) continue;
        count = count + 1;
        sum = sum + i;
    }
    printf("primes under 50: %d\n", count);
    printf("prime sum: %d\n", sum);
}

int main() {
    // EXPECT: skip multiples count: 35
    skip_multiples();
    // EXPECT: odd non-mult5 sum: 500
    continue_in_while();
    // EXPECT: do-while product: 180
    // EXPECT: do-while skipped: 1
    continue_in_do_while();
    // EXPECT: triple no-repeat sum8: 14
    nested_continue_pattern();
    // EXPECT: continue with switch: 150
    continue_with_switch();
    // EXPECT: primes under 50: 15
    // EXPECT: prime sum: 328
    sieve_continue();
    return 0;
}
