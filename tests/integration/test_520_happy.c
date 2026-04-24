int printf(const char *fmt, ...);
// EXPECT: Happy numbers up to 100:\n1 7 10 13 19 23 28 31 32 44 49 68 70 79 82 86 91 94 97 100 \nCount: 20\n\nIteration sequences:\n7: 49 97 130 10 1 (happy in 5 steps)\n13: 10 1 (happy in 2 steps)\n4: 16 37 58 89 145 42 20 4 16 37 58 89 145 42 20 4 16 37 58 89 (sad, cycle)\n19: 82 68 100 1 (happy in 4 steps)\n2: 4 16 37 58 89 145 42 20 4 16 37 58 89 145 42 20 4 16 37 58 (sad, cycle)\n44: 32 13 10 1 (happy in 4 steps)\n\nUnhappy cycle:\n4 -> 16 -> 37 -> 58 -> 89 -> 145 -> 42 -> 20 -> 4\n\nHappy primes up to 100:\n7 13 19 23 31 79 97 \n\nHappy number density:\nHappy numbers up to 10: 3\nHappy numbers up to 50: 11\nHappy numbers up to 100: 20\nHappy numbers up to 200: 32\nHappy numbers up to 500: 76
// Test: Happy numbers

int digit_sq_sum(int n) {
    int sum = 0;
    while (n > 0) {
        int d = n % 10;
        sum = sum + d * d;
        n = n / 10;
    }
    return sum;
}

int is_happy(int n) {
    // Use Floyd's cycle detection
    int slow = n;
    int fast = n;
    do {
        slow = digit_sq_sum(slow);
        fast = digit_sq_sum(digit_sq_sum(fast));
    } while (slow != fast);
    return slow == 1;
}

int main(void) {
    int i;

    printf("Happy numbers up to 100:\n");
    int happy_count = 0;
    for (i = 1; i <= 100; i++) {
        if (is_happy(i)) {
            printf("%d ", i);
            happy_count++;
        }
    }
    printf("\nCount: %d\n", happy_count);

    // Show the iteration sequence for a few numbers
    printf("\nIteration sequences:\n");
    int test_nums[6];
    test_nums[0] = 7;
    test_nums[1] = 13;
    test_nums[2] = 4;
    test_nums[3] = 19;
    test_nums[4] = 2;
    test_nums[5] = 44;

    for (i = 0; i < 6; i++) {
        int n = test_nums[i];
        printf("%d: ", n);
        int current = n;
        int steps = 0;
        while (current != 1 && steps < 20) {
            current = digit_sq_sum(current);
            printf("%d ", current);
            steps++;
        }
        if (current == 1) printf("(happy in %d steps)", steps);
        else printf("(sad, cycle)");
        printf("\n");
    }

    // Unhappy numbers cycle: 4 -> 16 -> 37 -> 58 -> 89 -> 145 -> 42 -> 20 -> 4
    printf("\nUnhappy cycle:\n");
    int n = 4;
    printf("%d", n);
    for (i = 0; i < 8; i++) {
        n = digit_sq_sum(n);
        printf(" -> %d", n);
    }
    printf("\n");

    // Happy primes
    printf("\nHappy primes up to 100:\n");
    for (i = 2; i <= 100; i++) {
        int is_prime = 1;
        int j;
        if (i < 2) is_prime = 0;
        for (j = 2; j * j <= i; j++) {
            if (i % j == 0) { is_prime = 0; break; }
        }
        if (is_prime && is_happy(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");

    // Count happy numbers in ranges
    printf("\nHappy number density:\n");
    int ranges[5];
    ranges[0] = 10; ranges[1] = 50; ranges[2] = 100; ranges[3] = 200; ranges[4] = 500;
    for (i = 0; i < 5; i++) {
        int count = 0;
        int j;
        for (j = 1; j <= ranges[i]; j++) {
            if (is_happy(j)) count++;
        }
        printf("Happy numbers up to %d: %d\n", ranges[i], count);
    }

    return 0;
}
