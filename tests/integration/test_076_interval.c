int printf(const char *fmt, ...);
// EXPECT: Merge Intervals\nMerged=3\n[1,6]\n[8,12]\n[15,20]\nMerged2=3\n[1,8]\n[10,15]\n[20,25]\nIntersection\nInterCnt4\n[1,4]\n[5,5]\n[8,10]\n[15,20]\nActivity Selection\nSelected=4\nOverlap Counts\nOverlapPrs5\nMaxOverlap3\nMinRooms=3\nCoverage=11\nGaps=0\nPoint 3 covered=1\nPoint 6 covered=1\nPoint 13 covered=0\nTotalLen18\nAvgLx1036\nMaxLen=4\nMinLen=3\nSpan=11\nDensityPc=100\nDisjoint=0\nContained 0-12=5\nStabbing 3=2\nStabbing 5=2\nStabbing 10=1\nWeightSch=10\nDisjTest=1\nDisjMerge=1\nAll  passed=1

// Interval operations: merging, intersection, scheduling, overlap detection

// Interval representation: parallel arrays for start and end
int starts[50];
int ends[50];
int interval_count;

// Merged intervals output
int merged_starts[50];
int merged_ends[50];
int merged_count;

// Intersection results
int inter_starts[50];
int inter_ends[50];
int inter_count;

// Helper: swap two elements
void swap_int(int arr[50], int i, int j) {
    int t;
    t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
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

// Sort intervals by start time (bubble sort)
void sort_intervals(int s[50], int e[50], int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (s[j] > s[j + 1] || (s[j] == s[j + 1] && e[j] > e[j + 1])) {
                swap_int(s, j, j + 1);
                swap_int(e, j, j + 1);
            }
        }
    }
}

// Sort by end time
void sort_by_end(int s[50], int e[50], int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (e[j] > e[j + 1] || (e[j] == e[j + 1] && s[j] > s[j + 1])) {
                swap_int(s, j, j + 1);
                swap_int(e, j, j + 1);
            }
        }
    }
}

// Merge overlapping intervals
void merge_intervals(int s[50], int e[50], int n) {
    int i;
    // Sort first
    sort_intervals(s, e, n);

    merged_count = 0;
    if (n == 0) {
        return;
    }

    merged_starts[0] = s[0];
    merged_ends[0] = e[0];
    merged_count = 1;

    for (i = 1; i < n; i = i + 1) {
        if (s[i] <= merged_ends[merged_count - 1]) {
            // Overlapping, merge
            if (e[i] > merged_ends[merged_count - 1]) {
                merged_ends[merged_count - 1] = e[i];
            }
        } else {
            // No overlap, add new
            merged_starts[merged_count] = s[i];
            merged_ends[merged_count] = e[i];
            merged_count = merged_count + 1;
        }
    }
}

// Interval intersection of two sorted interval lists
void interval_intersection(int s1[50], int e1[50], int n1,
                           int s2[50], int e2[50], int n2) {
    int i;
    int j;
    int lo;
    int hi;

    inter_count = 0;
    i = 0;
    j = 0;

    while (i < n1 && j < n2) {
        lo = max_int(s1[i], s2[j]);
        hi = min_int(e1[i], e2[j]);

        if (lo <= hi) {
            inter_starts[inter_count] = lo;
            inter_ends[inter_count] = hi;
            inter_count = inter_count + 1;
        }

        if (e1[i] < e2[j]) {
            i = i + 1;
        } else {
            j = j + 1;
        }
    }
}

// Activity selection (greedy: select maximum non-overlapping intervals)
int selected[50];
int selected_count;

void activity_selection(int s[50], int e[50], int n) {
    int i;
    int last_end;

    // Sort by end time
    sort_by_end(s, e, n);

    selected_count = 0;
    if (n == 0) {
        return;
    }

    selected[0] = 0;
    selected_count = 1;
    last_end = e[0];

    for (i = 1; i < n; i = i + 1) {
        if (s[i] >= last_end) {
            selected[selected_count] = i;
            selected_count = selected_count + 1;
            last_end = e[i];
        }
    }
}

// Check if two intervals overlap
int intervals_overlap(int s1, int e1, int s2, int e2) {
    if (s1 >= e2 || s2 >= e1) {
        return 0;
    }
    return 1;
}

// Count overlapping pairs
int count_overlapping_pairs(int s[50], int e[50], int n) {
    int count;
    int i;
    int j;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (intervals_overlap(s[i], e[i], s[j], e[j])) {
                count = count + 1;
            }
        }
    }
    return count;
}

