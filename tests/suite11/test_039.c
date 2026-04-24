int printf(const char *fmt, ...);

// Rate Monotonic Scheduler (RMS)
// Fixed-priority scheduling where shorter period = higher priority

struct RMTask {
    int tid;
    int period;
    int exec_time;
    int remaining;
    int next_deadline;
    int next_release;
    int completed_jobs;
    int missed;
};

struct RMScheduler {
    struct RMTask tasks[6];
    int count;
    int tick;
    int idle_ticks;
};

void rms_init(struct RMScheduler *s) {
    s->count = 0;
    s->tick = 0;
    s->idle_ticks = 0;
}

void rms_add_task(struct RMScheduler *s, int tid, int period, int exec) {
    int idx = s->count;
    s->tasks[idx].tid = tid;
    s->tasks[idx].period = period;
    s->tasks[idx].exec_time = exec;
    s->tasks[idx].remaining = exec;
    s->tasks[idx].next_deadline = period;
    s->tasks[idx].next_release = period;
    s->tasks[idx].completed_jobs = 0;
    s->tasks[idx].missed = 0;
    s->count = s->count + 1;
}

int rms_utilization_pct(struct RMScheduler *s) {
    int total = 0;
    int i;
    for (i = 0; i < s->count; i++) {
        total = total + (s->tasks[i].exec_time * 100) / s->tasks[i].period;
    }
    return total;
}

int rms_schedulable_bound(int n) {
    // n * (2^(1/n) - 1) * 100 approximation for small n
    // n=1: 100, n=2: 82, n=3: 78
    if (n == 1) return 100;
    if (n == 2) return 82;
    if (n == 3) return 78;
    if (n == 4) return 75;
    return 69;
}

void rms_check_releases(struct RMScheduler *s) {
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->tick == s->tasks[i].next_release) {
            if (s->tasks[i].remaining > 0) {
                s->tasks[i].missed = s->tasks[i].missed + 1;
            }
            s->tasks[i].remaining = s->tasks[i].exec_time;
            s->tasks[i].next_deadline = s->tick + s->tasks[i].period;
            s->tasks[i].next_release = s->tick + s->tasks[i].period;
        }
    }
}

int rms_pick_task(struct RMScheduler *s) {
    int best = -1;
    int best_period = 1000000;
    int i;
    for (i = 0; i < s->count; i++) {
        if (s->tasks[i].remaining > 0 && s->tasks[i].period < best_period) {
            best_period = s->tasks[i].period;
            best = i;
        }
    }
    return best;
}

void rms_run_tick(struct RMScheduler *s, int *ran_tid) {
    rms_check_releases(s);
    int pick = rms_pick_task(s);
    if (pick < 0) {
        s->idle_ticks = s->idle_ticks + 1;
        *ran_tid = -1;
    } else {
        s->tasks[pick].remaining = s->tasks[pick].remaining - 1;
        *ran_tid = s->tasks[pick].tid;
        if (s->tasks[pick].remaining == 0) {
            s->tasks[pick].completed_jobs = s->tasks[pick].completed_jobs + 1;
        }
    }
    s->tick = s->tick + 1;
}

void rms_run_n_ticks(struct RMScheduler *s, int n) {
    int i;
    int ran;
    for (i = 0; i < n; i++) {
        rms_run_tick(s, &ran);
    }
}

int rms_total_completed(struct RMScheduler *s) {
    int total = 0;
    int i;
    for (i = 0; i < s->count; i++) {
        total = total + s->tasks[i].completed_jobs;
    }
    return total;
}

int rms_total_missed(struct RMScheduler *s) {
    int total = 0;
    int i;
    for (i = 0; i < s->count; i++) {
        total = total + s->tasks[i].missed;
    }
    return total;
}

int main() {
    struct RMScheduler s;
    int i;
    int ran;

    // EXPECT: === Rate Monotonic Scheduler ===
    printf("=== Rate Monotonic Scheduler ===\n");

    rms_init(&s);
    rms_add_task(&s, 1, 4, 1);
    rms_add_task(&s, 2, 6, 2);
    rms_add_task(&s, 3, 12, 3);

    int util = rms_utilization_pct(&s);
    int bound = rms_schedulable_bound(3);
    // EXPECT: Utilization: 83 percent
    printf("Utilization: %d percent\n", util);
    // EXPECT: RMS bound for 3 tasks: 78 percent
    printf("RMS bound for %d tasks: %d percent\n", s.count, bound);

    int schedulable = (util <= bound) ? 1 : 0;
    // EXPECT: Guaranteed schedulable: 0
    printf("Guaranteed schedulable: %d\n", schedulable);

    // EXPECT: RMS schedule (12 ticks):
    printf("RMS schedule (12 ticks):\n");
    for (i = 0; i < 12; i++) {
        rms_run_tick(&s, &ran);
        if (ran >= 0) {
            printf("  t=%d: Task %d\n", s.tick - 1, ran);
        } else {
            printf("  t=%d: idle\n", s.tick - 1);
        }
    }
    // EXPECT:   t=0: Task 1
    // EXPECT:   t=1: Task 2
    // EXPECT:   t=2: Task 2
    // EXPECT:   t=3: Task 3
    // EXPECT:   t=4: Task 1
    // EXPECT:   t=5: Task 3
    // EXPECT:   t=6: Task 2
    // EXPECT:   t=7: Task 2
    // EXPECT:   t=8: Task 1
    // EXPECT:   t=9: Task 3
    // EXPECT:   t=10: idle
    // EXPECT:   t=11: idle

    // EXPECT: Task stats after 12 ticks:
    printf("Task stats after 12 ticks:\n");
    for (i = 0; i < s.count; i++) {
        printf("  T%d(p=%d,e=%d): completed=%d missed=%d\n",
               s.tasks[i].tid, s.tasks[i].period, s.tasks[i].exec_time,
               s.tasks[i].completed_jobs, s.tasks[i].missed);
    }
    // EXPECT:   T1(p=4,e=1): completed=3 missed=0
    // EXPECT:   T2(p=6,e=2): completed=2 missed=0
    // EXPECT:   T3(p=12,e=3): completed=1 missed=0

    // EXPECT: Total completed: 6 missed: 0
    printf("Total completed: %d missed: %d\n", rms_total_completed(&s), rms_total_missed(&s));
    // EXPECT: Idle ticks: 2
    printf("Idle ticks: %d\n", s.idle_ticks);

    struct RMScheduler s2;
    rms_init(&s2);
    rms_add_task(&s2, 10, 5, 1);
    rms_add_task(&s2, 20, 10, 2);

    int util2 = rms_utilization_pct(&s2);
    // EXPECT: Test 2 utilization: 40 percent
    printf("Test 2 utilization: %d percent\n", util2);

    rms_run_n_ticks(&s2, 20);
    // EXPECT: Test 2 after 20 ticks:
    printf("Test 2 after 20 ticks:\n");
    // EXPECT:   T10: completed=4 missed=0
    printf("  T10: completed=%d missed=%d\n", s2.tasks[0].completed_jobs, s2.tasks[0].missed);
    // EXPECT:   T20: completed=2 missed=0
    printf("  T20: completed=%d missed=%d\n", s2.tasks[1].completed_jobs, s2.tasks[1].missed);

    // EXPECT: All RMS tests passed
    printf("All RMS tests passed\n");
    return 0;
}
