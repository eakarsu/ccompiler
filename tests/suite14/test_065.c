int printf(const char *fmt, ...);

// Round-robin process scheduler simulation

struct Process {
    int pid;
    int burst;      // total CPU burst time remaining
    int arrival;    // arrival time
    int waiting;    // total waiting time
    int turnaround; // turnaround time
    int done;       // completion flag
    int started;    // has been started
    int first_run;  // first time it ran
};

struct Scheduler {
    struct Process procs[8];
    int count;
    int quantum;
    int current_time;
    int completed;
};

void sched_init(struct Scheduler *s, int quantum) {
    s->count = 0;
    s->quantum = quantum;
    s->current_time = 0;
    s->completed = 0;
}

void sched_add(struct Scheduler *s, int pid, int burst, int arrival) {
    int idx = s->count;
    s->procs[idx].pid = pid;
    s->procs[idx].burst = burst;
    s->procs[idx].arrival = arrival;
    s->procs[idx].waiting = 0;
    s->procs[idx].turnaround = 0;
    s->procs[idx].done = 0;
    s->procs[idx].started = 0;
    s->procs[idx].first_run = -1;
    s->count++;
}

int find_next_ready(struct Scheduler *s, int after) {
    // Round-robin: find next non-done process that has arrived
    int i;
    int idx;
    for (i = 0; i < s->count; i++) {
        idx = (after + i) % s->count;
        if (!s->procs[idx].done && s->procs[idx].arrival <= s->current_time) {
            return idx;
        }
    }
    return -1;
}

void run_round_robin(struct Scheduler *s) {
    int last = 0;
    int idle_count = 0;

    while (s->completed < s->count) {
        int idx = find_next_ready(s, last);
        if (idx == -1) {
            // No process ready, advance time
            s->current_time++;
            idle_count++;
            if (idle_count > 1000) break; // safety
            continue;
        }
        idle_count = 0;

        if (!s->procs[idx].started) {
            s->procs[idx].started = 1;
            s->procs[idx].first_run = s->current_time;
        }

        int run_time = s->procs[idx].burst;
        if (run_time > s->quantum) {
            run_time = s->quantum;
        }

        // Update waiting times for all other ready processes
        int j;
        for (j = 0; j < s->count; j++) {
            if (j != idx && !s->procs[j].done &&
                s->procs[j].arrival <= s->current_time) {
                s->procs[j].waiting = s->procs[j].waiting + run_time;
            }
        }

        s->procs[idx].burst = s->procs[idx].burst - run_time;
        s->current_time = s->current_time + run_time;

        if (s->procs[idx].burst == 0) {
            s->procs[idx].done = 1;
            s->procs[idx].turnaround = s->current_time - s->procs[idx].arrival;
            s->completed++;
            printf("P%d finished at t=%d, turnaround=%d, wait=%d\n",
                   s->procs[idx].pid, s->current_time,
                   s->procs[idx].turnaround, s->procs[idx].waiting);
        }

        last = (idx + 1) % s->count;
    }
}

int main(void) {
    struct Scheduler s;

    // Test 1: Simple RR with quantum=2
    // All arrive at t=0
    // P1: burst=5, P2: burst=3, P3: burst=1
    sched_init(&s, 2);
    sched_add(&s, 1, 5, 0);
    sched_add(&s, 2, 3, 0);
    sched_add(&s, 3, 1, 0);

    printf("=== Round Robin quantum=2 ===\n");
    // EXPECT: === Round Robin quantum=2 ===

    // Trace:
    // t=0-2: P1 runs 2 (burst 5->3). P2 waits+2, P3 waits+2
    // t=2-4: P2 runs 2 (burst 3->1). P1 waits+2, P3 waits+2
    // t=4-5: P3 runs 1 (burst 1->0, DONE). P1 waits+1, P2 waits+1
    //   P3 done at t=5, turnaround=5, wait=4
    // t=5-7: P1 runs 2 (burst 3->1). P2 waits+2
    // t=7-8: P2 runs 1 (burst 1->0, DONE). P1 waits+1
    //   P2 done at t=8, turnaround=8, wait=5
    // t=8-9: P1 runs 1 (burst 1->0, DONE)
    //   P1 done at t=9, turnaround=9, wait=4

    run_round_robin(&s);
    // EXPECT: P3 finished at t=5, turnaround=5, wait=4
    // EXPECT: P2 finished at t=8, turnaround=8, wait=5
    // EXPECT: P1 finished at t=9, turnaround=9, wait=4

    printf("Total time: %d\n", s.current_time);
    // EXPECT: Total time: 9

    // Average turnaround
    int avg_ta = (s.procs[0].turnaround + s.procs[1].turnaround +
                  s.procs[2].turnaround) / 3;
    printf("Avg turnaround: %d\n", avg_ta);
    // EXPECT: Avg turnaround: 7

    // Test 2: RR with quantum=4, staggered arrivals
    printf("=== Round Robin quantum=4, staggered ===\n");
    // EXPECT: === Round Robin quantum=4, staggered ===

    sched_init(&s, 4);
    sched_add(&s, 10, 6, 0);  // arrives at 0, burst 6
    sched_add(&s, 11, 4, 2);  // arrives at 2, burst 4

    // Trace:
    // t=0: only P10 ready
    // t=0-4: P10 runs 4 (burst 6->2). P11 not arrived at t=0, no wait
    // t=4-8: P11 runs 4 (burst 4->0, DONE). P10 waits 4
    //   P11 done at t=8, turnaround=8-2=6, wait=0
    // t=8-10: P10 runs 2 (burst 2->0, DONE)
    //   P10 done at t=10, turnaround=10-0=10, wait=4

    run_round_robin(&s);
    // EXPECT: P11 finished at t=8, turnaround=6, wait=0
    // EXPECT: P10 finished at t=10, turnaround=10, wait=4

    // Test 3: All same burst
    printf("=== Equal bursts quantum=1 ===\n");
    // EXPECT: === Equal bursts quantum=1 ===

    sched_init(&s, 1);
    sched_add(&s, 20, 2, 0);
    sched_add(&s, 21, 2, 0);

    // Trace:
    // t=0-1: P20 runs 1 (burst 2->1). P21 waits+1
    // t=1-2: P21 runs 1 (burst 2->1). P20 waits+1
    // t=2-3: P20 runs 1 (burst 1->0, DONE). P21 waits+1
    //   P20 done at t=3, turnaround=3, wait=1
    // t=3-4: P21 runs 1 (burst 1->0, DONE)
    //   P21 done at t=4, turnaround=4, wait=2

    run_round_robin(&s);
    // EXPECT: P20 finished at t=3, turnaround=3, wait=1
    // EXPECT: P21 finished at t=4, turnaround=4, wait=2

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
