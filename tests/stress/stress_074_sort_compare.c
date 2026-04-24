// stress_074_sort_compare.c - Sort algorithm comparison
// EXPECT: bubble_sorted=1
// EXPECT: insertion_sorted=1
// EXPECT: selection_sorted=1
// EXPECT: merge_sorted=1
// EXPECT: all_match=1
// EXPECT: bubble_sum=240
// EXPECT: insertion_sum=240
// EXPECT: selection_sum=240
// EXPECT: merge_sum=240
// EXPECT: bubble_comps=190
// EXPECT: insertion_comps=113
// EXPECT: selection_comps=190
// EXPECT: stable_ok=1

int printf(const char *fmt, ...);

void copy_arr(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

int is_sorted(int *a, int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1]) return 0;
    }
    return 1;
}

int arrays_equal(int *a, int *b, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int arr_sum(int *a, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) s += a[i];
    return s;
}

// Bubble sort
int bubble_sort(int *a, int n) {
    int comps = 0;
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            comps++;
            if (a[j] > a[j + 1]) {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
    return comps;
}

// Insertion sort
int insertion_sort(int *a, int n) {
    int comps = 0;
    int i;
    for (i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0) {
            comps++;
            if (a[j] > key) {
                a[j + 1] = a[j];
                j--;
            } else {
                break;
            }
        }
        a[j + 1] = key;
    }
    return comps;
}

// Selection sort
int selection_sort(int *a, int n) {
    int comps = 0;
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (j = i + 1; j < n; j++) {
            comps++;
            if (a[j] < a[min_idx]) min_idx = j;
        }
        if (min_idx != i) {
            int tmp = a[i];
            a[i] = a[min_idx];
            a[min_idx] = tmp;
        }
    }
    return comps;
}

// Merge sort
int merge_comps;

void merge(int *a, int *tmp, int lo, int mid, int hi) {
    int i;
    for (i = lo; i <= hi; i++) tmp[i] = a[i];
    int left = lo;
    int right = mid + 1;
    int k = lo;
    while (left <= mid && right <= hi) {
        merge_comps++;
        if (tmp[left] <= tmp[right]) {
            a[k++] = tmp[left++];
        } else {
            a[k++] = tmp[right++];
        }
    }
    while (left <= mid) a[k++] = tmp[left++];
    while (right <= hi) a[k++] = tmp[right++];
}

void merge_sort_rec(int *a, int *tmp, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    merge_sort_rec(a, tmp, lo, mid);
    merge_sort_rec(a, tmp, mid + 1, hi);
    merge(a, tmp, lo, mid, hi);
}

int merge_sort(int *a, int n) {
    int tmp[20];
    merge_comps = 0;
    merge_sort_rec(a, tmp, 0, n - 1);
    return merge_comps;
}

// Stability test: sort pairs (value, original_index) by value
// Stable sort preserves original order of equal elements
typedef struct {
    int val;
    int idx;
} Pair;

int stable_insertion_sort(Pair *a, int n) {
    int i;
    for (i = 1; i < n; i++) {
        Pair key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j].val > key.val) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
    // Check stability: equal values should maintain original order
    for (i = 0; i < n - 1; i++) {
        if (a[i].val == a[i + 1].val) {
            if (a[i].idx > a[i + 1].idx) return 0;
        }
    }
    return 1;
}

int main(void) {
    int orig[20] = {17, 3, 25, 13, 8, 1, 20, 15, 6, 11,
                    19, 2, 14, 9, 22, 7, 4, 18, 10, 16};
    int a[20];
    int b[20];
    int c[20];
    int d[20];

    copy_arr(a, orig, 20);
    copy_arr(b, orig, 20);
    copy_arr(c, orig, 20);
    copy_arr(d, orig, 20);

    int bc = bubble_sort(a, 20);
    int ic = insertion_sort(b, 20);
    int sc = selection_sort(c, 20);
    int mc = merge_sort(d, 20);

    printf("bubble_sorted=%d\n", is_sorted(a, 20));
    printf("insertion_sorted=%d\n", is_sorted(b, 20));
    printf("selection_sorted=%d\n", is_sorted(c, 20));
    printf("merge_sorted=%d\n", is_sorted(d, 20));

    // All should produce identical output
    int match = arrays_equal(a, b, 20) && arrays_equal(b, c, 20) && arrays_equal(c, d, 20);
    printf("all_match=%d\n", match);

    // Sum should be preserved (no corruption)
    printf("bubble_sum=%d\n", arr_sum(a, 20));
    printf("insertion_sum=%d\n", arr_sum(b, 20));
    printf("selection_sum=%d\n", arr_sum(c, 20));
    printf("merge_sum=%d\n", arr_sum(d, 20));

    printf("bubble_comps=%d\n", bc);
    printf("insertion_comps=%d\n", ic);
    printf("selection_comps=%d\n", sc);

    // Stability test with duplicates
    Pair pairs[10];
    pairs[0].val = 3; pairs[0].idx = 0;
    pairs[1].val = 1; pairs[1].idx = 1;
    pairs[2].val = 3; pairs[2].idx = 2;
    pairs[3].val = 2; pairs[3].idx = 3;
    pairs[4].val = 1; pairs[4].idx = 4;
    pairs[5].val = 2; pairs[5].idx = 5;
    pairs[6].val = 3; pairs[6].idx = 6;
    pairs[7].val = 1; pairs[7].idx = 7;
    pairs[8].val = 2; pairs[8].idx = 8;
    pairs[9].val = 3; pairs[9].idx = 9;

    printf("stable_ok=%d\n", stable_insertion_sort(pairs, 10));

    return 0;
}
