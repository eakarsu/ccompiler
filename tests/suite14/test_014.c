int printf(const char *fmt, ...);

// Prime factorization

typedef struct {
    int factors[20];
    int exponents[20];
    int count;
} Factorization;

void factorize(int n, Factorization *f) {
    f->count = 0;
    int d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            f->factors[f->count] = d;
            f->exponents[f->count] = 0;
            while (n % d == 0) {
                f->exponents[f->count] = f->exponents[f->count] + 1;
                n = n / d;
            }
            f->count = f->count + 1;
        }
        d = d + 1;
    }
    if (n > 1) {
        f->factors[f->count] = n;
        f->exponents[f->count] = 1;
        f->count = f->count + 1;
    }
}

// Count total number of divisors from factorization
int count_divisors(Factorization *f) {
    int result = 1;
    int i;
    for (i = 0; i < f->count; i++) {
        result = result * (f->exponents[i] + 1);
    }
    return result;
}

// Sum of divisors from factorization
int sum_divisors(Factorization *f) {
    int result = 1;
    int i;
    for (i = 0; i < f->count; i++) {
        int term = 0;
        int power = 1;
        int j;
        for (j = 0; j <= f->exponents[i]; j++) {
            term = term + power;
            power = power * f->factors[i];
        }
        result = result * term;
    }
    return result;
}

// Reconstruct number from factorization
int reconstruct(Factorization *f) {
    int result = 1;
    int i;
    for (i = 0; i < f->count; i++) {
        int j;
        for (j = 0; j < f->exponents[i]; j++) {
            result = result * f->factors[i];
        }
    }
    return result;
}

// Check if number is a perfect square using factorization
int is_perfect_square(Factorization *f) {
    int i;
    for (i = 0; i < f->count; i++) {
        if (f->exponents[i] % 2 != 0) {
            return 0;
        }
    }
    return 1;
}

// Radical of n: product of distinct prime factors
int radical(Factorization *f) {
    int result = 1;
    int i;
    for (i = 0; i < f->count; i++) {
        result = result * f->factors[i];
    }
    return result;
}

// Omega function: number of distinct prime factors
int omega(Factorization *f) {
    return f->count;
}

// Big omega: total number of prime factors with multiplicity
int big_omega(Factorization *f) {
    int total = 0;
    int i;
    for (i = 0; i < f->count; i++) {
        total = total + f->exponents[i];
    }
    return total;
}

void print_factorization(int n, Factorization *f) {
    printf("%d = ", n);
    int i;
    for (i = 0; i < f->count; i++) {
        if (i > 0) {
            printf(" * ");
        }
        if (f->exponents[i] == 1) {
            printf("%d", f->factors[i]);
        } else {
            printf("%d^%d", f->factors[i], f->exponents[i]);
        }
    }
    printf("\n");
}

int main(void) {
    Factorization f;

    factorize(60, &f);
    print_factorization(60, &f);
    // EXPECT: 60 = 2^2 * 3 * 5

    printf("divisors(60): %d\n", count_divisors(&f));
    // EXPECT: divisors(60): 12

    printf("sum_div(60): %d\n", sum_divisors(&f));
    // EXPECT: sum_div(60): 168

    printf("reconstruct(60): %d\n", reconstruct(&f));
    // EXPECT: reconstruct(60): 60

    printf("perfect_sq(60): %d\n", is_perfect_square(&f));
    // EXPECT: perfect_sq(60): 0

    factorize(36, &f);
    print_factorization(36, &f);
    // EXPECT: 36 = 2^2 * 3^2

    printf("perfect_sq(36): %d\n", is_perfect_square(&f));
    // EXPECT: perfect_sq(36): 1

    printf("radical(36): %d\n", radical(&f));
    // EXPECT: radical(36): 6

    factorize(360, &f);
    print_factorization(360, &f);
    // EXPECT: 360 = 2^3 * 3^2 * 5

    printf("divisors(360): %d\n", count_divisors(&f));
    // EXPECT: divisors(360): 24

    printf("sum_div(360): %d\n", sum_divisors(&f));
    // EXPECT: sum_div(360): 1170

    printf("omega(360): %d\n", omega(&f));
    // EXPECT: omega(360): 3

    printf("big_omega(360): %d\n", big_omega(&f));
    // EXPECT: big_omega(360): 6

    factorize(97, &f);
    print_factorization(97, &f);
    // EXPECT: 97 = 97

    printf("divisors(97): %d\n", count_divisors(&f));
    // EXPECT: divisors(97): 2

    factorize(1024, &f);
    print_factorization(1024, &f);
    // EXPECT: 1024 = 2^10

    printf("radical(1024): %d\n", radical(&f));
    // EXPECT: radical(1024): 2

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
