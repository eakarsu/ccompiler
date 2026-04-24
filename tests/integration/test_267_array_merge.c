int printf(const char *fmt, ...);
// EXPECT: merged: 1 2 3 4 5 6 7 8\nwith empty: 10 20 30\noverlapping: 1 3 3 5 5 7\nunique merge: 1 3 5 7\ndiff sizes: 1 2 3 50 60 70 100\nsequential: 1 2 3 10 20 30
// Test: merging two sorted arrays

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
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
            i = i + 1;
        } else {
            out[k] = b[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < na) {
        out[k] = a[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < nb) {
        out[k] = b[j];
        j = j + 1;
        k = k + 1;
    }
    return k;
}

// Merge and remove duplicates
int merge_unique(int *a, int na, int *b, int nb, int *out) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < na && j < nb) {
        if (a[i] < b[j]) {
            if (k == 0 || out[k-1] != a[i]) { out[k] = a[i]; k = k + 1; }
            i = i + 1;
        } else if (a[i] > b[j]) {
            if (k == 0 || out[k-1] != b[j]) { out[k] = b[j]; k = k + 1; }
            j = j + 1;
        } else {
            if (k == 0 || out[k-1] != a[i]) { out[k] = a[i]; k = k + 1; }
            i = i + 1;
            j = j + 1;
        }
    }
    while (i < na) {
        if (k == 0 || out[k-1] != a[i]) { out[k] = a[i]; k = k + 1; }
        i = i + 1;
    }
    while (j < nb) {
        if (k == 0 || out[k-1] != b[j]) { out[k] = b[j]; k = k + 1; }
        j = j + 1;
    }
    return k;
}

int main(void) {
    // Basic merge
    int a[4]; a[0]=1; a[1]=3; a[2]=5; a[3]=7;
    int b[4]; b[0]=2; b[1]=4; b[2]=6; b[3]=8;
    int merged[8];
    int n = merge_sorted(a, 4, b, 4, merged);
    printf("merged: ");
    print_arr(merged, n);

    // One empty
    int c[3]; c[0]=10; c[1]=20; c[2]=30;
    int empty[1];
    int out1[4];
    n = merge_sorted(c, 3, empty, 0, out1);
    printf("with empty: ");
    print_arr(out1, n);

    // Overlapping values
    int d[3]; d[0]=1; d[1]=3; d[2]=5;
    int e[3]; e[0]=3; e[1]=5; e[2]=7;
    int out2[6];
    n = merge_sorted(d, 3, e, 3, out2);
    printf("overlapping: ");
    print_arr(out2, n);

    // Merge unique (no dups)
    int out3[6];
    n = merge_unique(d, 3, e, 3, out3);
    printf("unique merge: ");
    print_arr(out3, n);

    // Different sizes
    int f[2]; f[0]=1; f[1]=100;
    int g[5]; g[0]=2; g[1]=3; g[2]=50; g[3]=60; g[4]=70;
    int out4[7];
    n = merge_sorted(f, 2, g, 5, out4);
    printf("diff sizes: ");
    print_arr(out4, n);

    // All from a before b
    int h[3]; h[0]=1; h[1]=2; h[2]=3;
    int k2[3]; k2[0]=10; k2[1]=20; k2[2]=30;
    int out5[6];
    n = merge_sorted(h, 3, k2, 3, out5);
    printf("sequential: ");
    print_arr(out5, n);

    return 0;
}
