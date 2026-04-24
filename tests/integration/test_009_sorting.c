int printf(const char *fmt, ...);
// EXPECT: bub_rand=0 1 2 3 4 5 6 7 8 9
// sorted=1
// bub_sorted=1 2 3 4 5
// bub_rev=1 2 3 4 5
// bub_swaps=6
// bub_swap_result=1 2 3 4
// sel_rand=0 1 2 3 4 5 6 7 8 9
// sel_dup=1 1 2 3 3 4 5 5 6 9
// sel_swaps=2
// ins_rand=0 1 2 3 4 5 6 7 8 9
// ins_near_shifts=1
// ins_near=1 2 3 4 5
// ins_rev_shifts=45
// ins_rev=1 2 3 4 5 6 7 8 9 10
// shell=0 1 2 3 4 5 6 7 8 9
// merge=0 1 2 3 4 5 6 7 8 9
// heap=0 1 2 3 4 5 6 7 8 9
// counting=0 1 2 2 3 3 4 5 7 8
// cocktail=0 1 2 3 4 5 6 7 8 9
// gnome=0 1 2 3 4 5 6 7 8 9
// all_zero=0 0 0 0 0
// single=42
// two_sorted=1 2
// two_rev=3 5
// inversions=3
// inversions_sorted=0
// inversions_rev=10
// partial3=0 1 2
// sum_preserved=1
// stable_keys=1 1 2 3 3
// stable_vals=1 4 3 0 2
// is_stable=1
// neg_sort=-7 -3 -1 0 5
// large=0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
// done=1

// ==========================================================================
// Array utility functions
// ==========================================================================

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(" ");
    }
    printf("\n");
}

void swap(int *a, int *b) {
    int t;
    t = *a;
    *a = *b;
    *b = t;
}

void copy_arr(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        dst[i] = src[i];
    }
}

int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int arr_equal(int *a, int *b, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int sum_arr(int *arr, int n) {
    int s;
    int i;
    s = 0;
    for (i = 0; i < n; i = i + 1) {
        s = s + arr[i];
    }
    return s;
}

// ==========================================================================
// Bubble sort
// ==========================================================================

void bubble_sort(int *arr, int n) {
    int i;
    int j;
    int swapped;
    for (i = 0; i < n - 1; i = i + 1) {
        swapped = 0;
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

int bubble_sort_count_swaps(int *arr, int n) {
    int i;
    int j;
    int count;
    count = 0;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                count = count + 1;
            }
        }
    }
    return count;
}

// ==========================================================================
// Selection sort
// ==========================================================================

void selection_sort(int *arr, int n) {
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
            swap(&arr[i], &arr[min_idx]);
        }
    }
}

int selection_sort_count_swaps(int *arr, int n) {
    int i;
    int j;
    int min_idx;
    int count;
    count = 0;
    for (i = 0; i < n - 1; i = i + 1) {
        min_idx = i;
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(&arr[i], &arr[min_idx]);
            count = count + 1;
        }
    }
    return count;
}

// ==========================================================================
// Insertion sort
// ==========================================================================

void insertion_sort(int *arr, int n) {
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

int insertion_sort_count_shifts(int *arr, int n) {
    int i;
    int j;
    int key;
    int count;
    count = 0;
    for (i = 1; i < n; i = i + 1) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
            count = count + 1;
        }
        arr[j + 1] = key;
    }
    return count;
}

// ==========================================================================
// Shell sort
// ==========================================================================

void shell_sort(int *arr, int n) {
    int gap;
    int i;
    int j;
    int temp;
    gap = n / 2;
    while (gap > 0) {
        for (i = gap; i < n; i = i + 1) {
            temp = arr[i];
            j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j = j - gap;
            }
            arr[j] = temp;
        }
        gap = gap / 2;
    }
}

// ==========================================================================
// Merge sort (iterative, bottom-up)
// ==========================================================================

