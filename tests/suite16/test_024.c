int printf(const char *fmt, ...);

// Test 024: Priority Scheduling (Non-Preemptive, lower number = higher priority)

struct Process {
    int pid;
    int arrival;
    int burst;
    int priority;
    int start;
    int finish;
    int waiting;
    int turnaround;
    int done;
};

int find_highest_priority(struct Process procs[], int n, int time) {
    int best;
    int i;
    int best_prio;
    best = -1;
    best_prio = 999;
    for (i = 0; i < n; i++) {
        if (procs[i].done == 0 && procs[i].arrival <= time) {
            if (procs[i].priority < best_prio) {
                best_prio = procs[i].priority;
                best = i;
            } else if (procs[i].priority == best_prio && best >= 0) {
                if (procs[i].arrival < procs[best].arrival) {
                    best = i;
                }
            }
        }
    }
    return best;
}

void priority_schedule(struct Process procs[], int n) {
    int time;
    int completed;
    int idx;

    time = 0;
    completed = 0;

    for (idx = 0; idx < n; idx++) {
        procs[idx].done = 0;
    }

    while (completed < n) {
        idx = find_highest_priority(procs, n, time);
        if (idx == -1) {
            time = time + 1;
        } else {
            procs[idx].start = time;
            procs[idx].finish = time + procs[idx].burst;
            procs[idx].waiting = procs[idx].start - procs[idx].arrival;
            procs[idx].turnaround = procs[idx].finish - procs[idx].arrival;
            procs[idx].done = 1;
            time = procs[idx].finish;
            completed = completed + 1;
        }
    }
}

int check_starvation(struct Process procs[], int n, int threshold) {
    int i;
    int starved;
    starved = 0;
    for (i = 0; i < n; i++) {
        if (procs[i].waiting > threshold) {
            starved = starved + 1;
        }
    }
    return starved;
}

int main() {
    struct Process procs[5];
    int i;
    int total_w;
    int total_t;
    int starved;

    procs[0].pid = 1; procs[0].arrival = 0; procs[0].burst = 10; procs[0].priority = 3;
    procs[1].pid = 2; procs[1].arrival = 0; procs[1].burst = 1;  procs[1].priority = 1;
    procs[2].pid = 3; procs[2].arrival = 0; procs[2].burst = 2;  procs[2].priority = 4;
    procs[3].pid = 4; procs[3].arrival = 0; procs[3].burst = 1;  procs[3].priority = 5;
    procs[4].pid = 5; procs[4].arrival = 0; procs[4].burst = 5;  procs[4].priority = 2;

    printf("=== Priority Scheduling ===\n");     // EXPECT: === Priority Scheduling ===

    priority_schedule(procs, 5);

    for (i = 0; i < 5; i++) {
        printf("P%d: prio=%d start=%d fin=%d wait=%d tat=%d\n",
               procs[i].pid, procs[i].priority,
               procs[i].start, procs[i].finish,
               procs[i].waiting, procs[i].turnaround);
    }
    // EXPECT: P1: prio=3 start=6 fin=16 wait=6 tat=16
    // EXPECT: P2: prio=1 start=0 fin=1 wait=0 tat=1
    // EXPECT: P3: prio=4 start=16 fin=18 wait=16 tat=18
    // EXPECT: P4: prio=5 start=18 fin=19 wait=18 tat=19
    // EXPECT: P5: prio=2 start=1 fin=6 wait=1 tat=6

    total_w = 0;
    total_t = 0;
    for (i = 0; i < 5; i++) {
        total_w = total_w + procs[i].waiting;
        total_t = total_t + procs[i].turnaround;
    }

    printf("Total wait: %d\n", total_w);         // EXPECT: Total wait: 41
    printf("Total tat: %d\n", total_t);           // EXPECT: Total tat: 60
    printf("Avg wait: %d\n", total_w / 5);        // EXPECT: Avg wait: 8
    printf("Avg tat: %d\n", total_t / 5);         // EXPECT: Avg tat: 12

    starved = check_starvation(procs, 5, 10);
    printf("Starved processes (wait>10): %d\n", starved); // EXPECT: Starved processes (wait>10): 2

    printf("Execution order by priority:\n");     // EXPECT: Execution order by priority:
    printf("  P2(prio1) -> P5(prio2) -> P1(prio3) -> P3(prio4) -> P4(prio5)\n"); // EXPECT:   P2(prio1) -> P5(prio2) -> P1(prio3) -> P3(prio4) -> P4(prio5)

    printf("Done\n");                            // EXPECT: Done
    return 0;
}
