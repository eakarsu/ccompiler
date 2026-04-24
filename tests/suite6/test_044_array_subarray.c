int printf(const char *fmt, ...);

int kadane_max_subarray(int *a, int n) {
    int max_ending = a[0];
    int max_so_far = a[0];
    int i;
    for (i = 1; i < n; i++) {
        if (max_ending + a[i] > a[i]) {
            max_ending = max_ending + a[i];
        } else {
            max_ending = a[i];
        }
        if (max_ending > max_so_far) {
            max_so_far = max_ending;
        }
    }
    return max_so_far;
}

int kadane_with_indices(int *a, int n, int *start_out, int *end_out) {
    int max_ending = a[0];
    int max_so_far = a[0];
    int s = 0;
    int best_s = 0;
    int best_e = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (max_ending + a[i] > a[i]) {
            max_ending = max_ending + a[i];
        } else {
            max_ending = a[i];
            s = i;
        }
        if (max_ending > max_so_far) {
            max_so_far = max_ending;
            best_s = s;
            best_e = i;
        }
    }
    *start_out = best_s;
    *end_out = best_e;
    return max_so_far;
}

int subarray_with_sum(int *a, int n, int target, int *start_out, int *end_out) {
    int sum;
    int start;
    int end;
    start = 0;
    sum = 0;
    for (end = 0; end < n; end++) {
        sum = sum + a[end];
        while (sum > target && start < end) {
            sum = sum - a[start];
            start++;
        }
        if (sum == target) {
            *start_out = start;
            *end_out = end;
            return 1;
        }
    }
    return 0;
}

int lis_length(int *a, int n) {
    int dp[20];
    int i;
    int j;
    int max_len;
    for (i = 0; i < n; i++) {
        dp[i] = 1;
    }
    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (a[j] < a[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
    }
    max_len = 1;
    for (i = 0; i < n; i++) {
        if (dp[i] > max_len) {
            max_len = dp[i];
        }
    }
    return max_len;
}

int main(void) {
    int a[9];
    int start;
    int end;
    int result;

    a[0] = -2; a[1] = 1; a[2] = -3; a[3] = 4;
    a[4] = -1; a[5] = 2; a[6] = 1; a[7] = -5; a[8] = 4;
    result = kadane_max_subarray(a, 9);
    // EXPECT: max_subarray=6
    printf("max_subarray=%d\n", result);

    result = kadane_with_indices(a, 9, &start, &end);
    // EXPECT: max=6 start=3 end=6
    printf("max=%d start=%d end=%d\n", result, start, end);

    int b[5];
    b[0] = -1; b[1] = -2; b[2] = -3; b[3] = -4; b[4] = -5;
    result = kadane_max_subarray(b, 5);
    // EXPECT: all_neg=-1
    printf("all_neg=%d\n", result);

    int c[4];
    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 4;
    result = kadane_max_subarray(c, 4);
    // EXPECT: all_pos=10
    printf("all_pos=%d\n", result);

    int d[1];
    d[0] = 42;
    result = kadane_max_subarray(d, 1);
    // EXPECT: single=42
    printf("single=%d\n", result);

    int e[8];
    e[0] = 1; e[1] = 4; e[2] = 20; e[3] = 3;
    e[4] = 10; e[5] = 5; e[6] = 2; e[7] = 8;
    int found;
    found = subarray_with_sum(e, 8, 33, &start, &end);
    // EXPECT: found=1 s=2 e=4
    printf("found=%d s=%d e=%d\n", found, start, end);

    found = subarray_with_sum(e, 8, 1, &start, &end);
    // EXPECT: found_1=1 s=0 e=0
    printf("found_1=%d s=%d e=%d\n", found, start, end);

    found = subarray_with_sum(e, 8, 100, &start, &end);
    // EXPECT: not_found=0
    printf("not_found=%d\n", found);

    found = subarray_with_sum(e, 8, 15, &start, &end);
    // EXPECT: sum15=1
    printf("sum15=%d\n", found);

    int f[8];
    f[0] = 10; f[1] = 22; f[2] = 9; f[3] = 33;
    f[4] = 21; f[5] = 50; f[6] = 41; f[7] = 60;
    result = lis_length(f, 8);
    // EXPECT: lis=5
    printf("lis=%d\n", result);

    int g[6];
    g[0] = 5; g[1] = 4; g[2] = 3; g[3] = 2; g[4] = 1; g[5] = 0;
    result = lis_length(g, 6);
    // EXPECT: lis_dec=1
    printf("lis_dec=%d\n", result);

    int h[6];
    h[0] = 1; h[1] = 2; h[2] = 3; h[3] = 4; h[4] = 5; h[5] = 6;
    result = lis_length(h, 6);
    // EXPECT: lis_inc=6
    printf("lis_inc=%d\n", result);

    int k[7];
    k[0] = 3; k[1] = 1; k[2] = 4; k[3] = 1; k[4] = 5; k[5] = 9; k[6] = 2;
    result = lis_length(k, 7);
    // EXPECT: lis_pi=4
    printf("lis_pi=%d\n", result);

    int m[5];
    m[0] = 2; m[1] = 5; m[2] = 3; m[3] = 7; m[4] = 11;
    result = kadane_max_subarray(m, 5);
    // EXPECT: kadane5=28
    printf("kadane5=%d\n", result);

    // EXPECT: done
    printf("done\n");

    return 0;
}
