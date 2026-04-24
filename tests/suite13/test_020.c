int printf(const char *fmt, ...);

// Shell sort with gap sequence analysis
// Tests multiple gap sequences and compares their performance

struct ShellStats {
    int comparisons;
    int shifts;
    int gap_count;
};

// Shell sort with Shell's original sequence: n/2, n/4, ..., 1
void shell_sort_original(int *arr, int n, struct ShellStats *stats) {
    stats->comparisons = 0;
    stats->shifts = 0;
    stats->gap_count = 0;

    int gap = n / 2;
    while (gap > 0) {
        stats->gap_count = stats->gap_count + 1;
        int i;
        for (i = gap; i < n; i = i + 1) {
            int temp = arr[i];
            int j = i;
            while (j >= gap) {
                stats->comparisons = stats->comparisons + 1;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j = j - gap;
                    stats->shifts = stats->shifts + 1;
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
        gap = gap / 2;
    }
}

// Shell sort with Knuth's sequence: 1, 4, 13, 40, 121, ...
// h = 3*h + 1
void shell_sort_knuth(int *arr, int n, struct ShellStats *stats) {
    stats->comparisons = 0;
    stats->shifts = 0;
    stats->gap_count = 0;

    // Find starting gap
    int gap = 1;
    while (gap < n / 3) {
        gap = 3 * gap + 1;
    }

    while (gap >= 1) {
        stats->gap_count = stats->gap_count + 1;
        int i;
        for (i = gap; i < n; i = i + 1) {
            int temp = arr[i];
            int j = i;
            while (j >= gap) {
                stats->comparisons = stats->comparisons + 1;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j = j - gap;
                    stats->shifts = stats->shifts + 1;
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
        gap = gap / 3;
    }
}

// Shell sort with Hibbard's sequence: 1, 3, 7, 15, 31, ...
// 2^k - 1
void shell_sort_hibbard(int *arr, int n, struct ShellStats *stats) {
    stats->comparisons = 0;
    stats->shifts = 0;
    stats->gap_count = 0;

    // Build gaps: 1, 3, 7, 15, ...
    int gaps[16];
    int num_gaps = 0;
    int g = 1;
    while (g < n) {
        gaps[num_gaps] = g;
        num_gaps = num_gaps + 1;
        g = g * 2 + 1;
    }

    int gi = num_gaps - 1;
    while (gi >= 0) {
        int gap = gaps[gi];
        stats->gap_count = stats->gap_count + 1;
        int i;
        for (i = gap; i < n; i = i + 1) {
            int temp = arr[i];
            int j = i;
            while (j >= gap) {
                stats->comparisons = stats->comparisons + 1;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j = j - gap;
                    stats->shifts = stats->shifts + 1;
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
        gi = gi - 1;
    }
}

void copy_array(int *dst, int *src, int n) {
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

int main() {
    struct ShellStats stats;

    // Test data
    int orig[10];
    orig[0] = 35; orig[1] = 33; orig[2] = 42; orig[3] = 10; orig[4] = 14;
    orig[5] = 19; orig[6] = 27; orig[7] = 44; orig[8] = 26; orig[9] = 31;
    int arr[10];

    // Test 1: Shell's original gap sequence
    copy_array(arr, orig, 10);
    shell_sort_original(arr, 10, &stats);
    printf("shell: %d %d %d %d %d %d %d %d %d %d\n",
           arr[0], arr[1], arr[2], arr[3], arr[4],
           arr[5], arr[6], arr[7], arr[8], arr[9]);
    // EXPECT: shell: 10 14 19 26 27 31 33 35 42 44
    printf("shell gaps: %d comps: %d shifts: %d\n",
           stats.gap_count, stats.comparisons, stats.shifts);
    // EXPECT: shell gaps: 3 comps: 29 shifts: 12

    // Test 2: Knuth gap sequence
    copy_array(arr, orig, 10);
    shell_sort_knuth(arr, 10, &stats);
    printf("knuth sorted: %d\n", is_sorted(arr, 10));
    // EXPECT: knuth sorted: 1
    printf("knuth gaps: %d comps: %d shifts: %d\n",
           stats.gap_count, stats.comparisons, stats.shifts);
    // EXPECT: knuth gaps: 2 comps: 25 shifts: 14

    // Test 3: Hibbard gap sequence
    copy_array(arr, orig, 10);
    shell_sort_hibbard(arr, 10, &stats);
    printf("hibbard sorted: %d\n", is_sorted(arr, 10));
    // EXPECT: hibbard sorted: 1
    printf("hibbard gaps: %d comps: %d shifts: %d\n",
           stats.gap_count, stats.comparisons, stats.shifts);
    // EXPECT: hibbard gaps: 3 comps: 24 shifts: 10

    // Test 4: Already sorted array - best case
    int sorted[8];
    sorted[0] = 1; sorted[1] = 2; sorted[2] = 3; sorted[3] = 4;
    sorted[4] = 5; sorted[5] = 6; sorted[6] = 7; sorted[7] = 8;
    shell_sort_original(sorted, 8, &stats);
    printf("presorted comps: %d shifts: %d\n", stats.comparisons, stats.shifts);
    // EXPECT: presorted comps: 17 shifts: 0

    // Test 5: Reverse sorted - stress test
    int rev[8];
    rev[0] = 8; rev[1] = 7; rev[2] = 6; rev[3] = 5;
    rev[4] = 4; rev[5] = 3; rev[6] = 2; rev[7] = 1;
    shell_sort_original(rev, 8, &stats);
    printf("reverse: %d %d %d %d %d %d %d %d\n",
           rev[0], rev[1], rev[2], rev[3], rev[4], rev[5], rev[6], rev[7]);
    // EXPECT: reverse: 1 2 3 4 5 6 7 8
    printf("rev gaps: %d comps: %d shifts: %d\n",
           stats.gap_count, stats.comparisons, stats.shifts);
    // EXPECT: rev gaps: 3 comps: 22 shifts: 12

    // Test 6: Small arrays
    int s1[2];
    s1[0] = 5; s1[1] = 3;
    shell_sort_original(s1, 2, &stats);
    printf("two elem: %d %d comps: %d\n", s1[0], s1[1], stats.comparisons);
    // EXPECT: two elem: 3 5 comps: 1

    int s2[3];
    s2[0] = 30; s2[1] = 10; s2[2] = 20;
    shell_sort_original(s2, 3, &stats);
    printf("three elem: %d %d %d comps: %d\n", s2[0], s2[1], s2[2], stats.comparisons);
    // EXPECT: three elem: 10 20 30 comps: 3

    // Test 7: Duplicates
    int dup[6];
    dup[0] = 3; dup[1] = 1; dup[2] = 3; dup[3] = 1; dup[4] = 2; dup[5] = 2;
    shell_sort_knuth(dup, 6, &stats);
    printf("dupes: %d %d %d %d %d %d\n",
           dup[0], dup[1], dup[2], dup[3], dup[4], dup[5]);
    // EXPECT: dupes: 1 1 2 2 3 3
    printf("dupes comps: %d shifts: %d\n", stats.comparisons, stats.shifts);
    // EXPECT: dupes comps: 11 shifts: 6

    printf("done\n");
    // EXPECT: done

    return 0;
}
