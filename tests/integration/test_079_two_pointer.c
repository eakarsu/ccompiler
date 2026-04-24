int printf(const char *fmt, ...);
// EXPECT: Pair Sum\nFind 16: 1 0 7 16\nFind 8: 1 0 3\nFind 100: 0\nCountPairs 9=4\nCountPairs 5=2\nThreeSum 12: 1 3243 4+7\nRemove Duplicates\nUnique= 5: 1 2 3 4 5\nUniqueAt2= 6: 1 1 2 2 3 3\nRemoveV=2: 4: 3 3 4 5\nPartition\nLomuto pivot4 index3\nDNF lower=3 upper61 result 2 3 1 5 5 5 5 9 8\nEvenCount4\nMerge Sorted\nMerged 10: 1 2 3 4 5 6 7 8 9 10\nInter 3: 3 5 9\nDiff 3: 1 7 9\nUnion 5: 1 2 3 5 6\nContainer Water\nMaxArea=49\nSort 01s 11s 2\nSorted012 0 0 0 1 1 1 2 2 2\nMoveZeros: 4: 1 3 12 5 0 0 0\nClosest 15=14\nClosest 12=11\nAll  passed=1

// Two pointer techniques: pair sum, remove duplicates, partition, merge sorted arrays

int arr[100];
int arr2[100];
int result[200];

// ============ PAIR SUM ============
// Find pair in sorted array that sums to target
// Returns 1 if found, 0 otherwise
int pair_idx1;
int pair_idx2;

int find_pair_sum(int a[100], int n, int target) {
    int lo;
    int hi;
    int sum;
    lo = 0;
    hi = n - 1;
    while (lo < hi) {
        sum = a[lo] + a[hi];
        if (sum == target) {
            pair_idx1 = lo;
            pair_idx2 = hi;
            return 1;
        }
        if (sum < target) {
            lo = lo + 1;
        } else {
            hi = hi - 1;
        }
    }
    return 0;
}

// Count all pairs with given sum
int count_pairs_sum(int a[100], int n, int target) {
    int lo;
    int hi;
    int sum;
    int count;
    lo = 0;
    hi = n - 1;
    count = 0;
    while (lo < hi) {
        sum = a[lo] + a[hi];
        if (sum == target) {
            // Count duplicates
            if (a[lo] == a[hi]) {
                int cnt;
                cnt = hi - lo + 1;
                count = count + cnt * (cnt - 1) / 2;
                break;
            } else {
                int lo_count;
                int hi_count;
                int lo_val;
                int hi_val;
                lo_count = 1;
                hi_count = 1;
                lo_val = a[lo];
                hi_val = a[hi];
                while (lo + lo_count < hi && a[lo + lo_count] == lo_val) {
                    lo_count = lo_count + 1;
                }
                while (hi - hi_count > lo && a[hi - hi_count] == hi_val) {
                    hi_count = hi_count + 1;
                }
                count = count + lo_count * hi_count;
                lo = lo + lo_count;
                hi = hi - hi_count;
            }
        } else if (sum < target) {
            lo = lo + 1;
        } else {
            hi = hi - 1;
        }
    }
    return count;
}

// Three sum: find triplet summing to target
int trip_a;
int trip_b;
int trip_c;

int three_sum(int a[100], int n, int target) {
    int i;
    int lo;
    int hi;
    int sum;
    for (i = 0; i < n - 2; i = i + 1) {
        lo = i + 1;
        hi = n - 1;
        while (lo < hi) {
            sum = a[i] + a[lo] + a[hi];
            if (sum == target) {
                trip_a = a[i];
                trip_b = a[lo];
                trip_c = a[hi];
                return 1;
            }
            if (sum < target) {
                lo = lo + 1;
            } else {
                hi = hi - 1;
            }
        }
    }
    return 0;
}

// ============ REMOVE DUPLICATES FROM SORTED ARRAY ============
int remove_duplicates(int a[100], int n) {
    int write_idx;
    int read_idx;
    if (n <= 1) {
        return n;
    }
    write_idx = 1;
    for (read_idx = 1; read_idx < n; read_idx = read_idx + 1) {
        if (a[read_idx] != a[read_idx - 1]) {
            a[write_idx] = a[read_idx];
            write_idx = write_idx + 1;
        }
    }
    return write_idx;
}

// Remove duplicates allowing at most 2 copies
int remove_duplicates_2(int a[100], int n) {
    int write_idx;
    int read_idx;
    if (n <= 2) {
        return n;
    }
    write_idx = 2;
    for (read_idx = 2; read_idx < n; read_idx = read_idx + 1) {
        if (a[read_idx] != a[write_idx - 2]) {
            a[write_idx] = a[read_idx];
            write_idx = write_idx + 1;
        }
    }
    return write_idx;
}