// Maximum number of overlapping intervals at any point
int max_overlap(int s[50], int e[50], int n) {
    int events[100]; // +1 for start, -1 for end
    int event_times[100];
    int event_count;
    int i;
    int j;
    int cur;
    int mx;

    event_count = 0;
    for (i = 0; i < n; i = i + 1) {
        event_times[event_count] = s[i];
        events[event_count] = 1;
        event_count = event_count + 1;
        event_times[event_count] = e[i];
        events[event_count] = -1;
        event_count = event_count + 1;
    }

    // Sort events by time (bubble sort), start before end at same time
    for (i = 0; i < event_count - 1; i = i + 1) {
        for (j = 0; j < event_count - 1 - i; j = j + 1) {
            if (event_times[j] > event_times[j + 1] ||
                (event_times[j] == event_times[j + 1] && events[j] < events[j + 1])) {
                int tt;
                int te;
                tt = event_times[j];
                event_times[j] = event_times[j + 1];
                event_times[j + 1] = tt;
                te = events[j];
                events[j] = events[j + 1];
                events[j + 1] = te;
            }
        }
    }

    cur = 0;
    mx = 0;
    for (i = 0; i < event_count; i = i + 1) {
        cur = cur + events[i];
        if (cur > mx) {
            mx = cur;
        }
    }
    return mx;
}

// Minimum number of rooms/resources needed (= max overlap)
int min_rooms(int s[50], int e[50], int n) {
    return max_overlap(s, e, n);
}

// Total coverage: sum of lengths of merged intervals
int total_coverage(int s[50], int e[50], int n) {
    int total;
    int i;
    merge_intervals(s, e, n);
    total = 0;
    for (i = 0; i < merged_count; i = i + 1) {
        total = total + (merged_ends[i] - merged_starts[i]);
    }
    return total;
}

// Gap analysis: find gaps between merged intervals
int gap_starts[50];
int gap_ends[50];
int gap_count;

void find_gaps(int s[50], int e[50], int n) {
    int i;
    merge_intervals(s, e, n);
    gap_count = 0;
    for (i = 0; i < merged_count - 1; i = i + 1) {
        if (merged_ends[i] < merged_starts[i + 1]) {
            gap_starts[gap_count] = merged_ends[i];
            gap_ends[gap_count] = merged_starts[i + 1];
            gap_count = gap_count + 1;
        }
    }
}

// Check if point is covered by any interval
int point_covered(int s[50], int e[50], int n, int point) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (point >= s[i] && point < e[i]) {
            return 1;
        }
    }
    return 0;
}

// Interval containing a specific point
int interval_containing(int s[50], int e[50], int n, int point) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (point >= s[i] && point < e[i]) {
            return i;
        }
    }
    return -1;
}

// Weighted interval scheduling (simple version with weights = length)
int weighted_schedule(int s[50], int e[50], int n) {
    int dp[50];
    int i;
    int j;
    int mx;
    int best;

    sort_by_end(s, e, n);

    for (i = 0; i < n; i = i + 1) {
        dp[i] = e[i] - s[i]; // weight = length
    }

    for (i = 1; i < n; i = i + 1) {
        for (j = 0; j < i; j = j + 1) {
            if (e[j] <= s[i]) {
                if (dp[j] + (e[i] - s[i]) > dp[i]) {
                    dp[i] = dp[j] + (e[i] - s[i]);
                }
            }
        }
    }

    mx = 0;
    for (i = 0; i < n; i = i + 1) {
        if (dp[i] > mx) {
            mx = dp[i];
        }
    }
    return mx;
}

// Interval length statistics
int total_length(int s[50], int e[50], int n) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < n; i = i + 1) {
        total = total + (e[i] - s[i]);
    }
    return total;
}

int avg_length_x10(int s[50], int e[50], int n) {
    if (n == 0) return 0;
    return (total_length(s, e, n) * 10) / n;
}

int max_length(int s[50], int e[50], int n) {
    int mx;
    int i;
    int len;
    mx = 0;
    for (i = 0; i < n; i = i + 1) {
        len = e[i] - s[i];
        if (len > mx) {
            mx = len;
        }
    }
    return mx;
}

int min_length(int s[50], int e[50], int n) {
    int mn;
    int i;
    int len;
    if (n == 0) return 0;
    mn = e[0] - s[0];
    for (i = 1; i < n; i = i + 1) {
        len = e[i] - s[i];
        if (len < mn) {
            mn = len;
        }
    }
    return mn;
}

// Find the interval with maximum length
int max_length_idx(int s[50], int e[50], int n) {
    int mx;
    int idx;
    int i;
    int len;
    mx = 0;
    idx = 0;
    for (i = 0; i < n; i = i + 1) {
        len = e[i] - s[i];
        if (len > mx) {
            mx = len;
            idx = i;
        }
    }
    return idx;
}

// Check if intervals are pairwise disjoint
int are_disjoint(int s[50], int e[50], int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (intervals_overlap(s[i], e[i], s[j], e[j])) {
                return 0;
            }
        }
    }
    return 1;
}

