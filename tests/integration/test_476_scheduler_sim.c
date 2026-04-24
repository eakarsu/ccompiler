int printf(const char *fmt, ...);
// EXPECT: === Priority Scheduler Simulation ===\nTick 0: Process 3 running (pri=4, rem=3)\nTick 2: Process 3 running (pri=4, rem=1)\n  Process 3 completed at tick 3 (waited 0)\nTick 3: Process 1 running (pri=3, rem=4)\nTick 5: Process 1 running (pri=3, rem=2)\n  Process 1 completed at tick 7 (waited 3)\nTick 7: Process 4 running (pri=2, rem=5)\nTick 9: Process 4 running (pri=2, rem=3)\nTick 11: Process 4 running (pri=2, rem=1)\n  Process 4 completed at tick 12 (waited 7)\nTick 12: Process 2 running (pri=1, rem=2)\n  Process 2 completed at tick 14 (waited 12)\n--- Results ---\nCompleted: 4 processes\nTotal wait time: 22

enum ProcState { READY, RUNNING, BLOCKED, DONE };

struct Process {
    int pid;
    int priority;
    int burst_time;
    int remaining;
    int wait_time;
    enum ProcState state;
};

void init_process(struct Process *p, int pid, int priority, int burst) {
    p->pid = pid;
    p->priority = priority;
    p->burst_time = burst;
    p->remaining = burst;
    p->wait_time = 0;
    p->state = READY;
}

int find_highest_priority(struct Process procs[], int n) {
    int best = -1;
    int best_prio = -1;
    int i;
    for (i = 0; i < n; i++) {
        if (procs[i].state == READY && procs[i].priority > best_prio) {
            best_prio = procs[i].priority;
            best = i;
        }
    }
    return best;
}

int all_done(struct Process procs[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (procs[i].state != DONE) return 0;
    }
    return 1;
}

int main(void) {
    struct Process procs[4];
    init_process(&procs[0], 1, 3, 4);
    init_process(&procs[1], 2, 1, 2);
    init_process(&procs[2], 3, 4, 3);
    init_process(&procs[3], 4, 2, 5);

    int time_quantum = 2;
    int tick = 0;
    int total_wait = 0;
    int completed = 0;

    printf("=== Priority Scheduler Simulation ===\n");

    while (!all_done(procs, 4) && tick < 30) {
        int idx = find_highest_priority(procs, 4);
        if (idx < 0) break;

        procs[idx].state = RUNNING;
        int run_time = procs[idx].remaining;
        if (run_time > time_quantum) run_time = time_quantum;

        printf("Tick %d: Process %d running (pri=%d, rem=%d)\n",
               tick, procs[idx].pid, procs[idx].priority, procs[idx].remaining);

        procs[idx].remaining = procs[idx].remaining - run_time;
        tick = tick + run_time;

        int i;
        for (i = 0; i < 4; i++) {
            if (i != idx && procs[i].state == READY) {
                procs[i].wait_time = procs[i].wait_time + run_time;
            }
        }

        if (procs[idx].remaining <= 0) {
            procs[idx].state = DONE;
            completed++;
            printf("  Process %d completed at tick %d (waited %d)\n",
                   procs[idx].pid, tick, procs[idx].wait_time);
        } else {
            procs[idx].state = READY;
        }
    }

    printf("--- Results ---\n");
    printf("Completed: %d processes\n", completed);
    int i;
    for (i = 0; i < 4; i++) {
        total_wait = total_wait + procs[i].wait_time;
    }
    printf("Total wait time: %d\n", total_wait);

    return 0;
}
