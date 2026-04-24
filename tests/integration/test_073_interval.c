int printf(const char *fmt, ...);
// EXPECT: === Merge intervals test 1 ===\nMerged count: 3\n[1,6] [8,12] [15,20]\n=== Merge intervals test 2 ===\nMerged count: 1\n[0,10]\n=== Overlap detection ===\n[1,5] & [3,7]: overlap=1\n[1,3] & [5,7]: overlap=0\n[1,5] & [5,7]: overlap=0\n[1,6] & [4,8]: overlap=1\n=== Intersection ===\n[1,5] & [3,7]: [3,5]\n[0,10] & [5,15]: [5,10]\n[1,3] & [5,7]: no intersection\n=== List intersection ===\nList intersect count: 3\n[5,7] [9,10] [15,16]\n=== Max non-overlapping ===\nMax non-overlap: 3\n=== Min rooms ===\nMin rooms: 4\n=== Total covered length ===\nCovered: 13\n=== Count overlaps ===\nPairwise overlaps: 4\n=== Containment ===\n[3,5] in [1,7]: 1\n[1,7] in [3,5]: 0\n[2,6] in [2,6]: 1\n=== Remove interval ===\n[1,10] minus [3,7]: count=2\n[1,3] [7,10]\n[1,10] minus [0,5]: count=1\n[5,10]\n=== Gap analysis ===\nGaps count: 3\n[0,1] [6,8] [12,20]\n=== Weighted scheduling ===\nWeighted max: 9\n=== Interval coloring ===\nColors needed: 4\n=== Union ===\n[1,5] U [3,7] = [1,7]\n=== Complement ===\nComplement count: 3\n=== Interval diff ===\n[1,10] - [4,6]: count=2\n[1,4] [6,10]\n=== K-covered ===\n2-covered points: 5\n=== Max depth ===\nMax depth: 3\n=== Contains point ===\n[2,8] contains 5: 1\n[2,8] contains 8: 0\nCount containing 5: 3\n=== Symmetric diff ===\nSym diff [1,5],[3,7]: 4\nSym diff [1,3],[5,7]: 4\nAll interval tests passed!
// ============================================================
// test_073_interval.c
// Interval merging, interval intersection, interval scheduling
// (greedy), overlap detection using struct arrays.
// ============================================================

// ---- Interval struct ----
struct Interval {
    int start;
    int end;
};

// ---- Swap two intervals ----
void swap_intervals(struct Interval *a, struct Interval *b) {
    struct Interval temp;
    temp.start = a->start;
    temp.end = a->end;
    a->start = b->start;
    a->end = b->end;
    b->start = temp.start;
    b->end = temp.end;
}

