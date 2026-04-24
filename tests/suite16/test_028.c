int printf(const char *fmt, ...);

// Test 028: Gantt Chart Generator with CPU Utilization Analysis

struct Proc {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int done;
    int finish;
};

struct GanttBar {
    int pid;   // 0 means idle
    int start;
    int end;
};

int generate_gantt_rr(struct Proc procs[], int n, int quantum, struct GanttBar bars[]) {
    int time;
    int completed;
    int i;
    int bar_count;
    int ran;

    for (i = 0; i < n; i++) {
        procs[i].remaining = procs[i].burst;
        procs[i].done = 0;
    }

    time = 0;
    completed = 0;
    bar_count = 0;

    while (completed < n) {
        ran = 0;
        for (i = 0; i < n; i++) {
            if (procs[i].remaining > 0 && procs[i].arrival <= time) {
                int slice;
                if (procs[i].remaining < quantum) {
                    slice = procs[i].remaining;
                } else {
                    slice = quantum;
                }
                bars[bar_count].pid = procs[i].pid;
                bars[bar_count].start = time;
                bars[bar_count].end = time + slice;
                bar_count = bar_count + 1;

                time = time + slice;
                procs[i].remaining = procs[i].remaining - slice;
                if (procs[i].remaining == 0) {
                    procs[i].done = 1;
                    procs[i].finish = time;
                    completed = completed + 1;
                }
                ran = 1;
            }
        }
        if (ran == 0) {
            // Idle slot
            bars[bar_count].pid = 0;
            bars[bar_count].start = time;
            bars[bar_count].end = time + 1;
            bar_count = bar_count + 1;
            time = time + 1;
        }
    }
    return bar_count;
}

void print_gantt_chart(struct GanttBar bars[], int count) {
    int i;
    int width;
    // Print top border
    printf("+");                                 // EXPECT_PARTIAL
    for (i = 0; i < count; i++) {
        width = bars[i].end - bars[i].start;
        int j;
        for (j = 0; j < width * 3; j++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n");

    // Print process IDs
    printf("|");
    for (i = 0; i < count; i++) {
        width = bars[i].end - bars[i].start;
        if (bars[i].pid == 0) {
            if (width == 1) {
                printf(" . ");
            } else {
                printf(" ..");
                int j;
                for (j = 1; j < width; j++) {
                    printf("   ");
                }
            }
        } else {
            printf("P%d ", bars[i].pid);
            int j;
            for (j = 1; j < width; j++) {
                printf("   ");
            }
        }
        printf("|");
    }
    printf("\n");

    // Print bottom border
    printf("+");
    for (i = 0; i < count; i++) {
        width = bars[i].end - bars[i].start;
        int j;
        for (j = 0; j < width * 3; j++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n");
}

int compute_utilization_pct(struct GanttBar bars[], int count, int total) {
    int busy;
    int i;
    busy = 0;
    for (i = 0; i < count; i++) {
        if (bars[i].pid != 0) {
            busy = busy + (bars[i].end - bars[i].start);
        }
    }
    return (busy * 100) / total;
}

int main() {
    struct Proc procs[3];
    struct GanttBar bars[30];
    int bar_count;
    int total_time;
    int util;
    int i;
    int ctx_switches;

    procs[0].pid = 1; procs[0].arrival = 0; procs[0].burst = 6;
    procs[1].pid = 2; procs[1].arrival = 2; procs[1].burst = 4;
    procs[2].pid = 3; procs[2].arrival = 4; procs[2].burst = 2;

    printf("=== Gantt Chart Generator ===\n");   // EXPECT: === Gantt Chart Generator ===

    bar_count = generate_gantt_rr(procs, 3, 2, bars);

    printf("Gantt entries: %d\n", bar_count);    // EXPECT: Gantt entries: 6

    for (i = 0; i < bar_count; i++) {
        if (bars[i].pid == 0) {
            printf("[IDLE %d-%d] ", bars[i].start, bars[i].end);
        } else {
            printf("[P%d %d-%d] ", bars[i].pid, bars[i].start, bars[i].end);
        }
    }
    printf("\n");
    // EXPECT: [P1 0-2] [P2 2-4] [P3 4-6] [P1 6-8] [P2 8-10] [P1 10-12] 

    total_time = bars[bar_count - 1].end;
    printf("Total time: %d\n", total_time);      // EXPECT: Total time: 12

    util = compute_utilization_pct(bars, bar_count, total_time);
    printf("CPU utilization: %d\n", util);       // EXPECT: CPU utilization: 100

    ctx_switches = bar_count - 1;
    printf("Context switches: %d\n", ctx_switches); // EXPECT: Context switches: 5

    for (i = 0; i < 3; i++) {
        printf("P%d finished at %d\n", procs[i].pid, procs[i].finish);
    }
    // EXPECT: P1 finished at 12
    // EXPECT: P2 finished at 10
    // EXPECT: P3 finished at 6

    printf("Done\n");                            // EXPECT: Done
    return 0;
}
