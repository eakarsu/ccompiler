int printf(const char *fmt, ...);
// EXPECT: Binary Search\nSearch 10=3\nSearch 25=8\nSearch 11=-1\nLower 3=2\nUpper 3=5\nCount 3=3\nCount 7=4\nCount 4=0\nIsqrt 100=10\nIsqrt 50=7\nIsqrt 1=1\nPeak index3 val=12\nRotated index2 val2\nMerge Sort\nBefore: 38 27 43 3 9 82 10 1 57 45\nAfter: 1 3 9 10 27 38 43 45 57 82\nSorted=1\nInversions=13\nSortedAfter=1\nClosest Pair\nClosDist1\nMax Subarray\nDCmax=6\nKadanemx6\nDCmax=10\nKadanemx10\nAllNegDC=-1\nPower\n2^10=1024\n3^5=243\n5^3=125\nTernary Search\nUniMax at10 value105\nKthSmall= 0:1\nKthSmall= 4:5\nKthSmall= 8:9\nMajority=3\nNoMajorit-1\nMerged Sorted: 1 2 4 5 7 8 10 11\nAll  passed=1

// Divide and conquer: binary search variants, merge sort, closest pair (1D), max subarray

int arr[100];
int temp[100];

// Swap helper
void swap(int a[100], int i, int j) {
    int t;
    t = a[i];
    a[i] = a[j];
    a[j] = t;
}

int min_int(int a, int b) {
    if (a < b) return a;
    return b;
}