void merge(int *arr, int *temp, int left, int mid, int right) {
    int i;
    int j;
    int k;
    i = left;
    j = mid;
    k = left;
    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i = i + 1;
        } else {
            temp[k] = arr[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < mid) {
        temp[k] = arr[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < right) {
        temp[k] = arr[j];
        j = j + 1;
        k = k + 1;
    }
    for (i = left; i < right; i = i + 1) {
        arr[i] = temp[i];
    }
}

void merge_sort(int *arr, int n) {
    int temp[50];
    int width;
    int left;
    int mid;
    int right;
    for (width = 1; width < n; width = width * 2) {
        for (left = 0; left < n; left = left + 2 * width) {
            mid = left + width;
            if (mid > n) mid = n;
            right = left + 2 * width;
            if (right > n) right = n;
            if (mid < right) {
                merge(arr, temp, left, mid, right);
            }
        }
    }
}

// ==========================================================================
// Heap sort
// ==========================================================================

void sift_down(int *arr, int start, int end) {
    int root;
    int child;
    int swap_idx;
    root = start;
    while (root * 2 + 1 <= end) {
        child = root * 2 + 1;
        swap_idx = root;
        if (arr[swap_idx] < arr[child]) {
            swap_idx = child;
        }
        if (child + 1 <= end && arr[swap_idx] < arr[child + 1]) {
            swap_idx = child + 1;
        }
        if (swap_idx == root) return;
        swap(&arr[root], &arr[swap_idx]);
        root = swap_idx;
    }
}

void heap_sort(int *arr, int n) {
    int start;
    int end;
    // Build heap
    start = (n - 2) / 2;
    while (start >= 0) {
        sift_down(arr, start, n - 1);
        start = start - 1;
    }
    // Sort
    end = n - 1;
    while (end > 0) {
        swap(&arr[0], &arr[end]);
        end = end - 1;
        sift_down(arr, 0, end);
    }
}

// ==========================================================================
// Counting sort (for small non-negative values)
// ==========================================================================

void counting_sort(int *arr, int n, int max_val) {
    int counts[50];
    int i;
    int j;
    int idx;
    for (i = 0; i <= max_val; i = i + 1) {
        counts[i] = 0;
    }
    for (i = 0; i < n; i = i + 1) {
        counts[arr[i]] = counts[arr[i]] + 1;
    }
    idx = 0;
    for (i = 0; i <= max_val; i = i + 1) {
        for (j = 0; j < counts[i]; j = j + 1) {
            arr[idx] = i;
            idx = idx + 1;
        }
    }
}

// ==========================================================================
// Cocktail shaker sort
// ==========================================================================

void cocktail_sort(int *arr, int n) {
    int swapped;
    int start;
    int end;
    int i;
    start = 0;
    end = n - 1;
    swapped = 1;
    while (swapped) {
        swapped = 0;
        for (i = start; i < end; i = i + 1) {
            if (arr[i] > arr[i + 1]) {
                swap(&arr[i], &arr[i + 1]);
                swapped = 1;
            }
        }
        if (!swapped) break;
        swapped = 0;
        end = end - 1;
        for (i = end; i > start; i = i - 1) {
            if (arr[i - 1] > arr[i]) {
                swap(&arr[i - 1], &arr[i]);
                swapped = 1;
            }
        }
        start = start + 1;
    }
}

// ==========================================================================
// Gnome sort
// ==========================================================================

void gnome_sort(int *arr, int n) {
    int pos;
    pos = 0;
    while (pos < n) {
        if (pos == 0 || arr[pos] >= arr[pos - 1]) {
            pos = pos + 1;
        } else {
            swap(&arr[pos], &arr[pos - 1]);
            pos = pos - 1;
        }
    }
}

// ==========================================================================
// Utility for inversions count
// ==========================================================================

int count_inversions(int *arr, int n) {
    int count;
    int i;
    int j;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[i] > arr[j]) count = count + 1;
        }
    }
    return count;
}

// ==========================================================================
// Partial sort (find k smallest)
// ==========================================================================

void partial_sort(int *arr, int n, int k) {
    // Simple: selection sort first k elements
    int i;
    int j;
    int min_idx;
    for (i = 0; i < k; i = i + 1) {
        min_idx = i;
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(&arr[i], &arr[min_idx]);
        }
    }
}

// ==========================================================================
// Check stability (sort pairs and see if equal keys maintain order)
// ==========================================================================

int check_stability(int *keys, int *vals, int n) {
    // After sorting by keys, check if vals for same keys are in original order
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (keys[i] == keys[i + 1]) {
            if (vals[i] > vals[i + 1]) return 0;
        }
    }
    return 1;
}

// ==========================================================================
// Main function
// ==========================================================================

