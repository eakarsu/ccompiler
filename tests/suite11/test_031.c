int printf(const char *fmt, ...);

// Weighted Job Scheduling
// Given jobs with start, end, weight - find max weight subset of non-overlapping jobs

struct Job {
    int start;
    int end;
    int weight;
};

void sort_jobs_by_end(struct Job *jobs, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (jobs[j].end > jobs[j + 1].end) {
                struct Job tmp;
                tmp.start = jobs[j].start;
                tmp.end = jobs[j].end;
                tmp.weight = jobs[j].weight;
                jobs[j].start = jobs[j + 1].start;
                jobs[j].end = jobs[j + 1].end;
                jobs[j].weight = jobs[j + 1].weight;
                jobs[j + 1].start = tmp.start;
                jobs[j + 1].end = tmp.end;
                jobs[j + 1].weight = tmp.weight;
            }
        }
    }
}

int find_last_non_conflicting(struct Job *jobs, int idx) {
    int i;
    for (i = idx - 1; i >= 0; i--) {
        if (jobs[i].end <= jobs[idx].start) {
            return i;
        }
    }
    return -1;
}

int weighted_job_schedule(struct Job *jobs, int n) {
    int dp[20];
    int i;
    sort_jobs_by_end(jobs, n);
    dp[0] = jobs[0].weight;
    for (i = 1; i < n; i++) {
        int incl = jobs[i].weight;
        int last = find_last_non_conflicting(jobs, i);
        if (last >= 0) {
            incl = incl + dp[last];
        }
        int excl = dp[i - 1];
        if (incl > excl) {
            dp[i] = incl;
        } else {
            dp[i] = excl;
        }
    }
    return dp[n - 1];
}

void print_sorted_jobs(struct Job *jobs, int n) {
    int i;
    printf("Sorted jobs by end time:\n");
    for (i = 0; i < n; i++) {
        printf("Job: start=%d end=%d weight=%d\n", jobs[i].start, jobs[i].end, jobs[i].weight);
    }
}

int main() {
    struct Job jobs1[4];
    jobs1[0].start = 1; jobs1[0].end = 3; jobs1[0].weight = 5;
    jobs1[1].start = 2; jobs1[1].end = 5; jobs1[1].weight = 6;
    jobs1[2].start = 4; jobs1[2].end = 6; jobs1[2].weight = 5;
    jobs1[3].start = 6; jobs1[3].end = 8; jobs1[3].weight = 4;

    // EXPECT: === Weighted Job Scheduling ===
    printf("=== Weighted Job Scheduling ===\n");

    int result1 = weighted_job_schedule(jobs1, 4);
    // EXPECT: Test 1 max weight: 14
    printf("Test 1 max weight: %d\n", result1);

    struct Job jobs2[5];
    jobs2[0].start = 1; jobs2[0].end = 2; jobs2[0].weight = 50;
    jobs2[1].start = 3; jobs2[1].end = 5; jobs2[1].weight = 20;
    jobs2[2].start = 6; jobs2[2].end = 19; jobs2[2].weight = 100;
    jobs2[3].start = 2; jobs2[3].end = 100; jobs2[3].weight = 200;
    jobs2[4].start = 5; jobs2[4].end = 6; jobs2[4].weight = 30;

    int result2 = weighted_job_schedule(jobs2, 5);
    // EXPECT: Test 2 max weight: 250
    printf("Test 2 max weight: %d\n", result2);

    struct Job jobs3[6];
    jobs3[0].start = 0; jobs3[0].end = 2; jobs3[0].weight = 10;
    jobs3[1].start = 1; jobs3[1].end = 3; jobs3[1].weight = 15;
    jobs3[2].start = 2; jobs3[2].end = 4; jobs3[2].weight = 10;
    jobs3[3].start = 3; jobs3[3].end = 5; jobs3[3].weight = 20;
    jobs3[4].start = 4; jobs3[4].end = 7; jobs3[4].weight = 25;
    jobs3[5].start = 6; jobs3[5].end = 8; jobs3[5].weight = 15;

    int result3 = weighted_job_schedule(jobs3, 6);
    // EXPECT: Test 3 max weight: 50
    printf("Test 3 max weight: %d\n", result3);

    struct Job display[3];
    display[0].start = 1; display[0].end = 4; display[0].weight = 3;
    display[1].start = 2; display[1].end = 3; display[1].weight = 7;
    display[2].start = 5; display[2].end = 8; display[2].weight = 2;
    sort_jobs_by_end(display, 3);
    print_sorted_jobs(display, 3);
    // EXPECT: Sorted jobs by end time:
    // EXPECT: Job: start=2 end=3 weight=7
    // EXPECT: Job: start=1 end=4 weight=3
    // EXPECT: Job: start=5 end=8 weight=2

    // EXPECT: All weighted job scheduling tests passed
    printf("All weighted job scheduling tests passed\n");
    return 0;
}
