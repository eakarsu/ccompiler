int printf(const char *fmt, ...);
// EXPECT: Data: 3 7 2 8 5 1 9 4 6 10 \nPrefix sums: 3 10 12 20 25 26 35 39 45 55 \nRunning max: 3 7 7 8 8 8 9 9 9 10 \nRunning min: 3 3 2 2 2 1 1 1 1 1 \nSuffix sums: 55 52 45 43 35 30 29 20 16 10 \nRunning average (x10): 30 50 40 50 50 43 50 48 50 55 \nRange sums [2..5]=16 [0..9]=55 [4..7]=19\nMax subarray (Kadane): 55
int main(void) {
    int data[10];
    int i;
    data[0] = 3; data[1] = 7; data[2] = 2; data[3] = 8; data[4] = 5;
    data[5] = 1; data[6] = 9; data[7] = 4; data[8] = 6; data[9] = 10;

    printf("Data: ");
    for (i = 0; i < 10; i++) printf("%d ", data[i]);
    printf("\n");

    printf("Prefix sums: ");
    int prefix[10];
    prefix[0] = data[0];
    for (i = 1; i < 10; i++) {
        prefix[i] = prefix[i - 1] + data[i];
    }
    for (i = 0; i < 10; i++) printf("%d ", prefix[i]);
    printf("\n");

    printf("Running max: ");
    int running_max[10];
    running_max[0] = data[0];
    for (i = 1; i < 10; i++) {
        if (data[i] > running_max[i - 1]) {
            running_max[i] = data[i];
        } else {
            running_max[i] = running_max[i - 1];
        }
    }
    for (i = 0; i < 10; i++) printf("%d ", running_max[i]);
    printf("\n");

    printf("Running min: ");
    int running_min[10];
    running_min[0] = data[0];
    for (i = 1; i < 10; i++) {
        if (data[i] < running_min[i - 1]) {
            running_min[i] = data[i];
        } else {
            running_min[i] = running_min[i - 1];
        }
    }
    for (i = 0; i < 10; i++) printf("%d ", running_min[i]);
    printf("\n");

    printf("Suffix sums: ");
    int suffix[10];
    suffix[9] = data[9];
    for (i = 8; i >= 0; i--) {
        suffix[i] = suffix[i + 1] + data[i];
    }
    for (i = 0; i < 10; i++) printf("%d ", suffix[i]);
    printf("\n");

    printf("Running average (x10): ");
    int sum = 0;
    for (i = 0; i < 10; i++) {
        sum = sum + data[i];
        printf("%d ", (sum * 10) / (i + 1));
    }
    printf("\n");

    printf("Range sums [2..5]=%d [0..9]=%d [4..7]=%d\n",
           prefix[5] - prefix[1],
           prefix[9],
           prefix[7] - prefix[3]);

    printf("Max subarray (Kadane): ");
    int max_here = data[0];
    int max_so_far = data[0];
    for (i = 1; i < 10; i++) {
        if (max_here + data[i] > data[i]) {
            max_here = max_here + data[i];
        } else {
            max_here = data[i];
        }
        if (max_here > max_so_far) {
            max_so_far = max_here;
        }
    }
    printf("%d\n", max_so_far);

    return 0;
}
