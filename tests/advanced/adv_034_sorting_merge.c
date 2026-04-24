int printf(const char *fmt, ...);
// EXPECT: === Merge Sort Tests ===
// EXPECT: --- Basic sort ---
// EXPECT: input: 38 27 43 3 9 82 10
// EXPECT: sorted: 3 9 10 27 38 43 82
// EXPECT: --- Already sorted ---
// EXPECT: input: 1 2 3 4 5 6
// EXPECT: sorted: 1 2 3 4 5 6
// EXPECT: --- Reverse sorted ---
// EXPECT: input: 9 8 7 6 5 4 3 2 1
// EXPECT: sorted: 1 2 3 4 5 6 7 8 9
// EXPECT: --- Single element ---
// EXPECT: sorted: 42
// EXPECT: --- Two elements ---
// EXPECT: input: 5 2
// EXPECT: sorted: 2 5
// EXPECT: --- All same ---
// EXPECT: input: 7 7 7 7 7
// EXPECT: sorted: 7 7 7 7 7
// EXPECT: --- Duplicates ---
// EXPECT: input: 4 2 4 1 3 2 1
// EXPECT: sorted: 1 1 2 2 3 4 4
// EXPECT: --- Negative values ---
// EXPECT: input: -5 10 -3 0 7 -8 2
// EXPECT: sorted: -8 -5 -3 0 2 7 10
// EXPECT: --- Merge two sorted arrays ---
// EXPECT: a: 1 3 5 7
// EXPECT: b: 2 4 6 8
// EXPECT: merged: 1 2 3 4 5 6 7 8
// EXPECT: --- Large array ---
// EXPECT: first: 0 last: 31
// EXPECT: --- Bottom-up merge sort ---
// EXPECT: input: 6 3 8 1 5 2 7 4
// EXPECT: sorted: 1 2 3 4 5 6 7 8
// EXPECT: --- Stability check ---
// EXPECT: keys: 1 1 2 3 3 5
// EXPECT: ids: 1 3 5 0 4 2
// EXPECT: === Merge Sort Done ===
// --- Basic sort ---
// input: 38 27 43 3 9 82 10
// sorted: 3 9 10 27 38 43 82
// --- Already sorted ---
// input: 1 2 3 4 5 6
// sorted: 1 2 3 4 5 6
// --- Reverse sorted ---
// input: 9 8 7 6 5 4 3 2 1
// sorted: 1 2 3 4 5 6 7 8 9
// --- Single element ---
// sorted: 42
// --- Two elements ---
// input: 5 2
// sorted: 2 5
// --- All same ---
// input: 7 7 7 7 7
// sorted: 7 7 7 7 7
// --- Duplicates ---
// input: 4 2 4 1 3 2 1
// sorted: 1 1 2 2 3 4 4
// --- Negative values ---
// input: -5 10 -3 0 7 -8 2
// sorted: -8 -5 -3 0 2 7 10
// --- Merge two sorted arrays ---
// a: 1 3 5 7
// b: 2 4 6 8
// merged: 1 2 3 4 5 6 7 8
// --- Large array ---
// first: 0 last: 31
// --- Bottom-up merge sort ---
// input: 6 3 8 1 5 2 7 4
// sorted: 1 2 3 4 5 6 7 8
// --- Stability check ---
// keys: 1 1 2 3 3 5
// ids: 1 3 5 0 4 2
// === Merge Sort Done ===

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void merge(int *arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int L[64];
    int R[64];
    int i;
    int j;
    int k;

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i = i + 1;
        } else {
            arr[k] = R[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < n1) {
        arr[k] = L[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < n2) {
        arr[k] = R[j];
        j = j + 1;
        k = k + 1;
    }
}

void merge_sort_helper(int *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_helper(arr, left, mid);
        merge_sort_helper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void merge_sort(int *arr, int n) {
    if (n > 1)
        merge_sort_helper(arr, 0, n - 1);
}

// Merge two separate sorted arrays into result
void merge_arrays(int *a, int na, int *b, int nb, int *result) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < na && j < nb) {
        if (a[i] <= b[j]) {
            result[k] = a[i];
            i = i + 1;
        } else {
            result[k] = b[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < na) {
        result[k] = a[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < nb) {
        result[k] = b[j];
        j = j + 1;
        k = k + 1;
    }
}

// Bottom-up iterative merge sort
void merge_sort_bottomup(int *arr, int n) {
    int width;
    int i;
    for (width = 1; width < n; width = width * 2) {
        for (i = 0; i < n; i = i + 2 * width) {
            int left = i;
            int mid = i + width - 1;
            int right = i + 2 * width - 1;
            if (mid >= n) mid = n - 1;
            if (right >= n) right = n - 1;
            if (mid < right)
                merge(arr, left, mid, right);
        }
    }
}

// Stable merge sort with parallel id tracking
void merge_stable(int *keys, int *ids, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int Lk[64];
    int Li[64];
    int Rk[64];
    int Ri[64];
    int i;
    int j;
    int k;
    for (i = 0; i < n1; i++) {
        Lk[i] = keys[left + i];
        Li[i] = ids[left + i];
    }
    for (j = 0; j < n2; j++) {
        Rk[j] = keys[mid + 1 + j];
        Ri[j] = ids[mid + 1 + j];
    }
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (Lk[i] <= Rk[j]) {
            keys[k] = Lk[i];
            ids[k] = Li[i];
            i = i + 1;
        } else {
            keys[k] = Rk[j];
            ids[k] = Ri[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < n1) {
        keys[k] = Lk[i];
        ids[k] = Li[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < n2) {
        keys[k] = Rk[j];
        ids[k] = Ri[j];
        j = j + 1;
        k = k + 1;
    }
}

void merge_sort_stable_helper(int *keys, int *ids, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_stable_helper(keys, ids, left, mid);
        merge_sort_stable_helper(keys, ids, mid + 1, right);
        merge_stable(keys, ids, left, mid, right);
    }
}

int main() {
    printf("=== Merge Sort Tests ===\n");

    // Basic sort
    printf("--- Basic sort ---\n");
    int a1[7] = {38, 27, 43, 3, 9, 82, 10};
    printf("input: ");
    print_array(a1, 7);
    merge_sort(a1, 7);
    printf("sorted: ");
    print_array(a1, 7);

    // Already sorted
    printf("--- Already sorted ---\n");
    int a2[6] = {1, 2, 3, 4, 5, 6};
    printf("input: ");
    print_array(a2, 6);
    merge_sort(a2, 6);
    printf("sorted: ");
    print_array(a2, 6);

    // Reverse sorted
    printf("--- Reverse sorted ---\n");
    int a3[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    printf("input: ");
    print_array(a3, 9);
    merge_sort(a3, 9);
    printf("sorted: ");
    print_array(a3, 9);

    // Single element
    printf("--- Single element ---\n");
    int a4[1] = {42};
    merge_sort(a4, 1);
    printf("sorted: ");
    print_array(a4, 1);

    // Two elements
    printf("--- Two elements ---\n");
    int a5[2] = {5, 2};
    printf("input: ");
    print_array(a5, 2);
    merge_sort(a5, 2);
    printf("sorted: ");
    print_array(a5, 2);

    // All same
    printf("--- All same ---\n");
    int a6[5] = {7, 7, 7, 7, 7};
    printf("input: ");
    print_array(a6, 5);
    merge_sort(a6, 5);
    printf("sorted: ");
    print_array(a6, 5);

    // Duplicates
    printf("--- Duplicates ---\n");
    int a7[7] = {4, 2, 4, 1, 3, 2, 1};
    printf("input: ");
    print_array(a7, 7);
    merge_sort(a7, 7);
    printf("sorted: ");
    print_array(a7, 7);

    // Negative values
    printf("--- Negative values ---\n");
    int a8[7] = {-5, 10, -3, 0, 7, -8, 2};
    printf("input: ");
    print_array(a8, 7);
    merge_sort(a8, 7);
    printf("sorted: ");
    print_array(a8, 7);

    // Merge two sorted arrays
    printf("--- Merge two sorted arrays ---\n");
    int ma[4] = {1, 3, 5, 7};
    int mb[4] = {2, 4, 6, 8};
    int mc[8];
    printf("a: ");
    print_array(ma, 4);
    printf("b: ");
    print_array(mb, 4);
    merge_arrays(ma, 4, mb, 4, mc);
    printf("merged: ");
    print_array(mc, 8);

    // Large array
    printf("--- Large array ---\n");
    int a9[32];
    int i;
    for (i = 0; i < 32; i++) {
        a9[i] = 31 - i;
    }
    merge_sort(a9, 32);
    printf("first: %d last: %d\n", a9[0], a9[31]);

    // Bottom-up merge sort
    printf("--- Bottom-up merge sort ---\n");
    int a10[8] = {6, 3, 8, 1, 5, 2, 7, 4};
    printf("input: ");
    print_array(a10, 8);
    merge_sort_bottomup(a10, 8);
    printf("sorted: ");
    print_array(a10, 8);

    // Stability check
    printf("--- Stability check ---\n");
    int keys[6] = {3, 1, 5, 1, 3, 2};
    int ids[6]  = {0, 1, 2, 3, 4, 5};
    // After stable sort by keys: 1(id=1), 1(id=3), 2(id=5), 3(id=0), 3(id=4), 5(id=2)
    merge_sort_stable_helper(keys, ids, 0, 5);
    printf("keys: ");
    print_array(keys, 6);
    printf("ids: ");
    print_array(ids, 6);

    printf("=== Merge Sort Done ===\n");
    return 0;
}
