int printf(const char *fmt, ...);

/* ---- Utilities ---- */
void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

void print_arr(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) printf("%d\n", arr[i]);
}

/* ---- Quicksort ---- */
int partition(int arr[], int lo, int hi) {
    int pivot = arr[hi];
    int i = lo - 1, j;
    for (j = lo; j < hi; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[hi]);
    return i + 1;
}

void quicksort(int arr[], int lo, int hi) {
    if (lo < hi) {
        int p = partition(arr, lo, hi);
        quicksort(arr, lo, p - 1);
        quicksort(arr, p + 1, hi);
    }
}

/* ---- Merge Sort (iterative) ---- */
void merge(int arr[], int l, int m, int r, int tmp[]) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
        else                  tmp[k++] = arr[j++];
    }
    while (i <= m) tmp[k++] = arr[i++];
    while (j <= r) tmp[k++] = arr[j++];
    for (k = l; k <= r; k++) arr[k] = tmp[k];
}

void mergesort(int arr[], int n) {
    int tmp[32];
    int width, lo;
    for (width = 1; width < n; width *= 2) {
        for (lo = 0; lo < n; lo += 2 * width) {
            int mid = lo + width - 1;
            int hi  = lo + 2 * width - 1;
            if (mid >= n) mid = n - 1;
            if (hi  >= n) hi  = n - 1;
            if (mid < hi) merge(arr, lo, mid, hi, tmp);
        }
    }
}

/* ---- Counting Sort (non-negative values <= 255) ---- */
void counting_sort(int arr[], int n, int out[]) {
    int cnt[256];
    int i;
    for (i = 0; i < 256; i++) cnt[i] = 0;
    for (i = 0; i < n; i++) cnt[arr[i]]++;
    int k = 0;
    for (i = 0; i < 256; i++) {
        int j;
        for (j = 0; j < cnt[i]; j++) out[k++] = i;
    }
}

/* ---- Radix Sort (LSD, base 10, non-negative) ---- */
void radix_sort(int arr[], int n) {
    int tmp[32], cnt[10];
    int exp, i, d;
    /* find max */
    int mx = arr[0];
    for (i = 1; i < n; i++) if (arr[i] > mx) mx = arr[i];
    for (exp = 1; mx / exp > 0; exp *= 10) {
        for (d = 0; d < 10; d++) cnt[d] = 0;
        for (i = 0; i < n; i++) cnt[(arr[i] / exp) % 10]++;
        /* prefix sum */
        for (d = 1; d < 10; d++) cnt[d] += cnt[d-1];
        for (i = n - 1; i >= 0; i--) {
            d = (arr[i] / exp) % 10;
            tmp[--cnt[d]] = arr[i];
        }
        for (i = 0; i < n; i++) arr[i] = tmp[i];
    }
}

/* ---- Binary Search ---- */
int binary_search(int arr[], int n, int target) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

/* First index >= target (lower bound) */
int lower_bound(int arr[], int n, int target) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/* ---- Insertion Sort ---- */
void insertion_sort(int arr[], int n) {
    int i, j, key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

int main(void) {
    int i;

    /* Quicksort */
    {
        int a[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
        quicksort(a, 0, 8);
        print_arr(a, 9);
        // EXPECT: 1
        // EXPECT: 2
        // EXPECT: 3
        // EXPECT: 4
        // EXPECT: 5
        // EXPECT: 6
        // EXPECT: 7
        // EXPECT: 8
        // EXPECT: 9
    }

    /* Mergesort */
    {
        int a[] = {38, 27, 43, 3, 9, 82, 10};
        mergesort(a, 7);
        print_arr(a, 7);
        // EXPECT: 3
        // EXPECT: 9
        // EXPECT: 10
        // EXPECT: 27
        // EXPECT: 38
        // EXPECT: 43
        // EXPECT: 82
    }

    /* Counting sort */
    {
        int a[] = {4, 2, 2, 8, 3, 3, 1};
        int out[7];
        counting_sort(a, 7, out);
        print_arr(out, 7);
        // EXPECT: 1
        // EXPECT: 2
        // EXPECT: 2
        // EXPECT: 3
        // EXPECT: 3
        // EXPECT: 4
        // EXPECT: 8
    }

    /* Radix sort */
    {
        int a[] = {170, 45, 75, 90, 802, 24, 2, 66};
        radix_sort(a, 8);
        print_arr(a, 8);
        // EXPECT: 2
        // EXPECT: 24
        // EXPECT: 45
        // EXPECT: 66
        // EXPECT: 75
        // EXPECT: 90
        // EXPECT: 170
        // EXPECT: 802
    }

    /* Binary search */
    {
        int a[] = {1, 3, 5, 7, 9, 11, 13};
        printf("%d\n", binary_search(a, 7, 7));   // EXPECT: 3
        printf("%d\n", binary_search(a, 7, 1));   // EXPECT: 0
        printf("%d\n", binary_search(a, 7, 13));  // EXPECT: 6
        printf("%d\n", binary_search(a, 7, 6));   // EXPECT: -1
    }

    /* Lower bound */
    {
        int a[] = {1, 3, 3, 5, 7};
        printf("%d\n", lower_bound(a, 5, 3));  // EXPECT: 1
        printf("%d\n", lower_bound(a, 5, 4));  // EXPECT: 3
        printf("%d\n", lower_bound(a, 5, 8));  // EXPECT: 5
    }

    /* Insertion sort */
    {
        int a[] = {64, 25, 12, 22, 11};
        insertion_sort(a, 5);
        print_arr(a, 5);
        // EXPECT: 11
        // EXPECT: 12
        // EXPECT: 22
        // EXPECT: 25
        // EXPECT: 64
    }

    return 0;
}
