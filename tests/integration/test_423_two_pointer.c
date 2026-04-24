int printf(const char *fmt, ...);
// EXPECT: Two sum 14: [1]=2 + [6]=12\nTwo sum 3: [0]=1 + [1]=2\nTwo sum 25: not found\nUnique: 1 2 3 4 5\nMax water: 49\nMerged: 1 2 3 4 5 6 7 8 10
// Test: Two-pointer technique

int two_sum_sorted(int arr[], int n, int target, int *i_out, int *j_out) {
    int left = 0;
    int right = n - 1;
    while (left < right) {
        int sum = arr[left] + arr[right];
        if (sum == target) {
            *i_out = left;
            *j_out = right;
            return 1;
        }
        if (sum < target) left++;
        else right--;
    }
    return 0;
}

int remove_duplicates(int arr[], int n) {
    if (n == 0) return 0;
    int write = 1;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] != arr[i - 1]) {
            arr[write] = arr[i];
            write++;
        }
    }
    return write;
}

int container_water(int heights[], int n) {
    int left = 0;
    int right = n - 1;
    int max_area = 0;
    while (left < right) {
        int h = heights[left];
        if (heights[right] < h) h = heights[right];
        int area = h * (right - left);
        if (area > max_area) max_area = area;
        if (heights[left] < heights[right]) left++;
        else right--;
    }
    return max_area;
}

void merge_sorted(int a[], int m, int b[], int n, int result[]) {
    int i = 0, j = 0, k = 0;
    while (i < m && j < n) {
        if (a[i] <= b[j]) {
            result[k] = a[i];
            i++;
        } else {
            result[k] = b[j];
            j++;
        }
        k++;
    }
    while (i < m) { result[k] = a[i]; i++; k++; }
    while (j < n) { result[k] = b[j]; j++; k++; }
}

int main(void) {
    int a[] = {1, 2, 4, 6, 8, 10, 12};
    int i, j;
    if (two_sum_sorted(a, 7, 14, &i, &j))
        printf("Two sum 14: [%d]=%d + [%d]=%d\n", i, a[i], j, a[j]);
    if (two_sum_sorted(a, 7, 3, &i, &j))
        printf("Two sum 3: [%d]=%d + [%d]=%d\n", i, a[i], j, a[j]);
    if (!two_sum_sorted(a, 7, 25, &i, &j))
        printf("Two sum 25: not found\n");

    int b[] = {1, 1, 2, 2, 3, 3, 3, 4, 5, 5};
    int new_len = remove_duplicates(b, 10);
    printf("Unique:");
    for (i = 0; i < new_len; i++) printf(" %d", b[i]);
    printf("\n");

    int h[] = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    printf("Max water: %d\n", container_water(h, 9));

    int x[] = {1, 3, 5, 7};
    int y[] = {2, 4, 6, 8, 10};
    int merged[16];
    merge_sorted(x, 4, y, 5, merged);
    printf("Merged:");
    for (i = 0; i < 9; i++) printf(" %d", merged[i]);
    printf("\n");

    return 0;
}
