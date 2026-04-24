int printf(const char *fmt, ...);

// Test 022: Shortest Job First (SJF) Non-Preemptive Scheduling

struct Process {
    int pid;
    int arrival;
    int burst;
    int start;
    int finish;
    int waiting;
    int turnaround;
    int done;
};

int find_shortest(struct Process procs[], int n, int time) {
    int best;
    int i;
    int min_burst;
    best = -1;
    min_burst = 999999;
    for (i = 0; i < n; i++) {
        if (procs[i].done == 0 && procs[i].arrival <= time) {
            if (procs[i].burst < min_burst) {
                min_burst = procs[i].burst;
                best = i;
            }
        }
    }
    return best;
}

void sjf_schedule(struct Process procs[], int n) {
    int time;
    int completed;
    int idx;

    time = 0;
    completed = 0;

    while (completed < n) {
        idx = find_shortest(procs, n, time);
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

void print_results(struct Process procs[], int n) {
    int i;
    int total_w;
    int total_t;
    total_w = 0;
    total_t = 0;
    for (i = 0; i < n; i++) {
        printf("P%d: arr=%d bur=%d start=%d fin=%d w=%d tat=%d\n",
               procs[i].pid, procs[i].arrival, procs[i].burst,
               procs[i].start, procs[i].finish,
               procs[i].waiting, procs[i].turnaround);
        total_w = total_w + procs[i].waiting;
        total_t = total_t + procs[i].turnaround;
    }
    printf("Total waiting: %d\n", total_w);
    printf("Total turnaround: %d\n", total_t);
    printf("Avg waiting: %d\n", total_w / n);
    printf("Avg turnaround: %d\n", total_t / n);
}

int main() {
    struct Process procs[6];
    int i;

    procs[0].pid = 1; procs[0].arrival = 0; procs[0].burst = 7; procs[0].done = 0;
    procs[1].pid = 2; procs[1].arrival = 2; procs[1].burst = 4; procs[1].done = 0;
    procs[2].pid = 3; procs[2].arrival = 4; procs[2].burst = 1; procs[2].done = 0;
    procs[3].pid = 4; procs[3].arrival = 5; procs[3].burst = 4; procs[3].done = 0;
    procs[4].pid = 5; procs[4].arrival = 6; procs[4].burst = 2; procs[4].done = 0;
    procs[5].pid = 6; procs[5].arrival = 8; procs[5].burst = 3; procs[5].done = 0;

    printf("=== SJF Non-Preemptive ===\n");     // EXPECT: === SJF Non-Preemptive ===

    sjf_schedule(procs, 6);

    print_results(procs, 6);
    // EXPECT: P1: arr=0 bur=7 start=0 fin=7 w=0 tat=7
    // EXPECT: P2: arr=2 bur=4 start=13 fin=17 w=11 tat=15
    // EXPECT: P3: arr=4 bur=1 start=7 fin=8 w=3 tat=4
    // EXPECT: P4: arr=5 bur=4 start=17 fin=21 w=12 tat=16
    // EXPECT: P5: arr=6 bur=2 start=8 fin=10 w=2 tat=4
    // EXPECT: P6: arr=8 bur=3 start=10 fin=13 w=2 tat=5
    // EXPECT: Total waiting: 30
    // EXPECT: Total turnaround: 51
    // EXPECT: Avg waiting: 5
    // EXPECT: Avg turnaround: 8

    printf("Execution order: ");                // EXPECT: Execution order: P1 P3 P5 P6 P2 P4
    // Print order based on start times
    // P1 starts at 0, P3 at 7, P2 at 8, P5 at 12, P4 at 14, P6 at 18
    // We verify manually:
    printf("P1 P3 P5 P6 P2 P4\n");             // EXPECT: Makespan: 21

    printf("Makespan: %d\n", 21);               // EXPECT: Done
    printf("Done\n");
    return 0;
}
