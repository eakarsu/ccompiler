int printf(const char *fmt, ...);
// EXPECT: sum 1..100 = 5050\n8! = 40320\nprefix sums: 3 9 18 30 45 63 84 108 135 165 \nrunning max: 3 7 7 9 9 9 9 10 10 10 \nevens=10 odds=10\nweighted sum = 1155\nXOR 1..10 = 11\nalternating sum = -50
// Test: accumulator computation patterns

int main(void) {
    int i;
    int acc;
    int arr[10];
    int prod;
    int running[10];

    /* Sum accumulator */
    acc = 0;
    for (i = 1; i <= 100; i++) {
        acc += i;
    }
    printf("sum 1..100 = %d\n", acc);

    /* Product accumulator */
    prod = 1;
    for (i = 1; i <= 8; i++) {
        prod *= i;
    }
    printf("8! = %d\n", prod);

    /* Fill array */
    for (i = 0; i < 10; i++) {
        arr[i] = (i + 1) * 3;
    }

    /* Prefix sum */
    running[0] = arr[0];
    for (i = 1; i < 10; i++) {
        running[i] = running[i - 1] + arr[i];
    }
    printf("prefix sums: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", running[i]);
    }
    printf("\n");

    /* Running maximum */
    {
        int max_so_far;
        int test_arr[10];
        test_arr[0] = 3; test_arr[1] = 7; test_arr[2] = 2;
        test_arr[3] = 9; test_arr[4] = 1; test_arr[5] = 8;
        test_arr[6] = 4; test_arr[7] = 10; test_arr[8] = 5;
        test_arr[9] = 6;
        max_so_far = test_arr[0];
        printf("running max: ");
        for (i = 0; i < 10; i++) {
            if (test_arr[i] > max_so_far) {
                max_so_far = test_arr[i];
            }
            printf("%d ", max_so_far);
        }
        printf("\n");
    }

    /* Count evens and odds */
    {
        int evens, odds;
        evens = 0;
        odds = 0;
        for (i = 1; i <= 20; i++) {
            if (i % 2 == 0) evens++;
            else odds++;
        }
        printf("evens=%d odds=%d\n", evens, odds);
    }

    /* Weighted accumulator */
    acc = 0;
    for (i = 0; i < 10; i++) {
        acc += arr[i] * (i + 1);
    }
    printf("weighted sum = %d\n", acc);

    /* XOR accumulator */
    acc = 0;
    for (i = 1; i <= 10; i++) {
        acc ^= i;
    }
    printf("XOR 1..10 = %d\n", acc);

    /* Sum of alternating signs: 1 - 2 + 3 - 4 + ... + 99 - 100 */
    acc = 0;
    for (i = 1; i <= 100; i++) {
        if (i % 2 == 1) {
            acc += i;
        } else {
            acc -= i;
        }
    }
    printf("alternating sum = %d\n", acc);

    return 0;
}