// Interval span (from earliest start to latest end)
int span(int s[50], int e[50], int n) {
    int min_s;
    int max_e;
    int i;
    if (n == 0) return 0;
    min_s = s[0];
    max_e = e[0];
    for (i = 1; i < n; i = i + 1) {
        if (s[i] < min_s) min_s = s[i];
        if (e[i] > max_e) max_e = e[i];
    }
    return max_e - min_s;
}

// Density: total_coverage / span (x100 for percentage)
int density_x100(int s[50], int e[50], int n) {
    int sp;
    int cov;
    sp = span(s, e, n);
    if (sp == 0) return 0;
    cov = total_coverage(s, e, n);
    return (cov * 100) / sp;
}

// Count intervals fully contained in [lo, hi]
int count_contained(int s[50], int e[50], int n, int lo, int hi) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        if (s[i] >= lo && e[i] <= hi) {
            count = count + 1;
        }
    }
    return count;
}

// Stabbing number: how many intervals contain a given point
int stabbing_number(int s[50], int e[50], int n, int point) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        if (point >= s[i] && point < e[i]) {
            count = count + 1;
        }
    }
    return count;
}

int main(void) {
    int s1[50];
    int e1[50];
    int s2[50];
    int e2[50];
    int i;
    int n;

    // Test set 1: merge intervals
    printf("%c%c%c%c%c %c%c%c%c%c%c%c%c%c", 'M','e','r','g','e', 'I','n','t','e','r','v','a','l','s');
    printf("%c", 10);

    s1[0] = 1;  e1[0] = 3;
    s1[1] = 2;  e1[1] = 6;
    s1[2] = 8;  e1[2] = 10;
    s1[3] = 15; e1[3] = 18;
    s1[4] = 9;  e1[4] = 12;
    s1[5] = 17; e1[5] = 20;
    n = 6;

    merge_intervals(s1, e1, n);
    printf("%c%c%c%c%c%c%c%d", 'M','e','r','g','e','d','=', merged_count);
    printf("%c", 10);
    for (i = 0; i < merged_count; i = i + 1) {
        printf("[%d,%d]", merged_starts[i], merged_ends[i]);
        printf("%c", 10);
    }

    // Test set 2: more intervals
    s1[0] = 1;  e1[0] = 5;
    s1[1] = 3;  e1[1] = 7;
    s1[2] = 4;  e1[2] = 6;
    s1[3] = 6;  e1[3] = 8;
    s1[4] = 10; e1[4] = 12;
    s1[5] = 11; e1[5] = 15;
    s1[6] = 20; e1[6] = 25;
    n = 7;

    merge_intervals(s1, e1, n);
    printf("%c%c%c%c%c%c%c%c%d", 'M','e','r','g','e','d','2','=', merged_count);
    printf("%c", 10);
    for (i = 0; i < merged_count; i = i + 1) {
        printf("[%d,%d]", merged_starts[i], merged_ends[i]);
        printf("%c", 10);
    }

    // Intersection test
    printf("%c%c%c%c%c%c%c%c%c%c%c%c", 'I','n','t','e','r','s','e','c','t','i','o','n');
    printf("%c", 10);

    s1[0] = 0;  e1[0] = 4;
    s1[1] = 5;  e1[1] = 10;
    s1[2] = 13; e1[2] = 20;

    s2[0] = 1;  e2[0] = 5;
    s2[1] = 8;  e2[1] = 12;
    s2[2] = 15; e2[2] = 24;

    interval_intersection(s1, e1, 3, s2, e2, 3);
    printf("%c%c%c%c%c%c%c%c%d", 'I','n','t','e','r','C','n','t', inter_count);
    printf("%c", 10);
    for (i = 0; i < inter_count; i = i + 1) {
        printf("[%d,%d]", inter_starts[i], inter_ends[i]);
        printf("%c", 10);
    }

    // Activity selection test
    printf("%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c", 'A','c','t','i','v','i','t','y', 'S','e','l','e','c','t','i','o','n');
    printf("%c", 10);

    s1[0] = 1; e1[0] = 4;
    s1[1] = 3; e1[1] = 5;
    s1[2] = 0; e1[2] = 6;
    s1[3] = 5; e1[3] = 7;
    s1[4] = 3; e1[4] = 9;
    s1[5] = 5; e1[5] = 9;
    s1[6] = 6; e1[6] = 10;
    s1[7] = 8; e1[7] = 11;
    s1[8] = 8; e1[8] = 12;
    s1[9] = 2; e1[9] = 14;
    s1[10] = 12; e1[10] = 16;
    n = 11;

    activity_selection(s1, e1, n);
    printf("%c%c%c%c%c%c%c%c%c%d", 'S','e','l','e','c','t','e','d','=', selected_count);
    printf("%c", 10);

    // Overlap counting
    printf("%c%c%c%c%c%c%c %c%c%c%c%c%c", 'O','v','e','r','l','a','p', 'C','o','u','n','t','s');
    printf("%c", 10);

    s1[0] = 1; e1[0] = 5;
    s1[1] = 2; e1[1] = 6;
    s1[2] = 4; e1[2] = 8;
    s1[3] = 7; e1[3] = 10;
    s1[4] = 9; e1[4] = 12;
    n = 5;

    printf("%c%c%c%c%c%c%c%c%c%c%d", 'O','v','e','r','l','a','p','P','r','s', count_overlapping_pairs(s1, e1, n));
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%c%d", 'M','a','x','O','v','e','r','l','a','p', max_overlap(s1, e1, n));
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%c%d", 'M','i','n','R','o','o','m','s','=', min_rooms(s1, e1, n));
    printf("%c", 10);

    // Coverage and gaps
    printf("%c%c%c%c%c%c%c%c%c%d", 'C','o','v','e','r','a','g','e','=', total_coverage(s1, e1, n));
    printf("%c", 10);

    find_gaps(s1, e1, n);
    printf("%c%c%c%c%c%d", 'G','a','p','s','=', gap_count);
    printf("%c", 10);

    // Point coverage
    printf("%c%c%c%c%c %d %c%c%c%c%c%c%c%c%d", 'P','o','i','n','t', 3, 'c','o','v','e','r','e','d','=', point_covered(s1, e1, n, 3));
    printf("%c", 10);
    printf("%c%c%c%c%c %d %c%c%c%c%c%c%c%c%d", 'P','o','i','n','t', 6, 'c','o','v','e','r','e','d','=', point_covered(s1, e1, n, 6));
    printf("%c", 10);
    printf("%c%c%c%c%c %d %c%c%c%c%c%c%c%c%d", 'P','o','i','n','t', 13, 'c','o','v','e','r','e','d','=', point_covered(s1, e1, n, 13));
    printf("%c", 10);

    // Length statistics
    printf("%c%c%c%c%c%c%c%c%d", 'T','o','t','a','l','L','e','n', total_length(s1, e1, n));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%d", 'A','v','g','L','x','1','0', avg_length_x10(s1, e1, n));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%d", 'M','a','x','L','e','n','=', max_length(s1, e1, n));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%d", 'M','i','n','L','e','n','=', min_length(s1, e1, n));
    printf("%c", 10);

    // Span
    printf("%c%c%c%c%c%d", 'S','p','a','n','=', span(s1, e1, n));
    printf("%c", 10);

    // Density
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'D','e','n','s','i','t','y','P','c','=', density_x100(s1, e1, n));
    printf("%c", 10);

    // Disjoint check
    printf("%c%c%c%c%c%c%c%c%c%d", 'D','i','s','j','o','i','n','t','=', are_disjoint(s1, e1, n));
    printf("%c", 10);

    // Contained count
    printf("%c%c%c%c%c%c%c%c%c %d%c%d%c%d", 'C','o','n','t','a','i','n','e','d', 0, '-', 12, '=', count_contained(s1, e1, n, 0, 12));
    printf("%c", 10);

    // Stabbing numbers
    printf("%c%c%c%c%c%c%c%c %d%c%d", 'S','t','a','b','b','i','n','g', 3, '=', stabbing_number(s1, e1, n, 3));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c %d%c%d", 'S','t','a','b','b','i','n','g', 5, '=', stabbing_number(s1, e1, n, 5));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c %d%c%d", 'S','t','a','b','b','i','n','g', 10, '=', stabbing_number(s1, e1, n, 10));
    printf("%c", 10);

    // Weighted scheduling
    s1[0] = 1; e1[0] = 4;
    s1[1] = 3; e1[1] = 5;
    s1[2] = 5; e1[2] = 9;
    s1[3] = 4; e1[3] = 7;
    s1[4] = 8; e1[4] = 12;
    n = 5;
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'W','e','i','g','h','t','S','c','h','=', weighted_schedule(s1, e1, n));
    printf("%c", 10);

    // Test with non-overlapping set
    s1[0] = 1; e1[0] = 3;
    s1[1] = 4; e1[1] = 6;
    s1[2] = 7; e1[2] = 9;
    n = 3;
    printf("%c%c%c%c%c%c%c%c%c%d", 'D','i','s','j','T','e','s','t','=', are_disjoint(s1, e1, n));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'D','i','s','j','M','e','r','g','e','=', merged_count);
    printf("%c", 10);

    printf("%c%c%c%c %c%c%c%c%c%c%c%d", 'A','l','l',' ', 'p','a','s','s','e','d','=', 1);
    printf("%c", 10);

    return 0;
}
