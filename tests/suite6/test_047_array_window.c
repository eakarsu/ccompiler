int printf(const char *fmt, ...);

void print_arr(int *a, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", a[i]);
    }
    printf("\n");
}

void sliding_window_max(int *a, int n, int k, int *out, int *out_len) {
    int i;
    int j;
    int mx;
    int idx = 0;
    for (i = 0; i <= n - k; i++) {
        mx = a[i];
        for (j = i + 1; j < i + k; j++) {
            if (a[j] > mx) mx = a[j];
        }
        out[idx] = mx;
        idx++;
    }
    *out_len = idx;
}

void sliding_window_min(int *a, int n, int k, int *out, int *out_len) {
    int i;
    int j;
    int mn;
    int idx = 0;
    for (i = 0; i <= n - k; i++) {
        mn = a[i];
        for (j = i + 1; j < i + k; j++) {
            if (a[j] < mn) mn = a[j];
        }
        out[idx] = mn;
        idx++;
    }
    *out_len = idx;
}

void sliding_window_sum(int *a, int n, int k, int *out, int *out_len) {
    int i;
    int sum = 0;
    int idx = 0;
    for (i = 0; i < k; i++) {
        sum = sum + a[i];
    }
    out[idx] = sum;
    idx++;
    for (i = k; i < n; i++) {
        sum = sum + a[i] - a[i - k];
        out[idx] = sum;
        idx++;
    }
    *out_len = idx;
}

void fixed_window_average(int *a, int n, int k, int *out, int *out_len) {
    int i;
    int sum = 0;
    int idx = 0;
    for (i = 0; i < k; i++) {
        sum = sum + a[i];
    }
    out[idx] = sum / k;
    idx++;
    for (i = k; i < n; i++) {
        sum = sum + a[i] - a[i - k];
        out[idx] = sum / k;
        idx++;
    }
    *out_len = idx;
}

int max_sum_subarray_k(int *a, int n, int k) {
    int i;
    int sum = 0;
    int max_sum;
    for (i = 0; i < k; i++) {
        sum = sum + a[i];
    }
    max_sum = sum;
    for (i = k; i < n; i++) {
        sum = sum + a[i] - a[i - k];
        if (sum > max_sum) max_sum = sum;
    }
    return max_sum;
}

int min_sum_subarray_k(int *a, int n, int k) {
    int i;
    int sum = 0;
    int min_sum;
    for (i = 0; i < k; i++) {
        sum = sum + a[i];
    }
    min_sum = sum;
    for (i = k; i < n; i++) {
        sum = sum + a[i] - a[i - k];
        if (sum < min_sum) min_sum = sum;
    }
    return min_sum;
}

int main(void) {
    int a[8];
    int out[20];
    int len;

    a[0] = 1; a[1] = 3; a[2] = -1; a[3] = -3;
    a[4] = 5; a[5] = 3; a[6] = 6; a[7] = 7;

    sliding_window_max(a, 8, 3, out, &len);
    // EXPECT: win_max_len=6
    printf("win_max_len=%d\n", len);
    // EXPECT: 3 3 5 5 6 7
    print_arr(out, len);

    sliding_window_min(a, 8, 3, out, &len);
    // EXPECT: -1 -3 -3 -3 3 3
    print_arr(out, len);

    sliding_window_sum(a, 8, 3, out, &len);
    // EXPECT: 3 -1 1 5 14 16
    print_arr(out, len);

    fixed_window_average(a, 8, 3, out, &len);
    // EXPECT: 1 0 0 1 4 5
    print_arr(out, len);

    int b[6];
    b[0] = 2; b[1] = 1; b[2] = 5; b[3] = 1; b[4] = 3; b[5] = 2;
    int ms = max_sum_subarray_k(b, 6, 3);
    // EXPECT: max_k3=9
    printf("max_k3=%d\n", ms);

    ms = max_sum_subarray_k(b, 6, 2);
    // EXPECT: max_k2=6
    printf("max_k2=%d\n", ms);

    int mins = min_sum_subarray_k(b, 6, 3);
    // EXPECT: min_k3=6
    printf("min_k3=%d\n", mins);

    sliding_window_max(b, 6, 1, out, &len);
    // EXPECT: 2 1 5 1 3 2
    print_arr(out, len);

    sliding_window_max(b, 6, 6, out, &len);
    // EXPECT: full_max=5
    printf("full_max=%d\n", out[0]);

    int c[5];
    c[0] = 10; c[1] = 20; c[2] = 30; c[3] = 40; c[4] = 50;
    sliding_window_sum(c, 5, 2, out, &len);
    // EXPECT: 30 50 70 90
    print_arr(out, len);

    fixed_window_average(c, 5, 2, out, &len);
    // EXPECT: 15 25 35 45
    print_arr(out, len);

    int d[4];
    d[0] = 100; d[1] = 200; d[2] = 300; d[3] = 400;
    fixed_window_average(d, 4, 4, out, &len);
    // EXPECT: avg_all=250
    printf("avg_all=%d\n", out[0]);

    ms = max_sum_subarray_k(d, 4, 4);
    // EXPECT: max_all=1000
    printf("max_all=%d\n", ms);

    // EXPECT: done
    printf("done\n");

    return 0;
}
