// stress_088_scheduler_sim.c - Process scheduler simulation: round-robin, priority
// EXPECT: rr_order=P3 P7 P0 P5 P6 P1 P2 P4
// EXPECT: rr_switches=16
// EXPECT: rr_total_turnaround=261
// EXPECT: rr_avg_turnaround=32
// EXPECT: pri_order=P7 P6 P5 P4 P3 P2 P1 P0
// EXPECT: pri_switches=7
// EXPECT: pri_total_turnaround=183
// EXPECT: pri_avg_turnaround=22
// EXPECT: gantt_len=44
// EXPECT: sjf_order=P7 P3 P0 P5 P6 P1 P2 P4
// EXPECT: sjf_total_turnaround=161
// EXPECT: idle_detected=0
// EXPECT: all_completed=1
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

struct Process {
    int pid;
    int burst_time;
    int remaining;
    int priority;     // lower = higher priority
    int arrival_time;
    int finish_time;
    int started;
    int completed;
};

void init_processes(struct Process *procs) {
    // 8 processes with different burst times and priorities
    //              pid, burst, remaining, priority, arrival, finish, started, completed
    procs[0].pid = 0; procs[0].burst_time = 5;  procs[0].remaining = 5;
    procs[0].priority = 8; procs[0].arrival_time = 0; procs[0].finish_time = 0;
    procs[0].started = 0; procs[0].completed = 0;

    procs[1].pid = 1; procs[1].burst_time = 7;  procs[1].remaining = 7;
    procs[1].priority = 7; procs[1].arrival_time = 0; procs[1].finish_time = 0;
    procs[1].started = 0; procs[1].completed = 0;

    procs[2].pid = 2; procs[2].burst_time = 8;  procs[2].remaining = 8;
    procs[2].priority = 6; procs[2].arrival_time = 0; procs[2].finish_time = 0;
    procs[2].started = 0; procs[2].completed = 0;

    procs[3].pid = 3; procs[3].burst_time = 3;  procs[3].remaining = 3;
    procs[3].priority = 5; procs[3].arrival_time = 0; procs[3].finish_time = 0;
    procs[3].started = 0; procs[3].completed = 0;

    procs[4].pid = 4; procs[4].burst_time = 8;  procs[4].remaining = 8;
    procs[4].priority = 4; procs[4].arrival_time = 0; procs[4].finish_time = 0;
    procs[4].started = 0; procs[4].completed = 0;

    procs[5].pid = 5; procs[5].burst_time = 5;  procs[5].remaining = 5;
    procs[5].priority = 3; procs[5].arrival_time = 0; procs[5].finish_time = 0;
    procs[5].started = 0; procs[5].completed = 0;

    procs[6].pid = 6; procs[6].burst_time = 6;  procs[6].remaining = 6;
    procs[6].priority = 2; procs[6].arrival_time = 0; procs[6].finish_time = 0;
    procs[6].started = 0; procs[6].completed = 0;

    procs[7].pid = 7; procs[7].burst_time = 2;  procs[7].remaining = 2;
    procs[7].priority = 1; procs[7].arrival_time = 0; procs[7].finish_time = 0;
    procs[7].started = 0; procs[7].completed = 0;
}

// Total burst = 5+7+8+3+8+5+6+2 = 44

// Round-robin scheduler with quantum=3
// Returns number of context switches
int round_robin(struct Process *procs, int n, int quantum, int *finish_order) {
    int time = 0;
    int switches = 0;
    int completed = 0;
    int current = 0;
    int order_idx = 0;
    int last_pid = -1;

    while (completed < n) {
        // Find next non-completed process starting from current
        int found = 0;
        int tries = 0;
        while (tries < n) {
            if (!procs[current].completed && procs[current].remaining > 0) {
                found = 1;
                break;
            }
            current = (current + 1) % n;
            tries++;
        }
        if (!found) break;

        // Context switch?
        if (last_pid != procs[current].pid) {
            if (last_pid != -1) switches++;
            last_pid = procs[current].pid;
        }

        if (!procs[current].started) {
            procs[current].started = 1;
        }

        // Execute for quantum or remaining time
        int exec_time = procs[current].remaining;
        if (exec_time > quantum) exec_time = quantum;
        procs[current].remaining -= exec_time;
        time += exec_time;

        if (procs[current].remaining == 0) {
            procs[current].completed = 1;
            procs[current].finish_time = time;
            finish_order[order_idx++] = procs[current].pid;
            completed++;
        }

        current = (current + 1) % n;
    }

    return switches;
}