// ---- Sort intervals by start time (bubble sort) ----
void sort_by_start(struct Interval arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j].start > arr[j + 1].start) {
                swap_intervals(&arr[j], &arr[j + 1]);
            } else if (arr[j].start == arr[j + 1].start &&
                       arr[j].end > arr[j + 1].end) {
                swap_intervals(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// ---- Sort intervals by end time (bubble sort) ----
void sort_by_end(struct Interval arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j].end > arr[j + 1].end) {
                swap_intervals(&arr[j], &arr[j + 1]);
            } else if (arr[j].end == arr[j + 1].end &&
                       arr[j].start > arr[j + 1].start) {
                swap_intervals(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// ---- Max of two ints ----
int max_int(int a, int b) {
    return (a > b) ? a : b;
}

// ---- Min of two ints ----
int min_int(int a, int b) {
    return (a < b) ? a : b;
}

// ---- Merge overlapping intervals ----
// Returns number of merged intervals
int merge_intervals(struct Interval arr[], int n, struct Interval result[]) {
    if (n == 0) return 0;

    // Sort by start
    sort_by_start(arr, n);

    int count = 0;
    result[0].start = arr[0].start;
    result[0].end = arr[0].end;
    count = 1;

    int i;
    for (i = 1; i < n; i++) {
        if (arr[i].start <= result[count - 1].end) {
            // Overlapping or adjacent: merge
            result[count - 1].end = max_int(result[count - 1].end, arr[i].end);
        } else {
            // No overlap: add new interval
            result[count].start = arr[i].start;
            result[count].end = arr[i].end;
            count++;
        }
    }
    return count;
}

// ---- Check if two intervals overlap ----
int intervals_overlap(struct Interval a, struct Interval b) {
    return (a.start < b.end && b.start < a.end) ? 1 : 0;
}

// ---- Compute intersection of two intervals ----
// Returns 1 if intersection exists, sets result
int interval_intersection(struct Interval a, struct Interval b,
                          struct Interval *result) {
    int s = max_int(a.start, b.start);
    int e = min_int(a.end, b.end);
    if (s < e) {
        result->start = s;
        result->end = e;
        return 1;
    }
    return 0;
}

// ---- Intersection of two lists of intervals ----
// Both lists are sorted and non-overlapping
int intersect_lists(struct Interval A[], int nA,
                    struct Interval B[], int nB,
                    struct Interval result[]) {
    int i = 0, j = 0, count = 0;
    while (i < nA && j < nB) {
        int s = max_int(A[i].start, B[j].start);
        int e = min_int(A[i].end, B[j].end);
        if (s < e) {
            result[count].start = s;
            result[count].end = e;
            count++;
        }
        if (A[i].end < B[j].end) {
            i++;
        } else {
            j++;
        }
    }
    return count;
}

// ---- Interval scheduling: maximum non-overlapping intervals ----
// Greedy: sort by end time, always pick earliest ending
int max_non_overlapping(struct Interval arr[], int n) {
    if (n == 0) return 0;
    sort_by_end(arr, n);

    int count = 1;
    int last_end = arr[0].end;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i].start >= last_end) {
            count++;
            last_end = arr[i].end;
        }
    }
    return count;
}

// ---- Minimum number of rooms for overlapping meetings ----
// Sweep line approach
int min_rooms(struct Interval arr[], int n) {
    // Flatten to events
    int events[200]; // start = +1, end = -1
    int times[200];
    int ne = 0;
    int i, j;

    for (i = 0; i < n; i++) {
        times[ne] = arr[i].start;
        events[ne] = 1;
        ne++;
        times[ne] = arr[i].end;
        events[ne] = -1;
        ne++;
    }

    // Sort events by time, break ties: end before start
    for (i = 0; i < ne - 1; i++) {
        for (j = 0; j < ne - 1 - i; j++) {
            int swap = 0;
            if (times[j] > times[j + 1]) {
                swap = 1;
            } else if (times[j] == times[j + 1] && events[j] > events[j + 1]) {
                swap = 1;
            }
            if (swap) {
                int tmp;
                tmp = times[j]; times[j] = times[j + 1]; times[j + 1] = tmp;
                tmp = events[j]; events[j] = events[j + 1]; events[j + 1] = tmp;
            }
        }
    }

    int max_rooms = 0;
    int current = 0;
    for (i = 0; i < ne; i++) {
        current = current + events[i];
        if (current > max_rooms) max_rooms = current;
    }
    return max_rooms;
}

// ---- Total length covered by intervals ----
int total_covered_length(struct Interval arr[], int n) {
    struct Interval merged[100];
    int m = merge_intervals(arr, n, merged);
    int total = 0;
    int i;
    for (i = 0; i < m; i++) {
        total = total + (merged[i].end - merged[i].start);
    }
    return total;
}

// ---- Count pairwise overlaps ----
int count_overlaps(struct Interval arr[], int n) {
    int count = 0;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (intervals_overlap(arr[i], arr[j])) {
                count++;
            }
        }
    }
    return count;
}

// ---- Check if an interval is contained in another ----
int is_contained(struct Interval inner, struct Interval outer) {
    return (inner.start >= outer.start && inner.end <= outer.end) ? 1 : 0;
}

// ---- Remove interval from a set (logical) ----
// Remove interval 'rem' from interval 'orig'
// Can produce 0, 1, or 2 result intervals
int remove_interval(struct Interval orig, struct Interval rem,
                    struct Interval result[]) {
    // No overlap
    if (rem.start >= orig.end || rem.end <= orig.start) {
        result[0] = orig;
        return 1;
    }
    int count = 0;
    // Left part
    if (rem.start > orig.start) {
        result[count].start = orig.start;
        result[count].end = rem.start;
        count++;
    }
    // Right part
    if (rem.end < orig.end) {
        result[count].start = rem.end;
        result[count].end = orig.end;
        count++;
    }
    return count;
}

