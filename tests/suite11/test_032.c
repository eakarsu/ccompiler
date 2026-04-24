int printf(const char *fmt, ...);

// Interval Scheduling Maximization
// Select maximum number of non-overlapping intervals

struct Interval {
    int start;
    int end;
    int id;
};

void sort_intervals(struct Interval *arr, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j].end > arr[j + 1].end) {
                struct Interval tmp;
                tmp.start = arr[j].start;
                tmp.end = arr[j].end;
                tmp.id = arr[j].id;
                arr[j].start = arr[j + 1].start;
                arr[j].end = arr[j + 1].end;
                arr[j].id = arr[j + 1].id;
                arr[j + 1].start = tmp.start;
                arr[j + 1].end = tmp.end;
                arr[j + 1].id = tmp.id;
            }
        }
    }
}

int greedy_interval_schedule(struct Interval *arr, int n, int *selected) {
    int count = 0;
    int last_end = -1;
    int i;
    sort_intervals(arr, n);
    for (i = 0; i < n; i++) {
        if (arr[i].start >= last_end) {
            selected[count] = arr[i].id;
            count = count + 1;
            last_end = arr[i].end;
        }
    }
    return count;
}

int check_no_overlap(struct Interval *arr, int *sel, int sel_count, int n) {
    int i;
    int j;
    for (i = 0; i < sel_count; i++) {
        for (j = i + 1; j < sel_count; j++) {
            int a = -1;
            int b = -1;
            int k;
            for (k = 0; k < n; k++) {
                if (arr[k].id == sel[i]) a = k;
                if (arr[k].id == sel[j]) b = k;
            }
            if (a >= 0 && b >= 0) {
                if (arr[a].start < arr[b].end && arr[b].start < arr[a].end) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int max_weight_intervals(struct Interval *arr, int n, int *weights) {
    int dp[20];
    int i;
    int j;
    sort_intervals(arr, n);
    for (i = 0; i < n; i++) {
        dp[i] = weights[arr[i].id];
        for (j = 0; j < i; j++) {
            if (arr[j].end <= arr[i].start) {
                int candidate = dp[j] + weights[arr[i].id];
                if (candidate > dp[i]) {
                    dp[i] = candidate;
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

int main() {
    struct Interval intervals[6];
    int selected[6];
    int i;

    // EXPECT: === Interval Scheduling ===
    printf("=== Interval Scheduling ===\n");

    intervals[0].start = 1; intervals[0].end = 4; intervals[0].id = 0;
    intervals[1].start = 3; intervals[1].end = 5; intervals[1].id = 1;
    intervals[2].start = 0; intervals[2].end = 6; intervals[2].id = 2;
    intervals[3].start = 5; intervals[3].end = 7; intervals[3].id = 3;
    intervals[4].start = 3; intervals[4].end = 9; intervals[4].id = 4;
    intervals[5].start = 5; intervals[5].end = 9; intervals[5].id = 5;

    int count = greedy_interval_schedule(intervals, 6, selected);
    // EXPECT: Max non-overlapping intervals: 2
    printf("Max non-overlapping intervals: %d\n", count);

    int valid = check_no_overlap(intervals, selected, count, 6);
    // EXPECT: No overlap verified: 1
    printf("No overlap verified: %d\n", valid);

    // EXPECT: Selected interval IDs:
    printf("Selected interval IDs:\n");
    for (i = 0; i < count; i++) {
        printf("  id=%d\n", selected[i]);
    }
    // EXPECT:   id=0
    // EXPECT:   id=3

    struct Interval intervals2[4];
    intervals2[0].start = 0; intervals2[0].end = 2; intervals2[0].id = 0;
    intervals2[1].start = 1; intervals2[1].end = 3; intervals2[1].id = 1;
    intervals2[2].start = 2; intervals2[2].end = 5; intervals2[2].id = 2;
    intervals2[3].start = 4; intervals2[3].end = 6; intervals2[3].id = 3;

    int count2 = greedy_interval_schedule(intervals2, 4, selected);
    // EXPECT: Test 2 count: 2
    printf("Test 2 count: %d\n", count2);

    int weights[4];
    weights[0] = 10; weights[1] = 20; weights[2] = 15; weights[3] = 25;
    struct Interval wint[4];
    wint[0].start = 0; wint[0].end = 2; wint[0].id = 0;
    wint[1].start = 1; wint[1].end = 3; wint[1].id = 1;
    wint[2].start = 2; wint[2].end = 5; wint[2].id = 2;
    wint[3].start = 4; wint[3].end = 6; wint[3].id = 3;

    int mw = max_weight_intervals(wint, 4, weights);
    // EXPECT: Max weight intervals: 45
    printf("Max weight intervals: %d\n", mw);

    struct Interval nonoverlap[3];
    nonoverlap[0].start = 0; nonoverlap[0].end = 1; nonoverlap[0].id = 0;
    nonoverlap[1].start = 2; nonoverlap[1].end = 3; nonoverlap[1].id = 1;
    nonoverlap[2].start = 4; nonoverlap[2].end = 5; nonoverlap[2].id = 2;
    int count3 = greedy_interval_schedule(nonoverlap, 3, selected);
    // EXPECT: All non-overlapping count: 3
    printf("All non-overlapping count: %d\n", count3);

    // EXPECT: All interval scheduling tests passed
    printf("All interval scheduling tests passed\n");
    return 0;
}
