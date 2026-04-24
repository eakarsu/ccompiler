int printf(const char *fmt, ...);
// EXPECT: rot left 2: 3 4 5 1 2\nrot right 2: 4 5 1 2 3\nsorted: 1 2 3 4 5 6 7 8 9\nmerged 9: 1 2 3 4 5 6 7 8 10\ndups: 3\ninversions: 10\ninversions sorted: 0\ninversions mixed: 3\nmax subarray: 6\nmax sub all pos: 15\nmax sub all neg: -1\nbsearch 7: 3\nbsearch 1: 0\nbsearch 13: 6\nbsearch 6: -1\nsum: 15\nproduct: 120\nmin*1000+max: 1009\nafter dedup 4: 1 2 3 5\nzeros moved 3: 1 3 5 0 0 0\nis sorted: 1\nnot sorted: 0\n2nd largest: 7\ndutch: 0 0 1 1 2 2\nprefix: 1 3 6 10 15\nrange 1-3: 9\nrange 0-4: 15\nbubble: 1 2 3 4 5\nselection: 1 3 5 7 9\ninsertion: 2 4 6 8 10

// ============================================================
// test_020_array_algo.c
// Array algorithms: rotation, partition, merge sorted,
// find duplicates, count inversions, Kadane's variant
// ============================================================

// -----------------------------------------------------------
// Print array helper
// -----------------------------------------------------------
void print_array(int arr[50], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (i > 0) {
            printf("%c", 32);
        }
        printf("%d", arr[i]);
    }
    printf("%c", 10);
}

// -----------------------------------------------------------
// Copy array
// -----------------------------------------------------------
void copy_array(int src[50], int dst[50], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        dst[i] = src[i];
    }
}

// -----------------------------------------------------------
// Swap two elements
// -----------------------------------------------------------
void swap(int arr[50], int i, int j) {
    int tmp;
    tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

// -----------------------------------------------------------
// Rotate array left by k positions
// -----------------------------------------------------------
void reverse_sub(int arr[50], int lo, int hi) {
    while (lo < hi) {
        swap(arr, lo, hi);
        lo = lo + 1;
        hi = hi - 1;
    }
}

void rotate_left(int arr[50], int n, int k) {
    k = k % n;
    if (k == 0) {
        return;
    }
    reverse_sub(arr, 0, k - 1);
    reverse_sub(arr, k, n - 1);
    reverse_sub(arr, 0, n - 1);
}

// -----------------------------------------------------------
// Rotate array right by k positions
// -----------------------------------------------------------
void rotate_right(int arr[50], int n, int k) {
    k = k % n;
    if (k == 0) {
        return;
    }
    reverse_sub(arr, 0, n - 1);
    reverse_sub(arr, 0, k - 1);
    reverse_sub(arr, k, n - 1);
}

// -----------------------------------------------------------
// Partition around pivot value (Lomuto scheme)
// Returns pivot index
// -----------------------------------------------------------
int partition(int arr[50], int lo, int hi) {
    int pivot;
    int i;
    int j;
    pivot = arr[hi];
    i = lo;
    for (j = lo; j < hi; j = j + 1) {
        if (arr[j] <= pivot) {
            swap(arr, i, j);
            i = i + 1;
        }
    }
    swap(arr, i, hi);
    return i;
}

// -----------------------------------------------------------
// QuickSort
// -----------------------------------------------------------
void quicksort(int arr[50], int lo, int hi) {
    int p;
    if (lo < hi) {
        p = partition(arr, lo, hi);
        quicksort(arr, lo, p - 1);
        quicksort(arr, p + 1, hi);
    }
}

// -----------------------------------------------------------
// Merge two sorted arrays into result
// Returns total length
// -----------------------------------------------------------
int merge_sorted(int a[50], int na, int b[50], int nb, int result[50]) {
    int i;
    int j;
    int k;
    i = 0;
    j = 0;
    k = 0;
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
    return k;
}

// -----------------------------------------------------------
// Find duplicates: return count of duplicate values
// (works on sorted array)
// -----------------------------------------------------------
int count_duplicates_sorted(int arr[50], int n) {
    int i;
    int cnt;
    cnt = 0;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] == arr[i - 1]) {
            cnt = cnt + 1;
        }
    }
    return cnt;
}