// ---- Gap analysis: find gaps between merged intervals ----
int find_gaps(struct Interval arr[], int n,
              struct Interval gaps[], int range_start, int range_end) {
    struct Interval merged[100];
    int m = merge_intervals(arr, n, merged);
    int count = 0;
    int i;

    if (m == 0) {
        gaps[0].start = range_start;
        gaps[0].end = range_end;
        return 1;
    }

    if (range_start < merged[0].start) {
        gaps[count].start = range_start;
        gaps[count].end = merged[0].start;
        count++;
    }

    for (i = 1; i < m; i++) {
        if (merged[i - 1].end < merged[i].start) {
            gaps[count].start = merged[i - 1].end;
            gaps[count].end = merged[i].start;
            count++;
        }
    }

    if (merged[m - 1].end < range_end) {
        gaps[count].start = merged[m - 1].end;
        gaps[count].end = range_end;
        count++;
    }

    return count;
}

// ---- Weighted interval scheduling (DP, small n) ----
// Each interval has a weight; maximize total weight of non-overlapping set
int weighted_scheduling(struct Interval arr[], int weights[], int n) {
    sort_by_end(arr, n);

    // dp[i] = max weight using intervals 0..i
    int dp[50];
    int i, j;

    for (i = 0; i < n; i++) {
        dp[i] = weights[i];
        for (j = 0; j < i; j++) {
            if (arr[j].end <= arr[i].start) {
                if (dp[j] + weights[i] > dp[i]) {
                    dp[i] = dp[j] + weights[i];
                }
            }
        }
    }

    int best = 0;
    for (i = 0; i < n; i++) {
        if (dp[i] > best) best = dp[i];
    }
    return best;
}

// ---- Interval partitioning: minimum colors needed ----
// Same as min rooms
int interval_coloring(struct Interval arr[], int n) {
    return min_rooms(arr, n);
}

// ---- Union of two intervals ----
struct Interval interval_union(struct Interval a, struct Interval b) {
    struct Interval result;
    result.start = min_int(a.start, b.start);
    result.end = max_int(a.end, b.end);
    return result;
}

// ---- Complement of intervals in a range ----
int complement_intervals(struct Interval arr[], int n,
                         int range_start, int range_end,
                         struct Interval result[]) {
    return find_gaps(arr, n, result, range_start, range_end);
}

// ---- Interval difference: A minus B ----
// A and B are single intervals
int interval_diff(struct Interval a, struct Interval b,
                  struct Interval result[]) {
    return remove_interval(a, b, result);
}

// ---- Count points covered by at least k intervals ----
// In range [0, max_point], count integer points covered by >= k intervals
int count_k_covered(struct Interval arr[], int n, int k, int max_point) {
    int coverage[200];
    int i, j;
    for (i = 0; i <= max_point; i++) coverage[i] = 0;

    for (i = 0; i < n; i++) {
        int s = arr[i].start;
        int e = arr[i].end;
        if (s < 0) s = 0;
        if (e > max_point) e = max_point;
        for (j = s; j < e; j++) {
            coverage[j]++;
        }
    }

    int count = 0;
    for (i = 0; i <= max_point; i++) {
        if (coverage[i] >= k) count++;
    }
    return count;
}

// ---- Maximum depth of overlapping intervals ----
int max_depth(struct Interval arr[], int n, int max_point) {
    int coverage[200];
    int i, j;
    for (i = 0; i <= max_point; i++) coverage[i] = 0;

    for (i = 0; i < n; i++) {
        int s = arr[i].start;
        int e = arr[i].end;
        if (s < 0) s = 0;
        if (e > max_point) e = max_point;
        for (j = s; j < e; j++) {
            coverage[j]++;
        }
    }

    int maxd = 0;
    for (i = 0; i <= max_point; i++) {
        if (coverage[i] > maxd) maxd = coverage[i];
    }
    return maxd;
}

// ---- Interval contains point ----
int interval_contains_point(struct Interval iv, int point) {
    return (point >= iv.start && point < iv.end) ? 1 : 0;
}

