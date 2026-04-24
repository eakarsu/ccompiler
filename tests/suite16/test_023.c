int printf(const char *fmt, ...);

// Test 023: Round-Robin Scheduling

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int finish;
    int waiting;
    int turnaround;
};

struct GanttEntry {
    int pid;
    int start;
    int end;
};

int round_robin(struct Process procs[], int n, int quantum, struct GanttEntry gantt[]) {
    int time;
    int done;
    int count;
    int i;
    int gantt_len;
    int all_done;

    time = 0;
    gantt_len = 0;

    for (i = 0; i < n; i++) {
        procs[i].remaining = procs[i].burst;
    }

    all_done = 0;
    while (all_done == 0) {
        all_done = 1;
        for (i = 0; i < n; i++) {
            if (procs[i].remaining > 0 && procs[i].arrival <= time) {
                all_done = 0;
                gantt[gantt_len].pid = procs[i].pid;
                gantt[gantt_len].start = time;

                if (procs[i].remaining <= quantum) {
                    time = time + procs[i].remaining;
                    procs[i].remaining = 0;
                    procs[i].finish = time;
                } else {
                    time = time + quantum;
                    procs[i].remaining = procs[i].remaining - quantum;
                }

                gantt[gantt_len].end = time;
                gantt_len = gantt_len + 1;
            }
        }
        if (all_done == 0) {
            // Check if any process is still not arrived
            done = 1;
            for (i = 0; i < n; i++) {
                if (procs[i].remaining > 0) {
                    done = 0;
                }
            }
            if (done) {
                all_done = 1;
            } else {
                // Check if we are idle
                count = 0;
                for (i = 0; i < n; i++) {
                    if (procs[i].remaining > 0 && procs[i].arrival <= time) {
                        count = count + 1;
                    }
                }
                if (count == 0) {
                    time = time + 1;
                }
            }
        }
    }

    for (i = 0; i < n; i++) {
        procs[i].turnaround = procs[i].finish - procs[i].arrival;
        procs[i].waiting = procs[i].turnaround - procs[i].burst;
    }

    return gantt_len;
}

int main() {
    struct Process procs[4];
    struct GanttEntry gantt[30];
    int gantt_len;
    int i;
    int total_w;
    int total_t;

    procs[0].pid = 1; procs[0].arrival = 0; procs[0].burst = 5;
    procs[1].pid = 2; procs[1].arrival = 1; procs[1].burst = 4;
    procs[2].pid = 3; procs[2].arrival = 2; procs[2].burst = 2;
    procs[3].pid = 4; procs[3].arrival = 3; procs[3].burst = 1;

    printf("=== Round Robin (quantum=2) ===\n"); // EXPECT: === Round Robin (quantum=2) ===

    gantt_len = round_robin(procs, 4, 2, gantt);

    for (i = 0; i < 4; i++) {
        printf("P%d: burst=%d finish=%d wait=%d tat=%d\n",
               procs[i].pid, procs[i].burst, procs[i].finish,
               procs[i].waiting, procs[i].turnaround);
    }
    // EXPECT: P1: burst=5 finish=12 wait=7 tat=12
    // EXPECT: P2: burst=4 finish=11 wait=6 tat=10
    // EXPECT: P3: burst=2 finish=6 wait=2 tat=4
    // EXPECT: P4: burst=1 finish=7 wait=3 tat=4

    total_w = 0;
    total_t = 0;
    for (i = 0; i < 4; i++) {
        total_w = total_w + procs[i].waiting;
        total_t = total_t + procs[i].turnaround;
    }
    printf("Avg wait: %d\n", total_w / 4);      // EXPECT: Avg wait: 4
    printf("Avg tat: %d\n", total_t / 4);        // EXPECT: Avg tat: 7

    printf("Gantt chart (%d entries):\n", gantt_len); // EXPECT: Gantt chart (7 entries):
    for (i = 0; i < gantt_len; i++) {
        printf("  P%d [%d-%d]\n", gantt[i].pid, gantt[i].start, gantt[i].end);
    }
    // EXPECT:   P1 [0-2]
    // EXPECT:   P2 [2-4]
    // EXPECT:   P3 [4-6]
    // EXPECT:   P4 [6-7]
    // EXPECT:   P1 [7-9]
    // EXPECT:   P2 [9-11]
    // EXPECT:   P1 [11-12]

    printf("Done\n");                            // EXPECT: Done
    return 0;
}