// -----------------------------------------------------------
// Count inversions (brute force O(n^2))
// An inversion is a pair (i,j) where i<j but arr[i]>arr[j]
// -----------------------------------------------------------
int count_inversions(int arr[50], int n) {
    int i;
    int j;
    int cnt;
    cnt = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[i] > arr[j]) {
                cnt = cnt + 1;
            }
        }
    }
    return cnt;
}

// -----------------------------------------------------------
// Kadane's algorithm variant: max subarray sum
// Returns the maximum sum of any contiguous subarray
// -----------------------------------------------------------
int max_subarray_sum(int arr[50], int n) {
    int max_ending;
    int max_so_far;
    int i;
    max_ending = arr[0];
    max_so_far = arr[0];
    for (i = 1; i < n; i = i + 1) {
        if (max_ending + arr[i] > arr[i]) {
            max_ending = max_ending + arr[i];
        } else {
            max_ending = arr[i];
        }
        if (max_ending > max_so_far) {
            max_so_far = max_ending;
        }
    }
    return max_so_far;
}

// -----------------------------------------------------------
// Binary search on sorted array, returns index or -1
// -----------------------------------------------------------
int binary_search(int arr[50], int n, int target) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = n - 1;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

// -----------------------------------------------------------
// Sum of array
// -----------------------------------------------------------
int array_sum(int arr[50], int n) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < n; i = i + 1) {
        s = s + arr[i];
    }
    return s;
}

// -----------------------------------------------------------
// Array product
// -----------------------------------------------------------
int array_product(int arr[50], int n) {
    int i;
    int p;
    p = 1;
    for (i = 0; i < n; i = i + 1) {
        p = p * arr[i];
    }
    return p;
}

// -----------------------------------------------------------
// Find min and max simultaneously, return min*1000+max
// -----------------------------------------------------------
int find_min_max(int arr[50], int n) {
    int mn;
    int mx;
    int i;
    mn = arr[0];
    mx = arr[0];
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] < mn) {
            mn = arr[i];
        }
        if (arr[i] > mx) {
            mx = arr[i];
        }
    }
    return mn * 1000 + mx;
}

// -----------------------------------------------------------
// Remove duplicates from sorted array in-place
// Returns new length
// -----------------------------------------------------------
int remove_dups(int arr[50], int n) {
    int i;
    int j;
    if (n <= 1) {
        return n;
    }
    j = 0;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] != arr[j]) {
            j = j + 1;
            arr[j] = arr[i];
        }
    }
    return j + 1;
}

// -----------------------------------------------------------
// Move all zeros to end, return count of non-zeros
// -----------------------------------------------------------
int move_zeros_end(int arr[50], int n) {
    int i;
    int j;
    j = 0;
    for (i = 0; i < n; i = i + 1) {
        if (arr[i] != 0) {
            arr[j] = arr[i];
            j = j + 1;
        }
    }
    for (i = j; i < n; i = i + 1) {
        arr[i] = 0;
    }
    return j;
}

// -----------------------------------------------------------
// Check if array is sorted (ascending)
// -----------------------------------------------------------
int is_sorted(int arr[50], int n) {
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] < arr[i - 1]) {
            return 0;
        }
    }
    return 1;
}

// -----------------------------------------------------------
// Second largest element
// -----------------------------------------------------------
int second_largest(int arr[50], int n) {
    int first;
    int second;
    int i;
    first = arr[0];
    second = arr[0];
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] > first) {
            second = first;
            first = arr[i];
        } else if (arr[i] > second && arr[i] != first) {
            second = arr[i];
        }
    }
    return second;
}

