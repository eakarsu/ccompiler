int printf(const char *fmt, ...);

/* Interval Scheduling
   1. Greedy: maximum number of non-overlapping intervals (unweighted)
   2. Weighted: maximum weight of non-overlapping intervals (DP) */

struct Interval {
    int start;
    int end;
    int weight;
};

struct Interval intervals[16];
int n_intervals;

/* Sort intervals by end time (bubble sort) */
void sort_by_end(void) {
    int i;
    int j;
    struct Interval tmp;
    for (i = 0; i < n_intervals - 1; i++) {
        for (j = 0; j < n_intervals - 1 - i; j++) {
            if (intervals[j].end > intervals[j + 1].end) {
                tmp = intervals[j];
                intervals[j] = intervals[j + 1];
                intervals[j + 1] = tmp;
            }
        }
    }
}

/* Greedy: pick earliest-finishing non-overlapping intervals */
int greedy_schedule(int *selected) {
    int count;
    int last_end;
    int i;

    sort_by_end();

    count = 0;
    last_end = -1;

    for (i = 0; i < n_intervals; i++) {
        if (intervals[i].start >= last_end) {
            selected[count] = i;
            count = count + 1;
            last_end = intervals[i].end;
        }
    }
    return count;
}

/* Find latest interval ending before or at time t */
int find_latest_before(int idx) {
    int i;
    int result = -1;
    for (i = 0; i < idx; i++) {
        if (intervals[i].end <= intervals[idx].start) {
            result = i;
        }
    }
    return result;
}

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

/* Weighted interval scheduling via DP */
int weighted_schedule(void) {
    int dp[16];
    int i;
    int p;
    int val;

    sort_by_end();

    for (i = 0; i < n_intervals; i++) {
        /* Option 1: include interval i */
        val = intervals[i].weight;
        p = find_latest_before(i);
        if (p >= 0) {
            val = val + dp[p];
        }

        /* Option 2: exclude interval i */
        if (i > 0) {
            dp[i] = max(val, dp[i - 1]);
        } else {
            dp[i] = val;
        }
    }
    return dp[n_intervals - 1];
}

void test_greedy_basic(void) {
    int selected[16];
    int count;
    int i;

    /* Intervals: [0,2), [1,3), [2,5), [3,4), [5,7), [6,8) */
    n_intervals = 6;
    intervals[0].start = 0; intervals[0].end = 2; intervals[0].weight = 1;
    intervals[1].start = 1; intervals[1].end = 3; intervals[1].weight = 1;
    intervals[2].start = 2; intervals[2].end = 5; intervals[2].weight = 1;
    intervals[3].start = 3; intervals[3].end = 4; intervals[3].weight = 1;
    intervals[4].start = 5; intervals[4].end = 7; intervals[4].weight = 1;
    intervals[5].start = 6; intervals[5].end = 8; intervals[5].weight = 1;

    count = greedy_schedule(selected);
    printf("=== Greedy Basic ===\n");
    // EXPECT: === Greedy Basic ===
    printf("Selected count: %d\n", count);
    // EXPECT: Selected count: 3
    /* After sort by end: [0,2) [1,3) [3,4) [2,5) [5,7) [6,8)
       Pick [0,2), skip [1,3), pick [3,4), skip [2,5), pick [5,7), skip [6,8)
       Intervals chosen: ends at 2, 4, 7 */
    for (i = 0; i < count; i++) {
        printf("  [%d,%d)\n", intervals[selected[i]].start, intervals[selected[i]].end);
    }
    // EXPECT:   [0,2)
    // EXPECT:   [3,4)
    // EXPECT:   [5,7)
}

