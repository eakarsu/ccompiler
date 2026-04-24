int printf(const char *fmt, ...);
// EXPECT: data: 42 17 93 8 56 31 74 65 29 50 \nsum=465 min=8 max=93 avg=46\nrange = 85\nabove avg: 5\nvariance*10 = 6285\nreversed: 50 29 65 74 31 56 8 93 17 42 \nrotated: 74 31 56 8 93 17 42 50 29 65 \nunique = 10
// Test: array-based mathematical algorithms

int main(void) {
    int arr[10];
    int i, j;
    int n;
    int sum, min_val, max_val;

    /* Initialize */
    arr[0] = 42; arr[1] = 17; arr[2] = 93; arr[3] = 8;
    arr[4] = 56; arr[5] = 31; arr[6] = 74; arr[7] = 65;
    arr[8] = 29; arr[9] = 50;
    n = 10;

    /* Print original */
    printf("data: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    /* Sum, min, max */
    sum = 0;
    min_val = arr[0];
    max_val = arr[0];
    for (i = 0; i < n; i++) {
        sum += arr[i];
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
    }
    printf("sum=%d min=%d max=%d avg=%d\n", sum, min_val, max_val, sum / n);

    /* Range */
    printf("range = %d\n", max_val - min_val);

    /* Count elements above average */
    {
        int avg, above;
        avg = sum / n;
        above = 0;
        for (i = 0; i < n; i++) {
            if (arr[i] > avg) above++;
        }
        printf("above avg: %d\n", above);
    }

    /* Variance (scaled by n, integer) */
    {
        int avg, var;
        avg = sum / n;
        var = 0;
        for (i = 0; i < n; i++) {
            int diff;
            diff = arr[i] - avg;
            var += diff * diff;
        }
        printf("variance*%d = %d\n", n, var);
    }

    /* Reverse array in-place */
    for (i = 0; i < n / 2; i++) {
        int temp;
        temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
    printf("reversed: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    /* Rotate left by 3 */
    {
        int temp_arr[10];
        int shift;
        shift = 3;
        for (i = 0; i < n; i++) {
            temp_arr[i] = arr[(i + shift) % n];
        }
        for (i = 0; i < n; i++) {
            arr[i] = temp_arr[i];
        }
    }
    printf("rotated: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    /* Unique count (brute force) */
    {
        int unique;
        unique = 0;
        for (i = 0; i < n; i++) {
            int found;
            found = 0;
            for (j = 0; j < i; j++) {
                if (arr[j] == arr[i]) {
                    found = 1;
                    break;
                }
            }
            if (!found) unique++;
        }
        printf("unique = %d\n", unique);
    }

    return 0;
}
