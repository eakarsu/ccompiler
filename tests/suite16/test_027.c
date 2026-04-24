int printf(const char *fmt, ...);

// Test 027: Earliest Deadline First (EDF) Scheduling

struct Job {
    int id;
    int arrival;
    int burst;
    int deadline;
    int remaining;
    int finish;
    int done;
    int missed;
};

struct Timeline {
    int job_id;
    int start;
    int end;
};

int edf_schedule(struct Job jobs[], int n, struct Timeline tl[]) {
    int time;
    int completed;
    int i;
    int best;
    int earliest_dl;
    int tl_count;
    int prev_id;

    for (i = 0; i < n; i++) {
        jobs[i].remaining = jobs[i].burst;
        jobs[i].done = 0;
        jobs[i].missed = 0;
    }

    time = 0;
    completed = 0;
    tl_count = 0;
    prev_id = -1;

    while (completed < n && time < 50) {
        best = -1;
        earliest_dl = 999999;
        for (i = 0; i < n; i++) {
            if (jobs[i].done == 0 && jobs[i].arrival <= time && jobs[i].remaining > 0) {
                if (jobs[i].deadline < earliest_dl) {
                    earliest_dl = jobs[i].deadline;
                    best = i;
                }
            }
        }

        if (best == -1) {
            time = time + 1;
            prev_id = -1;
            continue;
        }

        if (jobs[best].id != prev_id) {
            if (tl_count > 0 && prev_id != -1) {
                tl[tl_count - 1].end = time;
            }
            tl[tl_count].job_id = jobs[best].id;
            tl[tl_count].start = time;
            tl_count = tl_count + 1;
        }

        prev_id = jobs[best].id;
        jobs[best].remaining = jobs[best].remaining - 1;
        time = time + 1;

        if (jobs[best].remaining == 0) {
            jobs[best].finish = time;
            jobs[best].done = 1;
            if (jobs[best].finish > jobs[best].deadline) {
                jobs[best].missed = 1;
            }
            completed = completed + 1;
            tl[tl_count - 1].end = time;
            prev_id = -1;
        }
    }

    // Mark unfinished jobs as missed
    for (i = 0; i < n; i++) {
        if (jobs[i].done == 0) {
            jobs[i].missed = 1;
        }
    }

    return tl_count;
}

int count_missed(struct Job jobs[], int n) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < n; i++) {
        if (jobs[i].missed) {
            count = count + 1;
        }
    }
    return count;
}

int compute_slack(struct Job jobs[], int n) {
    int i;
    int total_slack;
    total_slack = 0;
    for (i = 0; i < n; i++) {
        if (jobs[i].done) {
            total_slack = total_slack + (jobs[i].deadline - jobs[i].finish);
        }
    }
    return total_slack;
}

int main() {
    struct Job jobs[5];
    struct Timeline tl[20];
    int tl_count;
    int i;
    int missed;
    int slack;

    jobs[0].id = 1; jobs[0].arrival = 0; jobs[0].burst = 3; jobs[0].deadline = 7;
    jobs[1].id = 2; jobs[1].arrival = 1; jobs[1].burst = 2; jobs[1].deadline = 5;
    jobs[2].id = 3; jobs[2].arrival = 2; jobs[2].burst = 4; jobs[2].deadline = 10;
    jobs[3].id = 4; jobs[3].arrival = 3; jobs[3].burst = 1; jobs[3].deadline = 6;
    jobs[4].id = 5; jobs[4].arrival = 4; jobs[4].burst = 3; jobs[4].deadline = 15;

    printf("=== EDF Scheduling ===\n");          // EXPECT: === EDF Scheduling ===

    tl_count = edf_schedule(jobs, 5, tl);

    for (i = 0; i < 5; i++) {
        printf("J%d: arr=%d bur=%d dl=%d fin=%d missed=%d\n",
               jobs[i].id, jobs[i].arrival, jobs[i].burst,
               jobs[i].deadline, jobs[i].finish, jobs[i].missed);
    }
    // EXPECT: J1: arr=0 bur=3 dl=7 fin=6 missed=0
    // EXPECT: J2: arr=1 bur=2 dl=5 fin=3 missed=0
    // EXPECT: J3: arr=2 bur=4 dl=10 fin=10 missed=0
    // EXPECT: J4: arr=3 bur=1 dl=6 fin=4 missed=0
    // EXPECT: J5: arr=4 bur=3 dl=15 fin=13 missed=0

    printf("Timeline (%d entries):\n", tl_count); // EXPECT: Timeline (6 entries):
    for (i = 0; i < tl_count; i++) {
        printf("  J%d [%d-%d]\n", tl[i].job_id, tl[i].start, tl[i].end);
    }
    // EXPECT:   J1 [0-1]
    // EXPECT:   J2 [1-3]
    // EXPECT:   J4 [3-4]
    // EXPECT:   J1 [4-6]
    // EXPECT:   J3 [6-10]
    // EXPECT:   J5 [10-13]

    missed = count_missed(jobs, 5);
    printf("Missed deadlines: %d\n", missed);    // EXPECT: Missed deadlines: 0

    slack = compute_slack(jobs, 5);
    printf("Total slack: %d\n", slack);          // EXPECT: Total slack: 7

    printf("Feasible: %s\n", missed == 0 ? "yes" : "no"); // EXPECT: Feasible: yes
    printf("Done\n");                            // EXPECT: Done
    return 0;
}