int main(void) {
    int arr[20];
    int arr2[20];
    int orig[20];
    int keys[10];
    int vals[10];
    int swaps;
    int shifts;

    // Test 1: Bubble sort random
    arr[0]=5; arr[1]=3; arr[2]=8; arr[3]=1; arr[4]=9;
    arr[5]=2; arr[6]=7; arr[7]=4; arr[8]=6; arr[9]=0;
    bubble_sort(arr, 10);
    printf("bub_rand=");
    print_arr(arr, 10);
    printf("sorted=%d\n", is_sorted(arr, 10));

    // Test 2: Bubble sort already sorted
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    bubble_sort(arr, 5);
    printf("bub_sorted=");
    print_arr(arr, 5);

    // Test 3: Bubble sort reverse
    arr[0]=5; arr[1]=4; arr[2]=3; arr[3]=2; arr[4]=1;
    bubble_sort(arr, 5);
    printf("bub_rev=");
    print_arr(arr, 5);

    // Test 4: Bubble sort count swaps
    arr[0]=4; arr[1]=3; arr[2]=2; arr[3]=1;
    swaps = bubble_sort_count_swaps(arr, 4);
    printf("bub_swaps=%d\n", swaps);
    printf("bub_swap_result=");
    print_arr(arr, 4);

    // Test 5: Selection sort random
    arr[0]=7; arr[1]=2; arr[2]=9; arr[3]=4; arr[4]=1;
    arr[5]=8; arr[6]=3; arr[7]=6; arr[8]=5; arr[9]=0;
    selection_sort(arr, 10);
    printf("sel_rand=");
    print_arr(arr, 10);

    // Test 6: Selection sort duplicates
    arr[0]=3; arr[1]=1; arr[2]=4; arr[3]=1; arr[4]=5;
    arr[5]=9; arr[6]=2; arr[7]=6; arr[8]=5; arr[9]=3;
    selection_sort(arr, 10);
    printf("sel_dup=");
    print_arr(arr, 10);

    // Test 7: Selection sort count swaps
    arr[0]=5; arr[1]=4; arr[2]=3; arr[3]=2; arr[4]=1;
    swaps = selection_sort_count_swaps(arr, 5);
    printf("sel_swaps=%d\n", swaps);

    // Test 8: Insertion sort random
    arr[0]=6; arr[1]=4; arr[2]=8; arr[3]=2; arr[4]=0;
    arr[5]=9; arr[6]=1; arr[7]=7; arr[8]=3; arr[9]=5;
    insertion_sort(arr, 10);
    printf("ins_rand=");
    print_arr(arr, 10);

    // Test 9: Insertion sort nearly sorted
    arr[0]=1; arr[1]=2; arr[2]=4; arr[3]=3; arr[4]=5;
    shifts = insertion_sort_count_shifts(arr, 5);
    printf("ins_near_shifts=%d\n", shifts);
    printf("ins_near=");
    print_arr(arr, 5);

    // Test 10: Insertion sort reverse
    arr[0]=10; arr[1]=9; arr[2]=8; arr[3]=7; arr[4]=6;
    arr[5]=5; arr[6]=4; arr[7]=3; arr[8]=2; arr[9]=1;
    shifts = insertion_sort_count_shifts(arr, 10);
    printf("ins_rev_shifts=%d\n", shifts);
    printf("ins_rev=");
    print_arr(arr, 10);

    // Test 11: Shell sort
    arr[0]=9; arr[1]=5; arr[2]=1; arr[3]=7; arr[4]=3;
    arr[5]=8; arr[6]=2; arr[7]=6; arr[8]=0; arr[9]=4;
    shell_sort(arr, 10);
    printf("shell=");
    print_arr(arr, 10);

    // Test 12: Merge sort
    arr[0]=3; arr[1]=7; arr[2]=1; arr[3]=9; arr[4]=5;
    arr[5]=0; arr[6]=8; arr[7]=2; arr[8]=6; arr[9]=4;
    merge_sort(arr, 10);
    printf("merge=");
    print_arr(arr, 10);

    // Test 13: Heap sort
    arr[0]=4; arr[1]=6; arr[2]=2; arr[3]=8; arr[4]=0;
    arr[5]=1; arr[6]=9; arr[7]=3; arr[8]=7; arr[9]=5;
    heap_sort(arr, 10);
    printf("heap=");
    print_arr(arr, 10);

    // Test 14: Counting sort
    arr[0]=4; arr[1]=2; arr[2]=2; arr[3]=8; arr[4]=3;
    arr[5]=3; arr[6]=1; arr[7]=0; arr[8]=5; arr[9]=7;
    counting_sort(arr, 10, 9);
    printf("counting=");
    print_arr(arr, 10);

    // Test 15: Cocktail sort
    arr[0]=8; arr[1]=3; arr[2]=6; arr[3]=1; arr[4]=9;
    arr[5]=0; arr[6]=7; arr[7]=4; arr[8]=2; arr[9]=5;
    cocktail_sort(arr, 10);
    printf("cocktail=");
    print_arr(arr, 10);

    // Test 16: Gnome sort
    arr[0]=7; arr[1]=1; arr[2]=5; arr[3]=3; arr[4]=9;
    arr[5]=2; arr[6]=8; arr[7]=0; arr[8]=6; arr[9]=4;
    gnome_sort(arr, 10);
    printf("gnome=");
    print_arr(arr, 10);

    // Test 17: All zeros
    arr[0]=0; arr[1]=0; arr[2]=0; arr[3]=0; arr[4]=0;
    bubble_sort(arr, 5);
    printf("all_zero=");
    print_arr(arr, 5);

    // Test 18: Single element
    arr[0]=42;
    selection_sort(arr, 1);
    printf("single=%d\n", arr[0]);

    // Test 19: Two elements (sorted)
    arr[0]=1; arr[1]=2;
    insertion_sort(arr, 2);
    printf("two_sorted=");
    print_arr(arr, 2);

    // Test 20: Two elements (reverse)
    arr[0]=5; arr[1]=3;
    bubble_sort(arr, 2);
    printf("two_rev=");
    print_arr(arr, 2);

    // Test 21: Count inversions
    arr[0]=2; arr[1]=4; arr[2]=1; arr[3]=3; arr[4]=5;
    printf("inversions=%d\n", count_inversions(arr, 5));

    // Test 22: Sorted has 0 inversions
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    printf("inversions_sorted=%d\n", count_inversions(arr, 5));

    // Test 23: Reverse has max inversions
    arr[0]=5; arr[1]=4; arr[2]=3; arr[3]=2; arr[4]=1;
    printf("inversions_rev=%d\n", count_inversions(arr, 5));

    // Test 24: Partial sort (3 smallest)
    arr[0]=7; arr[1]=2; arr[2]=9; arr[3]=4; arr[4]=1;
    arr[5]=8; arr[6]=3; arr[7]=6; arr[8]=5; arr[9]=0;
    partial_sort(arr, 10, 3);
    printf("partial3=");
    print_arr(arr, 3);

    // Test 25: Sum preserved after sort
    arr[0]=5; arr[1]=3; arr[2]=8; arr[3]=1; arr[4]=4;
    {
        int sum_before;
        int sum_after;
        sum_before = sum_arr(arr, 5);
        bubble_sort(arr, 5);
        sum_after = sum_arr(arr, 5);
        printf("sum_preserved=%d\n", sum_before == sum_after);
    }

    // Test 26: Stability check with insertion sort
    keys[0]=3; keys[1]=1; keys[2]=3; keys[3]=2; keys[4]=1;
    vals[0]=0; vals[1]=1; vals[2]=2; vals[3]=3; vals[4]=4;
    // insertion sort both arrays together by key
    {
        int ik;
        int jk;
        int kkey;
        int kval;
        for (ik = 1; ik < 5; ik = ik + 1) {
            kkey = keys[ik];
            kval = vals[ik];
            jk = ik - 1;
            while (jk >= 0 && keys[jk] > kkey) {
                keys[jk + 1] = keys[jk];
                vals[jk + 1] = vals[jk];
                jk = jk - 1;
            }
            keys[jk + 1] = kkey;
            vals[jk + 1] = kval;
        }
    }
    printf("stable_keys=");
    print_arr(keys, 5);
    printf("stable_vals=");
    print_arr(vals, 5);
    printf("is_stable=%d\n", check_stability(keys, vals, 5));

    // Test 27: Sort negative numbers
    arr[0]=-3; arr[1]=5; arr[2]=-1; arr[3]=0; arr[4]=-7;
    insertion_sort(arr, 5);
    printf("neg_sort=");
    print_arr(arr, 5);

    // Test 28: Large array
    arr[0]=15; arr[1]=8; arr[2]=12; arr[3]=3; arr[4]=19;
    arr[5]=7; arr[6]=14; arr[7]=1; arr[8]=16; arr[9]=10;
    arr[10]=5; arr[11]=18; arr[12]=2; arr[13]=11; arr[14]=17;
    arr[15]=6; arr[16]=13; arr[17]=4; arr[18]=9; arr[19]=0;
    merge_sort(arr, 20);
    printf("large=");
    print_arr(arr, 20);

    printf("done=1\n");

    return 0;
}
