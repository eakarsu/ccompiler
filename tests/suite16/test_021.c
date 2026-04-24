int printf(const char *fmt, ...);

// Test 021: First-Come First-Served (FCFS) Scheduling

struct Process {
    int pid;
    int arrival;
    int burst;
    int start;
    int finish;
    int waiting;
    int turnaround;
};

void sort_by_arrival(struct Process procs[], int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (procs[j].arrival > procs[j + 1].arrival) {
                struct Process tmp;
                tmp = procs[j];
                procs[j] = procs[j + 1];
                procs[j + 1] = tmp;
            }
        }
    }
}

void fcfs_schedule(struct Process procs[], int n) {
    int current_time;
    int i;

    sort_by_arrival(procs, n);

    current_time = 0;
    for (i = 0; i < n; i++) {
        if (current_time < procs[i].arrival) {
            current_time = procs[i].arrival;
        }
        procs[i].start = current_time;
        procs[i].finish = current_time + procs[i].burst;
        procs[i].waiting = procs[i].start - procs[i].arrival;
        procs[i].turnaround = procs[i].finish - procs[i].arrival;
        current_time = procs[i].finish;
    }
}

int compute_avg_waiting(struct Process procs[], int n) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < n; i++) {
        total = total + procs[i].waiting;
    }
    return total / n;
}

int compute_avg_turnaround(struct Process procs[], int n) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < n; i++) {
        total = total + procs[i].turnaround;
    }
    return total / n;
}

void print_gantt(struct Process procs[], int n) {
    int i;
    printf("Gantt: ");                          // EXPECT: === FCFS Scheduling ===
    for (i = 0; i < n; i++) {
        printf("[P%d:%d-%d] ", procs[i].pid, procs[i].start, procs[i].finish); // EXPECT_DYNAMIC
    }
    printf("\n");
}

int main() {
    struct Process procs[5];
    int i;
    int avg_w;
    int avg_t;

    procs[0].pid = 1; procs[0].arrival = 0; procs[0].burst = 5;
    procs[1].pid = 2; procs[1].arrival = 1; procs[1].burst = 3;
    procs[2].pid = 3; procs[2].arrival = 2; procs[2].burst = 8;
    procs[3].pid = 4; procs[3].arrival = 3; procs[3].burst = 6;
    procs[4].pid = 5; procs[4].arrival = 4; procs[4].burst = 4;

    printf("=== FCFS Scheduling ===\n");        // EXPECT: Process schedule:

    fcfs_schedule(procs, 5);

    printf("Process schedule:\n");               // EXPECT: P1: arrival=0 burst=5 start=0 finish=5 wait=0 tat=5
    for (i = 0; i < 5; i++) {
        printf("P%d: arrival=%d burst=%d start=%d finish=%d wait=%d tat=%d\n",
               procs[i].pid, procs[i].arrival, procs[i].burst,
               procs[i].start, procs[i].finish, procs[i].waiting,
               procs[i].turnaround);
    }
    // EXPECT: P2: arrival=1 burst=3 start=5 finish=8 wait=4 tat=7
    // EXPECT: P3: arrival=2 burst=8 start=8 finish=16 wait=6 tat=14
    // EXPECT: P4: arrival=3 burst=6 start=16 finish=22 wait=13 tat=19
    // EXPECT: P5: arrival=4 burst=4 start=22 finish=26 wait=18 tat=22
    // EXPECT: Avg waiting: 8

    avg_w = compute_avg_waiting(procs, 5);
    avg_t = compute_avg_turnaround(procs, 5);
    printf("Avg waiting: %d\n", avg_w);          // EXPECT: Avg turnaround: 13
    printf("Avg turnaround: %d\n", avg_t);       // EXPECT: Gantt: [P1:0-5] [P2:5-8] [P3:8-16] [P4:16-22] [P5:22-26] 

    print_gantt(procs, 5);

    printf("Total time: %d\n", procs[4].finish); // EXPECT: Total time: 26
    printf("Done\n");                             // EXPECT: Done
    return 0;
}
