int printf(const char *fmt, ...);
// EXPECT: init: 1 2 3 4 5 6 7 8 9 10
// sum=55
// max=10
// min=1
// linear_find_7=6
// linear_find_99=-1
// binary_find_5=4
// binary_find_1=0
// binary_find_10=9
// binary_find_11=-1
// reversed: 10 9 8 7 6 5 4 3 2 1
// rotate_left_3: 4 5 6 7 8 9 10 1 2 3
// prefix_sum: 1 3 6 10 15 21 28 36 45 55
// freq_0=0
// freq_1=3
// freq_2=2
// freq_3=3
// freq_4=1
// freq_5=1
// 2d_sum=150
// 2d_row0=15
// 2d_row1=40
// 2d_row2=65
// 2d_row3=30
// col_sum_0=18
// col_sum_1=21
// col_sum_2=24
// col_sum_3=27
// col_sum_4=60
// merge: 1 2 3 4 5 6 7 8
// remove_dups: 1 2 3 4 5 count=5
// intersection: 3 4 5 count=3
// second_largest=9
// is_palindrome_yes=1
// is_palindrome_no=0
// kadane_max=11
// zero_count=3
// even_count=5
// odd_count=5
// product_no_zero=3628800
// running_avg_last=5
// done

// =============================================================================
// TEST 005: Arrays
// =============================================================================

// --- Sum of array ---
int array_sum(int *arr, int n) {
    int s = 0;
    int i = 0;
    while (i < n) {
        s = s + arr[i];
        i = i + 1;
    }
    return s;
}

// --- Max of array ---
int array_max(int *arr, int n) {
    int mx = arr[0];
    int i = 1;
    while (i < n) {
        if (arr[i] > mx) {
            mx = arr[i];
        }
        i = i + 1;
    }
    return mx;
}

// --- Min of array ---
int array_min(int *arr, int n) {
    int mn = arr[0];
    int i = 1;
    while (i < n) {
        if (arr[i] < mn) {
            mn = arr[i];
        }
        i = i + 1;
    }
    return mn;
}

