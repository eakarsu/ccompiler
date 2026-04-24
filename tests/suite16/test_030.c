int printf(const char *fmt, ...);

// Test 030: Scheduling Algorithm Comparison
// Runs the same workload through FCFS, SJF, and RR, then compares metrics

struct Proc {
    int pid;
    int arrival;
    int burst;
};

struct Result {
    int pid;
    int finish;
    int waiting;
    int turnaround;
};

void init_workload(struct Proc procs[]) {
    procs[0].pid = 1; procs[0].arrival = 0; procs[0].burst = 6;
    procs[1].pid = 2; procs[1].arrival = 1; procs[1].burst = 2;
    procs[2].pid = 3; procs[2].arrival = 2; procs[2].burst = 8;
    procs[3].pid = 4; procs[3].arrival = 3; procs[3].burst = 3;
}

void run_fcfs(struct Proc procs[], int n, struct Result res[]) {
    int time;
    int i;
    time = 0;
    for (i = 0; i < n; i++) {
        if (time < procs[i].arrival) {
            time = procs[i].arrival;
        }
        res[i].pid = procs[i].pid;
        res[i].finish = time + procs[i].burst;
        res[i].waiting = time - procs[i].arrival;
        res[i].turnaround = res[i].finish - procs[i].arrival;
        time = res[i].finish;
    }
}

void run_sjf(struct Proc procs[], int n, struct Result res[]) {
    int time;
    int completed;
    int i;
    int best;
    int min_burst;
    int done[8];

    for (i = 0; i < n; i++) {
        done[i] = 0;
    }

    time = 0;
    completed = 0;
    while (completed < n) {
        best = -1;
        min_burst = 999999;
        for (i = 0; i < n; i++) {
            if (done[i] == 0 && procs[i].arrival <= time) {
                if (procs[i].burst < min_burst) {
                    min_burst = procs[i].burst;
                    best = i;
                }
            }
        }
        if (best == -1) {
            time = time + 1;
            continue;
        }
        res[completed].pid = procs[best].pid;
        res[completed].finish = time + procs[best].burst;
        res[completed].waiting = time - procs[best].arrival;
        res[completed].turnaround = res[completed].finish - procs[best].arrival;
        time = res[completed].finish;
        done[best] = 1;
        completed = completed + 1;
    }
}

void run_rr(struct Proc procs[], int n, int quantum, struct Result res[]) {
    int remaining[8];
    int time;
    int completed;
    int i;
    int ran;
    int slice;
    int done_count;

    for (i = 0; i < n; i++) {
        remaining[i] = procs[i].burst;
        res[i].pid = procs[i].pid;
        res[i].finish = 0;
        res[i].waiting = 0;
        res[i].turnaround = 0;
    }

    time = 0;
    completed = 0;

    while (completed < n) {
        ran = 0;
        for (i = 0; i < n; i++) {
            if (remaining[i] > 0 && procs[i].arrival <= time) {
                if (remaining[i] < quantum) {
                    slice = remaining[i];
                } else {
                    slice = quantum;
                }
                time = time + slice;
                remaining[i] = remaining[i] - slice;
                if (remaining[i] == 0) {
                    res[i].finish = time;
                    res[i].turnaround = time - procs[i].arrival;
                    res[i].waiting = res[i].turnaround - procs[i].burst;
                    completed = completed + 1;
                }
                ran = 1;
            }
        }
        if (ran == 0) {
            time = time + 1;
        }
    }
}

int avg_wait(struct Result res[], int n) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < n; i++) {
        total = total + res[i].waiting;
    }
    return total / n;
}

int avg_tat(struct Result res[], int n) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < n; i++) {
        total = total + res[i].turnaround;
    }
    return total / n;
}

int max_wait(struct Result res[], int n) {
    int mx;
    int i;
    mx = 0;
    for (i = 0; i < n; i++) {
        if (res[i].waiting > mx) {
            mx = res[i].waiting;
        }
    }
    return mx;
}

