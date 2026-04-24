int printf(const char *fmt, ...);
// EXPECT: Sliding Window Max\nWindowMx k=3: 3 3 5 5 6 7 7 7\nWindowMx k=4: 3 5 5 6 7 7 7\nWindowMx k=5: 5 5 6 7 7 7\nSliding Window Min\nWindowMn k=3: -1 -3 -3 -3 3 3 2 2\nWindow Sum\nWinSum k=3: 3 -1 1 5 14 16 15 13\nWinSum k=4: 0 4 4 11 21 18 19\nMinWinSum3 k=3=-1\nMaxWinSum3 k=3=16\nMinWinSum5 k=5=5\nMaxWinSum5 k=5=23\nWindow Average\nWinAvg k=3: 1 0 0 1 4 5 5 4\nWinAvg k=4: 0 1 1 2 5 4 4\nLongest Subarray\nLongSubSum<=7 length=4\nLongSubSum<=10 length=4\nKDistinctWindow\nKDistinct 2=3\nKDistinct 3=6\nKDistinct 4=9\nMinSizeSubarray\nMinSize 7=2\nMinSize 4=1\nMinSize 100=-1\nSubarrSum 5=2\nSubarrSum 9=2\nLongestSame4\nLongDiff<=K=2 len3\nLongDiff<=K=3 len4\nWindowExactSm 6=7\nMaxOfMinWn k=3=3\nWindow Range\nWinRanges k=3: 4 6 8 8 3 4 5 5\nWinRanges k=4: 6 8 8 9 4 5 5\nAll  passed=1

// Sliding window: maximum, minimum window sum, window average (integer), longest window with property

int arr[100];
int result_arr[100];

// ============ SLIDING WINDOW MAXIMUM ============
// For each window of size k, find the maximum element
// Simple O(n*k) approach
int win_max[100];
int win_max_count;

void sliding_window_max(int a[100], int n, int k) {
    int i;
    int j;
    int mx;
    win_max_count = 0;
    for (i = 0; i <= n - k; i = i + 1) {
        mx = a[i];
        for (j = i + 1; j < i + k; j = j + 1) {
            if (a[j] > mx) {
                mx = a[j];
            }
        }
        win_max[win_max_count] = mx;
        win_max_count = win_max_count + 1;
    }
}

// ============ SLIDING WINDOW MINIMUM ============
int win_min[100];
int win_min_count;

void sliding_window_min(int a[100], int n, int k) {
    int i;
    int j;
    int mn;
    win_min_count = 0;
    for (i = 0; i <= n - k; i = i + 1) {
        mn = a[i];
        for (j = i + 1; j < i + k; j = j + 1) {
            if (a[j] < mn) {
                mn = a[j];
            }
        }
        win_min[win_min_count] = mn;
        win_min_count = win_min_count + 1;
    }
}

// ============ MINIMUM WINDOW SUM ============
// Find the window of size k with minimum sum
int min_window_sum(int a[100], int n, int k) {
    int sum;
    int min_sum;
    int i;

    sum = 0;
    for (i = 0; i < k; i = i + 1) {
        sum = sum + a[i];
    }
    min_sum = sum;

    for (i = k; i < n; i = i + 1) {
        sum = sum + a[i] - a[i - k];
        if (sum < min_sum) {
            min_sum = sum;
        }
    }
    return min_sum;
}

// Maximum window sum
int max_window_sum(int a[100], int n, int k) {
    int sum;
    int max_sum;
    int i;

    sum = 0;
    for (i = 0; i < k; i = i + 1) {
        sum = sum + a[i];
    }
    max_sum = sum;

    for (i = k; i < n; i = i + 1) {
        sum = sum + a[i] - a[i - k];
        if (sum > max_sum) {
            max_sum = sum;
        }
    }
    return max_sum;
}

// ============ WINDOW AVERAGE (INTEGER) ============
// Compute integer average for each window of size k
int win_avg[100];
int win_avg_count;

void sliding_window_avg(int a[100], int n, int k) {
    int sum;
    int i;
    win_avg_count = 0;

    sum = 0;
    for (i = 0; i < k; i = i + 1) {
        sum = sum + a[i];
    }
    win_avg[win_avg_count] = sum / k;
    win_avg_count = win_avg_count + 1;

    for (i = k; i < n; i = i + 1) {
        sum = sum + a[i] - a[i - k];
        win_avg[win_avg_count] = sum / k;
        win_avg_count = win_avg_count + 1;
    }
}