// --- Linear search: returns index or -1 ---
int linear_search(int *arr, int n, int val) {
    int i = 0;
    while (i < n) {
        if (arr[i] == val) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

// --- Binary search on sorted array: returns index or -1 ---
int binary_search(int *arr, int n, int val) {
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == val) {
            return mid;
        } else if (arr[mid] < val) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

// --- Reverse array in-place ---
void array_reverse(int *arr, int n) {
    int i = 0;
    int j = n - 1;
    while (i < j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

// --- Rotate left by k positions using reversal algorithm ---
void rotate_left(int *arr, int n, int k) {
    int i; int j; int tmp;
    // reverse [0..k-1]
    i = 0; j = k - 1;
    while (i < j) {
        tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        i = i + 1; j = j - 1;
    }
    // reverse [k..n-1]
    i = k; j = n - 1;
    while (i < j) {
        tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        i = i + 1; j = j - 1;
    }
    // reverse [0..n-1]
    i = 0; j = n - 1;
    while (i < j) {
        tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        i = i + 1; j = j - 1;
    }
}

// --- Prefix sum: out[i] = arr[0]+...+arr[i] ---
void prefix_sum(int *arr, int *out, int n) {
    out[0] = arr[0];
    int i = 1;
    while (i < n) {
        out[i] = out[i - 1] + arr[i];
        i = i + 1;
    }
}

// --- Frequency counting (values 0..max_val) ---
void freq_count(int *arr, int n, int *freq, int max_val) {
    int i = 0;
    while (i <= max_val) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < n) {
        if (arr[i] >= 0 && arr[i] <= max_val) {
            freq[arr[i]] = freq[arr[i]] + 1;
        }
        i = i + 1;
    }
}

// --- 2D array row sum (flat pointer) ---
int row_sum_2d(int *matrix, int cols, int row) {
    int s = 0;
    int c = 0;
    while (c < cols) {
        s = s + *(matrix + row * cols + c);
        c = c + 1;
    }
    return s;
}

// --- 2D array column sum (flat pointer) ---
int col_sum_2d(int *matrix, int rows, int cols, int col) {
    int s = 0;
    int r = 0;
    while (r < rows) {
        s = s + *(matrix + r * cols + col);
        r = r + 1;
    }
    return s;
}

// --- 2D array total sum ---
int total_sum_2d(int *matrix, int rows, int cols) {
    int s = 0;
    int r = 0;
    while (r < rows) {
        int c = 0;
        while (c < cols) {
            s = s + *(matrix + r * cols + c);
            c = c + 1;
        }
        r = r + 1;
    }
    return s;
}

// --- Merge two sorted arrays ---
void merge_sorted(int *a, int na, int *b, int nb, int *out) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < na && j < nb) {
        if (a[i] <= b[j]) {
            out[k] = a[i];
            i = i + 1;
        } else {
            out[k] = b[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < na) {
        out[k] = a[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < nb) {
        out[k] = b[j];
        j = j + 1;
        k = k + 1;
    }
}

// --- Remove duplicates from sorted array, return new length ---
int remove_duplicates(int *arr, int n) {
    if (n == 0) return 0;
    int write = 1;
    int i = 1;
    while (i < n) {
        if (arr[i] != arr[write - 1]) {
            arr[write] = arr[i];
            write = write + 1;
        }
        i = i + 1;
    }
    return write;
}

// --- Intersection of two sorted arrays ---
int array_intersection(int *a, int na, int *b, int nb, int *out) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < na && j < nb) {
        if (a[i] == b[j]) {
            out[k] = a[i];
            k = k + 1;
            i = i + 1;
            j = j + 1;
        } else if (a[i] < b[j]) {
            i = i + 1;
        } else {
            j = j + 1;
        }
    }
    return k;
}

// --- Second largest element ---
int second_largest(int *arr, int n) {
    int first = arr[0];
    int i = 1;
    while (i < n) {
        if (arr[i] > first) {
            first = arr[i];
        }
        i = i + 1;
    }
    int second = arr[0];
    if (second == first) {
        i = 1;
        while (i < n) {
            if (arr[i] != first) {
                second = arr[i];
                break;
            }
            i = i + 1;
        }
    }
    i = 0;
    while (i < n) {
        if (arr[i] > second && arr[i] < first) {
            second = arr[i];
        }
        i = i + 1;
    }
    return second;
}

// --- Check if array is a palindrome ---
int is_palindrome(int *arr, int n) {
    int i = 0;
    int j = n - 1;
    while (i < j) {
        if (arr[i] != arr[j]) {
            return 0;
        }
        i = i + 1;
        j = j - 1;
    }
    return 1;
}

// --- Kadane's algorithm: maximum subarray sum ---
int kadane_max_subarray(int *arr, int n) {
    int max_ending = arr[0];
    int max_so_far = arr[0];
    int i = 1;
    while (i < n) {
        if (max_ending + arr[i] > arr[i]) {
            max_ending = max_ending + arr[i];
        } else {
            max_ending = arr[i];
        }
        if (max_ending > max_so_far) {
            max_so_far = max_ending;
        }
        i = i + 1;
    }
    return max_so_far;
}

// --- Count zeros ---
int count_zeros(int *arr, int n) {
    int c = 0;
    int i = 0;
    while (i < n) {
        if (arr[i] == 0) {
            c = c + 1;
        }
        i = i + 1;
    }
    return c;
}

// --- Count even and odd ---
void count_even_odd(int *arr, int n, int *even_count, int *odd_count) {
    *even_count = 0;
    *odd_count = 0;
    int i = 0;
    while (i < n) {
        if (arr[i] % 2 == 0) {
            *even_count = *even_count + 1;
        } else {
            *odd_count = *odd_count + 1;
        }
        i = i + 1;
    }
}

// --- Product of non-zero elements ---
int product_nonzero(int *arr, int n) {
    int p = 1;
    int i = 0;
    while (i < n) {
        if (arr[i] != 0) {
            p = p * arr[i];
        }
        i = i + 1;
    }
    return p;
}

// --- Copy array ---
void array_copy(int *dst, int *src, int n) {
    int i = 0;
    while (i < n) {
        dst[i] = src[i];
        i = i + 1;
    }
}

// --- Fill array with value ---
void array_fill(int *arr, int n, int val) {
    int i = 0;
    while (i < n) {
        arr[i] = val;
        i = i + 1;
    }
}

// --- Compute running average (integer division) ---
void running_average(int *arr, int n, int *out) {
    int s = 0;
    int i = 0;
    while (i < n) {
        s = s + arr[i];
        out[i] = s / (i + 1);
        i = i + 1;
    }
}

// --- Shift array right by 1 ---
void shift_right_one(int *arr, int n) {
    int last = arr[n - 1];
    int i = n - 1;
    while (i > 0) {
        arr[i] = arr[i - 1];
        i = i - 1;
    }
    arr[0] = last;
}

// --- Check if sorted ascending ---
int is_sorted_asc(int *arr, int n) {
    int i = 0;
    while (i < n - 1) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

// --- Swap two elements in array ---
void array_swap(int *arr, int i, int j) {
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

int main() {
    // --- Initialize array 1..10 ---
    int arr[10];
    int i = 0;
    while (i < 10) {
        arr[i] = i + 1;
        i = i + 1;
    }
    printf("init: %d %d %d %d %d %d %d %d %d %d\n",
        arr[0], arr[1], arr[2], arr[3], arr[4],
        arr[5], arr[6], arr[7], arr[8], arr[9]);

    // sum=55
    printf("sum=%d\n", array_sum(arr, 10));
    printf("max=%d\n", array_max(arr, 10));
    printf("min=%d\n", array_min(arr, 10));

    // linear search: 7 is at index 6
    printf("linear_find_7=%d\n", linear_search(arr, 10, 7));
    printf("linear_find_99=%d\n", linear_search(arr, 10, 99));

    // binary search on sorted {1..10}: 5 at idx 4, 1 at idx 0, 10 at idx 9
    printf("binary_find_5=%d\n", binary_search(arr, 10, 5));
    printf("binary_find_1=%d\n", binary_search(arr, 10, 1));
    printf("binary_find_10=%d\n", binary_search(arr, 10, 10));
    printf("binary_find_11=%d\n", binary_search(arr, 10, 11));

    // reverse: {1..10} => {10..1}
    array_reverse(arr, 10);
    printf("reversed: %d %d %d %d %d %d %d %d %d %d\n",
        arr[0], arr[1], arr[2], arr[3], arr[4],
        arr[5], arr[6], arr[7], arr[8], arr[9]);

    // re-init {1..10}, rotate left 3 => {4,5,6,7,8,9,10,1,2,3}
    i = 0;
    while (i < 10) {
        arr[i] = i + 1;
        i = i + 1;
    }
    rotate_left(arr, 10, 3);
    printf("rotate_left_3: %d %d %d %d %d %d %d %d %d %d\n",
        arr[0], arr[1], arr[2], arr[3], arr[4],
        arr[5], arr[6], arr[7], arr[8], arr[9]);

    // prefix sum of {1..10}
    int orig[10];
    i = 0;
    while (i < 10) {
        orig[i] = i + 1;
        i = i + 1;
    }
    int ps[10];
    prefix_sum(orig, ps, 10);
    printf("prefix_sum: %d %d %d %d %d %d %d %d %d %d\n",
        ps[0], ps[1], ps[2], ps[3], ps[4],
        ps[5], ps[6], ps[7], ps[8], ps[9]);

    // frequency count: {1,2,3,1,3,2,3,1,4,5}
    int arr2[10];
    arr2[0] = 1; arr2[1] = 2; arr2[2] = 3; arr2[3] = 1; arr2[4] = 3;
    arr2[5] = 2; arr2[6] = 3; arr2[7] = 1; arr2[8] = 4; arr2[9] = 5;
    int freq[6];
    freq_count(arr2, 10, freq, 5);
    printf("freq_0=%d\n", freq[0]);
    printf("freq_1=%d\n", freq[1]);
    printf("freq_2=%d\n", freq[2]);
    printf("freq_3=%d\n", freq[3]);
    printf("freq_4=%d\n", freq[4]);
    printf("freq_5=%d\n", freq[5]);

    // 2D array: 4x5
    // row0: 1..5 sum=15, row1: 6..10 sum=40, row2: 11..15 sum=65
    // row3: 0,0,0,0,30 sum=30. total=150
    int mat[4][5];
    int r = 0;
    while (r < 3) {
        int c = 0;
        while (c < 5) {
            mat[r][c] = r * 5 + c + 1;
            c = c + 1;
        }
        r = r + 1;
    }
    mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 0; mat[3][4] = 30;

    int *mat_ptr = &mat[0][0];
    printf("2d_sum=%d\n", total_sum_2d(mat_ptr, 4, 5));
    printf("2d_row0=%d\n", row_sum_2d(mat_ptr, 5, 0));
    printf("2d_row1=%d\n", row_sum_2d(mat_ptr, 5, 1));
    printf("2d_row2=%d\n", row_sum_2d(mat_ptr, 5, 2));
    printf("2d_row3=%d\n", row_sum_2d(mat_ptr, 5, 3));

    // col sums: col0=1+6+11+0=18, col1=2+7+12+0=21,
    // col2=3+8+13+0=24, col3=4+9+14+0=27, col4=5+10+15+30=60
    printf("col_sum_0=%d\n", col_sum_2d(mat_ptr, 4, 5, 0));
    printf("col_sum_1=%d\n", col_sum_2d(mat_ptr, 4, 5, 1));
    printf("col_sum_2=%d\n", col_sum_2d(mat_ptr, 4, 5, 2));
    printf("col_sum_3=%d\n", col_sum_2d(mat_ptr, 4, 5, 3));
    printf("col_sum_4=%d\n", col_sum_2d(mat_ptr, 4, 5, 4));

    // merge sorted: {1,3,5,7} + {2,4,6,8} => {1,2,3,4,5,6,7,8}
    int ma[4]; ma[0] = 1; ma[1] = 3; ma[2] = 5; ma[3] = 7;
    int mb[4]; mb[0] = 2; mb[1] = 4; mb[2] = 6; mb[3] = 8;
    int merged[8];
    merge_sorted(ma, 4, mb, 4, merged);
    printf("merge: %d %d %d %d %d %d %d %d\n",
        merged[0], merged[1], merged[2], merged[3],
        merged[4], merged[5], merged[6], merged[7]);

    // remove duplicates: {1,1,2,3,3,3,4,5} => {1,2,3,4,5} len=5
    int dup_arr[8];
    dup_arr[0] = 1; dup_arr[1] = 1; dup_arr[2] = 2; dup_arr[3] = 3;
    dup_arr[4] = 3; dup_arr[5] = 3; dup_arr[6] = 4; dup_arr[7] = 5;
    int new_len = remove_duplicates(dup_arr, 8);
    printf("remove_dups: %d %d %d %d %d count=%d\n",
        dup_arr[0], dup_arr[1], dup_arr[2], dup_arr[3], dup_arr[4], new_len);

    // intersection: {1,3,4,5,7} and {2,3,4,5} => {3,4,5}
    int ia[5]; ia[0] = 1; ia[1] = 3; ia[2] = 4; ia[3] = 5; ia[4] = 7;
    int ib[4]; ib[0] = 2; ib[1] = 3; ib[2] = 4; ib[3] = 5;
    int inter[5];
    int inter_count = array_intersection(ia, 5, ib, 4, inter);
    printf("intersection: %d %d %d count=%d\n", inter[0], inter[1], inter[2], inter_count);

    // second largest of {1..10} is 9
    i = 0;
    while (i < 10) {
        arr[i] = i + 1;
        i = i + 1;
    }
    printf("second_largest=%d\n", second_largest(arr, 10));

    // palindrome checks
    int pal_yes[5]; pal_yes[0] = 1; pal_yes[1] = 2; pal_yes[2] = 3; pal_yes[3] = 2; pal_yes[4] = 1;
    int pal_no[5]; pal_no[0] = 1; pal_no[1] = 2; pal_no[2] = 3; pal_no[3] = 4; pal_no[4] = 5;
    printf("is_palindrome_yes=%d\n", is_palindrome(pal_yes, 5));
    printf("is_palindrome_no=%d\n", is_palindrome(pal_no, 5));

    // kadane: {-1,3,-2,5,-1,6,-3,2} => max subarray sum = 11
    int kad[8];
    kad[0] = -1; kad[1] = 3; kad[2] = -2; kad[3] = 5; kad[4] = -1;
    kad[5] = 6; kad[6] = -3; kad[7] = 2;
    printf("kadane_max=%d\n", kadane_max_subarray(kad, 8));

    // count zeros: {0,1,0,2,3,0,4,5,6,7} => 3
    int zc[10];
    zc[0] = 0; zc[1] = 1; zc[2] = 0; zc[3] = 2; zc[4] = 3;
    zc[5] = 0; zc[6] = 4; zc[7] = 5; zc[8] = 6; zc[9] = 7;
    printf("zero_count=%d\n", count_zeros(zc, 10));

    // even/odd count of {1..10}: 5 each
    int ec; int oc;
    i = 0;
    while (i < 10) {
        arr[i] = i + 1;
        i = i + 1;
    }
    count_even_odd(arr, 10, &ec, &oc);
    printf("even_count=%d\n", ec);
    printf("odd_count=%d\n", oc);

    // product of {1..10} = 3628800
    printf("product_no_zero=%d\n", product_nonzero(arr, 10));

    // running average of {1..10}: last = 55/10 = 5
    int ravg[10];
    running_average(arr, 10, ravg);
    printf("running_avg_last=%d\n", ravg[9]);

    printf("done\n");
    return 0;
}