// ---- Count intervals containing a point ----
int count_containing(struct Interval arr[], int n, int point) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (interval_contains_point(arr[i], point)) count++;
    }
    return count;
}

// ---- Interval length ----
int interval_length(struct Interval iv) {
    int len = iv.end - iv.start;
    return (len > 0) ? len : 0;
}

// ---- Symmetric difference of two intervals ----
// Returns length of symmetric difference
int symmetric_diff_length(struct Interval a, struct Interval b) {
    struct Interval u = interval_union(a, b);
    struct Interval inter;
    int has_inter = interval_intersection(a, b, &inter);
    int union_len = interval_length(u);
    int inter_len = has_inter ? interval_length(inter) : 0;
    // Sym diff = union - intersection
    // Actually: sym diff = |a| + |b| - 2*|intersection|
    return interval_length(a) + interval_length(b) - 2 * inter_len;
}


int main() {
    // === Merge intervals test 1 ===
    printf("=== Merge intervals test 1 ===\n");
    {
        struct Interval arr[6];
        arr[0].start = 1; arr[0].end = 3;
        arr[1].start = 2; arr[1].end = 6;
        arr[2].start = 8; arr[2].end = 10;
        arr[3].start = 15; arr[3].end = 18;
        arr[4].start = 17; arr[4].end = 20;
        arr[5].start = 9; arr[5].end = 12;

        struct Interval merged[10];
        int m = merge_intervals(arr, 6, merged);
        printf("Merged count: %d\n", m);
        int i;
        for (i = 0; i < m; i++) {
            printf("[%d,%d]", merged[i].start, merged[i].end);
            if (i < m - 1) printf(" ");
        }
        printf("\n");
    }

    // === Merge intervals test 2 ===
    printf("=== Merge intervals test 2 ===\n");
    {
        struct Interval arr[4];
        arr[0].start = 0; arr[0].end = 4;
        arr[1].start = 3; arr[1].end = 7;
        arr[2].start = 5; arr[2].end = 10;
        arr[3].start = 1; arr[3].end = 6;

        struct Interval merged[10];
        int m = merge_intervals(arr, 4, merged);
        printf("Merged count: %d\n", m);
        int i;
        for (i = 0; i < m; i++) {
            printf("[%d,%d]", merged[i].start, merged[i].end);
            if (i < m - 1) printf(" ");
        }
        printf("\n");
    }

    // === Overlap detection ===
    printf("=== Overlap detection ===\n");
    {
        struct Interval a, b;
        a.start = 1; a.end = 5; b.start = 3; b.end = 7;
        printf("[1,5] & [3,7]: overlap=%d\n", intervals_overlap(a, b));
        a.start = 1; a.end = 3; b.start = 5; b.end = 7;
        printf("[1,3] & [5,7]: overlap=%d\n", intervals_overlap(a, b));
        a.start = 1; a.end = 5; b.start = 5; b.end = 7;
        printf("[1,5] & [5,7]: overlap=%d\n", intervals_overlap(a, b));
        a.start = 1; a.end = 6; b.start = 4; b.end = 8;
        printf("[1,6] & [4,8]: overlap=%d\n", intervals_overlap(a, b));
    }

    // === Intersection ===
    printf("=== Intersection ===\n");
    {
        struct Interval a, b, result;
        a.start = 1; a.end = 5; b.start = 3; b.end = 7;
        if (interval_intersection(a, b, &result)) {
            printf("[1,5] & [3,7]: [%d,%d]\n", result.start, result.end);
        }
        a.start = 0; a.end = 10; b.start = 5; b.end = 15;
        if (interval_intersection(a, b, &result)) {
            printf("[0,10] & [5,15]: [%d,%d]\n", result.start, result.end);
        }
        a.start = 1; a.end = 3; b.start = 5; b.end = 7;
        if (!interval_intersection(a, b, &result)) {
            printf("[1,3] & [5,7]: no intersection\n");
        }
    }

    // === List intersection ===
    printf("=== List intersection ===\n");
    {
        struct Interval A[3], B[3], result[10];
        A[0].start = 1; A[0].end = 7;
        A[1].start = 9; A[1].end = 12;
        A[2].start = 15; A[2].end = 20;
        B[0].start = 5; B[0].end = 10;
        B[1].start = 14; B[1].end = 16;

        int cnt = intersect_lists(A, 3, B, 2, result);
        printf("List intersect count: %d\n", cnt);
        int i;
        for (i = 0; i < cnt; i++) {
            printf("[%d,%d]", result[i].start, result[i].end);
            if (i < cnt - 1) printf(" ");
        }
        printf("\n");
    }

    // === Max non-overlapping ===
    printf("=== Max non-overlapping ===\n");
    {
        struct Interval arr[5];
        arr[0].start = 1; arr[0].end = 3;
        arr[1].start = 2; arr[1].end = 5;
        arr[2].start = 4; arr[2].end = 7;
        arr[3].start = 6; arr[3].end = 9;
        arr[4].start = 8; arr[4].end = 10;
        printf("Max non-overlap: %d\n", max_non_overlapping(arr, 5));
    }

    // === Min rooms ===
    printf("=== Min rooms ===\n");
    {
        struct Interval arr[5];
        arr[0].start = 0; arr[0].end = 30;
        arr[1].start = 5; arr[1].end = 10;
        arr[2].start = 15; arr[2].end = 20;
        arr[3].start = 7; arr[3].end = 25;
        arr[4].start = 3; arr[4].end = 8;
        printf("Min rooms: %d\n", min_rooms(arr, 5));
    }

    // === Total covered length ===
    printf("=== Total covered length ===\n");
    {
        struct Interval arr[4];
        arr[0].start = 1; arr[0].end = 5;
        arr[1].start = 3; arr[1].end = 7;
        arr[2].start = 10; arr[2].end = 15;
        arr[3].start = 13; arr[3].end = 17;
        printf("Covered: %d\n", total_covered_length(arr, 4));
    }

    // === Count overlaps ===
    printf("=== Count overlaps ===\n");
    {
        struct Interval arr[4];
        arr[0].start = 1; arr[0].end = 5;
        arr[1].start = 3; arr[1].end = 7;
        arr[2].start = 4; arr[2].end = 9;
        arr[3].start = 8; arr[3].end = 12;
        printf("Pairwise overlaps: %d\n", count_overlaps(arr, 4));
    }

    // === Containment ===
    printf("=== Containment ===\n");
    {
        struct Interval a, b;
        a.start = 3; a.end = 5; b.start = 1; b.end = 7;
        printf("[3,5] in [1,7]: %d\n", is_contained(a, b));
        a.start = 1; a.end = 7; b.start = 3; b.end = 5;
        printf("[1,7] in [3,5]: %d\n", is_contained(a, b));
        a.start = 2; a.end = 6; b.start = 2; b.end = 6;
        printf("[2,6] in [2,6]: %d\n", is_contained(a, b));
    }

    // === Remove interval ===
    printf("=== Remove interval ===\n");
    {
        struct Interval orig, rem, result[2];
        orig.start = 1; orig.end = 10;
        rem.start = 3; rem.end = 7;
        int cnt = remove_interval(orig, rem, result);
        printf("[1,10] minus [3,7]: count=%d\n", cnt);
        int i;
        for (i = 0; i < cnt; i++) {
            printf("[%d,%d]", result[i].start, result[i].end);
            if (i < cnt - 1) printf(" ");
        }
        printf("\n");

        orig.start = 1; orig.end = 10;
        rem.start = 0; rem.end = 5;
        cnt = remove_interval(orig, rem, result);
        printf("[1,10] minus [0,5]: count=%d\n", cnt);
        for (i = 0; i < cnt; i++) {
            printf("[%d,%d]", result[i].start, result[i].end);
            if (i < cnt - 1) printf(" ");
        }
        printf("\n");
    }

    // === Gap analysis ===
    printf("=== Gap analysis ===\n");
    {
        struct Interval arr[3], gaps[10];
        arr[0].start = 1; arr[0].end = 6;
        arr[1].start = 8; arr[1].end = 12;
        arr[2].start = 3; arr[2].end = 5;
        int cnt = find_gaps(arr, 3, gaps, 0, 20);
        printf("Gaps count: %d\n", cnt);
        int i;
        for (i = 0; i < cnt; i++) {
            printf("[%d,%d]", gaps[i].start, gaps[i].end);
            if (i < cnt - 1) printf(" ");
        }
        printf("\n");
    }

    // === Weighted scheduling ===
    printf("=== Weighted scheduling ===\n");
    {
        struct Interval arr[4];
        int weights[4];
        arr[0].start = 1; arr[0].end = 4; weights[0] = 5;
        arr[1].start = 3; arr[1].end = 5; weights[1] = 1;
        arr[2].start = 0; arr[2].end = 6; weights[2] = 8;
        arr[3].start = 5; arr[3].end = 7; weights[3] = 4;
        printf("Weighted max: %d\n", weighted_scheduling(arr, weights, 4));
    }

    // === Interval coloring ===
    printf("=== Interval coloring ===\n");
    {
        struct Interval arr[5];
        arr[0].start = 0; arr[0].end = 30;
        arr[1].start = 5; arr[1].end = 10;
        arr[2].start = 15; arr[2].end = 20;
        arr[3].start = 7; arr[3].end = 25;
        arr[4].start = 3; arr[4].end = 8;
        printf("Colors needed: %d\n", interval_coloring(arr, 5));
    }

    // === Union ===
    printf("=== Union ===\n");
    {
        struct Interval a, b;
        a.start = 1; a.end = 5;
        b.start = 3; b.end = 7;
        struct Interval u = interval_union(a, b);
        printf("[1,5] U [3,7] = [%d,%d]\n", u.start, u.end);
    }

    // === Complement ===
    printf("=== Complement ===\n");
    {
        struct Interval arr[3], result[10];
        arr[0].start = 1; arr[0].end = 6;
        arr[1].start = 8; arr[1].end = 12;
        arr[2].start = 3; arr[2].end = 5;
        int cnt = complement_intervals(arr, 3, 0, 20, result);
        printf("Complement count: %d\n", cnt);
    }

    // === Interval diff ===
    printf("=== Interval diff ===\n");
    {
        struct Interval a, b, result[2];
        a.start = 1; a.end = 10;
        b.start = 4; b.end = 6;
        int cnt = interval_diff(a, b, result);
        printf("[1,10] - [4,6]: count=%d\n", cnt);
        int i;
        for (i = 0; i < cnt; i++) {
            printf("[%d,%d]", result[i].start, result[i].end);
            if (i < cnt - 1) printf(" ");
        }
        printf("\n");
    }

    // === K-covered ===
    printf("=== K-covered ===\n");
    {
        struct Interval arr[3];
        arr[0].start = 1; arr[0].end = 6;
        arr[1].start = 3; arr[1].end = 8;
        arr[2].start = 5; arr[2].end = 10;
        printf("2-covered points: %d\n", count_k_covered(arr, 3, 2, 15));
    }

    // === Max depth ===
    printf("=== Max depth ===\n");
    {
        struct Interval arr[3];
        arr[0].start = 1; arr[0].end = 6;
        arr[1].start = 3; arr[1].end = 8;
        arr[2].start = 5; arr[2].end = 10;
        printf("Max depth: %d\n", max_depth(arr, 3, 15));
    }

    // === Contains point ===
    printf("=== Contains point ===\n");
    {
        struct Interval iv;
        iv.start = 2; iv.end = 8;
        printf("[2,8] contains 5: %d\n", interval_contains_point(iv, 5));
        printf("[2,8] contains 8: %d\n", interval_contains_point(iv, 8));

        struct Interval arr[4];
        arr[0].start = 1; arr[0].end = 6;
        arr[1].start = 3; arr[1].end = 8;
        arr[2].start = 5; arr[2].end = 10;
        arr[3].start = 12; arr[3].end = 15;
        printf("Count containing 5: %d\n", count_containing(arr, 4, 5));
    }

    // === Symmetric difference ===
    printf("=== Symmetric diff ===\n");
    {
        struct Interval a, b;
        a.start = 1; a.end = 5; b.start = 3; b.end = 7;
        printf("Sym diff [1,5],[3,7]: %d\n", symmetric_diff_length(a, b));
        a.start = 1; a.end = 3; b.start = 5; b.end = 7;
        printf("Sym diff [1,3],[5,7]: %d\n", symmetric_diff_length(a, b));
    }

    printf("All interval tests passed!\n");
    return 0;
}
