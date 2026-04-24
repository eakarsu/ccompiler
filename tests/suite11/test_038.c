int printf(const char *fmt, ...);

// Earliest Deadline First (EDF) Scheduler
// Preemptive scheduler that always runs the task with the nearest deadline

struct EDFTask {
    int tid;
    int period;
    int exec_time;
    int deadline;
    int remaining;
    int next_release;
    int completed_jobs;
    int missed_deadlines;
};

struct EDFScheduler {
    struct EDFTask tasks[6];
    int count;
    int tick;
    int idle_ticks;
    int total_preemptions;
};

void edf_init(struct EDFScheduler *s) {
    s->count = 0;
    s->tick = 0;
    s->idle_ticks = 0;
    s->total_preemptions = 0;
}

void edf_add_task(struct EDFScheduler *s, int tid, int period, int exec) {
    int idx = s->count;
    s->tasks[idx].tid = tid;
    s->tasks[idx].period = period;
    s->tasks[idx].exec_time = exec;
    s->tasks[idx].deadline = period;
    s->tasks[idx].remaining = exec;
    s->tasks[idx].next_release = period;
    s->tasks[idx].completed_jobs = 0;
    s->tasks[idx].missed_deadlines = 0;
    s->count = s->count + 1;
}

void edf_check_releases(struct EDFScheduler *s) {
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->tick == s->tasks[i].next_release) {
            if (s->tasks[i].remaining > 0) {
                s->tasks[i].missed_deadlines = s->tasks[i].missed_deadlines + 1;
            }
            s->tasks[i].remaining = s->tasks[i].exec_time;
            s->tasks[i].deadline = s->tick + s->tasks[i].period;
            s->tasks[i].next_release = s->tick + s->tasks[i].period;
        }
    }
}

int edf_pick_task(struct EDFScheduler *s) {
    int best = -1;
    int best_deadline = 1000000;
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->tasks[i].remaining > 0) {
            if (s->tasks[i].deadline < best_deadline) {
                best_deadline = s->tasks[i].deadline;
                best = i;
            }
        }
    }
    return best;
}

void edf_run_tick(struct EDFScheduler *s, int *ran_tid) {
    int prev_running;
    int next;

    edf_check_releases(s);

    prev_running = edf_pick_task(s);
    next = edf_pick_task(s);

    if (next < 0) {
        s->idle_ticks = s->idle_ticks + 1;
        *ran_tid = -1;
    } else {
        s->tasks[next].remaining = s->tasks[next].remaining - 1;
        *ran_tid = s->tasks[next].tid;
        if (s->tasks[next].remaining == 0) {
            s->tasks[next].completed_jobs = s->tasks[next].completed_jobs + 1;
        }
    }
    s->tick = s->tick + 1;
}

int edf_utilization_pct(struct EDFScheduler *s) {
    int total = 0;
    int i;
    for (i = 0; i < s->count; i++) {
        total = total + (s->tasks[i].exec_time * 100) / s->tasks[i].period;
    }
    return total;
}

int edf_total_missed(struct EDFScheduler *s) {
    int total = 0;
    int i;
    for (i = 0; i < s->count; i++) {
        total = total + s->tasks[i].missed_deadlines;
    }
    return total;
}

int edf_total_completed(struct EDFScheduler *s) {
    int total = 0;
    int i;
    for (i = 0; i < s->count; i++) {
        total = total + s->tasks[i].completed_jobs;
    }
    return total;
}

int main() {
    struct EDFScheduler s;
    int i;
    int ran;

    // EXPECT: === Earliest Deadline First ===
    printf("=== Earliest Deadline First ===\n");

    edf_init(&s);
    edf_add_task(&s, 1, 5, 2);
    edf_add_task(&s, 2, 7, 3);

    int util = edf_utilization_pct(&s);
    // EXPECT: Utilization: 82 percent
    printf("Utilization: %d percent\n", util);

    // EXPECT: EDF schedule (20 ticks):
    printf("EDF schedule (20 ticks):\n");
    for (i = 0; i < 20; i++) {
        edf_run_tick(&s, &ran);
        if (ran >= 0) {
            printf("  t=%d: Task %d\n", s.tick - 1, ran);
        } else {
            printf("  t=%d: idle\n", s.tick - 1);
        }
    }
    // EXPECT:   t=0: Task 1
    // EXPECT:   t=1: Task 1
    // EXPECT:   t=2: Task 2
    // EXPECT:   t=3: Task 2
    // EXPECT:   t=4: Task 2
    // EXPECT:   t=5: Task 1
    // EXPECT:   t=6: Task 1
    // EXPECT:   t=7: Task 2
    // EXPECT:   t=8: Task 2
    // EXPECT:   t=9: Task 2
    // EXPECT:   t=10: Task 1
    // EXPECT:   t=11: Task 1
    // EXPECT:   t=12: idle
    // EXPECT:   t=13: idle
    // EXPECT:   t=14: Task 2
    // EXPECT:   t=15: Task 1
    // EXPECT:   t=16: Task 1
    // EXPECT:   t=17: Task 2
    // EXPECT:   t=18: Task 2
    // EXPECT:   t=19: idle

    // EXPECT: After 20 ticks:
    printf("After 20 ticks:\n");
    // EXPECT:   Task 1: completed=4 missed=0
    printf("  Task 1: completed=%d missed=%d\n", s.tasks[0].completed_jobs, s.tasks[0].missed_deadlines);
    // EXPECT:   Task 2: completed=3 missed=0
    printf("  Task 2: completed=%d missed=%d\n", s.tasks[1].completed_jobs, s.tasks[1].missed_deadlines);

    int total_comp = edf_total_completed(&s);
    int total_miss = edf_total_missed(&s);
    // EXPECT: Total completed: 7 missed: 0
    printf("Total completed: %d missed: %d\n", total_comp, total_miss);
    // EXPECT: Idle ticks: 3
    printf("Idle ticks: %d\n", s.idle_ticks);

    // EXPECT: All EDF tests passed
    printf("All EDF tests passed\n");
    return 0;
}
