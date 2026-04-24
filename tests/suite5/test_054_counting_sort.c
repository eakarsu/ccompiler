int printf(const char *fmt, ...);

void counting_sort(int arr[], int n, int max_val) {
    int count[101];
    int output[20];
    int i;
    int j;
    int idx;

    i = 0;
    while (i <= max_val) {
        count[i] = 0;
        i = i + 1;
    }

    i = 0;
    while (i < n) {
        count[arr[i]] = count[arr[i]] + 1;
        i = i + 1;
    }

    idx = 0;
    i = 0;
    while (i <= max_val) {
        j = 0;
        while (j < count[i]) {
            output[idx] = i;
            idx = idx + 1;
            j = j + 1;
        }
        i = i + 1;
    }

    i = 0;
    while (i < n) {
        arr[i] = output[i];
        i = i + 1;
    }
}

void build_histogram(int arr[], int n, int hist[], int max_val) {
    int i;
    i = 0;
    while (i <= max_val) {
        hist[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < n) {
        hist[arr[i]] = hist[arr[i]] + 1;
        i = i + 1;
    }
}

int find_mode(int hist[], int max_val) {
    int mode;
    int max_count;
    int i;
    mode = 0;
    max_count = 0;
    i = 0;
    while (i <= max_val) {
        if (hist[i] > max_count) {
            max_count = hist[i];
            mode = i;
        }
        i = i + 1;
    }
    return mode;
}

int is_sorted(int arr[], int n) {
    int i;
    i = 0;
    while (i < n - 1) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

void print_array(int arr[], int n) {
    int i;
    i = 0;
    while (i < n) {
        printf("%d ", arr[i]);
        i = i + 1;
    }
}

int main(void) {
    int a[10];
    int b[8];
    int c[5];
    int hist[101];
    int mode;

    a[0] = 4; a[1] = 2; a[2] = 2; a[3] = 8; a[4] = 3;
    a[5] = 3; a[6] = 1; a[7] = 7; a[8] = 5; a[9] = 6;
    counting_sort(a, 10, 10);
    // EXPECT: cs10: 1
    printf("cs10: %d\n", is_sorted(a, 10));
    // EXPECT: 1 2 2 3 3 4 5 6 7 8 done
    print_array(a, 10);
    printf("done\n");

    b[0] = 5; b[1] = 5; b[2] = 5; b[3] = 5;
    b[4] = 5; b[5] = 5; b[6] = 5; b[7] = 5;
    counting_sort(b, 8, 10);
    // EXPECT: allsame: 1
    printf("allsame: %d\n", is_sorted(b, 8));
    // EXPECT: 5 5 5 5 5 5 5 5 done
    print_array(b, 8);
    printf("done\n");

    c[0] = 0; c[1] = 0; c[2] = 1; c[3] = 0; c[4] = 1;
    counting_sort(c, 5, 1);
    // EXPECT: binary: 1
    printf("binary: %d\n", is_sorted(c, 5));
    // EXPECT: 0 0 0 1 1 done
    print_array(c, 5);
    printf("done\n");

    a[0] = 9; a[1] = 1; a[2] = 3; a[3] = 5; a[4] = 7;
    a[5] = 2; a[6] = 4; a[7] = 6; a[8] = 8; a[9] = 0;
    build_histogram(a, 10, hist, 10);
    // EXPECT: h0: 1
    printf("h0: %d\n", hist[0]);
    // EXPECT: h5: 1
    printf("h5: %d\n", hist[5]);
    // EXPECT: h10: 0
    printf("h10: %d\n", hist[10]);

    a[0] = 3; a[1] = 1; a[2] = 3; a[3] = 3; a[4] = 2;
    a[5] = 1; a[6] = 3; a[7] = 2; a[8] = 1; a[9] = 3;
    build_histogram(a, 10, hist, 5);
    // EXPECT: freq1: 3
    printf("freq1: %d\n", hist[1]);
    // EXPECT: freq2: 2
    printf("freq2: %d\n", hist[2]);
    // EXPECT: freq3: 5
    printf("freq3: %d\n", hist[3]);
    mode = find_mode(hist, 5);
    // EXPECT: mode: 3
    printf("mode: %d\n", mode);

    counting_sort(a, 10, 5);
    // EXPECT: freq_sorted: 1
    printf("freq_sorted: %d\n", is_sorted(a, 10));
    // EXPECT: 1 1 1 2 2 3 3 3 3 3 done
    print_array(a, 10);
    printf("done\n");

    b[0] = 100; b[1] = 50; b[2] = 75; b[3] = 25;
    b[4] = 0; b[5] = 100; b[6] = 50; b[7] = 75;
    counting_sort(b, 8, 100);
    // EXPECT: wide: 1
    printf("wide: %d\n", is_sorted(b, 8));
    // EXPECT: 0 25 50 50 75 75 100 100 done
    print_array(b, 8);
    printf("done\n");

    return 0;
}
