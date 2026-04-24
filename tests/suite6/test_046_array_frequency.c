int printf(const char *fmt, ...);

void print_arr(int *a, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", a[i]);
    }
    printf("\n");
}

void sort_array(int *a, int n) {
    int i;
    int j;
    int temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

void frequency_count(int *a, int n, int *vals, int *counts, int *num_unique) {
    int sorted[20];
    int i;
    int idx;
    for (i = 0; i < n; i++) sorted[i] = a[i];
    sort_array(sorted, n);

    idx = 0;
    i = 0;
    while (i < n) {
        vals[idx] = sorted[i];
        counts[idx] = 1;
        while (i + 1 < n && sorted[i + 1] == sorted[i]) {
            counts[idx]++;
            i++;
        }
        idx++;
        i++;
    }
    *num_unique = idx;
}

int find_mode(int *a, int n) {
    int sorted[20];
    int i;
    int mode;
    int max_count;
    int count;
    for (i = 0; i < n; i++) sorted[i] = a[i];
    sort_array(sorted, n);

    mode = sorted[0];
    max_count = 1;
    count = 1;
    for (i = 1; i < n; i++) {
        if (sorted[i] == sorted[i - 1]) {
            count++;
        } else {
            count = 1;
        }
        if (count > max_count) {
            max_count = count;
            mode = sorted[i];
        }
    }
    return mode;
}

int boyer_moore_majority(int *a, int n) {
    int candidate = a[0];
    int count = 1;
    int i;

    for (i = 1; i < n; i++) {
        if (count == 0) {
            candidate = a[i];
            count = 1;
        } else if (a[i] == candidate) {
            count++;
        } else {
            count--;
        }
    }

    count = 0;
    for (i = 0; i < n; i++) {
        if (a[i] == candidate) count++;
    }
    if (count > n / 2) return candidate;
    return -1;
}

int find_duplicates(int *a, int n, int *dups) {
    int sorted[20];
    int i;
    int k = 0;
    for (i = 0; i < n; i++) sorted[i] = a[i];
    sort_array(sorted, n);

    for (i = 1; i < n; i++) {
        if (sorted[i] == sorted[i - 1]) {
            if (k == 0 || dups[k - 1] != sorted[i]) {
                dups[k] = sorted[i];
                k++;
            }
        }
    }
    return k;
}

int count_distinct(int *a, int n) {
    int sorted[20];
    int i;
    int count;
    for (i = 0; i < n; i++) sorted[i] = a[i];
    sort_array(sorted, n);
    count = 1;
    for (i = 1; i < n; i++) {
        if (sorted[i] != sorted[i - 1]) count++;
    }
    return count;
}

int main(void) {
    int a[10];
    int vals[10];
    int counts[10];
    int num;
    int i;

    a[0] = 3; a[1] = 1; a[2] = 2; a[3] = 3;
    a[4] = 1; a[5] = 3; a[6] = 2; a[7] = 1;
    frequency_count(a, 8, vals, counts, &num);
    // EXPECT: unique=3
    printf("unique=%d\n", num);
    for (i = 0; i < num; i++) {
        // EXPECT: val=1 count=3
        // EXPECT: val=2 count=2
        // EXPECT: val=3 count=3
        printf("val=%d count=%d\n", vals[i], counts[i]);
    }

    a[0] = 5; a[1] = 5; a[2] = 5; a[3] = 5; a[4] = 5;
    frequency_count(a, 5, vals, counts, &num);
    // EXPECT: all_same=1
    printf("all_same=%d\n", num);

    a[0] = 1; a[1] = 2; a[2] = 2; a[3] = 3; a[4] = 3; a[5] = 3;
    int mode = find_mode(a, 6);
    // EXPECT: mode=3
    printf("mode=%d\n", mode);

    a[0] = 7; a[1] = 7; a[2] = 7; a[3] = 7;
    mode = find_mode(a, 4);
    // EXPECT: mode_all=7
    printf("mode_all=%d\n", mode);

    a[0] = 2; a[1] = 2; a[2] = 1; a[3] = 1;
    a[4] = 1; a[5] = 2; a[6] = 2;
    int maj = boyer_moore_majority(a, 7);
    // EXPECT: majority=2
    printf("majority=%d\n", maj);

    a[0] = 3; a[1] = 3; a[2] = 4; a[3] = 2;
    a[4] = 4; a[5] = 4; a[6] = 2; a[7] = 4; a[8] = 4;
    maj = boyer_moore_majority(a, 9);
    // EXPECT: majority2=4
    printf("majority2=%d\n", maj);

    a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4; a[4] = 5;
    maj = boyer_moore_majority(a, 5);
    // EXPECT: no_majority=-1
    printf("no_majority=%d\n", maj);

    int dups[10];
    a[0] = 4; a[1] = 2; a[2] = 4; a[3] = 5;
    a[4] = 2; a[5] = 3; a[6] = 1; a[7] = 5;
    int nd = find_duplicates(a, 8, dups);
    // EXPECT: num_dups=3
    printf("num_dups=%d\n", nd);
    // EXPECT: dups: 2 4 5
    printf("dups: %d %d %d\n", dups[0], dups[1], dups[2]);

    a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4; a[4] = 5;
    nd = find_duplicates(a, 5, dups);
    // EXPECT: no_dups=0
    printf("no_dups=%d\n", nd);

    a[0] = 5; a[1] = 3; a[2] = 1; a[3] = 5; a[4] = 3;
    a[5] = 1; a[6] = 2; a[7] = 4;
    int dist = count_distinct(a, 8);
    // EXPECT: distinct=5
    printf("distinct=%d\n", dist);

    // EXPECT: done
    printf("done\n");

    return 0;
}
