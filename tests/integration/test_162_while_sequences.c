int printf(const char *fmt, ...);
// EXPECT: Fibonacci: 0 1 1 2 3 5 8 13 21 34 55 89 \nCollatz(27): 27 82 41 124 62 31 94 47 142 71 214 107 322 161 484 242 121 364 182 91 274 137 412 206 103 310 155 466 233 700 350 175 526 263 790 395 1186 593 1780 890 445 1336 668 334 167 502 251 754 377 1132 566 1\nsteps=51\nTriangular: 1 3 6 10 15 21 28 36 45 55 66 78 91 105 \nPowers of 3: 1 3 9 27 81 243 729 2187 6561 \nFactorials: 1 2 6 24 120 720 5040 40320 362880 3628800 \nPentagonal: 1 5 12 22 35 51 70 92 
int main(void) {
    int i;

    printf("Fibonacci: ");
    int a = 0;
    int b = 1;
    i = 0;
    while (i < 12) {
        printf("%d ", a);
        int temp = a + b;
        a = b;
        b = temp;
        i++;
    }
    printf("\n");

    printf("Collatz(27): ");
    int n = 27;
    int steps = 0;
    while (n != 1) {
        printf("%d ", n);
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        steps++;
        if (steps > 50) break;
    }
    printf("1\nsteps=%d\n", steps);

    printf("Triangular: ");
    int sum = 0;
    i = 1;
    while (sum < 100) {
        sum = sum + i;
        printf("%d ", sum);
        i++;
    }
    printf("\n");

    printf("Powers of 3: ");
    int p = 1;
    while (p < 10000) {
        printf("%d ", p);
        p = p * 3;
    }
    printf("\n");

    printf("Factorials: ");
    int fact = 1;
    i = 1;
    while (i <= 10) {
        fact = fact * i;
        printf("%d ", fact);
        i++;
    }
    printf("\n");

    printf("Pentagonal: ");
    i = 1;
    while (i <= 8) {
        int pent = i * (3 * i - 1) / 2;
        printf("%d ", pent);
        i++;
    }
    printf("\n");

    return 0;
}
