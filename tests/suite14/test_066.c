int printf(const char *fmt, ...);

// Priority-based process scheduler simulation
// Lower priority number = higher priority
// Non-preemptive: once a process starts, it runs to completion

struct ProcInfo {
    int pid;
    int burst;
    int priority;
    int arrival;
    int start_time;
    int finish_time;
    int waiting;
    int turnaround;
    int done;
};

struct PriSched {
    struct ProcInfo procs[8];
    int count;
    int current_time;
    int completed;
};

void ps_init(struct PriSched *s) {
    s->count = 0;
    s->current_time = 0;
    s->completed = 0;
}

void ps_add(struct PriSched *s, int pid, int burst, int priority, int arrival) {
    int idx = s->count;
    s->procs[idx].pid = pid;
    s->procs[idx].burst = burst;
    s->procs[idx].priority = priority;
    s->procs[idx].arrival = arrival;
    s->procs[idx].start_time = -1;
    s->procs[idx].finish_time = -1;
    s->procs[idx].waiting = 0;
    s->procs[idx].turnaround = 0;
    s->procs[idx].done = 0;
    s->count++;
}

int ps_find_highest(struct PriSched *s) {
    int best = -1;
    int best_pri = 999;
    int i;
    for (i = 0; i < s->count; i++) {
        if (!s->procs[i].done && s->procs[i].arrival <= s->current_time) {
            if (s->procs[i].priority < best_pri) {
                best_pri = s->procs[i].priority;
                best = i;
            } else if (s->procs[i].priority == best_pri && best != -1) {
                // Tie-break: earlier arrival wins
                if (s->procs[i].arrival < s->procs[best].arrival) {
                    best = i;
                }
            }
        }
    }
    return best;
}

void ps_run(struct PriSched *s) {
    int safety = 0;
    while (s->completed < s->count && safety < 500) {
        int idx = ps_find_highest(s);
        if (idx == -1) {
            s->current_time++;
            safety++;
            continue;
        }

        s->procs[idx].start_time = s->current_time;
        s->procs[idx].waiting = s->current_time - s->procs[idx].arrival;
        s->current_time = s->current_time + s->procs[idx].burst;
        s->procs[idx].finish_time = s->current_time;
        s->procs[idx].turnaround = s->procs[idx].finish_time - s->procs[idx].arrival;
        s->procs[idx].done = 1;
        s->completed++;

        printf("P%d: start=%d finish=%d wait=%d ta=%d\n",
               s->procs[idx].pid,
               s->procs[idx].start_time,
               s->procs[idx].finish_time,
               s->procs[idx].waiting,
               s->procs[idx].turnaround);
    }
}

int main(void) {
    struct PriSched s;

    // Test 1: Basic priority scheduling
    // All arrive at t=0
    printf("=== Priority Scheduling Test 1 ===\n");
    // EXPECT: === Priority Scheduling Test 1 ===

    ps_init(&s);
    ps_add(&s, 1, 10, 3, 0);  // P1: burst=10, pri=3
    ps_add(&s, 2, 5, 1, 0);   // P2: burst=5, pri=1 (highest)
    ps_add(&s, 3, 8, 2, 0);   // P3: burst=8, pri=2

    // Order: P2 (pri=1), P3 (pri=2), P1 (pri=3)
    // P2: start=0, finish=5, wait=0, ta=5
    // P3: start=5, finish=13, wait=5, ta=13
    // P1: start=13, finish=23, wait=13, ta=23
    ps_run(&s);
    // EXPECT: P2: start=0 finish=5 wait=0 ta=5
    // EXPECT: P3: start=5 finish=13 wait=5 ta=13
    // EXPECT: P1: start=13 finish=23 wait=13 ta=23

    int avg_wait = (s.procs[0].waiting + s.procs[1].waiting + s.procs[2].waiting) / 3;
    printf("Avg waiting: %d\n", avg_wait);
    // EXPECT: Avg waiting: 6

    // Test 2: Staggered arrivals
    printf("=== Priority Scheduling Test 2 ===\n");
    // EXPECT: === Priority Scheduling Test 2 ===

    ps_init(&s);
    ps_add(&s, 10, 6, 2, 0);   // P10: burst=6, pri=2, arrive=0
    ps_add(&s, 11, 3, 1, 3);   // P11: burst=3, pri=1, arrive=3
    ps_add(&s, 12, 4, 3, 1);   // P12: burst=4, pri=3, arrive=1

    // At t=0: only P10 arrived -> P10 runs (non-preemptive)
    // P10: start=0, finish=6, wait=0, ta=6
    // At t=6: P11 (pri=1) and P12 (pri=3) both arrived -> P11 runs
    // P11: start=6, finish=9, wait=6-3=3, ta=9-3=6
    // At t=9: P12 runs
    // P12: start=9, finish=13, wait=9-1=8, ta=13-1=12

    ps_run(&s);
    // EXPECT: P10: start=0 finish=6 wait=0 ta=6
    // EXPECT: P11: start=6 finish=9 wait=3 ta=6
    // EXPECT: P12: start=9 finish=13 wait=8 ta=12

    printf("Total time: %d\n", s.current_time);
    // EXPECT: Total time: 13

    // Test 3: Same priority, FCFS tiebreak
    printf("=== Priority Scheduling Test 3 ===\n");
    // EXPECT: === Priority Scheduling Test 3 ===

    ps_init(&s);
    ps_add(&s, 20, 3, 2, 0);  // arrives first
    ps_add(&s, 21, 4, 2, 1);  // arrives second, same priority

    // P20 arrives first, same priority -> P20 goes first
    // P20: start=0, finish=3, wait=0, ta=3
    // P21: start=3, finish=7, wait=3-1=2, ta=7-1=6
    ps_run(&s);
    // EXPECT: P20: start=0 finish=3 wait=0 ta=3
    // EXPECT: P21: start=3 finish=7 wait=2 ta=6

    // Test 4: Gap in arrivals
    printf("=== Priority Scheduling Test 4 ===\n");
    // EXPECT: === Priority Scheduling Test 4 ===

    ps_init(&s);
    ps_add(&s, 30, 2, 1, 0);
    ps_add(&s, 31, 3, 1, 10); // arrives much later

    // P30: start=0, finish=2, wait=0, ta=2
    // idle t=2..10
    // P31: start=10, finish=13, wait=0, ta=3
    ps_run(&s);
    // EXPECT: P30: start=0 finish=2 wait=0 ta=2
    // EXPECT: P31: start=10 finish=13 wait=0 ta=3

    printf("Idle time: %d\n", s.current_time - s.procs[0].burst - s.procs[1].burst);
    // EXPECT: Idle time: 8

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