// -----------------------------------------------------------
// Dutch national flag partition (0s, 1s, 2s)
// -----------------------------------------------------------
void dutch_flag(int arr[50], int n) {
    int lo;
    int mid;
    int hi;
    lo = 0;
    mid = 0;
    hi = n - 1;
    while (mid <= hi) {
        if (arr[mid] == 0) {
            swap(arr, lo, mid);
            lo = lo + 1;
            mid = mid + 1;
        } else if (arr[mid] == 1) {
            mid = mid + 1;
        } else {
            swap(arr, mid, hi);
            hi = hi - 1;
        }
    }
}

// -----------------------------------------------------------
// Prefix sum array
// -----------------------------------------------------------
void prefix_sum(int arr[50], int pre[50], int n) {
    int i;
    pre[0] = arr[0];
    for (i = 1; i < n; i = i + 1) {
        pre[i] = pre[i - 1] + arr[i];
    }
}

// -----------------------------------------------------------
// Range sum using prefix sums (inclusive [l,r])
// -----------------------------------------------------------
int range_sum(int pre[50], int l, int r) {
    if (l == 0) {
        return pre[r];
    }
    return pre[r] - pre[l - 1];
}

// -----------------------------------------------------------
// Bubble sort
// -----------------------------------------------------------
void bubble_sort(int arr[50], int n) {
    int i;
    int j;
    int swapped;
    for (i = 0; i < n - 1; i = i + 1) {
        swapped = 0;
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                swap(arr, j, j + 1);
                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
    }
}

// -----------------------------------------------------------
// Selection sort
// -----------------------------------------------------------
void selection_sort(int arr[50], int n) {
    int i;
    int j;
    int min_idx;
    for (i = 0; i < n - 1; i = i + 1) {
        min_idx = i;
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(arr, i, min_idx);
        }
    }
}