int main() {
    struct Proc procs[4];
    struct Result fcfs_res[4];
    struct Result sjf_res[4];
    struct Result rr_res[4];
    int i;
    int fcfs_aw;
    int sjf_aw;
    int rr_aw;
    int fcfs_at;
    int sjf_at;
    int rr_at;

    init_workload(procs);

    printf("=== Scheduling Comparison ===\n");   // EXPECT: === Scheduling Comparison ===
    printf("Workload: P1(0,6) P2(1,2) P3(2,8) P4(3,3)\n"); // EXPECT: Workload: P1(0,6) P2(1,2) P3(2,8) P4(3,3)

    // Run all three algorithms
    run_fcfs(procs, 4, fcfs_res);
    run_sjf(procs, 4, sjf_res);
    run_rr(procs, 4, 3, rr_res);

    // Print FCFS results
    printf("--- FCFS ---\n");                    // EXPECT: --- FCFS ---
    for (i = 0; i < 4; i++) {
        printf("P%d: fin=%d w=%d tat=%d\n",
               fcfs_res[i].pid, fcfs_res[i].finish,
               fcfs_res[i].waiting, fcfs_res[i].turnaround);
    }
    // EXPECT: P1: fin=6 w=0 tat=6
    // EXPECT: P2: fin=8 w=5 tat=7
    // EXPECT: P3: fin=16 w=6 tat=14
    // EXPECT: P4: fin=19 w=13 tat=16

    // Print SJF results
    printf("--- SJF ---\n");                     // EXPECT: --- SJF ---
    for (i = 0; i < 4; i++) {
        printf("P%d: fin=%d w=%d tat=%d\n",
               sjf_res[i].pid, sjf_res[i].finish,
               sjf_res[i].waiting, sjf_res[i].turnaround);
    }
    // EXPECT: P1: fin=6 w=0 tat=6
    // EXPECT: P2: fin=8 w=5 tat=7
    // EXPECT: P4: fin=11 w=5 tat=8
    // EXPECT: P3: fin=19 w=9 tat=17

    // Print RR results
    printf("--- RR (q=3) ---\n");                // EXPECT: --- RR (q=3) ---
    for (i = 0; i < 4; i++) {
        printf("P%d: fin=%d w=%d tat=%d\n",
               rr_res[i].pid, rr_res[i].finish,
               rr_res[i].waiting, rr_res[i].turnaround);
    }
    // EXPECT: P1: fin=14 w=8 tat=14
    // EXPECT: P2: fin=5 w=2 tat=4
    // EXPECT: P3: fin=19 w=9 tat=17
    // EXPECT: P4: fin=11 w=5 tat=8

    // Compare
    printf("--- Comparison ---\n");              // EXPECT: --- Comparison ---
    fcfs_aw = avg_wait(fcfs_res, 4);
    sjf_aw = avg_wait(sjf_res, 4);
    rr_aw = avg_wait(rr_res, 4);
    printf("Avg wait - FCFS:%d SJF:%d RR:%d\n", fcfs_aw, sjf_aw, rr_aw); // EXPECT: Avg wait - FCFS:6 SJF:4 RR:6

    fcfs_at = avg_tat(fcfs_res, 4);
    sjf_at = avg_tat(sjf_res, 4);
    rr_at = avg_tat(rr_res, 4);
    printf("Avg tat  - FCFS:%d SJF:%d RR:%d\n", fcfs_at, sjf_at, rr_at); // EXPECT: Avg tat  - FCFS:10 SJF:9 RR:10

    printf("Max wait - FCFS:%d SJF:%d RR:%d\n",
           max_wait(fcfs_res, 4), max_wait(sjf_res, 4), max_wait(rr_res, 4));
    // EXPECT: Max wait - FCFS:13 SJF:9 RR:9

    // Determine winner
    if (sjf_aw <= fcfs_aw && sjf_aw <= rr_aw) {
        printf("Best avg wait: SJF\n");          // EXPECT: Best avg wait: SJF
    } else if (fcfs_aw <= rr_aw) {
        printf("Best avg wait: FCFS\n");
    } else {
        printf("Best avg wait: RR\n");
    }

    if (max_wait(rr_res, 4) <= max_wait(fcfs_res, 4) && max_wait(rr_res, 4) <= max_wait(sjf_res, 4)) {
        printf("Best fairness: RR\n");           // EXPECT: Best fairness: RR
    } else {
        printf("Best fairness: other\n");
    }

    printf("Done\n");                            // EXPECT: Done
    return 0;
}
