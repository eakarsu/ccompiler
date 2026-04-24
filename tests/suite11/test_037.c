int printf(const char *fmt, ...);

// Priority Aging Scheduler
// Tasks have priorities that age (increase) over time to prevent starvation

struct Task {
    int tid;
    int base_priority;
    int effective_priority;
    int remaining;
    int wait_time;
    int completed;
    int finish_tick;
};

struct AgingScheduler {
    struct Task tasks[8];
    int count;
    int tick;
    int aging_rate;
    int completed_count;
};

void sched_init(struct AgingScheduler *s, int aging_rate) {
    s->count = 0;
    s->tick = 0;
    s->aging_rate = aging_rate;
    s->completed_count = 0;
}

void sched_add_task(struct AgingScheduler *s, int tid, int priority, int burst) {
    int idx = s->count;
    s->tasks[idx].tid = tid;
    s->tasks[idx].base_priority = priority;
    s->tasks[idx].effective_priority = priority;
    s->tasks[idx].remaining = burst;
    s->tasks[idx].wait_time = 0;
    s->tasks[idx].completed = 0;
    s->tasks[idx].finish_tick = 0;
    s->count = s->count + 1;
}

int sched_pick_highest(struct AgingScheduler *s) {
    int best = -1;
    int best_prio = -1;
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->tasks[i].completed == 0 && s->tasks[i].effective_priority > best_prio) {
            best_prio = s->tasks[i].effective_priority;
            best = i;
        }
    }
    return best;
}

void sched_age_all(struct AgingScheduler *s, int running_idx) {
    int i;
    for (i = 0; i < s->count; i++) {
        if (i != running_idx && s->tasks[i].completed == 0) {
            s->tasks[i].wait_time = s->tasks[i].wait_time + 1;
            if (s->tasks[i].wait_time % s->aging_rate == 0) {
                s->tasks[i].effective_priority = s->tasks[i].effective_priority + 1;
            }
        }
    }
}

void sched_run_tick(struct AgingScheduler *s) {
    int idx = sched_pick_highest(s);
    if (idx < 0) return;

    s->tasks[idx].remaining = s->tasks[idx].remaining - 1;
    s->tasks[idx].wait_time = 0;
    s->tasks[idx].effective_priority = s->tasks[idx].base_priority;

    sched_age_all(s, idx);
    s->tick = s->tick + 1;

    if (s->tasks[idx].remaining == 0) {
        s->tasks[idx].completed = 1;
        s->tasks[idx].finish_tick = s->tick;
        s->completed_count = s->completed_count + 1;
    }
}

int sched_all_done(struct AgingScheduler *s) {
    return s->completed_count == s->count;
}

void sched_run_to_completion(struct AgingScheduler *s) {
    int safety = 0;
    while (sched_all_done(s) == 0 && safety < 200) {
        sched_run_tick(s);
        safety = safety + 1;
    }
}

int sched_check_starvation(struct AgingScheduler *s, int max_wait) {
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->tasks[i].finish_tick > s->tasks[i].remaining + max_wait) {
            return 1;
        }
    }
    return 0;
}

int main() {
    struct AgingScheduler s;
    int i;

    // EXPECT: === Priority Aging Scheduler ===
    printf("=== Priority Aging Scheduler ===\n");

    sched_init(&s, 2);
    sched_add_task(&s, 1, 10, 5);
    sched_add_task(&s, 2, 5, 3);
    sched_add_task(&s, 3, 1, 2);

    // EXPECT: Initial priorities:
    printf("Initial priorities:\n");
    for (i = 0; i < s.count; i++) {
        printf("  Task %d: prio=%d burst=%d\n", s.tasks[i].tid,
               s.tasks[i].base_priority, s.tasks[i].remaining);
    }
    // EXPECT:   Task 1: prio=10 burst=5
    // EXPECT:   Task 2: prio=5 burst=3
    // EXPECT:   Task 3: prio=1 burst=2

    // Run first 5 ticks and trace
    // EXPECT: Running 5 ticks:
    printf("Running 5 ticks:\n");
    for (i = 0; i < 5; i++) {
        int pick = sched_pick_highest(&s);
        printf("  Tick %d: run Task %d (eff_prio=%d)\n", s.tick,
               s.tasks[pick].tid, s.tasks[pick].effective_priority);
        sched_run_tick(&s);
    }
    // EXPECT:   Tick 0: run Task 1 (eff_prio=10)
    // EXPECT:   Tick 1: run Task 1 (eff_prio=10)
    // EXPECT:   Tick 2: run Task 1 (eff_prio=10)
    // EXPECT:   Tick 3: run Task 1 (eff_prio=10)
    // EXPECT:   Tick 4: run Task 1 (eff_prio=10)

    // EXPECT: After 5 ticks, Task 1 completed: 1
    printf("After 5 ticks, Task 1 completed: %d\n", s.tasks[0].completed);

    // EXPECT: Effective priorities after aging:
    printf("Effective priorities after aging:\n");
    for (i = 0; i < s.count; i++) {
        printf("  Task %d: eff=%d base=%d\n", s.tasks[i].tid,
               s.tasks[i].effective_priority, s.tasks[i].base_priority);
    }
    // EXPECT:   Task 1: eff=10 base=10
    // EXPECT:   Task 2: eff=7 base=5
    // EXPECT:   Task 3: eff=3 base=1

    sched_run_to_completion(&s);

    // EXPECT: All tasks completed at tick: 10
    printf("All tasks completed at tick: %d\n", s.tick);
    // EXPECT: Task finish times:
    printf("Task finish times:\n");
    for (i = 0; i < s.count; i++) {
        printf("  Task %d finished at tick %d\n", s.tasks[i].tid, s.tasks[i].finish_tick);
    }
    // EXPECT:   Task 1 finished at tick 5
    // EXPECT:   Task 2 finished at tick 8
    // EXPECT:   Task 3 finished at tick 10

    struct AgingScheduler s2;
    sched_init(&s2, 1);
    sched_add_task(&s2, 10, 100, 8);
    sched_add_task(&s2, 11, 1, 2);

    sched_run_to_completion(&s2);
    // EXPECT: High vs low priority:
    printf("High vs low priority:\n");
    // EXPECT:   Task 10 finished at: 8
    printf("  Task 10 finished at: %d\n", s2.tasks[0].finish_tick);
    // EXPECT:   Task 11 finished at: 10
    printf("  Task 11 finished at: %d\n", s2.tasks[1].finish_tick);
    // EXPECT:   Total ticks: 10
    printf("  Total ticks: %d\n", s2.tick);

    // EXPECT: All priority aging tests passed
    printf("All priority aging tests passed\n");
    return 0;
}
