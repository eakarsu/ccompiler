int printf(const char *fmt, ...);
// EXPECT: Collatz sequences:\n6: 3 10 5 16 8 4 2 1 \n11: 34 17 52 26 13 40 20 10 5 16 8 4 2 1 \n27: 82 41 124 62 31 94 47 142 71 214 107 322 161 484 242 121 364 182 91 274 137 412 206 103 310 155 466 233 700 350 ...\n15: 46 23 70 35 106 53 160 80 40 20 10 5 16 8 4 2 1 \n97: 292 146 73 220 110 55 166 83 250 125 376 188 94 47 142 71 214 107 322 161 484 242 121 364 182 91 274 137 412 206 ...\n\nSteps to reach 1:\nn=1: 0 steps\nn=2: 1 steps\nn=3: 7 steps\nn=4: 2 steps\nn=5: 5 steps\nn=6: 8 steps\nn=7: 16 steps\nn=8: 3 steps\nn=9: 19 steps\nn=10: 6 steps\nn=11: 14 steps\nn=12: 9 steps\nn=13: 9 steps\nn=14: 17 steps\nn=15: 17 steps\nn=16: 4 steps\nn=17: 12 steps\nn=18: 20 steps\nn=19: 20 steps\nn=20: 7 steps\nn=21: 7 steps\nn=22: 15 steps\nn=23: 15 steps\nn=24: 10 steps\nn=25: 23 steps\nn=26: 10 steps\nn=27: 111 steps\nn=28: 18 steps\nn=29: 18 steps\nn=30: 18 steps\n\nMaximum value in sequence:\nn=1: max=1\nn=2: max=2\nn=3: max=16\nn=4: max=4\nn=5: max=16\nn=6: max=16\nn=7: max=52\nn=8: max=8\nn=9: max=52\nn=10: max=16\nn=11: max=52\nn=12: max=16\nn=13: max=40\nn=14: max=52\nn=15: max=160\nn=16: max=16\nn=17: max=52\nn=18: max=52\nn=19: max=88\nn=20: max=20\n\nRecord holders (longest sequences up to 100):\nn=2: 1 steps\nn=3: 7 steps\nn=6: 8 steps\nn=7: 16 steps\nn=9: 19 steps\nn=18: 20 steps\nn=25: 23 steps\nn=27: 111 steps\nn=54: 112 steps\nn=73: 115 steps\nn=97: 118 steps\n\nStep count distribution (1-50):\n0-9 steps: 16 numbers\n10-19 steps: 19 numbers\n20-29 steps: 10 numbers\n30-39 steps: 1 numbers\n100-109 steps: 3 numbers\n110-119 steps: 1 numbers
// Test: Collatz conjecture analysis

int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0)
            n = n / 2;
        else
            n = 3 * n + 1;
        steps++;
    }
    return steps;
}

int collatz_max(int n) {
    int max = n;
    while (n != 1) {
        if (n % 2 == 0)
            n = n / 2;
        else
            n = 3 * n + 1;
        if (n > max) max = n;
    }
    return max;
}

int main(void) {
    int i;

    // Collatz sequence for specific numbers
    printf("Collatz sequences:\n");
    int starts[5];
    starts[0] = 6;
    starts[1] = 11;
    starts[2] = 27;
    starts[3] = 15;
    starts[4] = 97;

    for (i = 0; i < 5; i++) {
        int n = starts[i];
        printf("%d: ", n);
        int steps = 0;
        while (n != 1 && steps < 30) {
            if (n % 2 == 0)
                n = n / 2;
            else
                n = 3 * n + 1;
            printf("%d ", n);
            steps++;
        }
        if (n != 1) printf("...");
        printf("\n");
    }

    // Steps to reach 1
    printf("\nSteps to reach 1:\n");
    for (i = 1; i <= 30; i++) {
        printf("n=%d: %d steps\n", i, collatz_steps(i));
    }

    // Maximum value reached
    printf("\nMaximum value in sequence:\n");
    for (i = 1; i <= 20; i++) {
        printf("n=%d: max=%d\n", i, collatz_max(i));
    }

    // Numbers with longest sequences up to 100
    printf("\nRecord holders (longest sequences up to 100):\n");
    int record = 0;
    for (i = 1; i <= 100; i++) {
        int s = collatz_steps(i);
        if (s > record) {
            record = s;
            printf("n=%d: %d steps\n", i, s);
        }
    }

    // Histogram of step counts for 1-50
    printf("\nStep count distribution (1-50):\n");
    int hist[20];
    for (i = 0; i < 20; i++) hist[i] = 0;
    for (i = 1; i <= 50; i++) {
        int s = collatz_steps(i);
        int bucket = s / 10;
        if (bucket >= 20) bucket = 19;
        hist[bucket]++;
    }
    for (i = 0; i < 15; i++) {
        if (hist[i] > 0) {
            printf("%d-%d steps: %d numbers\n", i * 10, i * 10 + 9, hist[i]);
        }
    }

    return 0;
}