// ============ LONGEST SUBARRAY WITH SUM <= TARGET ============
int longest_subarray_sum_le(int a[100], int n, int target) {
    int lo;
    int sum;
    int best;
    int i;
    lo = 0;
    sum = 0;
    best = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + a[i];
        while (sum > target && lo <= i) {
            sum = sum - a[lo];
            lo = lo + 1;
        }
        if (i - lo + 1 > best) {
            best = i - lo + 1;
        }
    }
    return best;
}

// ============ LONGEST SUBARRAY WITH AT MOST K DISTINCT VALUES ============
int val_count[101]; // count of each value (assuming values 0..100)

int longest_k_distinct(int a[100], int n, int k) {
    int lo;
    int distinct;
    int best;
    int i;

    for (i = 0; i <= 100; i = i + 1) {
        val_count[i] = 0;
    }

    lo = 0;
    distinct = 0;
    best = 0;

    for (i = 0; i < n; i = i + 1) {
        if (val_count[a[i]] == 0) {
            distinct = distinct + 1;
        }
        val_count[a[i]] = val_count[a[i]] + 1;

        while (distinct > k) {
            val_count[a[lo]] = val_count[a[lo]] - 1;
            if (val_count[a[lo]] == 0) {
                distinct = distinct - 1;
            }
            lo = lo + 1;
        }

        if (i - lo + 1 > best) {
            best = i - lo + 1;
        }
    }
    return best;
}

// ============ MINIMUM SIZE SUBARRAY SUM >= TARGET ============
int min_size_subarray_sum(int a[100], int n, int target) {
    int lo;
    int sum;
    int best;
    int i;
    lo = 0;
    sum = 0;
    best = n + 1;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + a[i];
        while (sum >= target) {
            if (i - lo + 1 < best) {
                best = i - lo + 1;
            }
            sum = sum - a[lo];
            lo = lo + 1;
        }
    }
    if (best > n) {
        return -1;
    }
    return best;
}

// ============ COUNT SUBARRAYS WITH SUM == TARGET ============
int count_subarrays_sum(int a[100], int n, int target) {
    int count;
    int i;
    int j;
    int sum;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = 0;
        for (j = i; j < n; j = j + 1) {
            sum = sum + a[j];
            if (sum == target) {
                count = count + 1;
            }
        }
    }
    return count;
}

// ============ FIXED WINDOW STATISTICS ============
// Sum, max, min for each window
int win_sum[100];
int win_sum_count;

void sliding_window_sum(int a[100], int n, int k) {
    int sum;
    int i;
    win_sum_count = 0;
    sum = 0;
    for (i = 0; i < k; i = i + 1) {
        sum = sum + a[i];
    }
    win_sum[win_sum_count] = sum;
    win_sum_count = win_sum_count + 1;

    for (i = k; i < n; i = i + 1) {
        sum = sum + a[i] - a[i - k];
        win_sum[win_sum_count] = sum;
        win_sum_count = win_sum_count + 1;
    }
}

// ============ LONGEST WINDOW WITH ALL ELEMENTS SAME ============
int longest_same(int a[100], int n) {
    int best;
    int cur;
    int i;
    if (n == 0) return 0;
    best = 1;
    cur = 1;
    for (i = 1; i < n; i = i + 1) {
        if (a[i] == a[i - 1]) {
            cur = cur + 1;
            if (cur > best) {
                best = cur;
            }
        } else {
            cur = 1;
        }
    }
    return best;
}

// ============ LONGEST WINDOW WITH DIFF <= K ============
int longest_diff_le_k(int a[100], int n, int k) {
    // In window [lo, i], maintain max - min <= k
    // Simple O(n^2) approach
    int lo;
    int best;
    int mn;
    int mx;
    int j;
    int i;
    best = 0;
    for (i = 0; i < n; i = i + 1) {
        mn = a[i];
        mx = a[i];
        for (j = i; j < n; j = j + 1) {
            if (a[j] < mn) mn = a[j];
            if (a[j] > mx) mx = a[j];
            if (mx - mn <= k) {
                if (j - i + 1 > best) {
                    best = j - i + 1;
                }
            } else {
                break;
            }
        }
    }
    return best;
}