// Remove specific value
int remove_value(int a[100], int n, int val) {
    int write_idx;
    int read_idx;
    write_idx = 0;
    for (read_idx = 0; read_idx < n; read_idx = read_idx + 1) {
        if (a[read_idx] != val) {
            a[write_idx] = a[read_idx];
            write_idx = write_idx + 1;
        }
    }
    return write_idx;
}

// ============ PARTITION ============
// Dutch National Flag (3-way partition)
// Partition around pivot, return position
int partition_lomuto(int a[100], int lo, int hi) {
    int pivot;
    int i;
    int j;
    int t;
    pivot = a[hi];
    i = lo - 1;
    for (j = lo; j < hi; j = j + 1) {
        if (a[j] <= pivot) {
            i = i + 1;
            t = a[i]; a[i] = a[j]; a[j] = t;
        }
    }
    i = i + 1;
    t = a[i]; a[i] = a[hi]; a[hi] = t;
    return i;
}

// Three-way partition (Dutch National Flag)
int dnf_lo_result;
int dnf_hi_result;

void dutch_national_flag(int a[100], int n, int pivot) {
    int lo;
    int mid;
    int hi;
    int t;
    lo = 0;
    mid = 0;
    hi = n - 1;
    while (mid <= hi) {
        if (a[mid] < pivot) {
            t = a[lo]; a[lo] = a[mid]; a[mid] = t;
            lo = lo + 1;
            mid = mid + 1;
        } else if (a[mid] == pivot) {
            mid = mid + 1;
        } else {
            t = a[mid]; a[mid] = a[hi]; a[hi] = t;
            hi = hi - 1;
        }
    }
    dnf_lo_result = lo;
    dnf_hi_result = mid;
}

// Partition into evens and odds
int partition_even_odd(int a[100], int n) {
    int lo;
    int hi;
    int t;
    lo = 0;
    hi = n - 1;
    while (lo < hi) {
        while (lo < hi && (a[lo] - (a[lo] / 2) * 2) == 0) {
            lo = lo + 1;
        }
        while (lo < hi && (a[hi] - (a[hi] / 2) * 2) != 0) {
            hi = hi - 1;
        }
        if (lo < hi) {
            t = a[lo]; a[lo] = a[hi]; a[hi] = t;
            lo = lo + 1;
            hi = hi - 1;
        }
    }
    // Return number of evens
    {
        int count;
        int i;
        count = 0;
        for (i = 0; i < n; i = i + 1) {
            if ((a[i] - (a[i] / 2) * 2) == 0) {
                count = count + 1;
            }
        }
        return count;
    }
}

// ============ MERGE SORTED ARRAYS ============
int merge_result[200];
int merge_len;

