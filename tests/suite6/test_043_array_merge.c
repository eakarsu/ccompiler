int printf(const char *fmt, ...);

void print_arr(int *a, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", a[i]);
    }
    printf("\n");
}

int merge_sorted(int *a, int na, int *b, int nb, int *out) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < na && j < nb) {
        if (a[i] <= b[j]) {
            out[k] = a[i];
            i++;
        } else {
            out[k] = b[j];
            j++;
        }
        k++;
    }
    while (i < na) {
        out[k] = a[i];
        i++;
        k++;
    }
    while (j < nb) {
        out[k] = b[j];
        j++;
        k++;
    }
    return k;
}

int merge_two(int *arr, int start, int mid, int end, int *temp) {
    int i = start;
    int j = mid;
    int k = start;
    while (i < mid && j < end) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i++;
        } else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }
    while (i < mid) {
        temp[k] = arr[i];
        i++;
        k++;
    }
    while (j < end) {
        temp[k] = arr[j];
        j++;
        k++;
    }
    for (i = start; i < end; i++) {
        arr[i] = temp[i];
    }
    return end - start;
}

int merge_k_sorted(int *arrs, int *sizes, int k, int *out) {
    int total = 0;
    int i;
    int merged[40];
    int merged_len = 0;
    int temp[40];

    for (i = 0; i < sizes[0]; i++) {
        merged[i] = arrs[i];
    }
    merged_len = sizes[0];

    int offset;
    offset = sizes[0];
    for (i = 1; i < k; i++) {
        int new_len;
        new_len = merge_sorted(merged, merged_len, arrs + offset, sizes[i], temp);
        int j;
        for (j = 0; j < new_len; j++) {
            merged[j] = temp[j];
        }
        merged_len = new_len;
        offset = offset + sizes[i];
    }
    for (i = 0; i < merged_len; i++) {
        out[i] = merged[i];
    }
    return merged_len;
}

int intersection(int *a, int na, int *b, int nb, int *out) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < na && j < nb) {
        if (a[i] < b[j]) {
            i++;
        } else if (a[i] > b[j]) {
            j++;
        } else {
            if (k == 0 || out[k - 1] != a[i]) {
                out[k] = a[i];
                k++;
            }
            i++;
            j++;
        }
    }
    return k;
}

int main(void) {
    int a[4];
    int b[5];
    int out[20];
    int n;

    a[0] = 1; a[1] = 3; a[2] = 5; a[3] = 7;
    b[0] = 2; b[1] = 4; b[2] = 6; b[3] = 8; b[4] = 10;
    n = merge_sorted(a, 4, b, 5, out);
    // EXPECT: merged_len=9
    printf("merged_len=%d\n", n);
    // EXPECT: 1 2 3 4 5 6 7 8 10
    print_arr(out, n);

    a[0] = 1; a[1] = 1; a[2] = 2; a[3] = 3;
    b[0] = 1; b[1] = 2; b[2] = 2; b[3] = 4; b[4] = 5;
    n = merge_sorted(a, 4, b, 5, out);
    // EXPECT: 1 1 1 2 2 2 3 4 5
    print_arr(out, n);

    int empty[1];
    n = merge_sorted(a, 4, empty, 0, out);
    // EXPECT: merge_empty=4
    printf("merge_empty=%d\n", n);
    // EXPECT: 1 1 2 3
    print_arr(out, n);

    int all_arrs[12];
    int sizes[3];
    all_arrs[0] = 1; all_arrs[1] = 5; all_arrs[2] = 9;
    all_arrs[3] = 2; all_arrs[4] = 6; all_arrs[5] = 10;
    all_arrs[6] = 3; all_arrs[7] = 4; all_arrs[8] = 7;
    all_arrs[9] = 8; all_arrs[10] = 11; all_arrs[11] = 12;
    sizes[0] = 3; sizes[1] = 3; sizes[2] = 6;
    n = merge_k_sorted(all_arrs, sizes, 3, out);
    // EXPECT: k_merged_len=12
    printf("k_merged_len=%d\n", n);
    // EXPECT: 1 2 3 4 5 6 7 8 9 10 11 12
    print_arr(out, n);

    int c[6];
    int d[6];
    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 5; c[4] = 7; c[5] = 9;
    d[0] = 2; d[1] = 3; d[2] = 5; d[3] = 6; d[4] = 7; d[5] = 8;
    n = intersection(c, 6, d, 6, out);
    // EXPECT: inter_len=4
    printf("inter_len=%d\n", n);
    // EXPECT: 2 3 5 7
    print_arr(out, n);

    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 4; c[4] = 5; c[5] = 6;
    d[0] = 7; d[1] = 8; d[2] = 9; d[3] = 10; d[4] = 11; d[5] = 12;
    n = intersection(c, 6, d, 6, out);
    // EXPECT: no_inter=0
    printf("no_inter=%d\n", n);

    c[0] = 1; c[1] = 1; c[2] = 2; c[3] = 2; c[4] = 3; c[5] = 3;
    d[0] = 1; d[1] = 2; d[2] = 2; d[3] = 3; d[4] = 3; d[5] = 3;
    n = intersection(c, 6, d, 6, out);
    // EXPECT: dup_inter=3
    printf("dup_inter=%d\n", n);
    // EXPECT: 1 2 3
    print_arr(out, n);

    int e[8];
    int temp[20];
    e[0] = 3; e[1] = 7; e[2] = 1; e[3] = 5;
    e[4] = 2; e[5] = 8; e[6] = 4; e[7] = 6;
    merge_two(e, 0, 2, 4, temp);
    merge_two(e, 4, 6, 8, temp);
    merge_two(e, 0, 4, 8, temp);
    // EXPECT: 1 2 3 4 5 6 7 8
    print_arr(e, 8);

    int f[3];
    int g[3];
    f[0] = 1; f[1] = 3; f[2] = 5;
    g[0] = 1; g[1] = 3; g[2] = 5;
    n = intersection(f, 3, g, 3, out);
    // EXPECT: same_inter=3
    printf("same_inter=%d\n", n);

    // EXPECT: done
    printf("done\n");

    return 0;
}
