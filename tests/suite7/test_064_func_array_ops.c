int printf(const char *fmt, ...);

void fill(int *arr, int n, int val) {
    int i = 0;
    while (i < n) {
        arr[i] = val;
        i = i + 1;
    }
}

void fill_sequence(int *arr, int n, int start) {
    int i = 0;
    while (i < n) {
        arr[i] = start + i;
        i = i + 1;
    }
}

int sum(int *arr, int n) {
    int total = 0;
    int i = 0;
    while (i < n) {
        total = total + arr[i];
        i = i + 1;
    }
    return total;
}

int find(int *arr, int n, int val) {
    int i = 0;
    while (i < n) {
        if (arr[i] == val) return i;
        i = i + 1;
    }
    return -1;
}

int count(int *arr, int n, int val) {
    int c = 0;
    int i = 0;
    while (i < n) {
        if (arr[i] == val) c = c + 1;
        i = i + 1;
    }
    return c;
}

void reverse(int *arr, int n) {
    int left = 0;
    int right = n - 1;
    while (left < right) {
        int tmp = arr[left];
        arr[left] = arr[right];
        arr[right] = tmp;
        left = left + 1;
        right = right - 1;
    }
}

int min_val(int *arr, int n) {
    int m = arr[0];
    int i = 1;
    while (i < n) {
        if (arr[i] < m) m = arr[i];
        i = i + 1;
    }
    return m;
}

int max_val(int *arr, int n) {
    int m = arr[0];
    int i = 1;
    while (i < n) {
        if (arr[i] > m) m = arr[i];
        i = i + 1;
    }
    return m;
}

void bubble_sort(int *arr, int n) {
    int i = 0;
    while (i < n - 1) {
        int j = 0;
        while (j < n - 1 - i) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

void copy_arr(int *dst, int *src, int n) {
    int i = 0;
    while (i < n) {
        dst[i] = src[i];
        i = i + 1;
    }
}

int is_sorted(int *arr, int n) {
    int i = 0;
    while (i < n - 1) {
        if (arr[i] > arr[i + 1]) return 0;
        i = i + 1;
    }
    return 1;
}

void scale(int *arr, int n, int factor) {
    int i = 0;
    while (i < n) {
        arr[i] = arr[i] * factor;
        i = i + 1;
    }
}

int dot_product(int *a, int *b, int n) {
    int result = 0;
    int i = 0;
    while (i < n) {
        result = result + a[i] * b[i];
        i = i + 1;
    }
    return result;
}

void print_arr(int *arr, int n) {
    int i = 0;
    while (i < n) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
        i = i + 1;
    }
    printf("\n");
}

int main(void) {
    int a[8];
    fill(a, 8, 5);
    // EXPECT: fill_sum: 40
    printf("fill_sum: %d\n", sum(a, 8));

    int b[6];
    fill_sequence(b, 6, 1);
    // EXPECT: seq_sum: 21
    printf("seq_sum: %d\n", sum(b, 6));

    // EXPECT: 1 2 3 4 5 6
    print_arr(b, 6);

    // EXPECT: find3: 2
    printf("find3: %d\n", find(b, 6, 3));

    // EXPECT: find9: -1
    printf("find9: %d\n", find(b, 6, 9));

    reverse(b, 6);
    // EXPECT: 6 5 4 3 2 1
    print_arr(b, 6);

    int c[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    // EXPECT: min: 1
    printf("min: %d\n", min_val(c, 9));

    // EXPECT: max: 9
    printf("max: %d\n", max_val(c, 9));

    // EXPECT: sorted_before: 0
    printf("sorted_before: %d\n", is_sorted(c, 9));

    bubble_sort(c, 9);
    // EXPECT: 1 2 3 4 5 6 7 8 9
    print_arr(c, 9);

    // EXPECT: sorted_after: 1
    printf("sorted_after: %d\n", is_sorted(c, 9));

    int d[4];
    int src[] = {10, 20, 30, 40};
    copy_arr(d, src, 4);
    // EXPECT: copy_sum: 100
    printf("copy_sum: %d\n", sum(d, 4));

    scale(d, 4, 3);
    // EXPECT: 30 60 90 120
    print_arr(d, 4);

    int v1[] = {1, 2, 3};
    int v2[] = {4, 5, 6};
    // EXPECT: dot: 32
    printf("dot: %d\n", dot_product(v1, v2, 3));

    int rep[] = {3, 1, 4, 1, 5, 1};
    // EXPECT: count1: 3
    printf("count1: %d\n", count(rep, 6, 1));

    // EXPECT: count9: 0
    printf("count9: %d\n", count(rep, 6, 9));

    return 0;
}