void test_greedy_all_overlap(void) {
    int selected[16];
    int count;

    /* All overlapping: [0,10), [1,10), [2,10) */
    n_intervals = 3;
    intervals[0].start = 0; intervals[0].end = 10; intervals[0].weight = 1;
    intervals[1].start = 1; intervals[1].end = 10; intervals[1].weight = 1;
    intervals[2].start = 2; intervals[2].end = 10; intervals[2].weight = 1;

    count = greedy_schedule(selected);
    printf("=== All Overlap ===\n");
    // EXPECT: === All Overlap ===
    printf("Selected count: %d\n", count);
    // EXPECT: Selected count: 1
}

void test_greedy_no_overlap(void) {
    int selected[16];
    int count;

    /* Non-overlapping: [0,1), [2,3), [4,5), [6,7) */
    n_intervals = 4;
    intervals[0].start = 0; intervals[0].end = 1; intervals[0].weight = 1;
    intervals[1].start = 2; intervals[1].end = 3; intervals[1].weight = 1;
    intervals[2].start = 4; intervals[2].end = 5; intervals[2].weight = 1;
    intervals[3].start = 6; intervals[3].end = 7; intervals[3].weight = 1;

    count = greedy_schedule(selected);
    printf("=== No Overlap ===\n");
    // EXPECT: === No Overlap ===
    printf("Selected count: %d\n", count);
    // EXPECT: Selected count: 4
}

void test_weighted(void) {
    int result;

    /* Intervals with weights:
       [0,3) w=2, [1,4) w=4, [3,6) w=4, [4,7) w=7, [6,8) w=2
       After sort by end: same order.

       DP:
       i=0: [0,3) w=2, p=-1 => dp[0] = 2
       i=1: [1,4) w=4, p=-1 => val=4, dp[1] = max(4, dp[0]=2) = 4
       i=2: [3,6) w=4, latest ending<=3 is i=0 ([0,3) end=3, 3<=3 yes)
            val = 4 + dp[0]=2 = 6, dp[2] = max(6, dp[1]=4) = 6
       i=3: [4,7) w=7, latest ending<=4 is i=1 ([1,4) end=4, 4<=4 yes)
            val = 7 + dp[1]=4 = 11, dp[3] = max(11, dp[2]=6) = 11
       i=4: [6,8) w=2, latest ending<=6 is i=2 ([3,6) end=6, 6<=6 yes)
            val = 2 + dp[2]=6 = 8, dp[4] = max(8, dp[3]=11) = 11
    */
    n_intervals = 5;
    intervals[0].start = 0; intervals[0].end = 3; intervals[0].weight = 2;
    intervals[1].start = 1; intervals[1].end = 4; intervals[1].weight = 4;
    intervals[2].start = 3; intervals[2].end = 6; intervals[2].weight = 4;
    intervals[3].start = 4; intervals[3].end = 7; intervals[3].weight = 7;
    intervals[4].start = 6; intervals[4].end = 8; intervals[4].weight = 2;

    result = weighted_schedule();
    printf("=== Weighted Scheduling ===\n");
    // EXPECT: === Weighted Scheduling ===
    printf("Max weight: %d\n", result);
    // EXPECT: Max weight: 11
}

void test_weighted_simple(void) {
    int result;

    /* Two intervals: [0,5) w=10, [5,10) w=5
       Both can be taken: total = 15
       dp[0] = 10, dp[1]: val = 5 + dp[0]=10 = 15, dp[1] = max(15, 10) = 15
    */
    n_intervals = 2;
    intervals[0].start = 0; intervals[0].end = 5; intervals[0].weight = 10;
    intervals[1].start = 5; intervals[1].end = 10; intervals[1].weight = 5;

    result = weighted_schedule();
    printf("=== Weighted Simple ===\n");
    // EXPECT: === Weighted Simple ===
    printf("Max weight: %d\n", result);
    // EXPECT: Max weight: 15
}

int main(void) {
    test_greedy_basic();
    test_greedy_all_overlap();
    test_greedy_no_overlap();
    test_weighted();
    test_weighted_simple();

    printf("All interval scheduling tests passed!\n");
    // EXPECT: All interval scheduling tests passed!
    return 0;
}
