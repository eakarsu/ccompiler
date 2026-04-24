int printf(const char *fmt, ...);

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int *a, int lo, int hi) {
    int pivot = a[hi];
    int i = lo - 1;
    int j;
    for (j = lo; j < hi; j++) {
        if (a[j] <= pivot) {
            i++;
            swap(a + i, a + j);
        }
    }
    swap(a + i + 1, a + hi);
    return i + 1;
}

void dutch_flag(int *a, int n, int pivot) {
    int lo = 0;
    int mid = 0;
    int hi = n - 1;
    while (mid <= hi) {
        if (a[mid] < pivot) {
            swap(a + lo, a + mid);
            lo++;
            mid++;
        } else if (a[mid] == pivot) {
            mid++;
        } else {
            swap(a + mid, a + hi);
            hi--;
        }
    }
}

void stable_partition_neg_pos(int *a, int n) {
    int temp[20];
    int i;
    int idx = 0;
    for (i = 0; i < n; i++) {
        if (a[i] < 0) {
            temp[idx] = a[i];
            idx++;
        }
    }
    for (i = 0; i < n; i++) {
        if (a[i] >= 0) {
            temp[idx] = a[i];
            idx++;
        }
    }
    for (i = 0; i < n; i++) {
        a[i] = temp[i];
    }
}

void print_arr(int *a, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", a[i]);
    }
    printf("\n");
}

void copy_arr(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

int main(void) {
    int a[8];
    int b[9];
    int c[7];
    int p;

    a[0] = 10; a[1] = 80; a[2] = 30; a[3] = 90;
    a[4] = 40; a[5] = 50; a[6] = 70; a[7] = 60;
    p = partition(a, 0, 7);
    // EXPECT: pivot_pos=4
    printf("pivot_pos=%d\n", p);

    // EXPECT: pivot_val=60
    printf("pivot_val=%d\n", a[p]);

    int i;
    int ok;
    ok = 1;
    for (i = 0; i < p; i++) {
        if (a[i] > a[p]) ok = 0;
    }
    for (i = p + 1; i < 8; i++) {
        if (a[i] < a[p]) ok = 0;
    }
    // EXPECT: partition_valid=1
    printf("partition_valid=%d\n", ok);

    b[0] = 2; b[1] = 0; b[2] = 1; b[3] = 2;
    b[4] = 0; b[5] = 1; b[6] = 1; b[7] = 0; b[8] = 2;
    dutch_flag(b, 9, 1);
    // EXPECT: 0 0 0 1 1 1 2 2 2
    print_arr(b, 9);

    b[0] = 1; b[1] = 1; b[2] = 1; b[3] = 1;
    b[4] = 1; b[5] = 1; b[6] = 1; b[7] = 1; b[8] = 1;
    dutch_flag(b, 9, 1);
    // EXPECT: 1 1 1 1 1 1 1 1 1
    print_arr(b, 9);

    b[0] = 0; b[1] = 0; b[2] = 0; b[3] = 2;
    b[4] = 2; b[5] = 2; b[6] = 1; b[7] = 1; b[8] = 1;
    dutch_flag(b, 9, 1);
    // EXPECT: 0 0 0 1 1 1 2 2 2
    print_arr(b, 9);

    c[0] = 3; c[1] = -2; c[2] = 5; c[3] = -1;
    c[4] = -4; c[5] = 7; c[6] = 0;
    stable_partition_neg_pos(c, 7);
    // EXPECT: -2 -1 -4 3 5 7 0
    print_arr(c, 7);

    c[0] = -1; c[1] = -2; c[2] = -3; c[3] = 4;
    c[4] = 5; c[5] = 6; c[6] = 7;
    stable_partition_neg_pos(c, 7);
    // EXPECT: -1 -2 -3 4 5 6 7
    print_arr(c, 7);

    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 4;
    c[4] = 5; c[5] = 6; c[6] = 7;
    stable_partition_neg_pos(c, 7);
    // EXPECT: 1 2 3 4 5 6 7
    print_arr(c, 7);

    int d[5];
    d[0] = 5; d[1] = 3; d[2] = 8; d[3] = 1; d[4] = 2;
    p = partition(d, 0, 4);
    ok = 1;
    for (i = 0; i < p; i++) {
        if (d[i] > d[p]) ok = 0;
    }
    for (i = p + 1; i < 5; i++) {
        if (d[i] < d[p]) ok = 0;
    }
    // EXPECT: small_partition_valid=1
    printf("small_partition_valid=%d\n", ok);

    int e[6];
    e[0] = 2; e[1] = 0; e[2] = 2; e[3] = 1; e[4] = 0; e[5] = 1;
    dutch_flag(e, 6, 1);
    // EXPECT: 0 0 1 1 2 2
    print_arr(e, 6);

    int f[4];
    f[0] = -5; f[1] = 10; f[2] = -3; f[3] = 8;
    stable_partition_neg_pos(f, 4);
    // EXPECT: -5 -3 10 8
    print_arr(f, 4);

    int g[3];
    g[0] = 0; g[1] = 0; g[2] = 0;
    dutch_flag(g, 3, 1);
    // EXPECT: 0 0 0
    print_arr(g, 3);

    int h[1];
    h[0] = 42;
    p = partition(h, 0, 0);
    // EXPECT: single=42
    printf("single=%d\n", h[p]);

    // EXPECT: done
    printf("done\n");

    return 0;
}
