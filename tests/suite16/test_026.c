int printf(const char *fmt, ...);

// Test 026: Multilevel Feedback Queue (3 queues with different quanta)
// Queue 0: quantum=2, Queue 1: quantum=4, Queue 2: FCFS

struct Task {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int queue_level;
    int finish;
    int waiting;
    int turnaround;
};

struct Event {
    int pid;
    int queue;
    int start;
    int end;
};

int run_mlfq(struct Task tasks[], int n, struct Event events[]) {
    int time;
    int completed;
    int i;
    int found;
    int best;
    int q;
    int quantum;
    int run_time;
    int evt_count;

    for (i = 0; i < n; i++) {
        tasks[i].remaining = tasks[i].burst;
        tasks[i].queue_level = 0;
    }

    time = 0;
    completed = 0;
    evt_count = 0;

    while (completed < n) {
        found = 0;
        for (q = 0; q < 3 && found == 0; q++) {
            best = -1;
            for (i = 0; i < n; i++) {
                if (tasks[i].remaining > 0 && tasks[i].arrival <= time && tasks[i].queue_level == q) {
                    if (best == -1 || tasks[i].arrival < tasks[best].arrival) {
                        best = i;
                    }
                }
            }
            if (best >= 0) {
                found = 1;
                if (q == 0) {
                    quantum = 2;
                } else if (q == 1) {
                    quantum = 4;
                } else {
                    quantum = tasks[best].remaining;
                }

                if (tasks[best].remaining <= quantum) {
                    run_time = tasks[best].remaining;
                } else {
                    run_time = quantum;
                }

                events[evt_count].pid = tasks[best].pid;
                events[evt_count].queue = q;
                events[evt_count].start = time;
                events[evt_count].end = time + run_time;
                evt_count = evt_count + 1;

                tasks[best].remaining = tasks[best].remaining - run_time;
                time = time + run_time;

                if (tasks[best].remaining == 0) {
                    tasks[best].finish = time;
                    tasks[best].turnaround = tasks[best].finish - tasks[best].arrival;
                    tasks[best].waiting = tasks[best].turnaround - tasks[best].burst;
                    completed = completed + 1;
                } else {
                    if (tasks[best].queue_level < 2) {
                        tasks[best].queue_level = tasks[best].queue_level + 1;
                    }
                }
            }
        }
        if (found == 0) {
            time = time + 1;
        }
    }
    return evt_count;
}

int main() {
    struct Task tasks[3];
    struct Event events[20];
    int evt_count;
    int i;
    int total_w;

    tasks[0].pid = 1; tasks[0].arrival = 0; tasks[0].burst = 10;
    tasks[1].pid = 2; tasks[1].arrival = 0; tasks[1].burst = 3;
    tasks[2].pid = 3; tasks[2].arrival = 0; tasks[2].burst = 1;

    printf("=== MLFQ Scheduling ===\n");         // EXPECT: === MLFQ Scheduling ===
    printf("Queue 0: quantum=2\n");              // EXPECT: Queue 0: quantum=2
    printf("Queue 1: quantum=4\n");              // EXPECT: Queue 1: quantum=4
    printf("Queue 2: FCFS\n");                   // EXPECT: Queue 2: FCFS

    evt_count = run_mlfq(tasks, 3, events);

    printf("Events (%d):\n", evt_count);         // EXPECT: Events (6):
    for (i = 0; i < evt_count; i++) {
        printf("  Q%d: P%d [%d-%d]\n", events[i].queue, events[i].pid,
               events[i].start, events[i].end);
    }
    // EXPECT:   Q0: P1 [0-2]
    // EXPECT:   Q0: P2 [2-4]
    // EXPECT:   Q0: P3 [4-5]
    // EXPECT:   Q1: P1 [5-9]
    // EXPECT:   Q1: P2 [9-10]
    // EXPECT:   Q2: P1 [10-14]

    printf("Results:\n");                        // EXPECT: Results:
    total_w = 0;
    for (i = 0; i < 3; i++) {
        printf("P%d: fin=%d wait=%d tat=%d\n",
               tasks[i].pid, tasks[i].finish,
               tasks[i].waiting, tasks[i].turnaround);
        total_w = total_w + tasks[i].waiting;
    }
    // EXPECT: P1: fin=14 wait=4 tat=14
    // EXPECT: P2: fin=10 wait=7 tat=10
    // EXPECT: P3: fin=5 wait=4 tat=5
    printf("Total waiting: %d\n", total_w);      // EXPECT: Total waiting: 15
    printf("Avg waiting: %d\n", total_w / 3);    // EXPECT: Avg waiting: 5
    printf("Done\n");                            // EXPECT: Done
    return 0;
}