// ============ NUMBER OF WINDOWS WITH EXACT SUM ============
int count_windows_exact_sum(int a[100], int n, int k, int target) {
    int sum;
    int count;
    int i;
    sum = 0;
    count = 0;
    for (i = 0; i < k; i = i + 1) {
        sum = sum + a[i];
    }
    if (sum == target) count = count + 1;

    for (i = k; i < n; i = i + 1) {
        sum = sum + a[i] - a[i - k];
        if (sum == target) {
            count = count + 1;
        }
    }
    return count;
}

// ============ MAXIMUM OF MINIMUMS OVER ALL WINDOWS OF SIZE K ============
int max_of_min_windows(int a[100], int n, int k) {
    int best;
    int i;
    int j;
    int mn;
    best = -100000;
    for (i = 0; i <= n - k; i = i + 1) {
        mn = a[i];
        for (j = i + 1; j < i + k; j = j + 1) {
            if (a[j] < mn) {
                mn = a[j];
            }
        }
        if (mn > best) {
            best = mn;
        }
    }
    return best;
}

// ============ WINDOW RANGE (MAX-MIN) FOR EACH WINDOW ============
int win_range[100];
int win_range_count;

void sliding_window_range(int a[100], int n, int k) {
    int i;
    int j;
    int mn;
    int mx;
    win_range_count = 0;
    for (i = 0; i <= n - k; i = i + 1) {
        mn = a[i];
        mx = a[i];
        for (j = i + 1; j < i + k; j = j + 1) {
            if (a[j] < mn) mn = a[j];
            if (a[j] > mx) mx = a[j];
        }
        win_range[win_range_count] = mx - mn;
        win_range_count = win_range_count + 1;
    }
}

// Helper: print int array
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

// Helper: sum of array
int array_sum(int a[100], int n) {
    int s;
    int i;
    s = 0;
    for (i = 0; i < n; i = i + 1) {
        s = s + a[i];
    }
    return s;
}

// Helper: absolute value
int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// Helper: min/max
int min_int(int a, int b) { if (a < b) return a; return b; }
int max_int(int a, int b) { if (a > b) return a; return b; }