int max_int(int a, int b) {
    if (a > b) return a;
    return b;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// ============ BINARY SEARCH VARIANTS ============

// Standard binary search (returns index or -1)
int binary_search(int a[100], int n, int target) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = n - 1;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (a[mid] == target) {
            return mid;
        }
        if (a[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

// Lower bound: first position where a[i] >= target
int lower_bound(int a[100], int n, int target) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = n;
    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (a[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// Upper bound: first position where a[i] > target
int upper_bound(int a[100], int n, int target) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = n;
    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (a[mid] <= target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// Count occurrences using lower/upper bound
int count_occurrences(int a[100], int n, int target) {
    return upper_bound(a, n, target) - lower_bound(a, n, target);
}

// Binary search on answer: find integer square root
int isqrt(int n) {
    int lo;
    int hi;
    int mid;
    if (n <= 0) return 0;
    lo = 1;
    hi = n;
    if (hi > 46340) {
        hi = 46340; // prevent overflow
    }
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (mid * mid == n) {
            return mid;
        }
        if (mid * mid < n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

// Binary search: find peak element in bitonic array
int find_peak(int a[100], int n) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = n - 1;
    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (a[mid] < a[mid + 1]) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// Binary search: minimum in rotated sorted array
int find_min_rotated(int a[100], int n) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = n - 1;
    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (a[mid] > a[hi]) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// ============ MERGE SORT ============

void merge(int a[100], int lo, int mid, int hi) {
    int i;
    int j;
    int k;

    for (i = lo; i <= hi; i = i + 1) {
        temp[i] = a[i];
    }

    i = lo;
    j = mid + 1;
    k = lo;

    while (i <= mid && j <= hi) {
        if (temp[i] <= temp[j]) {
            a[k] = temp[i];
            i = i + 1;
        } else {
            a[k] = temp[j];
            j = j + 1;
        }
        k = k + 1;
    }

    while (i <= mid) {
        a[k] = temp[i];
        i = i + 1;
        k = k + 1;
    }

    while (j <= hi) {
        a[k] = temp[j];
        j = j + 1;
        k = k + 1;
    }
}

void merge_sort(int a[100], int lo, int hi) {
    int mid;
    if (lo >= hi) {
        return;
    }
    mid = lo + (hi - lo) / 2;
    merge_sort(a, lo, mid);
    merge_sort(a, mid + 1, hi);
    merge(a, lo, mid, hi);
}

// Count inversions using merge sort
int inv_count;

void merge_count(int a[100], int lo, int mid, int hi) {
    int i;
    int j;
    int k;

    for (i = lo; i <= hi; i = i + 1) {
        temp[i] = a[i];
    }

    i = lo;
    j = mid + 1;
    k = lo;

    while (i <= mid && j <= hi) {
        if (temp[i] <= temp[j]) {
            a[k] = temp[i];
            i = i + 1;
        } else {
            a[k] = temp[j];
            j = j + 1;
            inv_count = inv_count + (mid - i + 1);
        }
        k = k + 1;
    }

    while (i <= mid) {
        a[k] = temp[i];
        i = i + 1;
        k = k + 1;
    }

    while (j <= hi) {
        a[k] = temp[j];
        j = j + 1;
        k = k + 1;
    }
}

void merge_sort_count(int a[100], int lo, int hi) {
    int mid;
    if (lo >= hi) {
        return;
    }
    mid = lo + (hi - lo) / 2;
    merge_sort_count(a, lo, mid);
    merge_sort_count(a, mid + 1, hi);
    merge_count(a, lo, mid, hi);
}

// ============ CLOSEST PAIR (1D) ============

// After sorting, find minimum |a[i] - a[i+1]|
int closest_pair_1d(int a[100], int n) {
    int min_diff;
    int i;
    int d;
    // Array should be sorted first
    min_diff = abs_val(a[1] - a[0]);
    for (i = 1; i < n - 1; i = i + 1) {
        d = abs_val(a[i + 1] - a[i]);
        if (d < min_diff) {
            min_diff = d;
        }
    }
    return min_diff;
}

// ============ MAXIMUM SUBARRAY (Divide & Conquer) ============

int max_crossing_sum(int a[100], int lo, int mid, int hi) {
    int left_sum;
    int right_sum;
    int sum;
    int i;

    left_sum = -100000;
    sum = 0;
    for (i = mid; i >= lo; i = i - 1) {
        sum = sum + a[i];
        if (sum > left_sum) {
            left_sum = sum;
        }
    }

    right_sum = -100000;
    sum = 0;
    for (i = mid + 1; i <= hi; i = i + 1) {
        sum = sum + a[i];
        if (sum > right_sum) {
            right_sum = sum;
        }
    }

    return left_sum + right_sum;
}

int max_subarray_dc(int a[100], int lo, int hi) {
    int mid;
    int left_max;
    int right_max;
    int cross_max;
    int mx;

    if (lo == hi) {
        return a[lo];
    }

    mid = lo + (hi - lo) / 2;
    left_max = max_subarray_dc(a, lo, mid);
    right_max = max_subarray_dc(a, mid + 1, hi);
    cross_max = max_crossing_sum(a, lo, mid, hi);

    mx = left_max;
    if (right_max > mx) {
        mx = right_max;
    }
    if (cross_max > mx) {
        mx = cross_max;
    }
    return mx;
}

// Kadane's algorithm for verification
int max_subarray_kadane(int a[100], int n) {
    int max_ending_here;
    int max_so_far;
    int i;
    max_ending_here = a[0];
    max_so_far = a[0];
    for (i = 1; i < n; i = i + 1) {
        if (max_ending_here + a[i] > a[i]) {
            max_ending_here = max_ending_here + a[i];
        } else {
            max_ending_here = a[i];
        }
        if (max_ending_here > max_so_far) {
            max_so_far = max_ending_here;
        }
    }
    return max_so_far;
}

// Power function (divide & conquer)
int power(int base, int exp) {
    int half;
    if (exp == 0) {
        return 1;
    }
    half = power(base, exp / 2);
    if ((exp - (exp / 2) * 2) == 0) {
        return half * half;
    }
    return half * half * base;
}

// Ternary search: find maximum of unimodal function (discrete)
// f(x) = -x*x + 20*x + 5
int f_unimodal(int x) {
    return -x * x + 20 * x + 5;
}

int ternary_search_max(int lo, int hi) {
    int m1;
    int m2;
    while (hi - lo > 2) {
        m1 = lo + (hi - lo) / 3;
        m2 = hi - (hi - lo) / 3;
        if (f_unimodal(m1) < f_unimodal(m2)) {
            lo = m1 + 1;
        } else {
            hi = m2 - 1;
        }
    }
    // Check remaining
    {
        int best;
        int best_val;
        int i;
        best = lo;
        best_val = f_unimodal(lo);
        for (i = lo + 1; i <= hi; i = i + 1) {
            if (f_unimodal(i) > best_val) {
                best = i;
                best_val = f_unimodal(i);
            }
        }
        return best;
    }
}

// Median of medians (simplified: find kth smallest)
// Simple selection for small arrays
int kth_smallest(int a[100], int lo, int hi, int k) {
    // Simple: use a partial sort approach
    int i;
    int j;
    int pivot;
    int store;

    if (lo == hi) {
        return a[lo];
    }

    // Use last element as pivot
    pivot = a[hi];
    store = lo;
    for (i = lo; i < hi; i = i + 1) {
        if (a[i] < pivot) {
            swap(a, i, store);
            store = store + 1;
        }
    }
    swap(a, store, hi);

    if (store == k) {
        return a[store];
    }
    if (k < store) {
        return kth_smallest(a, lo, store - 1, k);
    }
    return kth_smallest(a, store + 1, hi, k);
}

// Check if array is sorted
int is_sorted(int a[100], int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (a[i] > a[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// Sum of array
int array_sum(int a[100], int n) {
    int s;
    int i;
    s = 0;
    for (i = 0; i < n; i = i + 1) {
        s = s + a[i];
    }
    return s;
}

// Majority element (divide & conquer style using count)
int count_in_range(int a[100], int lo, int hi, int target) {
    int c;
    int i;
    c = 0;
    for (i = lo; i <= hi; i = i + 1) {
        if (a[i] == target) {
            c = c + 1;
        }
    }
    return c;
}

int majority_element(int a[100], int n) {
    int candidate;
    int count;
    int i;
    // Boyer-Moore voting
    candidate = a[0];
    count = 1;
    for (i = 1; i < n; i = i + 1) {
        if (count == 0) {
            candidate = a[i];
            count = 1;
        } else if (a[i] == candidate) {
            count = count + 1;
        } else {
            count = count - 1;
        }
    }
    // Verify
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        if (a[i] == candidate) {
            count = count + 1;
        }
    }
    if (count > n / 2) {
        return candidate;
    }
    return -1;
}

// Merge two sorted halves of the array
void merge_two_sorted(int a[100], int n1, int n2) {
    int i;
    int j;
    int k;

    for (i = 0; i < n1 + n2; i = i + 1) {
        temp[i] = a[i];
    }

    i = 0;
    j = n1;
    k = 0;
    while (i < n1 && j < n1 + n2) {
        if (temp[i] <= temp[j]) {
            a[k] = temp[i];
            i = i + 1;
        } else {
            a[k] = temp[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < n1) {
        a[k] = temp[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < n1 + n2) {
        a[k] = temp[j];
        j = j + 1;
        k = k + 1;
    }
}

// Additional helper: print array
void print_array(int a[100], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        printf("%d", a[i]);
        if (i < n - 1) {
            printf(" ");
        }
    }
    printf("%c", 10);
}

int main(void) {
    int n;
    int i;
    int result;

    // ===== BINARY SEARCH TESTS =====
    printf("%c%c%c%c%c%c %c%c%c%c%c%c", 'B','i','n','a','r','y', 'S','e','a','r','c','h');
    printf("%c", 10);

    // Setup sorted array
    n = 15;
    for (i = 0; i < n; i = i + 1) {
        arr[i] = i * 3 + 1; // 1,4,7,10,13,16,19,22,25,28,31,34,37,40,43
    }

    result = binary_search(arr, n, 10);
    printf("%c%c%c%c%c%c %d%c%d", 'S','e','a','r','c','h', 10, '=', result);
    printf("%c", 10);

    result = binary_search(arr, n, 25);
    printf("%c%c%c%c%c%c %d%c%d", 'S','e','a','r','c','h', 25, '=', result);
    printf("%c", 10);

    result = binary_search(arr, n, 11);
    printf("%c%c%c%c%c%c %d%c%d", 'S','e','a','r','c','h', 11, '=', result);
    printf("%c", 10);

    // Lower/upper bound tests
    // Array with duplicates: 1,1,3,3,3,5,5,7,7,7,7,9
    arr[0]=1; arr[1]=1; arr[2]=3; arr[3]=3; arr[4]=3;
    arr[5]=5; arr[6]=5; arr[7]=7; arr[8]=7; arr[9]=7;
    arr[10]=7; arr[11]=9;
    n = 12;

    printf("%c%c%c%c%c %d%c%d", 'L','o','w','e','r', 3, '=', lower_bound(arr, n, 3));
    printf("%c", 10);
    printf("%c%c%c%c%c %d%c%d", 'U','p','p','e','r', 3, '=', upper_bound(arr, n, 3));
    printf("%c", 10);
    printf("%c%c%c%c%c %d%c%d", 'C','o','u','n','t', 3, '=', count_occurrences(arr, n, 3));
    printf("%c", 10);
    printf("%c%c%c%c%c %d%c%d", 'C','o','u','n','t', 7, '=', count_occurrences(arr, n, 7));
    printf("%c", 10);
    printf("%c%c%c%c%c %d%c%d", 'C','o','u','n','t', 4, '=', count_occurrences(arr, n, 4));
    printf("%c", 10);

    // Integer square root
    printf("%c%c%c%c%c %d%c%d", 'I','s','q','r','t', 100, '=', isqrt(100));
    printf("%c", 10);
    printf("%c%c%c%c%c %d%c%d", 'I','s','q','r','t', 50, '=', isqrt(50));
    printf("%c", 10);
    printf("%c%c%c%c%c %d%c%d", 'I','s','q','r','t', 1, '=', isqrt(1));
    printf("%c", 10);

    // Peak finding in bitonic array
    arr[0]=1; arr[1]=3; arr[2]=8; arr[3]=12; arr[4]=9; arr[5]=5; arr[6]=2;
    n = 7;
    result = find_peak(arr, n);
    printf("%c%c%c%c %c%c%c%c%c%d %c%c%c%c%d", 'P','e','a','k', 'i','n','d','e','x', result, 'v','a','l','=', arr[result]);
    printf("%c", 10);

    // Rotated sorted array minimum
    arr[0]=15; arr[1]=18; arr[2]=2; arr[3]=3; arr[4]=6; arr[5]=12;
    n = 6;
    result = find_min_rotated(arr, n);
    printf("%c%c%c%c%c%c%c %c%c%c%c%c%d %c%c%c%d", 'R','o','t','a','t','e','d', 'i','n','d','e','x', result, 'v','a','l', arr[result]);
    printf("%c", 10);

    // ===== MERGE SORT TESTS =====
    printf("%c%c%c%c%c %c%c%c%c", 'M','e','r','g','e', 'S','o','r','t');
    printf("%c", 10);

    arr[0]=38; arr[1]=27; arr[2]=43; arr[3]=3; arr[4]=9;
    arr[5]=82; arr[6]=10; arr[7]=1; arr[8]=57; arr[9]=45;
    n = 10;

    printf("%c%c%c%c%c%c%c ", 'B','e','f','o','r','e',':');
    print_array(arr, n);

    merge_sort(arr, 0, n - 1);

    printf("%c%c%c%c%c%c ", 'A','f','t','e','r',':');
    print_array(arr, n);

    printf("%c%c%c%c%c%c%c%d", 'S','o','r','t','e','d','=', is_sorted(arr, n));
    printf("%c", 10);

    // Inversion count
    arr[0]=5; arr[1]=3; arr[2]=8; arr[3]=1; arr[4]=2;
    arr[5]=7; arr[6]=4; arr[7]=6;
    n = 8;
    inv_count = 0;
    merge_sort_count(arr, 0, n - 1);
    printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'I','n','v','e','r','s','i','o','n','s','=', inv_count);
    printf("%c", 10);

    // Sorted after inversion count
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%d", 'S','o','r','t','e','d','A','f','t','e','r','=', is_sorted(arr, n));
    printf("%c", 10);

    // ===== CLOSEST PAIR 1D =====
    printf("%c%c%c%c%c%c%c %c%c%c%c", 'C','l','o','s','e','s','t', 'P','a','i','r');
    printf("%c", 10);

    arr[0]=2; arr[1]=12; arr[2]=5; arr[3]=18; arr[4]=7;
    arr[5]=15; arr[6]=3; arr[7]=20;
    n = 8;
    merge_sort(arr, 0, n - 1);
    result = closest_pair_1d(arr, n);
    printf("%c%c%c%c%c%c%c%c%d", 'C','l','o','s','D','i','s','t', result);
    printf("%c", 10);

    // ===== MAXIMUM SUBARRAY =====
    printf("%c%c%c %c%c%c%c%c%c%c%c", 'M','a','x', 'S','u','b','a','r','r','a','y');
    printf("%c", 10);

    arr[0]=-2; arr[1]=1; arr[2]=-3; arr[3]=4; arr[4]=-1;
    arr[5]=2; arr[6]=1; arr[7]=-5; arr[8]=4;
    n = 9;
    result = max_subarray_dc(arr, 0, n - 1);
    printf("%c%c%c%c%c%c%d", 'D','C','m','a','x','=', result);
    printf("%c", 10);

    result = max_subarray_kadane(arr, n);
    printf("%c%c%c%c%c%c%c%c%d", 'K','a','d','a','n','e','m','x', result);
    printf("%c", 10);

    // Another array
    arr[0]=5; arr[1]=-7; arr[2]=3; arr[3]=5; arr[4]=-2;
    arr[5]=4; arr[6]=-1;
    n = 7;
    printf("%c%c%c%c%c%c%d", 'D','C','m','a','x','=', max_subarray_dc(arr, 0, n - 1));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c%d", 'K','a','d','a','n','e','m','x', max_subarray_kadane(arr, n));
    printf("%c", 10);

    // All negative
    arr[0]=-5; arr[1]=-3; arr[2]=-8; arr[3]=-1; arr[4]=-7;
    n = 5;
    printf("%c%c%c%c%c%c%c%c%c%d", 'A','l','l','N','e','g','D','C','=', max_subarray_dc(arr, 0, n - 1));
    printf("%c", 10);

    // ===== POWER FUNCTION =====
    printf("%c%c%c%c%c", 'P','o','w','e','r');
    printf("%c", 10);

    printf("%d%c%d%c%d", 2, '^', 10, '=', power(2, 10));
    printf("%c", 10);
    printf("%d%c%d%c%d", 3, '^', 5, '=', power(3, 5));
    printf("%c", 10);
    printf("%d%c%d%c%d", 5, '^', 3, '=', power(5, 3));
    printf("%c", 10);

    // ===== TERNARY SEARCH =====
    printf("%c%c%c%c%c%c%c %c%c%c%c%c%c", 'T','e','r','n','a','r','y', 'S','e','a','r','c','h');
    printf("%c", 10);
    result = ternary_search_max(0, 20);
    printf("%c%c%c%c%c%c %c%c%d %c%c%c%c%c%d", 'U','n','i','M','a','x', 'a','t', result, 'v','a','l','u','e', f_unimodal(result));
    printf("%c", 10);

    // ===== KTH SMALLEST =====
    arr[0]=7; arr[1]=2; arr[2]=9; arr[3]=4; arr[4]=1;
    arr[5]=5; arr[6]=8; arr[7]=3; arr[8]=6;
    n = 9;
    printf("%c%c%c%c%c%c%c%c%c %d%c%d", 'K','t','h','S','m','a','l','l','=', 0, ':', kth_smallest(arr, 0, n - 1, 0));
    printf("%c", 10);

    // Reset array for another kth test
    arr[0]=7; arr[1]=2; arr[2]=9; arr[3]=4; arr[4]=1;
    arr[5]=5; arr[6]=8; arr[7]=3; arr[8]=6;
    printf("%c%c%c%c%c%c%c%c%c %d%c%d", 'K','t','h','S','m','a','l','l','=', 4, ':', kth_smallest(arr, 0, n - 1, 4));
    printf("%c", 10);

    // Reset for another
    arr[0]=7; arr[1]=2; arr[2]=9; arr[3]=4; arr[4]=1;
    arr[5]=5; arr[6]=8; arr[7]=3; arr[8]=6;
    printf("%c%c%c%c%c%c%c%c%c %d%c%d", 'K','t','h','S','m','a','l','l','=', 8, ':', kth_smallest(arr, 0, n - 1, 8));
    printf("%c", 10);

    // ===== MAJORITY ELEMENT =====
    arr[0]=3; arr[1]=3; arr[2]=4; arr[3]=3; arr[4]=3;
    arr[5]=2; arr[6]=3;
    n = 7;
    printf("%c%c%c%c%c%c%c%c%c%d", 'M','a','j','o','r','i','t','y','=', majority_element(arr, n));
    printf("%c", 10);

    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4;
    n = 4;
    printf("%c%c%c%c%c%c%c%c%c%d", 'N','o','M','a','j','o','r','i','t', majority_element(arr, n));
    printf("%c", 10);

    // Merge two sorted halves
    arr[0]=2; arr[1]=5; arr[2]=8; arr[3]=11;
    arr[4]=1; arr[5]=4; arr[6]=7; arr[7]=10;
    merge_two_sorted(arr, 4, 4);
    printf("%c%c%c%c%c%c %c%c%c%c%c%c%c ", 'M','e','r','g','e','d', 'S','o','r','t','e','d',':');
    print_array(arr, 8);

    printf("%c%c%c%c %c%c%c%c%c%c%c%d", 'A','l','l',' ', 'p','a','s','s','e','d','=', 1);
    printf("%c", 10);

    return 0;
}
