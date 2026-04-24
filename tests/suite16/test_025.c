int printf(const char *fmt, ...);

// Test 025: Shortest Remaining Time First (SRTF) - Preemptive SJF

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int finish;
    int waiting;
    int turnaround;
    int started;
    int first_start;
};

struct GanttSlot {
    int pid;
    int start;
    int end;
};

int srtf_schedule(struct Process procs[], int n, struct GanttSlot gantt[]) {
    int time;
    int completed;
    int i;
    int best;
    int min_rem;
    int gantt_len;
    int prev_pid;

    for (i = 0; i < n; i++) {
        procs[i].remaining = procs[i].burst;
        procs[i].started = 0;
    }

    time = 0;
    completed = 0;
    gantt_len = 0;
    prev_pid = -1;

    while (completed < n) {
        best = -1;
        min_rem = 999999;
        for (i = 0; i < n; i++) {
            if (procs[i].remaining > 0 && procs[i].arrival <= time) {
                if (procs[i].remaining < min_rem) {
                    min_rem = procs[i].remaining;
                    best = i;
                }
            }
        }

        if (best == -1) {
            time = time + 1;
            prev_pid = -1;
            continue;
        }

        if (procs[best].started == 0) {
            procs[best].started = 1;
            procs[best].first_start = time;
        }

        if (procs[best].pid != prev_pid) {
            if (gantt_len > 0 && prev_pid != -1) {
                gantt[gantt_len - 1].end = time;
            }
            gantt[gantt_len].pid = procs[best].pid;
            gantt[gantt_len].start = time;
            gantt_len = gantt_len + 1;
        }

        prev_pid = procs[best].pid;
        procs[best].remaining = procs[best].remaining - 1;
        time = time + 1;

        if (procs[best].remaining == 0) {
            procs[best].finish = time;
            procs[best].turnaround = procs[best].finish - procs[best].arrival;
            procs[best].waiting = procs[best].turnaround - procs[best].burst;
            completed = completed + 1;
            gantt[gantt_len - 1].end = time;
            prev_pid = -1;
        }
    }

    return gantt_len;
}

int main() {
    struct Process procs[4];
    struct GanttSlot gantt[20];
    int gantt_len;
    int i;
    int total_w;
    int total_t;

    procs[0].pid = 1; procs[0].arrival = 0; procs[0].burst = 8;
    procs[1].pid = 2; procs[1].arrival = 1; procs[1].burst = 4;
    procs[2].pid = 3; procs[2].arrival = 2; procs[2].burst = 2;
    procs[3].pid = 4; procs[3].arrival = 3; procs[3].burst = 1;

    printf("=== SRTF Scheduling ===\n");         // EXPECT: === SRTF Scheduling ===

    gantt_len = srtf_schedule(procs, 4, gantt);

    for (i = 0; i < 4; i++) {
        printf("P%d: arr=%d bur=%d fin=%d wait=%d tat=%d\n",
               procs[i].pid, procs[i].arrival, procs[i].burst,
               procs[i].finish, procs[i].waiting, procs[i].turnaround);
    }
    // EXPECT: P1: arr=0 bur=8 fin=15 wait=7 tat=15
    // EXPECT: P2: arr=1 bur=4 fin=8 wait=3 tat=7
    // EXPECT: P3: arr=2 bur=2 fin=4 wait=0 tat=2
    // EXPECT: P4: arr=3 bur=1 fin=5 wait=1 tat=2

    total_w = 0;
    total_t = 0;
    for (i = 0; i < 4; i++) {
        total_w = total_w + procs[i].waiting;
        total_t = total_t + procs[i].turnaround;
    }
    printf("Total wait: %d\n", total_w);         // EXPECT: Total wait: 11
    printf("Avg wait: %d\n", total_w / 4);       // EXPECT: Avg wait: 2
    printf("Avg tat: %d\n", total_t / 4);        // EXPECT: Avg tat: 6

    printf("Gantt (%d slots):\n", gantt_len);    // EXPECT: Gantt (6 slots):
    for (i = 0; i < gantt_len; i++) {
        printf("  P%d [%d-%d]\n", gantt[i].pid, gantt[i].start, gantt[i].end);
    }
    // EXPECT:   P1 [0-1]
    // EXPECT:   P2 [1-2]
    // EXPECT:   P3 [2-4]
    // EXPECT:   P4 [4-5]
    // EXPECT:   P2 [5-8]
    // EXPECT:   P1 [8-15]

    printf("Preemptions improve avg wait vs non-preemptive\n"); // EXPECT: Preemptions improve avg wait vs non-preemptive
    printf("Response time P1: %d\n", procs[0].first_start); // EXPECT: Response time P1: 0
    printf("Response time P2: %d\n", procs[1].first_start); // EXPECT: Response time P2: 1
    printf("Response time P3: %d\n", procs[2].first_start); // EXPECT: Response time P3: 2
    printf("Response time P4: %d\n", procs[3].first_start); // EXPECT: Response time P4: 4
    printf("Done\n");                            // EXPECT: Done
    return 0;
}
