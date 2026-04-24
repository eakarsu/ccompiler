int printf(const char *fmt, ...);
// EXPECT: === FCFS Scheduling ===\n  t=0: Run Init (burst=3) -> done at t=3, wait=0\n  t=3: Run Shell (burst=5) -> done at t=8, wait=2\n  t=8: Run Compile (burst=8) -> done at t=16, wait=6\n  t=16: Run Editor (burst=4) -> done at t=20, wait=13\n  t=20: Run Browser (burst=6) -> done at t=26, wait=16\n  Avg wait: 7, Avg turnaround: 12\nProcess summary:\n  PID 1 (Init): burst=3 arrival=0\n  PID 2 (Shell): burst=5 arrival=1\n  PID 3 (Compile): burst=8 arrival=2\n  PID 4 (Editor): burst=4 arrival=3\n  PID 5 (Browser): burst=6 arrival=4

#define MAX_PROCS 6

enum ProcState {
    PROC_READY = 0,
    PROC_RUNNING = 1,
    PROC_DONE = 2,
    PROC_WAITING = 3
};

struct Process {
    int pid;
    char *name;
    int burst_time;
    int remaining;
    int arrival;
    int priority;
    int state;
    int start_time;
    int end_time;
    int wait_time;
};

struct Scheduler {
    struct Process procs[MAX_PROCS];
    int count;
    int current_time;
    int completed;
};

void sched_init(struct Scheduler *s) {
    s->count = 0;
    s->current_time = 0;
    s->completed = 0;
}

void sched_add(struct Scheduler *s, char *name, int burst, int arrival, int prio) {
    if (s->count >= MAX_PROCS) return;
    int i = s->count;
    s->procs[i].pid = i + 1;
    s->procs[i].name = name;
    s->procs[i].burst_time = burst;
    s->procs[i].remaining = burst;
    s->procs[i].arrival = arrival;
    s->procs[i].priority = prio;
    s->procs[i].state = PROC_READY;
    s->procs[i].start_time = -1;
    s->procs[i].end_time = -1;
    s->procs[i].wait_time = 0;
    s->count++;
}

void run_fcfs(struct Scheduler *s) {
    printf("=== FCFS Scheduling ===\n");
    s->current_time = 0;
    s->completed = 0;
    int done[MAX_PROCS];
    int i;
    for (i = 0; i < s->count; i++) {
        done[i] = 0;
        s->procs[i].remaining = s->procs[i].burst_time;
    }

    while (s->completed < s->count) {
        int next = -1;
        int i;
        for (i = 0; i < s->count; i++) {
            if (!done[i] && s->procs[i].arrival <= s->current_time) {
                if (next == -1 || s->procs[i].arrival < s->procs[next].arrival) {
                    next = i;
                }
            }
        }
        if (next == -1) {
            s->current_time++;
            continue;
        }
        s->procs[next].start_time = s->current_time;
        s->procs[next].wait_time = s->current_time - s->procs[next].arrival;
        s->current_time = s->current_time + s->procs[next].burst_time;
        s->procs[next].end_time = s->current_time;
        done[next] = 1;
        s->completed++;
        printf("  t=%d: Run %s (burst=%d) -> done at t=%d, wait=%d\n",
               s->procs[next].start_time, s->procs[next].name,
               s->procs[next].burst_time, s->procs[next].end_time,
               s->procs[next].wait_time);
    }

    int total_wait = 0;
    int total_turnaround = 0;
    for (i = 0; i < s->count; i++) {
        total_wait = total_wait + s->procs[i].wait_time;
        total_turnaround = total_turnaround + (s->procs[i].end_time - s->procs[i].arrival);
    }
    printf("  Avg wait: %d, Avg turnaround: %d\n",
           total_wait / s->count, total_turnaround / s->count);
}

int main(void) {
    struct Scheduler sched;
    sched_init(&sched);

    sched_add(&sched, "Init", 3, 0, 1);
    sched_add(&sched, "Shell", 5, 1, 2);
    sched_add(&sched, "Compile", 8, 2, 3);
    sched_add(&sched, "Editor", 4, 3, 2);
    sched_add(&sched, "Browser", 6, 4, 4);

    run_fcfs(&sched);

    printf("Process summary:\n");
    int i;
    for (i = 0; i < sched.count; i++) {
        printf("  PID %d (%s): burst=%d arrival=%d\n",
               sched.procs[i].pid, sched.procs[i].name,
               sched.procs[i].burst_time, sched.procs[i].arrival);
    }

    return 0;
}