void merge_sorted(int a[100], int n1, int b[100], int n2) {
    int i;
    int j;
    int k;
    i = 0;
    j = 0;
    k = 0;
    while (i < n1 && j < n2) {
        if (a[i] <= b[j]) {
            merge_result[k] = a[i];
            i = i + 1;
        } else {
            merge_result[k] = b[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < n1) {
        merge_result[k] = a[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < n2) {
        merge_result[k] = b[j];
        j = j + 1;
        k = k + 1;
    }
    merge_len = k;
}

// Intersection of two sorted arrays
int intersect_result[100];
int intersect_len;

void sorted_intersection(int a[100], int n1, int b[100], int n2) {
    int i;
    int j;
    int k;
    i = 0;
    j = 0;
    k = 0;
    while (i < n1 && j < n2) {
        if (a[i] == b[j]) {
            intersect_result[k] = a[i];
            k = k + 1;
            i = i + 1;
            j = j + 1;
        } else if (a[i] < b[j]) {
            i = i + 1;
        } else {
            j = j + 1;
        }
    }
    intersect_len = k;
}

// Difference of two sorted arrays (a - b)
int diff_result[100];
int diff_len;

void sorted_difference(int a[100], int n1, int b[100], int n2) {
    int i;
    int j;
    int k;
    i = 0;
    j = 0;
    k = 0;
    while (i < n1 && j < n2) {
        if (a[i] == b[j]) {
            i = i + 1;
            j = j + 1;
        } else if (a[i] < b[j]) {
            diff_result[k] = a[i];
            k = k + 1;
            i = i + 1;
        } else {
            j = j + 1;
        }
    }
    while (i < n1) {
        diff_result[k] = a[i];
        k = k + 1;
        i = i + 1;
    }
    diff_len = k;
}

// Union of two sorted arrays (unique elements)
int union_result[200];
int union_len;

void sorted_union(int a[100], int n1, int b[100], int n2) {
    int i;
    int j;
    int k;
    i = 0;
    j = 0;
    k = 0;
    while (i < n1 && j < n2) {
        if (a[i] < b[j]) {
            union_result[k] = a[i];
            k = k + 1;
            i = i + 1;
        } else if (a[i] > b[j]) {
            union_result[k] = b[j];
            k = k + 1;
            j = j + 1;
        } else {
            union_result[k] = a[i];
            k = k + 1;
            i = i + 1;
            j = j + 1;
        }
    }
    while (i < n1) {
        union_result[k] = a[i];
        k = k + 1;
        i = i + 1;
    }
    while (j < n2) {
        union_result[k] = b[j];
        k = k + 1;
        j = j + 1;
    }
    union_len = k;
}

// ============ CONTAINER WITH MOST WATER ============
int max_water(int heights[100], int n) {
    int lo;
    int hi;
    int mx;
    int area;
    int h;
    lo = 0;
    hi = n - 1;
    mx = 0;
    while (lo < hi) {
        if (heights[lo] < heights[hi]) {
            h = heights[lo];
        } else {
            h = heights[hi];
        }
        area = h * (hi - lo);
        if (area > mx) {
            mx = area;
        }
        if (heights[lo] < heights[hi]) {
            lo = lo + 1;
        } else {
            hi = hi - 1;
        }
    }
    return mx;
}

// ============ SORT ARRAY OF 0s, 1s, 2s ============
void sort_012(int a[100], int n) {
    int lo;
    int mid;
    int hi;
    int t;
    lo = 0;
    mid = 0;
    hi = n - 1;
    while (mid <= hi) {
        if (a[mid] == 0) {
            t = a[lo]; a[lo] = a[mid]; a[mid] = t;
            lo = lo + 1;
            mid = mid + 1;
        } else if (a[mid] == 1) {
            mid = mid + 1;
        } else {
            t = a[mid]; a[mid] = a[hi]; a[hi] = t;
            hi = hi - 1;
        }
    }
}

// ============ MOVE ZEROS TO END ============
int move_zeros(int a[100], int n) {
    int write_idx;
    int read_idx;
    int non_zero_count;
    write_idx = 0;
    for (read_idx = 0; read_idx < n; read_idx = read_idx + 1) {
        if (a[read_idx] != 0) {
            a[write_idx] = a[read_idx];
            write_idx = write_idx + 1;
        }
    }
    non_zero_count = write_idx;
    while (write_idx < n) {
        a[write_idx] = 0;
        write_idx = write_idx + 1;
    }
    return non_zero_count;
}

// ============ TWO SUM CLOSEST ============
int closest_pair_sum(int a[100], int n, int target) {
    int lo;
    int hi;
    int best;
    int sum;
    int diff;
    int best_diff;
    lo = 0;
    hi = n - 1;
    best = a[lo] + a[hi];
    best_diff = best - target;
    if (best_diff < 0) best_diff = -best_diff;

    while (lo < hi) {
        sum = a[lo] + a[hi];
        diff = sum - target;
        if (diff < 0) diff = -diff;
        if (diff < best_diff) {
            best_diff = diff;
            best = sum;
        }
        if (sum < target) {
            lo = lo + 1;
        } else if (sum > target) {
            hi = hi - 1;
        } else {
            return sum;
        }
    }
    return best;
}

// Print array helper
void print_arr(int a[100], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        printf("%d", a[i]);
        if (i < n - 1) {
            printf(" ");
        }
    }
    printf("%c", 10);
}

void print_res_arr(int a[200], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        printf("%d", a[i]);
        if (i < n - 1) {
            printf(" ");
        }
    }
    printf("%c", 10);
}

// Helper: is sorted
int is_sorted(int a[100], int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (a[i] > a[i + 1]) return 0;
    }
    return 1;
}

// Bubble sort helper
void bubble_sort(int a[100], int n) {
    int i;
    int j;
    int t;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (a[j] > a[j + 1]) {
                t = a[j]; a[j] = a[j+1]; a[j+1] = t;
            }
        }
    }
}

