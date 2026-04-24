int printf(const char *fmt, ...);

// Process Scheduler Simulation: FIFO and SJF scheduling

struct Process {
    int pid;
    int arrival;
    int burst;
    int priority;
    int wait_time;
    int turnaround;
    int completed;
};

void fifo_schedule(struct Process *procs, int n) {
    int current_time;
    int i;
    int total_wait;
    int total_turn;

    current_time = 0;
    total_wait = 0;
    total_turn = 0;

    // FIFO: process in order of arrival (already sorted by index)
    printf("=== FIFO Scheduling ===\n"); // EXPECT: === FIFO Scheduling ===
    for (i = 0; i < n; i = i + 1) {
        if (current_time < procs[i].arrival) {
            current_time = procs[i].arrival;
        }
        procs[i].wait_time = current_time - procs[i].arrival;
        procs[i].turnaround = procs[i].wait_time + procs[i].burst;
        printf("P%d: wait=%d turnaround=%d\n", procs[i].pid, procs[i].wait_time, procs[i].turnaround); // EXPECT: P1: wait=0 turnaround=5
        // EXPECT: P2: wait=4 turnaround=7
        // EXPECT: P3: wait=6 turnaround=8
        // EXPECT: P4: wait=7 turnaround=11
        // EXPECT: P5: wait=10 turnaround=13
        total_wait = total_wait + procs[i].wait_time;
        total_turn = total_turn + procs[i].turnaround;
        current_time = current_time + procs[i].burst;
    }
    printf("FIFO avg_wait=%d avg_turn=%d\n", total_wait / n, total_turn / n); // EXPECT: FIFO avg_wait=5 avg_turn=8
}

void sjf_schedule(struct Process *procs, int n) {
    int current_time;
    int completed;
    int i;
    int min_burst;
    int min_idx;
    int total_wait;
    int total_turn;
    int order[10];
    int oc;

    current_time = 0;
    completed = 0;
    total_wait = 0;
    total_turn = 0;
    oc = 0;

    for (i = 0; i < n; i = i + 1) {
        procs[i].completed = 0;
    }

    printf("=== SJF Scheduling ===\n"); // EXPECT: === SJF Scheduling ===
    while (completed < n) {
        min_burst = 9999;
        min_idx = -1;
        for (i = 0; i < n; i = i + 1) {
            if (procs[i].completed == 0 && procs[i].arrival <= current_time) {
                if (procs[i].burst < min_burst) {
                    min_burst = procs[i].burst;
                    min_idx = i;
                }
            }
        }
        if (min_idx == -1) {
            current_time = current_time + 1;
        } else {
            procs[min_idx].wait_time = current_time - procs[min_idx].arrival;
            procs[min_idx].turnaround = procs[min_idx].wait_time + procs[min_idx].burst;
            total_wait = total_wait + procs[min_idx].wait_time;
            total_turn = total_turn + procs[min_idx].turnaround;
            current_time = current_time + procs[min_idx].burst;
            procs[min_idx].completed = 1;
            order[oc] = min_idx;
            oc = oc + 1;
            completed = completed + 1;
        }
    }
    for (i = 0; i < oc; i = i + 1) {
        int idx;
        idx = order[i];
        printf("P%d: wait=%d turnaround=%d\n", procs[idx].pid, procs[idx].wait_time, procs[idx].turnaround);
    }
    // EXPECT: P1: wait=0 turnaround=5
    // EXPECT: P3: wait=3 turnaround=5
    // EXPECT: P2: wait=6 turnaround=9
    // EXPECT: P5: wait=6 turnaround=9
    // EXPECT: P4: wait=10 turnaround=14
    printf("SJF avg_wait=%d avg_turn=%d\n", total_wait / n, total_turn / n); // EXPECT: SJF avg_wait=5 avg_turn=8
}

int main() {
    struct Process procs[5];
    // PID, arrival, burst
    procs[0].pid = 1; procs[0].arrival = 0; procs[0].burst = 5; procs[0].priority = 3;
    procs[1].pid = 2; procs[1].arrival = 1; procs[1].burst = 3; procs[1].priority = 1;
    procs[2].pid = 3; procs[2].arrival = 2; procs[2].burst = 2; procs[2].priority = 4;
    procs[3].pid = 4; procs[3].arrival = 3; procs[3].burst = 4; procs[3].priority = 2;
    procs[4].pid = 5; procs[4].arrival = 4; procs[4].burst = 3; procs[4].priority = 5;

    fifo_schedule(procs, 5);
    sjf_schedule(procs, 5);

    printf("Scheduler simulation complete\n"); // EXPECT: Scheduler simulation complete
    return 0;
}
