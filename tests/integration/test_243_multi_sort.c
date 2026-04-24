int printf(const char *fmt, ...);
// EXPECT: original: 64 25 12 22 11 90 45 78 33 55\nbubble: 11 12 22 25 33 45 55 64 78 90\nsorted=1\nselection: 11 12 22 25 33 45 55 64 78 90\nsorted=1\ninsertion: 11 12 22 25 33 45 55 64 78 90\nsorted=1\nmerge: 11 12 22 25 33 45 55 64 78 90\nsorted=1\nsmall_bubble: 1 2 3\nsingle: 42
// Test 243: Multiple sorting algorithms in one program

void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
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

void copy_arr(int *dst, int *src, int n) {
    int i = 0;
    while (i < n) { dst[i] = src[i]; i = i + 1; }
}

int is_sorted(int *arr, int n) {
    int i = 1;
    while (i < n) {
        if (arr[i] < arr[i - 1]) return 0;
        i = i + 1;
    }
    return 1;
}

void bubble_sort(int *arr, int n) {
    int i = 0;
    while (i < n - 1) {
        int j = 0;
        while (j < n - 1 - i) {
            if (arr[j] > arr[j + 1]) swap(&arr[j], &arr[j + 1]);
            j = j + 1;
        }
        i = i + 1;
    }
}

void selection_sort(int *arr, int n) {
    int i = 0;
    while (i < n - 1) {
        int min_idx = i;
        int j = i + 1;
        while (j < n) {
            if (arr[j] < arr[min_idx]) min_idx = j;
            j = j + 1;
        }
        swap(&arr[i], &arr[min_idx]);
        i = i + 1;
    }
}

void insertion_sort(int *arr, int n) {
    int i = 1;
    while (i < n) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
        i = i + 1;
    }
}

int temp_merge[50];
void merge(int *arr, int l, int m, int r) {
    int i = l; int j = m + 1; int k = l;
    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) { temp_merge[k] = arr[i]; i = i + 1; }
        else { temp_merge[k] = arr[j]; j = j + 1; }
        k = k + 1;
    }
    while (i <= m) { temp_merge[k] = arr[i]; i = i + 1; k = k + 1; }
    while (j <= r) { temp_merge[k] = arr[j]; j = j + 1; k = k + 1; }
    i = l;
    while (i <= r) { arr[i] = temp_merge[i]; i = i + 1; }
}

void merge_sort(int *arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    merge_sort(arr, l, m);
    merge_sort(arr, m + 1, r);
    merge(arr, l, m, r);
}

int main(void) {
    int orig[10];
    orig[0]=64; orig[1]=25; orig[2]=12; orig[3]=22; orig[4]=11;
    orig[5]=90; orig[6]=45; orig[7]=78; orig[8]=33; orig[9]=55;
    int work[10];

    printf("original: "); print_arr(orig, 10);

    copy_arr(work, orig, 10);
    bubble_sort(work, 10);
    printf("bubble: "); print_arr(work, 10);
    printf("sorted=%d\n", is_sorted(work, 10));

    copy_arr(work, orig, 10);
    selection_sort(work, 10);
    printf("selection: "); print_arr(work, 10);
    printf("sorted=%d\n", is_sorted(work, 10));

    copy_arr(work, orig, 10);
    insertion_sort(work, 10);
    printf("insertion: "); print_arr(work, 10);
    printf("sorted=%d\n", is_sorted(work, 10));

    copy_arr(work, orig, 10);
    merge_sort(work, 0, 9);
    printf("merge: "); print_arr(work, 10);
    printf("sorted=%d\n", is_sorted(work, 10));

    int small[3]; small[0] = 3; small[1] = 1; small[2] = 2;
    bubble_sort(small, 3);
    printf("small_bubble: "); print_arr(small, 3);

    int single[1]; single[0] = 42;
    insertion_sort(single, 1);
    printf("single: "); print_arr(single, 1);

    return 0;
}
