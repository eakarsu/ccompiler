int printf(const char *fmt, ...);

int lcg_next(int state) {
    int a = 1103;
    int c = 12345;
    int m = 100000;
    return (state * a + c) % m;
}

int lcg_range(int state, int lo, int hi) {
    int range = hi - lo + 1;
    return lo + (state % range);
}

int my_abs(int x) {
    if (x < 0) return -x;
    return x;
}

int mean(int arr[], int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sum + arr[i];
    }
    return sum / n;
}

int min_val(int arr[], int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] < m) m = arr[i];
    }
    return m;
}

int max_val(int arr[], int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] > m) m = arr[i];
    }
    return m;
}

int range_val(int arr[], int n) {
    return max_val(arr, n) - min_val(arr, n);
}

int variance_times_n(int arr[], int n) {
    int m = mean(arr, n);
    int sum_sq = 0;
    int i;
    for (i = 0; i < n; i++) {
        int diff = arr[i] - m;
        sum_sq = sum_sq + diff * diff;
    }
    return sum_sq;
}

int median_of_three(int a, int b, int c) {
    if ((a >= b && a <= c) || (a <= b && a >= c)) return a;
    if ((b >= a && b <= c) || (b <= a && b >= c)) return b;
    return c;
}

void histogram(int arr[], int n, int bins[], int num_bins, int bin_width) {
    int i;
    for (i = 0; i < num_bins; i++) bins[i] = 0;
    for (i = 0; i < n; i++) {
        int idx = arr[i] / bin_width;
        if (idx >= 0 && idx < num_bins) {
            bins[idx] = bins[idx] + 1;
        }
    }
}

int count_in_range(int arr[], int n, int lo, int hi) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] >= lo && arr[i] <= hi) {
            count = count + 1;
        }
    }
    return count;
}

int sum_arr(int arr[], int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) s = s + arr[i];
    return s;
}

void selection_sort(int arr[], int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        int mi = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[mi]) mi = j;
        }
        if (mi != i) {
            int tmp = arr[i];
            arr[i] = arr[mi];
            arr[mi] = tmp;
        }
    }
}

int percentile(int arr[], int n, int p) {
    int sorted[20];
    int i;
    for (i = 0; i < n; i++) sorted[i] = arr[i];
    selection_sort(sorted, n);
    int idx = (p * (n - 1)) / 100;
    return sorted[idx];
}

int main(void) {
    int state = 42;

    state = lcg_next(state);
    // EXPECT: lcg1=58671
    printf("lcg1=%d\n", state);

    state = lcg_next(state);
    // EXPECT: lcg2=26458
    printf("lcg2=%d\n", state);

    state = lcg_next(state);
    // EXPECT: lcg3=95519
    printf("lcg3=%d\n", state);

    // EXPECT: range(100,1,10)=1
    printf("range(100,1,10)=%d\n", lcg_range(100, 1, 10));

    // EXPECT: range(77,0,99)=77
    printf("range(77,0,99)=%d\n", lcg_range(77, 0, 99));

    int data[10];
    data[0] = 15;
    data[1] = 8;
    data[2] = 23;
    data[3] = 42;
    data[4] = 4;
    data[5] = 16;
    data[6] = 30;
    data[7] = 11;
    data[8] = 27;
    data[9] = 19;

    // EXPECT: mean=19
    printf("mean=%d\n", mean(data, 10));

    // EXPECT: min=4
    printf("min=%d\n", min_val(data, 10));

    // EXPECT: max=42
    printf("max=%d\n", max_val(data, 10));

    // EXPECT: range=38
    printf("range=%d\n", range_val(data, 10));

    // EXPECT: var_n=1165
    printf("var_n=%d\n", variance_times_n(data, 10));

    // EXPECT: sum=195
    printf("sum=%d\n", sum_arr(data, 10));

    // EXPECT: median3(5,3,7)=5
    printf("median3(5,3,7)=%d\n", median_of_three(5, 3, 7));

    // EXPECT: median3(1,1,2)=1
    printf("median3(1,1,2)=%d\n", median_of_three(1, 1, 2));

    int bins[5];
    histogram(data, 10, bins, 5, 10);
    // EXPECT: hist: 2 4 2 1 1
    printf("hist: %d %d %d %d %d\n", bins[0], bins[1], bins[2], bins[3], bins[4]);

    // EXPECT: count(10,30)=7
    printf("count(10,30)=%d\n", count_in_range(data, 10, 10, 30));

    // EXPECT: count(0,9)=2
    printf("count(0,9)=%d\n", count_in_range(data, 10, 0, 9));

    // EXPECT: p25=11
    printf("p25=%d\n", percentile(data, 10, 25));

    // EXPECT: p50=16
    printf("p50=%d\n", percentile(data, 10, 50));

    // EXPECT: p75=23
    printf("p75=%d\n", percentile(data, 10, 75));

    int gen[8];
    int s = 1;
    int i;
    for (i = 0; i < 8; i++) {
        s = lcg_next(s);
        gen[i] = lcg_range(s, 0, 99);
    }
    // EXPECT: gen_mean_ok=1
    int gm = mean(gen, 8);
    printf("gen_mean_ok=%d\n", gm >= 0 && gm <= 99);

    return 0;
}