// -----------------------------------------------------------
// Insertion sort
// -----------------------------------------------------------
void insertion_sort(int arr[50], int n) {
    int i;
    int j;
    int key;
    for (i = 1; i < n; i = i + 1) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// -----------------------------------------------------------
// MAIN
// -----------------------------------------------------------
int main(void) {
    int arr[50];
    int arr2[50];
    int result[50];
    int pre[50];
    int n;
    int r;
    int i;

    // ---- Rotate left ----
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    rotate_left(arr, 5, 2);
    printf("rot left 2: ");
    print_array(arr, 5);

    // ---- Rotate right ----
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    rotate_right(arr, 5, 2);
    printf("rot right 2: ");
    print_array(arr, 5);

    // ---- Quicksort ----
    arr[0]=5; arr[1]=3; arr[2]=8; arr[3]=1; arr[4]=9; arr[5]=2; arr[6]=7; arr[7]=4; arr[8]=6;
    quicksort(arr, 0, 8);
    printf("sorted: ");
    print_array(arr, 9);

    // ---- Merge sorted ----
    arr[0]=1; arr[1]=3; arr[2]=5; arr[3]=7;
    arr2[0]=2; arr2[1]=4; arr2[2]=6; arr2[3]=8; arr2[4]=10;
    n = merge_sorted(arr, 4, arr2, 5, result);
    printf("merged %d: ", n);
    print_array(result, n);

    // ---- Count duplicates ----
    arr[0]=1; arr[1]=1; arr[2]=2; arr[3]=3; arr[4]=3; arr[5]=3; arr[6]=5;
    printf("dups: %d\n", count_duplicates_sorted(arr, 7));

    // ---- Count inversions ----
    arr[0]=5; arr[1]=4; arr[2]=3; arr[3]=2; arr[4]=1;
    printf("inversions: %d\n", count_inversions(arr, 5));

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    printf("inversions sorted: %d\n", count_inversions(arr, 5));

    arr[0]=2; arr[1]=4; arr[2]=1; arr[3]=3; arr[4]=5;
    printf("inversions mixed: %d\n", count_inversions(arr, 5));

    // ---- Max subarray sum (Kadane) ----
    arr[0]=-2; arr[1]=1; arr[2]=-3; arr[3]=4; arr[4]=-1; arr[5]=2; arr[6]=1; arr[7]=-5; arr[8]=4;
    printf("max subarray: %d\n", max_subarray_sum(arr, 9));

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    printf("max sub all pos: %d\n", max_subarray_sum(arr, 5));

    arr[0]=-1; arr[1]=-2; arr[2]=-3;
    printf("max sub all neg: %d\n", max_subarray_sum(arr, 3));

    // ---- Binary search ----
    arr[0]=1; arr[1]=3; arr[2]=5; arr[3]=7; arr[4]=9; arr[5]=11; arr[6]=13;
    printf("bsearch 7: %d\n", binary_search(arr, 7, 7));
    printf("bsearch 1: %d\n", binary_search(arr, 7, 1));
    printf("bsearch 13: %d\n", binary_search(arr, 7, 13));
    printf("bsearch 6: %d\n", binary_search(arr, 7, 6));

    // ---- Array sum and product ----
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    printf("sum: %d\n", array_sum(arr, 5));
    printf("product: %d\n", array_product(arr, 5));

    // ---- Min max ----
    arr[0]=7; arr[1]=2; arr[2]=9; arr[3]=1; arr[4]=5;
    r = find_min_max(arr, 5);
    printf("min*1000+max: %d\n", r);

    // ---- Remove duplicates ----
    arr[0]=1; arr[1]=1; arr[2]=2; arr[3]=3; arr[4]=3; arr[5]=5;
    n = remove_dups(arr, 6);
    printf("after dedup %d: ", n);
    print_array(arr, n);

    // ---- Move zeros ----
    arr[0]=0; arr[1]=1; arr[2]=0; arr[3]=3; arr[4]=0; arr[5]=5;
    n = move_zeros_end(arr, 6);
    printf("zeros moved %d: ", n);
    print_array(arr, 6);

    // ---- Is sorted ----
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4;
    printf("is sorted: %d\n", is_sorted(arr, 4));
    arr[0]=1; arr[1]=3; arr[2]=2; arr[3]=4;
    printf("not sorted: %d\n", is_sorted(arr, 4));

    // ---- Second largest ----
    arr[0]=5; arr[1]=3; arr[2]=8; arr[3]=1; arr[4]=7;
    printf("2nd largest: %d\n", second_largest(arr, 5));

    // ---- Dutch flag ----
    arr[0]=2; arr[1]=0; arr[2]=1; arr[3]=2; arr[4]=0; arr[5]=1;
    dutch_flag(arr, 6);
    printf("dutch: ");
    print_array(arr, 6);

    // ---- Prefix sums ----
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    prefix_sum(arr, pre, 5);
    printf("prefix: ");
    print_array(pre, 5);
    printf("range 1-3: %d\n", range_sum(pre, 1, 3));
    printf("range 0-4: %d\n", range_sum(pre, 0, 4));

    // ---- Bubble sort ----
    arr[0]=4; arr[1]=2; arr[2]=5; arr[3]=1; arr[4]=3;
    bubble_sort(arr, 5);
    printf("bubble: ");
    print_array(arr, 5);

    // ---- Selection sort ----
    arr[0]=9; arr[1]=7; arr[2]=5; arr[3]=3; arr[4]=1;
    selection_sort(arr, 5);
    printf("selection: ");
    print_array(arr, 5);

    // ---- Insertion sort ----
    arr[0]=8; arr[1]=4; arr[2]=6; arr[3]=2; arr[4]=10;
    insertion_sort(arr, 5);
    printf("insertion: ");
    print_array(arr, 5);

    return 0;
}
