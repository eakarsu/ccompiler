int printf(const char *fmt, ...);
// EXPECT: Simple countdown:\n10 9 8 7 6 5 4 3 2 1 0 \nStep countdown:\n100 85 70 55 40 25 10 \nHalving countdown:\n1024 512 256 128 64 32 16 8 4 2 1 \nAlternating countdown:\n20 1 18 3 16 5 14 7 12 9 \nFactorial countdown:\n10:10 9:90 8:720 7:5040 6:30240 5:151200 4:604800 3:1814400 2:3628800 1:3628800 \nDigit extraction (reverse):\n12345 -> 5 4 3 2 1 \n9876 -> 6 7 8 9 \n100 -> 0 0 1 \n7 -> 7 \nCountdown sum:\nsum(50..1)=1275\nGeometric countdown: 10000 6666 4444 2962 1974 1316 877 584 389 259 172 114 76 50 33 22 14 9 6 4 2 1 
int main(void) {
    int i;

    printf("Simple countdown:\n");
    for (i = 10; i >= 0; i--) {
        printf("%d ", i);
    }
    printf("\n");

    printf("Step countdown:\n");
    for (i = 100; i >= 0; i = i - 15) {
        printf("%d ", i);
    }
    printf("\n");

    printf("Halving countdown:\n");
    i = 1024;
    while (i >= 1) {
        printf("%d ", i);
        i = i / 2;
    }
    printf("\n");

    printf("Alternating countdown:\n");
    int a = 20;
    int b = 1;
    while (a >= b) {
        printf("%d %d ", a, b);
        a = a - 2;
        b = b + 2;
    }
    printf("\n");

    printf("Factorial countdown:\n");
    int fact = 1;
    for (i = 10; i >= 1; i--) {
        fact = fact * i;
        printf("%d:%d ", i, fact);
    }
    printf("\n");

    printf("Digit extraction (reverse):\n");
    int nums[4];
    nums[0] = 12345;
    nums[1] = 9876;
    nums[2] = 100;
    nums[3] = 7;
    int j;
    for (j = 0; j < 4; j++) {
        int n = nums[j];
        printf("%d -> ", n);
        while (n > 0) {
            printf("%d ", n % 10);
            n = n / 10;
        }
        printf("\n");
    }

    printf("Countdown sum:\n");
    int sum = 0;
    for (i = 50; i > 0; i--) {
        sum = sum + i;
    }
    printf("sum(50..1)=%d\n", sum);

    printf("Geometric countdown: ");
    int val = 10000;
    while (val > 0) {
        printf("%d ", val);
        val = val * 2 / 3;
    }
    printf("\n");

    return 0;
}