// Priority scheduler (non-preemptive, lower number = higher priority)
int priority_schedule(struct Process *procs, int n, int *finish_order) {
    int time = 0;
    int switches = 0;
    int completed = 0;
    int order_idx = 0;
    int i;

    while (completed < n) {
        // Find highest priority (lowest number) non-completed process
        int best = -1;
        int best_pri = 9999;
        for (i = 0; i < n; i++) {
            if (!procs[i].completed && procs[i].priority < best_pri) {
                best_pri = procs[i].priority;
                best = i;
            }
        }
        if (best == -1) break;

        if (completed > 0) switches++;

        procs[best].started = 1;
        time += procs[best].burst_time;
        procs[best].remaining = 0;
        procs[best].completed = 1;
        procs[best].finish_time = time;
        finish_order[order_idx++] = procs[best].pid;
        completed++;
    }

    return switches;
}

// Shortest Job First (non-preemptive)
int sjf_schedule(struct Process *procs, int n, int *finish_order) {
    int time = 0;
    int completed = 0;
    int order_idx = 0;
    int i;

    while (completed < n) {
        int best = -1;
        int best_burst = 9999;
        for (i = 0; i < n; i++) {
            if (!procs[i].completed && procs[i].burst_time < best_burst) {
                best_burst = procs[i].burst_time;
                best = i;
            }
        }
        if (best == -1) break;

        time += procs[best].burst_time;
        procs[best].remaining = 0;
        procs[best].completed = 1;
        procs[best].finish_time = time;
        finish_order[order_idx++] = procs[best].pid;
        completed++;
    }

    return completed;
}

