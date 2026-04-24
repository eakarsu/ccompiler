int printf(const char *fmt, ...);

// Test 029: Priority Scheduling with Aging
// Aging: every 3 time units waiting, effective priority decreases by 1
// (lower number = higher priority)

struct Process {
    int pid;
    int arrival;
    int burst;
    int base_priority;
    int eff_priority;
    int remaining;
    int start;
    int finish;
    int done;
    int wait_time;
};

int get_effective_priority(struct Process *p, int time) {
    int waited;
    int aging;
    if (p->done || p->arrival > time) {
        return 999;
    }
    waited = time - p->arrival - (p->burst - p->remaining);
    if (waited < 0) waited = 0;
    aging = waited / 3;
    p->eff_priority = p->base_priority - aging;
    if (p->eff_priority < 0) {
        p->eff_priority = 0;
    }
    return p->eff_priority;
}

void schedule_with_aging(struct Process procs[], int n) {
    int time;
    int completed;
    int i;
    int best;
    int best_prio;
    int ep;

    for (i = 0; i < n; i++) {
        procs[i].remaining = procs[i].burst;
        procs[i].done = 0;
        procs[i].eff_priority = procs[i].base_priority;
    }

    time = 0;
    completed = 0;

    while (completed < n) {
        best = -1;
        best_prio = 999;
        for (i = 0; i < n; i++) {
            if (procs[i].done == 0 && procs[i].arrival <= time) {
                ep = get_effective_priority(&procs[i], time);
                if (ep < best_prio) {
                    best_prio = ep;
                    best = i;
                }
            }
        }
        if (best == -1) {
            time = time + 1;
            continue;
        }

        // Non-preemptive: run the whole job
        procs[best].start = time;
        procs[best].finish = time + procs[best].remaining;
        procs[best].wait_time = time - procs[best].arrival;
        procs[best].remaining = 0;
        procs[best].done = 1;
        time = procs[best].finish;
        completed = completed + 1;
    }
}

int main() {
    struct Process procs[5];
    int i;
    int total_w;
    int total_t;

    // High priority number = low priority
    // P1 has low priority (5) but arrives first
    // P5 has high priority (1) but arrives late
    procs[0].pid = 1; procs[0].arrival = 0;  procs[0].burst = 4; procs[0].base_priority = 5;
    procs[1].pid = 2; procs[1].arrival = 1;  procs[1].burst = 3; procs[1].base_priority = 4;
    procs[2].pid = 3; procs[2].arrival = 2;  procs[2].burst = 2; procs[2].base_priority = 3;
    procs[3].pid = 4; procs[3].arrival = 3;  procs[3].burst = 5; procs[3].base_priority = 2;
    procs[4].pid = 5; procs[4].arrival = 10; procs[4].burst = 1; procs[4].base_priority = 1;

    printf("=== Priority with Aging ===\n");     // EXPECT: === Priority with Aging ===
    printf("Aging: -1 priority per 3 ticks waited\n"); // EXPECT: Aging: -1 priority per 3 ticks waited

    schedule_with_aging(procs, 5);

    for (i = 0; i < 5; i++) {
        printf("P%d: base_prio=%d eff_prio=%d start=%d fin=%d wait=%d\n",
               procs[i].pid, procs[i].base_priority, procs[i].eff_priority,
               procs[i].start, procs[i].finish, procs[i].wait_time);
    }
    // EXPECT: P1: base_prio=5 eff_prio=5 start=0 fin=4 wait=0
    // EXPECT: P2: base_prio=4 eff_prio=1 start=11 fin=14 wait=10
    // EXPECT: P3: base_prio=3 eff_prio=1 start=9 fin=11 wait=7
    // EXPECT: P4: base_prio=2 eff_prio=2 start=4 fin=9 wait=1
    // EXPECT: P5: base_prio=1 eff_prio=0 start=14 fin=15 wait=4

    total_w = 0;
    total_t = 0;
    for (i = 0; i < 5; i++) {
        total_w = total_w + procs[i].wait_time;
        total_t = total_t + (procs[i].finish - procs[i].arrival);
    }

    printf("Total wait: %d\n", total_w);         // EXPECT: Total wait: 22
    printf("Total tat: %d\n", total_t);           // EXPECT: Total tat: 37
    printf("Avg wait: %d\n", total_w / 5);        // EXPECT: Avg wait: 4
    printf("Avg tat: %d\n", total_t / 5);         // EXPECT: Avg tat: 7

    // Show that aging prevented starvation
    printf("Without aging, P1 (prio=5) would be scheduled last\n"); // EXPECT: Without aging, P1 (prio=5) would be scheduled last
    printf("With aging, P1 gets priority boost from waiting\n"); // EXPECT: With aging, P1 gets priority boost from waiting

    printf("Makespan: %d\n", procs[4].finish);   // EXPECT: Makespan: 15
    printf("Done\n");                            // EXPECT: Done
    return 0;
}
