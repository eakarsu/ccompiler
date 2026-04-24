int printf(const char *fmt, ...);
// EXPECT: === perms of [1,2,3] ===\n1 2 3\n1 3 2\n2 1 3\n2 3 1\n3 2 1\n3 1 2\ncount: 6\n=== perms of [1,2,3,4] count ===\n1 2 3 4\n1 2 4 3\n1 3 2 4\n1 3 4 2\n1 4 3 2\n1 4 2 3\n2 1 3 4\n2 1 4 3\n2 3 1 4\n2 3 4 1\n2 4 3 1\n2 4 1 3\n3 2 1 4\n3 2 4 1\n3 1 2 4\n3 1 4 2\n3 4 1 2\n3 4 2 1\n4 2 3 1\n4 2 1 3\n4 3 2 1\n4 3 1 2\n4 1 3 2\n4 1 2 3\ncount: 24\n=== count_perms formula ===\nP(1)=1\nP(2)=2\nP(3)=6\nP(4)=24\nP(5)=120\nP(6)=720\nP(7)=5040\n=== next_permutation ===\n1 2 3 4\n1 2 4 3\n1 3 2 4\n1 3 4 2\n1 4 2 3\n1 4 3 2
// Test 215: Recursive permutation generation

int perm_count;

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
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

void permute(int *arr, int start, int end) {
    if (start == end) {
        print_arr(arr, end + 1);
        perm_count = perm_count + 1;
        return;
    }
    int i = start;
    while (i <= end) {
        swap(&arr[start], &arr[i]);
        permute(arr, start + 1, end);
        swap(&arr[start], &arr[i]);
        i = i + 1;
    }
}

int count_perms(int n) {
    if (n <= 1) return 1;
    return n * count_perms(n - 1);
}

void next_permutation(int *arr, int n) {
    int i = n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i = i - 1;
    }
    if (i < 0) return;
    int j = n - 1;
    while (arr[j] <= arr[i]) {
        j = j - 1;
    }
    swap(&arr[i], &arr[j]);
    int left = i + 1;
    int right = n - 1;
    while (left < right) {
        swap(&arr[left], &arr[right]);
        left = left + 1;
        right = right - 1;
    }
}

int main(void) {
    printf("=== perms of [1,2,3] ===\n");
    int a[3];
    a[0] = 1; a[1] = 2; a[2] = 3;
    perm_count = 0;
    permute(a, 0, 2);
    printf("count: %d\n", perm_count);

    printf("=== perms of [1,2,3,4] count ===\n");
    int b[4];
    b[0] = 1; b[1] = 2; b[2] = 3; b[3] = 4;
    perm_count = 0;
    permute(b, 0, 3);
    printf("count: %d\n", perm_count);

    printf("=== count_perms formula ===\n");
    int i = 1;
    while (i <= 7) {
        printf("P(%d)=%d\n", i, count_perms(i));
        i = i + 1;
    }

    printf("=== next_permutation ===\n");
    int c[4];
    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 4;
    i = 0;
    while (i < 6) {
        print_arr(c, 4);
        next_permutation(c, 4);
        i = i + 1;
    }

    return 0;
}
