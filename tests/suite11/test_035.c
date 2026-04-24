int printf(const char *fmt, ...);

// CPU Time Slice Simulator
// Simulates round-robin CPU scheduling with time quantum

struct Process {
    int pid;
    int burst_time;
    int remaining;
    int arrival;
    int finish_time;
    int wait_time;
    int started;
};

struct CPUSim {
    struct Process procs[8];
    int count;
    int quantum;
    int current_time;
    int completed;
    int context_switches;
};

void sim_init(struct CPUSim *sim, int quantum) {
    sim->count = 0;
    sim->quantum = quantum;
    sim->current_time = 0;
    sim->completed = 0;
    sim->context_switches = 0;
}

void sim_add_process(struct CPUSim *sim, int pid, int burst, int arrival) {
    int idx = sim->count;
    sim->procs[idx].pid = pid;
    sim->procs[idx].burst_time = burst;
    sim->procs[idx].remaining = burst;
    sim->procs[idx].arrival = arrival;
    sim->procs[idx].finish_time = 0;
    sim->procs[idx].wait_time = 0;
    sim->procs[idx].started = 0;
    sim->count = sim->count + 1;
}

int sim_all_done(struct CPUSim *sim) {
    int i;
    for (i = 0; i < sim->count; i++) {
        if (sim->procs[i].remaining > 0) return 0;
    }
    return 1;
}

void sim_run(struct CPUSim *sim) {
    int queue[64];
    int qfront = 0;
    int qback = 0;
    int in_queue[8];
    int i;
    int done_count = 0;

    for (i = 0; i < sim->count; i++) in_queue[i] = 0;

    for (i = 0; i < sim->count; i++) {
        if (sim->procs[i].arrival == 0) {
            queue[qback] = i;
            qback = qback + 1;
            in_queue[i] = 1;
        }
    }

    while (done_count < sim->count) {
        if (qfront == qback) {
            sim->current_time = sim->current_time + 1;
            for (i = 0; i < sim->count; i++) {
                if (sim->procs[i].arrival <= sim->current_time && in_queue[i] == 0 && sim->procs[i].remaining > 0) {
                    queue[qback] = i;
                    qback = qback + 1;
                    in_queue[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[qfront];
        qfront = qfront + 1;

        if (sim->procs[idx].started == 0) {
            sim->procs[idx].started = 1;
        }

        int run_time = sim->quantum;
        if (sim->procs[idx].remaining < run_time) {
            run_time = sim->procs[idx].remaining;
        }

        sim->current_time = sim->current_time + run_time;
        sim->procs[idx].remaining = sim->procs[idx].remaining - run_time;
        sim->context_switches = sim->context_switches + 1;

        for (i = 0; i < sim->count; i++) {
            if (sim->procs[i].arrival <= sim->current_time && in_queue[i] == 0 && sim->procs[i].remaining > 0) {
                queue[qback] = i;
                qback = qback + 1;
                in_queue[i] = 1;
            }
        }

        if (sim->procs[idx].remaining == 0) {
            sim->procs[idx].finish_time = sim->current_time;
            sim->procs[idx].wait_time = sim->procs[idx].finish_time - sim->procs[idx].arrival - sim->procs[idx].burst_time;
            done_count = done_count + 1;
        } else {
            queue[qback] = idx;
            qback = qback + 1;
        }
    }
    sim->completed = done_count;
}

int avg_wait_time(struct CPUSim *sim) {
    int total = 0;
    int i;
    for (i = 0; i < sim->count; i++) {
        total = total + sim->procs[i].wait_time;
    }
    return total / sim->count;
}

int avg_turnaround(struct CPUSim *sim) {
    int total = 0;
    int i;
    for (i = 0; i < sim->count; i++) {
        total = total + (sim->procs[i].finish_time - sim->procs[i].arrival);
    }
    return total / sim->count;
}

int main() {
    struct CPUSim sim;
    int i;

    // EXPECT: === CPU Time Slice Simulator ===
    printf("=== CPU Time Slice Simulator ===\n");

    sim_init(&sim, 4);
    sim_add_process(&sim, 1, 10, 0);
    sim_add_process(&sim, 2, 5, 0);
    sim_add_process(&sim, 3, 8, 0);

    sim_run(&sim);

    // EXPECT: Quantum: 4
    printf("Quantum: %d\n", sim.quantum);
    // EXPECT: Process results:
    printf("Process results:\n");
    for (i = 0; i < sim.count; i++) {
        printf("  PID %d: burst=%d finish=%d wait=%d\n",
               sim.procs[i].pid, sim.procs[i].burst_time,
               sim.procs[i].finish_time, sim.procs[i].wait_time);
    }
    // EXPECT:   PID 1: burst=10 finish=23 wait=13
    // EXPECT:   PID 2: burst=5 finish=17 wait=12
    // EXPECT:   PID 3: burst=8 finish=21 wait=13

    // EXPECT: Context switches: 7
    printf("Context switches: %d\n", sim.context_switches);
    // EXPECT: Avg wait: 12
    printf("Avg wait: %d\n", avg_wait_time(&sim));
    // EXPECT: Avg turnaround: 20
    printf("Avg turnaround: %d\n", avg_turnaround(&sim));

    struct CPUSim sim2;
    sim_init(&sim2, 2);
    sim_add_process(&sim2, 10, 4, 0);
    sim_add_process(&sim2, 11, 2, 0);
    sim_add_process(&sim2, 12, 6, 0);
    sim_run(&sim2);

    // EXPECT: Small quantum test:
    printf("Small quantum test:\n");
    // EXPECT:   PID 10 finish: 8 wait: 4
    printf("  PID 10 finish: %d wait: %d\n", sim2.procs[0].finish_time, sim2.procs[0].wait_time);
    // EXPECT:   PID 11 finish: 4 wait: 2
    printf("  PID 11 finish: %d wait: %d\n", sim2.procs[1].finish_time, sim2.procs[1].wait_time);
    // EXPECT:   PID 12 finish: 12 wait: 6
    printf("  PID 12 finish: %d wait: %d\n", sim2.procs[2].finish_time, sim2.procs[2].wait_time);

    // EXPECT: All CPU simulator tests passed
    printf("All CPU simulator tests passed\n");
    return 0;
}
