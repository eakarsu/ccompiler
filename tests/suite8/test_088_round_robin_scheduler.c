int printf(const char *fmt, ...);

enum TaskState { TASK_READY, TASK_RUNNING, TASK_BLOCKED, TASK_DONE };

struct Task {
    int id;
    int state;
    int remaining_time;
    int total_time;
    int wait_time;
    int turnaround_time;
    int priority;
};

struct Scheduler {
    struct Task tasks[8];
    int task_count;
    int current_task;
    int quantum;
    int clock;
    int completed;
    int context_switches;
};

void init_scheduler(struct Scheduler *s, int quantum) {
    s->task_count = 0;
    s->current_task = -1;
    s->quantum = quantum;
    s->clock = 0;
    s->completed = 0;
    s->context_switches = 0;
}

void add_task(struct Scheduler *s, int id, int burst_time, int priority) {
    if (s->task_count < 8) {
        int idx = s->task_count;
        s->tasks[idx].id = id;
        s->tasks[idx].state = TASK_READY;
        s->tasks[idx].remaining_time = burst_time;
        s->tasks[idx].total_time = burst_time;
        s->tasks[idx].wait_time = 0;
        s->tasks[idx].turnaround_time = 0;
        s->tasks[idx].priority = priority;
        s->task_count = s->task_count + 1;
    }
}

int find_next_ready(struct Scheduler *s, int start) {
    int i;
    int idx;
    for (i = 0; i < s->task_count; i = i + 1) {
        idx = (start + i) % s->task_count;
        if (s->tasks[idx].state == TASK_READY) return idx;
    }
    return -1;
}

int all_done(struct Scheduler *s) {
    int i;
    for (i = 0; i < s->task_count; i = i + 1) {
        if (s->tasks[i].state != TASK_DONE) return 0;
    }
    return 1;
}

void run_scheduler(struct Scheduler *s) {
    while (!all_done(s)) {
        int start_from;
        if (s->current_task >= 0)
            start_from = (s->current_task + 1) % s->task_count;
        else
            start_from = 0;

        int next = find_next_ready(s, start_from);
        if (next < 0) break;

        if (s->current_task != next) {
            s->context_switches = s->context_switches + 1;
        }
        s->current_task = next;
        s->tasks[next].state = TASK_RUNNING;

        int run_time;
        if (s->tasks[next].remaining_time < s->quantum)
            run_time = s->tasks[next].remaining_time;
        else
            run_time = s->quantum;

        printf("Clock %d: Task %d runs for %d\n", s->clock, s->tasks[next].id, run_time);

        int i;
        for (i = 0; i < s->task_count; i = i + 1) {
            if (i != next && s->tasks[i].state == TASK_READY) {
                s->tasks[i].wait_time = s->tasks[i].wait_time + run_time;
            }
        }

        s->tasks[next].remaining_time = s->tasks[next].remaining_time - run_time;
        s->clock = s->clock + run_time;

        if (s->tasks[next].remaining_time <= 0) {
            s->tasks[next].state = TASK_DONE;
            s->tasks[next].turnaround_time = s->clock;
            s->completed = s->completed + 1;
            printf("  Task %d DONE at clock=%d\n", s->tasks[next].id, s->clock);
        } else {
            s->tasks[next].state = TASK_READY;
        }
    }
}

const char *task_state_name(int st) {
    if (st == TASK_READY) return "READY";
    if (st == TASK_RUNNING) return "RUNNING";
    if (st == TASK_BLOCKED) return "BLOCKED";
    if (st == TASK_DONE) return "DONE";
    return "???";
}

void print_task_stats(struct Scheduler *s) {
    int i;
    for (i = 0; i < s->task_count; i = i + 1) {
        printf("Task %d: wait=%d turnaround=%d state=%s\n",
               s->tasks[i].id, s->tasks[i].wait_time,
               s->tasks[i].turnaround_time, task_state_name(s->tasks[i].state));
    }
}

int avg_wait(struct Scheduler *s) {
    int total = 0;
    int i;
    for (i = 0; i < s->task_count; i = i + 1) {
        total = total + s->tasks[i].wait_time;
    }
    return total / s->task_count;
}

int main() {
    struct Scheduler sched;
    init_scheduler(&sched, 3);

    add_task(&sched, 1, 6, 1);
    add_task(&sched, 2, 4, 2);
    add_task(&sched, 3, 2, 3);

    printf("=== Round Robin (quantum=3) ===\n");
    // EXPECT: === Round Robin (quantum=3) ===

    run_scheduler(&sched);
    // EXPECT: Clock 0: Task 1 runs for 3
    // EXPECT: Clock 3: Task 2 runs for 3
    // EXPECT: Clock 6: Task 3 runs for 2
    // EXPECT:   Task 3 DONE at clock=8
    // EXPECT: Clock 8: Task 1 runs for 3
    // EXPECT:   Task 1 DONE at clock=11
    // EXPECT: Clock 11: Task 2 runs for 1
    // EXPECT:   Task 2 DONE at clock=12

    printf("--- Stats ---\n");
    // EXPECT: --- Stats ---
    print_task_stats(&sched);
    // EXPECT: Task 1: wait=5 turnaround=11 state=DONE
    // EXPECT: Task 2: wait=8 turnaround=12 state=DONE
    // EXPECT: Task 3: wait=6 turnaround=8 state=DONE

    printf("Completed: %d\n", sched.completed);
    // EXPECT: Completed: 3
    printf("Context switches: %d\n", sched.context_switches);
    // EXPECT: Context switches: 5
    printf("Total clock: %d\n", sched.clock);
    // EXPECT: Total clock: 12
    printf("Avg wait: %d\n", avg_wait(&sched));
    // EXPECT: Avg wait: 6

    return 0;
}
