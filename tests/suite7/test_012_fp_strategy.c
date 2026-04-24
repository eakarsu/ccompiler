int printf(const char *fmt, ...);

typedef int (*Comparator)(int, int);

int cmp_asc(int a, int b) { return a - b; }
int cmp_desc(int a, int b) { return b - a; }
int cmp_abs_asc(int a, int b) {
    int aa;
    int ab;
    aa = a;
    ab = b;
    if (aa < 0) aa = -aa;
    if (ab < 0) ab = -ab;
    return aa - ab;
}
int cmp_even_first(int a, int b) {
    int ea;
    int eb;
    ea = a % 2;
    eb = b % 2;
    if (ea < 0) ea = -ea;
    if (eb < 0) eb = -eb;
    if (ea != eb) return ea - eb;
    return a - b;
}

void sort(int arr[], int n, Comparator cmp) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (cmp(arr[j], arr[j + 1]) > 0) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void print_arr(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void copy_arr(int dst[], int src[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

int is_sorted(int arr[], int n, Comparator cmp) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (cmp(arr[i], arr[i + 1]) > 0) return 0;
    }
    return 1;
}

int find_min(int arr[], int n, Comparator cmp) {
    int best;
    int i;
    best = 0;
    for (i = 1; i < n; i++) {
        if (cmp(arr[i], arr[best]) < 0) {
            best = i;
        }
    }
    return arr[best];
}

int main(void) {
    int data[8];
    int work[8];
    int n;

    data[0] = 5; data[1] = -3; data[2] = 8; data[3] = -1;
    data[4] = 4; data[5] = -7; data[6] = 2; data[7] = 6;
    n = 8;

    copy_arr(work, data, n);
    sort(work, n, cmp_asc);
    // EXPECT: asc: -7 -3 -1 2 4 5 6 8
    printf("asc: ");
    print_arr(work, n);

    copy_arr(work, data, n);
    sort(work, n, cmp_desc);
    // EXPECT: desc: 8 6 5 4 2 -1 -3 -7
    printf("desc: ");
    print_arr(work, n);

    copy_arr(work, data, n);
    sort(work, n, cmp_abs_asc);
    // EXPECT: abs: -1 2 -3 4 5 6 -7 8
    printf("abs: ");
    print_arr(work, n);

    copy_arr(work, data, n);
    sort(work, n, cmp_even_first);
    // EXPECT: even_first: 2 4 6 8 -7 -3 -1 5
    printf("even_first: ");
    print_arr(work, n);

    copy_arr(work, data, n);
    sort(work, n, cmp_asc);
    // EXPECT: sorted asc: 1
    printf("sorted asc: %d\n", is_sorted(work, n, cmp_asc));
    // EXPECT: sorted desc: 0
    printf("sorted desc: %d\n", is_sorted(work, n, cmp_desc));

    copy_arr(work, data, n);
    sort(work, n, cmp_desc);
    // EXPECT: sorted desc2: 1
    printf("sorted desc2: %d\n", is_sorted(work, n, cmp_desc));

    // EXPECT: min asc: -7
    printf("min asc: %d\n", find_min(data, n, cmp_asc));
    // EXPECT: min desc: 8
    printf("min desc: %d\n", find_min(data, n, cmp_desc));
    // EXPECT: min abs: -1
    printf("min abs: %d\n", find_min(data, n, cmp_abs_asc));

    data[0] = 3; data[1] = 1; data[2] = 4; data[3] = 1;
    data[4] = 5; data[5] = 9; data[6] = 2; data[7] = 6;
    sort(data, n, cmp_asc);
    // EXPECT: pi: 1 1 2 3 4 5 6 9
    printf("pi: ");
    print_arr(data, n);

    // EXPECT: pi sorted: 1
    printf("pi sorted: %d\n", is_sorted(data, n, cmp_asc));

    sort(data, n, cmp_desc);
    // EXPECT: pi desc: 9 6 5 4 3 2 1 1
    printf("pi desc: ");
    print_arr(data, n);

    // EXPECT: pi desc sorted: 1
    printf("pi desc sorted: %d\n", is_sorted(data, n, cmp_desc));

    // EXPECT: min abs pi: 1
    printf("min abs pi: %d\n", find_min(data, n, cmp_abs_asc));

    return 0;
}