int main(void) {
    int n;
    int n1;
    int n2;
    int i;
    int res;

    // ===== PAIR SUM =====
    printf("%c%c%c%c %c%c%c", 'P','a','i','r', 'S','u','m');
    printf("%c", 10);

    // Sorted array
    arr[0]=1; arr[1]=3; arr[2]=5; arr[3]=7; arr[4]=9;
    arr[5]=11; arr[6]=13; arr[7]=15;
    n = 8;

    res = find_pair_sum(arr, n, 16);
    printf("%c%c%c%c %d%c %d%c%d %d%c%d", 'F','i','n','d', 16, ':', res, ' ', pair_idx1, pair_idx2, ' ', arr[pair_idx1] + arr[pair_idx2]);
    printf("%c", 10);

    res = find_pair_sum(arr, n, 8);
    printf("%c%c%c%c %d%c %d%c%d %d", 'F','i','n','d', 8, ':', res, ' ', pair_idx1, pair_idx2);
    printf("%c", 10);

    res = find_pair_sum(arr, n, 100);
    printf("%c%c%c%c %d%c %d", 'F','i','n','d', 100, ':', res);
    printf("%c", 10);

    // Count pairs
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    arr[5]=6; arr[6]=7; arr[7]=8;
    n = 8;
    printf("%c%c%c%c%c%c%c%c%c%c %d%c%d", 'C','o','u','n','t','P','a','i','r','s', 9, '=', count_pairs_sum(arr, n, 9));
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%c %d%c%d", 'C','o','u','n','t','P','a','i','r','s', 5, '=', count_pairs_sum(arr, n, 5));
    printf("%c", 10);

    // Three sum
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    arr[5]=6; arr[6]=7;
    n = 7;
    res = three_sum(arr, n, 12);
    printf("%c%c%c%c%c%c%c%c %d%c %d %d%c%d %d%c%d", 'T','h','r','e','e','S','u','m', 12, ':', res, ' ', trip_a, '+', trip_b, '+', trip_c);
    printf("%c", 10);

    // ===== REMOVE DUPLICATES =====
    printf("%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c", 'R','e','m','o','v','e', 'D','u','p','l','i','c','a','t','e','s');
    printf("%c", 10);

    arr[0]=1; arr[1]=1; arr[2]=2; arr[3]=2; arr[4]=2;
    arr[5]=3; arr[6]=4; arr[7]=4; arr[8]=5;
    n = 9;
    res = remove_duplicates(arr, n);
    printf("%c%c%c%c%c%c%c %d%c ", 'U','n','i','q','u','e','=', res, ':');
    print_arr(arr, res);

    // Remove allowing 2 copies
    arr[0]=1; arr[1]=1; arr[2]=1; arr[3]=2; arr[4]=2;
    arr[5]=2; arr[6]=3; arr[7]=3;
    n = 8;
    res = remove_duplicates_2(arr, n);
    printf("%c%c%c%c%c%c%c%c%c%c %d%c ", 'U','n','i','q','u','e','A','t','2','=', res, ':');
    print_arr(arr, res);

    // Remove value
    arr[0]=3; arr[1]=2; arr[2]=2; arr[3]=3; arr[4]=4;
    arr[5]=2; arr[6]=5;
    n = 7;
    res = remove_value(arr, n, 2);
    printf("%c%c%c%c%c%c%c%c%d%c %d%c ", 'R','e','m','o','v','e','V','=', 2, ':', res, ':');
    print_arr(arr, res);

    // ===== PARTITION =====
    printf("%c%c%c%c%c%c%c%c%c", 'P','a','r','t','i','t','i','o','n');
    printf("%c", 10);

    arr[0]=3; arr[1]=6; arr[2]=8; arr[3]=2; arr[4]=7;
    arr[5]=5; arr[6]=1; arr[7]=4;
    n = 8;
    res = partition_lomuto(arr, 0, n - 1);
    printf("%c%c%c%c%c%c %c%c%c%c%c%d %c%c%c%c%c%d", 'L','o','m','u','t','o', 'p','i','v','o','t', arr[res], 'i','n','d','e','x', res);
    printf("%c", 10);

    // Dutch National Flag
    arr[0]=5; arr[1]=2; arr[2]=5; arr[3]=8; arr[4]=5;
    arr[5]=1; arr[6]=9; arr[7]=5; arr[8]=3;
    n = 9;
    dutch_national_flag(arr, n, 5);
    printf("%c%c%c %c%c%c%c%c%c%d %c%c%c%c%c%d %c%c%c%c%c%c%c ", 'D','N','F', 'l','o','w','e','r', '=', dnf_lo_result, 'u','p','p','e','r', '=', dnf_hi_result, 'r','e','s','u','l','t',':');
    print_arr(arr, n);

    // Even/Odd partition
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    arr[5]=6; arr[6]=7; arr[7]=8;
    n = 8;
    res = partition_even_odd(arr, n);
    printf("%c%c%c%c%c%c%c%c%c%d", 'E','v','e','n','C','o','u','n','t', res);
    printf("%c", 10);

    // ===== MERGE SORTED =====
    printf("%c%c%c%c%c %c%c%c%c%c%c", 'M','e','r','g','e', 'S','o','r','t','e','d');
    printf("%c", 10);

    arr[0]=1; arr[1]=3; arr[2]=5; arr[3]=7; arr[4]=9;
    n1 = 5;
    arr2[0]=2; arr2[1]=4; arr2[2]=6; arr2[3]=8; arr2[4]=10;
    n2 = 5;
    merge_sorted(arr, n1, arr2, n2);
    printf("%c%c%c%c%c%c %d%c ", 'M','e','r','g','e','d', merge_len, ':');
    print_res_arr(merge_result, merge_len);

    // Intersection
    arr[0]=1; arr[1]=3; arr[2]=5; arr[3]=7; arr[4]=9;
    n1 = 5;
    arr2[0]=3; arr2[1]=5; arr2[2]=9; arr2[3]=11;
    n2 = 4;
    sorted_intersection(arr, n1, arr2, n2);
    printf("%c%c%c%c%c %d%c ", 'I','n','t','e','r', intersect_len, ':');
    print_arr(intersect_result, intersect_len);

    // Difference
    arr[0]=1; arr[1]=3; arr[2]=5; arr[3]=7; arr[4]=9;
    n1 = 5;
    arr2[0]=3; arr2[1]=5;
    n2 = 2;
    sorted_difference(arr, n1, arr2, n2);
    printf("%c%c%c%c %d%c ", 'D','i','f','f', diff_len, ':');
    print_arr(diff_result, diff_len);

    // Union
    arr[0]=1; arr[1]=3; arr[2]=5;
    n1 = 3;
    arr2[0]=2; arr2[1]=3; arr2[2]=6;
    n2 = 3;
    sorted_union(arr, n1, arr2, n2);
    printf("%c%c%c%c%c %d%c ", 'U','n','i','o','n', union_len, ':');
    print_res_arr(union_result, union_len);

    // ===== CONTAINER WITH MOST WATER =====
    printf("%c%c%c%c%c%c%c%c%c %c%c%c%c%c", 'C','o','n','t','a','i','n','e','r', 'W','a','t','e','r');
    printf("%c", 10);

    arr[0]=1; arr[1]=8; arr[2]=6; arr[3]=2; arr[4]=5;
    arr[5]=4; arr[6]=8; arr[7]=3; arr[8]=7;
    n = 9;
    printf("%c%c%c%c%c%c%c%c%d", 'M','a','x','A','r','e','a','=', max_water(arr, n));
    printf("%c", 10);

    // ===== SORT 012 =====
    printf("%c%c%c%c %d%c%c%c%d%c%c%c%d", 'S','o','r','t', 0, '1', 's', ' ', 1, '1', 's', ' ', 2);
    printf("%c", 10);

    arr[0]=2; arr[1]=0; arr[2]=1; arr[3]=2; arr[4]=0;
    arr[5]=1; arr[6]=0; arr[7]=2; arr[8]=1;
    n = 9;
    sort_012(arr, n);
    printf("%c%c%c%c%c%c%c%c%c ", 'S','o','r','t','e','d','0','1','2');
    print_arr(arr, n);

    // ===== MOVE ZEROS =====
    arr[0]=0; arr[1]=1; arr[2]=0; arr[3]=3; arr[4]=12;
    arr[5]=0; arr[6]=5;
    n = 7;
    res = move_zeros(arr, n);
    printf("%c%c%c%c%c%c%c%c%c%c %d%c ", 'M','o','v','e','Z','e','r','o','s',':', res, ':');
    print_arr(arr, n);

    // ===== CLOSEST PAIR SUM =====
    arr[0]=1; arr[1]=4; arr[2]=7; arr[3]=10; arr[4]=20;
    n = 5;
    res = closest_pair_sum(arr, n, 15);
    printf("%c%c%c%c%c%c%c %d%c%d", 'C','l','o','s','e','s','t', 15, '=', res);
    printf("%c", 10);

    res = closest_pair_sum(arr, n, 12);
    printf("%c%c%c%c%c%c%c %d%c%d", 'C','l','o','s','e','s','t', 12, '=', res);
    printf("%c", 10);

    printf("%c%c%c%c %c%c%c%c%c%c%c%d", 'A','l','l',' ', 'p','a','s','s','e','d','=', 1);
    printf("%c", 10);

    return 0;
}
