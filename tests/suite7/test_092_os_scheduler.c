int printf(const char *fmt, ...);

// OS Process Scheduler: process table, round-robin, priority scheduling

enum {
    STATE_READY = 0,
    STATE_RUNNING = 1,
    STATE_BLOCKED = 2,
    STATE_DONE = 3
};

struct Process {
    int pid;
    int state;
    int priority;
    int burst_remaining;
    int total_burst;
    int wait_time;
    int turnaround;
};

struct Scheduler {
    struct Process procs[8];
    int count;
    int current;
    int time;
    int context_switches;
    int quantum;
};

void sched_init(struct Scheduler *s, int quantum) {
    s->count = 0;
    s->current = -1;
    s->time = 0;
    s->context_switches = 0;
    s->quantum = quantum;
}

void add_process(struct Scheduler *s, int pid, int priority, int burst) {
    int idx = s->count;
    s->procs[idx].pid = pid;
    s->procs[idx].state = STATE_READY;
    s->procs[idx].priority = priority;
    s->procs[idx].burst_remaining = burst;
    s->procs[idx].total_burst = burst;
    s->procs[idx].wait_time = 0;
    s->procs[idx].turnaround = 0;
    s->count = s->count + 1;
}

int all_done(struct Scheduler *s) {
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->procs[i].state != STATE_DONE) return 0;
    }
    return 1;
}

int find_next_rr(struct Scheduler *s) {
    int i;
    int start = (s->current + 1) % s->count;
    for (i = 0; i < s->count; i++) {
        int idx = (start + i) % s->count;
        if (s->procs[idx].state == STATE_READY) return idx;
    }
    return -1;
}

int find_highest_priority(struct Scheduler *s) {
    int best = -1;
    int best_prio = -1;
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->procs[i].state == STATE_READY) {
            if (best == -1 || s->procs[i].priority > best_prio) {
                best = i;
                best_prio = s->procs[i].priority;
            }
        }
    }
    return best;
}

void run_round_robin(struct Scheduler *s) {
    while (all_done(s) == 0) {
        int next = find_next_rr(s);
        if (next == -1) break;

        if (next != s->current) {
            s->context_switches = s->context_switches + 1;
            s->current = next;
        }

        s->procs[next].state = STATE_RUNNING;

        int run_time = s->quantum;
        if (s->procs[next].burst_remaining < run_time) {
            run_time = s->procs[next].burst_remaining;
        }

        int t;
        for (t = 0; t < run_time; t++) {
            s->time = s->time + 1;
            s->procs[next].burst_remaining = s->procs[next].burst_remaining - 1;
            // Update wait times for other ready processes
            int j;
            for (j = 0; j < s->count; j++) {
                if (j != next && s->procs[j].state == STATE_READY) {
                    s->procs[j].wait_time = s->procs[j].wait_time + 1;
                }
            }
        }

        if (s->procs[next].burst_remaining == 0) {
            s->procs[next].state = STATE_DONE;
            s->procs[next].turnaround = s->time;
        } else {
            s->procs[next].state = STATE_READY;
        }
    }
}

void run_priority(struct Scheduler *s) {
    while (all_done(s) == 0) {
        int next = find_highest_priority(s);
        if (next == -1) break;

        if (next != s->current) {
            s->context_switches = s->context_switches + 1;
            s->current = next;
        }

        s->procs[next].state = STATE_RUNNING;

        // Run entire burst (non-preemptive)
        while (s->procs[next].burst_remaining > 0) {
            s->time = s->time + 1;
            s->procs[next].burst_remaining = s->procs[next].burst_remaining - 1;
            int j;
            for (j = 0; j < s->count; j++) {
                if (j != next && s->procs[j].state == STATE_READY) {
                    s->procs[j].wait_time = s->procs[j].wait_time + 1;
                }
            }
        }
        s->procs[next].state = STATE_DONE;
        s->procs[next].turnaround = s->time;
    }
}

int main(void) {
    struct Scheduler s;
    int i;
    int total_wait;
    int total_turn;

    // --- Round Robin with quantum=2 ---
    sched_init(&s, 2);
    add_process(&s, 1, 1, 4);
    add_process(&s, 2, 2, 3);
    add_process(&s, 3, 3, 2);

    // EXPECT: Processes added: 3
    printf("Processes added: %d\n", s.count);

    run_round_robin(&s);

    // EXPECT: RR total time: 9
    printf("RR total time: %d\n", s.time);

    // EXPECT: RR context switches: 5
    printf("RR context switches: %d\n", s.context_switches);

    // EXPECT: P1 state=3 turnaround=8
    printf("P1 state=%d turnaround=%d\n", s.procs[0].state, s.procs[0].turnaround);

    // EXPECT: P2 state=3 turnaround=9
    printf("P2 state=%d turnaround=%d\n", s.procs[1].state, s.procs[1].turnaround);

    // EXPECT: P3 state=3 turnaround=6
    printf("P3 state=%d turnaround=%d\n", s.procs[2].state, s.procs[2].turnaround);

    // EXPECT: P1 wait=4 P2 wait=6 P3 wait=4
    printf("P1 wait=%d P2 wait=%d P3 wait=%d\n",
           s.procs[0].wait_time, s.procs[1].wait_time, s.procs[2].wait_time);

    total_wait = 0;
    for (i = 0; i < s.count; i++) total_wait = total_wait + s.procs[i].wait_time;
    // EXPECT: RR avg wait (x3): 14
    printf("RR avg wait (x3): %d\n", total_wait);

    // --- Priority Scheduling (non-preemptive) ---
    sched_init(&s, 1);
    add_process(&s, 10, 3, 3);  // highest priority
    add_process(&s, 20, 1, 5);  // lowest priority
    add_process(&s, 30, 2, 4);  // medium priority

    run_priority(&s);

    // EXPECT: Priority total time: 12
    printf("Priority total time: %d\n", s.time);

    // EXPECT: Priority switches: 3
    printf("Priority switches: %d\n", s.context_switches);

    // P10 runs first (prio 3), then P30 (prio 2), then P20 (prio 1)
    // EXPECT: P10 turnaround=3 wait=0
    printf("P10 turnaround=%d wait=%d\n", s.procs[0].turnaround, s.procs[0].wait_time);

    // EXPECT: P20 turnaround=12 wait=7
    printf("P20 turnaround=%d wait=%d\n", s.procs[1].turnaround, s.procs[1].wait_time);

    // EXPECT: P30 turnaround=7 wait=3
    printf("P30 turnaround=%d wait=%d\n", s.procs[2].turnaround, s.procs[2].wait_time);

    total_turn = 0;
    for (i = 0; i < s.count; i++) total_turn = total_turn + s.procs[i].turnaround;
    // EXPECT: Priority total turnaround: 22
    printf("Priority total turnaround: %d\n", total_turn);

    // --- Single process ---
    sched_init(&s, 3);
    add_process(&s, 100, 5, 7);
    run_round_robin(&s);
    // EXPECT: Single proc time=7 switches=1
    printf("Single proc time=%d switches=%d\n", s.time, s.context_switches);

    // EXPECT: Single proc wait=0
    printf("Single proc wait=%d\n", s.procs[0].wait_time);

    return 0;
}