int main(void) {
    struct Process procs[8];
    int finish_order[8];
    int i;

    // --- Round-Robin ---
    init_processes(procs);
    int rr_switches = round_robin(procs, 8, 3, finish_order);

    // Print completion order
    printf("rr_order=");
    for (i = 0; i < 8; i++) {
        if (i > 0) printf(" ");
        printf("P%d", finish_order[i]);
    }
    printf("\n");

    printf("rr_switches=%d\n", rr_switches);

    // Compute turnaround times
    int total_tt = 0;
    for (i = 0; i < 8; i++) {
        total_tt += procs[i].finish_time;
    }
    printf("rr_total_turnaround=%d\n", total_tt);
    printf("rr_avg_turnaround=%d\n", total_tt / 8);

    // --- Priority Scheduling ---
    init_processes(procs);
    int pri_switches = priority_schedule(procs, 8, finish_order);

    printf("pri_order=");
    for (i = 0; i < 8; i++) {
        if (i > 0) printf(" ");
        printf("P%d", finish_order[i]);
    }
    printf("\n");

    printf("pri_switches=%d\n", pri_switches);

    total_tt = 0;
    for (i = 0; i < 8; i++) {
        total_tt += procs[i].finish_time;
    }
    printf("pri_total_turnaround=%d\n", total_tt);
    printf("pri_avg_turnaround=%d\n", total_tt / 8);

    // --- Gantt Chart (from round-robin) ---
    // Recreate RR and record gantt
    init_processes(procs);
    int gantt[64];
    int gantt_len = 0;
    {
        int time = 0;
        int completed = 0;
        int current = 0;
        int quantum = 3;

        while (completed < 8 && gantt_len < 64) {
            int found = 0;
            int tries = 0;
            while (tries < 8) {
                if (!procs[current].completed && procs[current].remaining > 0) {
                    found = 1;
                    break;
                }
                current = (current + 1) % 8;
                tries++;
            }
            if (!found) break;

            int exec_time = procs[current].remaining;
            if (exec_time > quantum) exec_time = quantum;

            int t;
            for (t = 0; t < exec_time && gantt_len < 64; t++) {
                gantt[gantt_len++] = procs[current].pid;
            }

            procs[current].remaining -= exec_time;
            time += exec_time;
            if (procs[current].remaining == 0) {
                procs[current].completed = 1;
                procs[current].finish_time = time;
                completed++;
            }
            current = (current + 1) % 8;
        }
    }
    // Total burst = 44, but gantt records each time unit
    // Actually wait. Let me recount: 5+7+8+3+8+5+6+2=44. Hmm but I said gantt_len=40. Let me check.
    // Actually it should be 44. Let me fix the EXPECT.
    // No wait, I wrote gantt_len=40 above. Let me recalculate properly.
    // Actually 5+7+8+3+8+5+6+2 = 44. So gantt_len should be 44.
    // But I already wrote EXPECT: gantt_len=40. I need to fix that.
    // Hmm, but I can't change the EXPECT comments since I already have them at the top.
    // Actually, I'm writing the file fresh. Let me just print whatever we get.
    // But I need to know the value for EXPECT. Let me trace through the RR:
    // All arrive at t=0. Quantum=3.
    // P0: burst=5, P1: burst=7, P2: burst=8, P3: burst=3, P4: burst=8, P5: burst=5, P6: burst=6, P7: burst=2
    // Round 1: P0(3,rem=2), P1(3,rem=4), P2(3,rem=5), P3(3,rem=0,done@12), P4(3,rem=5), P5(3,rem=2), P6(3,rem=3), P7(2,rem=0,done@23)
    // t=3: P0 rem=2
    // t=6: P1 rem=4
    // t=9: P2 rem=5
    // t=12: P3 rem=0, completed
    // t=15: P4 rem=5
    // t=18: P5 rem=2
    // t=21: P6 rem=3
    // t=23: P7 rem=0, completed (only 2 units)
    // Round 2: P0(2,rem=0,done@25), P1(3,rem=1), P2(3,rem=2), P4(3,rem=2), P5(2,rem=0,done@36), P6(3,rem=0,done@39)
    // t=25: P0 done
    // t=28: P1 rem=1
    // t=31: P2 rem=2
    // t=34: P4 rem=2
    // t=36: P5 done
    // t=39: P6 done
    // Round 3: P1(1,rem=0,done@40), P2(2,rem=0,done@42), P4(2,rem=0,done@44)
    // t=40: P1 done
    // t=42: P2 done
    // t=44: P4 done
    // Total time = 44. gantt_len = 44.
    // Completion order: P3(12), P7(23), P0(25), P1(40), P2(42), P5(36), P6(39), P4(44)
    // Wait, P5 finishes at 36 and P6 at 39, P1 at 40. So order: P3, P7, P0, P5, P6, P1, P2, P4
    // Hmm that doesn't match what I wrote above as "P0 P1 P2 P3 P4 P5 P6 P7".
    // Let me retrace the RR. Order: P0, P1, P2, P3, P4, P5, P6, P7 cyclic.
    //
    // Actually I realize the finish order from my round_robin function may differ from
    // what I calculated. Let me just compile and run with gcc to get the actual values.

    printf("gantt_len=%d\n", gantt_len);

    // --- SJF ---
    init_processes(procs);
    int sjf_completed = sjf_schedule(procs, 8, finish_order);

    printf("sjf_order=");
    for (i = 0; i < 8; i++) {
        if (i > 0) printf(" ");
        printf("P%d", finish_order[i]);
    }
    printf("\n");

    total_tt = 0;
    for (i = 0; i < 8; i++) {
        total_tt += procs[i].finish_time;
    }
    printf("sjf_total_turnaround=%d\n", total_tt);

    // Check for idle time (all arrivals at 0, so no idle)
    int idle = 0;
    printf("idle_detected=%d\n", idle);

    // Verify all completed
    int all_done = 1;
    for (i = 0; i < 8; i++) {
        if (!procs[i].completed) all_done = 0;
    }
    printf("all_completed=%d\n", all_done);

    printf("pass_done=1\n");
    return 0;
}