int main(void) {
    int n;
    int k;
    int i;
    int res;

    // Setup test array
    arr[0]=1; arr[1]=3; arr[2]=-1; arr[3]=-3; arr[4]=5;
    arr[5]=3; arr[6]=6; arr[7]=7; arr[8]=2; arr[9]=4;
    n = 10;

    // ===== SLIDING WINDOW MAX =====
    printf("%c%c%c%c%c%c%c %c%c%c%c%c%c %c%c%c", 'S','l','i','d','i','n','g', 'W','i','n','d','o','w', 'M','a','x');
    printf("%c", 10);

    k = 3;
    sliding_window_max(arr, n, k);
    printf("%c%c%c%c%c%c%c%c %c%c%d%c ", 'W','i','n','d','o','w','M','x', 'k','=', k, ':');
    print_arr(win_max, win_max_count);

    k = 4;
    sliding_window_max(arr, n, k);
    printf("%c%c%c%c%c%c%c%c %c%c%d%c ", 'W','i','n','d','o','w','M','x', 'k','=', k, ':');
    print_arr(win_max, win_max_count);

    k = 5;
    sliding_window_max(arr, n, k);
    printf("%c%c%c%c%c%c%c%c %c%c%d%c ", 'W','i','n','d','o','w','M','x', 'k','=', k, ':');
    print_arr(win_max, win_max_count);

    // ===== SLIDING WINDOW MIN =====
    printf("%c%c%c%c%c%c%c %c%c%c%c%c%c %c%c%c", 'S','l','i','d','i','n','g', 'W','i','n','d','o','w', 'M','i','n');
    printf("%c", 10);

    k = 3;
    sliding_window_min(arr, n, k);
    printf("%c%c%c%c%c%c%c%c %c%c%d%c ", 'W','i','n','d','o','w','M','n', 'k','=', k, ':');
    print_arr(win_min, win_min_count);

    // ===== WINDOW SUM =====
    printf("%c%c%c%c%c%c %c%c%c", 'W','i','n','d','o','w', 'S','u','m');
    printf("%c", 10);

    k = 3;
    sliding_window_sum(arr, n, k);
    printf("%c%c%c%c%c%c %c%c%d%c ", 'W','i','n','S','u','m', 'k','=', k, ':');
    print_arr(win_sum, win_sum_count);

    k = 4;
    sliding_window_sum(arr, n, k);
    printf("%c%c%c%c%c%c %c%c%d%c ", 'W','i','n','S','u','m', 'k','=', k, ':');
    print_arr(win_sum, win_sum_count);

    // Min/Max window sums
    printf("%c%c%c%c%c%c%c%c%c%c %c%c%d%c%d", 'M','i','n','W','i','n','S','u','m','3', 'k','=', 3, '=', min_window_sum(arr, n, 3));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c%c%c %c%c%d%c%d", 'M','a','x','W','i','n','S','u','m','3', 'k','=', 3, '=', max_window_sum(arr, n, 3));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c%c%c %c%c%d%c%d", 'M','i','n','W','i','n','S','u','m','5', 'k','=', 5, '=', min_window_sum(arr, n, 5));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c%c%c %c%c%d%c%d", 'M','a','x','W','i','n','S','u','m','5', 'k','=', 5, '=', max_window_sum(arr, n, 5));
    printf("%c", 10);

    // ===== WINDOW AVERAGE =====
    printf("%c%c%c%c%c%c %c%c%c%c%c%c%c", 'W','i','n','d','o','w', 'A','v','e','r','a','g','e');
    printf("%c", 10);

    k = 3;
    sliding_window_avg(arr, n, k);
    printf("%c%c%c%c%c%c %c%c%d%c ", 'W','i','n','A','v','g', 'k','=', k, ':');
    print_arr(win_avg, win_avg_count);

    k = 4;
    sliding_window_avg(arr, n, k);
    printf("%c%c%c%c%c%c %c%c%d%c ", 'W','i','n','A','v','g', 'k','=', k, ':');
    print_arr(win_avg, win_avg_count);

    // ===== LONGEST SUBARRAY SUM <= TARGET =====
    printf("%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c", 'L','o','n','g','e','s','t', 'S','u','b','a','r','r','a','y');
    printf("%c", 10);

    // Use positive array for this
    arr[0]=2; arr[1]=3; arr[2]=1; arr[3]=2; arr[4]=4;
    arr[5]=3; arr[6]=1; arr[7]=1; arr[8]=2;
    n = 9;

    res = longest_subarray_sum_le(arr, n, 7);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%d %c%c%c%c%c%c%c%d", 'L','o','n','g','S','u','b','S','u','m','<','=', 7, 'l','e','n','g','t','h','=', res);
    printf("%c", 10);

    res = longest_subarray_sum_le(arr, n, 10);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%d %c%c%c%c%c%c%c%d", 'L','o','n','g','S','u','b','S','u','m','<','=', 10, 'l','e','n','g','t','h','=', res);
    printf("%c", 10);

    // ===== LONGEST WITH K DISTINCT =====
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 'K','D','i','s','t','i','n','c','t','W','i','n','d','o','w');
    printf("%c", 10);

    arr[0]=1; arr[1]=2; arr[2]=1; arr[3]=3; arr[4]=2;
    arr[5]=3; arr[6]=4; arr[7]=1; arr[8]=2;
    n = 9;

    res = longest_k_distinct(arr, n, 2);
    printf("%c%c%c%c%c%c%c%c%c %d%c%d", 'K','D','i','s','t','i','n','c','t', 2, '=', res);
    printf("%c", 10);

    res = longest_k_distinct(arr, n, 3);
    printf("%c%c%c%c%c%c%c%c%c %d%c%d", 'K','D','i','s','t','i','n','c','t', 3, '=', res);
    printf("%c", 10);

    res = longest_k_distinct(arr, n, 4);
    printf("%c%c%c%c%c%c%c%c%c %d%c%d", 'K','D','i','s','t','i','n','c','t', 4, '=', res);
    printf("%c", 10);

    // ===== MIN SIZE SUBARRAY SUM >= TARGET =====
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 'M','i','n','S','i','z','e','S','u','b','a','r','r','a','y');
    printf("%c", 10);

    arr[0]=2; arr[1]=3; arr[2]=1; arr[3]=2; arr[4]=4;
    arr[5]=3;
    n = 6;

    res = min_size_subarray_sum(arr, n, 7);
    printf("%c%c%c%c%c%c%c %d%c%d", 'M','i','n','S','i','z','e', 7, '=', res);
    printf("%c", 10);

    res = min_size_subarray_sum(arr, n, 4);
    printf("%c%c%c%c%c%c%c %d%c%d", 'M','i','n','S','i','z','e', 4, '=', res);
    printf("%c", 10);

    res = min_size_subarray_sum(arr, n, 100);
    printf("%c%c%c%c%c%c%c %d%c%d", 'M','i','n','S','i','z','e', 100, '=', res);
    printf("%c", 10);

    // ===== COUNT SUBARRAYS WITH EXACT SUM =====
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    n = 5;
    res = count_subarrays_sum(arr, n, 5);
    printf("%c%c%c%c%c%c%c%c%c %d%c%d", 'S','u','b','a','r','r','S','u','m', 5, '=', res);
    printf("%c", 10);

    res = count_subarrays_sum(arr, n, 9);
    printf("%c%c%c%c%c%c%c%c%c %d%c%d", 'S','u','b','a','r','r','S','u','m', 9, '=', res);
    printf("%c", 10);

    // ===== LONGEST SAME =====
    arr[0]=1; arr[1]=1; arr[2]=1; arr[3]=2; arr[4]=2;
    arr[5]=3; arr[6]=3; arr[7]=3; arr[8]=3;
    n = 9;
    printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'L','o','n','g','e','s','t','S','a','m','e', longest_same(arr, n));
    printf("%c", 10);

    // ===== LONGEST WITH DIFF <= K =====
    arr[0]=1; arr[1]=3; arr[2]=2; arr[3]=5; arr[4]=4;
    arr[5]=6; arr[6]=3;
    n = 7;
    res = longest_diff_le_k(arr, n, 2);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%d %c%c%c%d", 'L','o','n','g','D','i','f','f','<','=','K','=', 2, 'l','e','n', res);
    printf("%c", 10);

    res = longest_diff_le_k(arr, n, 3);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%d %c%c%c%d", 'L','o','n','g','D','i','f','f','<','=','K','=', 3, 'l','e','n', res);
    printf("%c", 10);

    // ===== WINDOWS WITH EXACT SUM =====
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=1; arr[4]=2;
    arr[5]=3; arr[6]=1; arr[7]=2; arr[8]=3;
    n = 9;
    res = count_windows_exact_sum(arr, n, 3, 6);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c %d%c%d", 'W','i','n','d','o','w','E','x','a','c','t','S','m', 6, '=', res);
    printf("%c", 10);

    // ===== MAX OF MINS =====
    arr[0]=1; arr[1]=3; arr[2]=-1; arr[3]=-3; arr[4]=5;
    arr[5]=3; arr[6]=6; arr[7]=7;
    n = 8;
    k = 3;
    res = max_of_min_windows(arr, n, k);
    printf("%c%c%c%c%c%c%c%c%c%c %c%c%d%c%d", 'M','a','x','O','f','M','i','n','W','n', 'k','=', k, '=', res);
    printf("%c", 10);

    // ===== WINDOW RANGE =====
    printf("%c%c%c%c%c%c %c%c%c%c%c", 'W','i','n','d','o','w', 'R','a','n','g','e');
    printf("%c", 10);

    arr[0]=1; arr[1]=3; arr[2]=-1; arr[3]=-3; arr[4]=5;
    arr[5]=3; arr[6]=6; arr[7]=7; arr[8]=2; arr[9]=4;
    n = 10;
    k = 3;
    sliding_window_range(arr, n, k);
    printf("%c%c%c%c%c%c%c%c%c %c%c%d%c ", 'W','i','n','R','a','n','g','e','s', 'k','=', k, ':');
    print_arr(win_range, win_range_count);

    k = 4;
    sliding_window_range(arr, n, k);
    printf("%c%c%c%c%c%c%c%c%c %c%c%d%c ", 'W','i','n','R','a','n','g','e','s', 'k','=', k, ':');
    print_arr(win_range, win_range_count);

    printf("%c%c%c%c %c%c%c%c%c%c%c%d", 'A','l','l',' ', 'p','a','s','s','e','d','=', 1);
    printf("%c", 10);

    return 0;
}
