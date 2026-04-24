int printf(const char *fmt, ...);

struct Task {
    int id;
    int remaining_time;
    int priority;
    int completed;
    int completion_order;
};

int init_task(struct Task *t, int id, int remaining, int prio) {
    t->id = id;
    t->remaining_time = remaining;
    t->priority = prio;
    t->completed = 0;
    t->completion_order = -1;
    return 0;
}

int all_done(struct Task *tasks, int n) {
    int i;
    i = 0;
    while (i < n) {
        if (tasks[i].completed == 0) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

int count_completed(struct Task *tasks, int n) {
    int i;
    int count;
    count = 0;
    i = 0;
    while (i < n) {
        if (tasks[i].completed) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

int total_remaining(struct Task *tasks, int n) {
    int i;
    int total;
    total = 0;
    i = 0;
    while (i < n) {
        total = total + tasks[i].remaining_time;
        i = i + 1;
    }
    return total;
}

int find_highest_priority_pending(struct Task *tasks, int n) {
    int i;
    int best;
    int best_prio;
    best = -1;
    best_prio = 9999;
    i = 0;
    while (i < n) {
        if (tasks[i].completed == 0 && tasks[i].priority < best_prio) {
            best_prio = tasks[i].priority;
            best = i;
        }
        i = i + 1;
    }
    return best;
}

int run_round_robin(struct Task *tasks, int n, int quantum, int *order, int *order_count) {
    int current_time;
    int completed_count;
    int idx;
    int slice;

    current_time = 0;
    completed_count = 0;
    *order_count = 0;

    while (completed_count < n) {
        idx = 0;
        while (idx < n) {
            if (tasks[idx].completed == 0) {
                if (tasks[idx].remaining_time <= quantum) {
                    slice = tasks[idx].remaining_time;
                } else {
                    slice = quantum;
                }
                current_time = current_time + slice;
                tasks[idx].remaining_time = tasks[idx].remaining_time - slice;
                if (tasks[idx].remaining_time == 0) {
                    tasks[idx].completed = 1;
                    tasks[idx].completion_order = completed_count;
                    order[*order_count] = tasks[idx].id;
                    *order_count = *order_count + 1;
                    completed_count = completed_count + 1;
                }
            }
            idx = idx + 1;
        }
    }
    return current_time;
}

int run_priority_schedule(struct Task *tasks, int n, int *order, int *order_count) {
    int current_time;
    int completed_count;
    int best;

    current_time = 0;
    completed_count = 0;
    *order_count = 0;

    while (completed_count < n) {
        best = find_highest_priority_pending(tasks, n);
        if (best < 0) {
            break;
        }
        current_time = current_time + tasks[best].remaining_time;
        tasks[best].remaining_time = 0;
        tasks[best].completed = 1;
        tasks[best].completion_order = completed_count;
        order[*order_count] = tasks[best].id;
        *order_count = *order_count + 1;
        completed_count = completed_count + 1;
    }
    return current_time;
}

int main(void) {
    struct Task tasks[8];
    int order[8];
    int order_count;
    int total_time;
    int i;

    /* Test 1: Basic round-robin with quantum=2, 3 tasks */
    init_task(&tasks[0], 1, 4, 1);
    init_task(&tasks[1], 2, 3, 2);
    init_task(&tasks[2], 3, 1, 3);

    // EXPECT: 8
    printf("%d\n", total_remaining(tasks, 3));

    total_time = run_round_robin(tasks, 3, 2, order, &order_count);

    // EXPECT: 3
    printf("%d\n", order_count);

    /* Task 3 (time=1) completes first pass, task 2 (time=3) completes second pass, task 1 (time=4) last */
    // EXPECT: 3
    printf("%d\n", order[0]);

    // EXPECT: 1
    printf("%d\n", order[1]);

    // EXPECT: 2
    printf("%d\n", order[2]);

    // EXPECT: 8
    printf("%d\n", total_time);

    // EXPECT: 3
    printf("%d\n", count_completed(tasks, 3));

    // EXPECT: 1
    printf("%d\n", all_done(tasks, 3));

    /* Test 2: Round-robin with quantum=1, 4 tasks */
    init_task(&tasks[0], 10, 2, 1);
    init_task(&tasks[1], 20, 1, 2);
    init_task(&tasks[2], 30, 3, 3);
    init_task(&tasks[3], 40, 2, 4);

    total_time = run_round_robin(tasks, 4, 1, order, &order_count);

    // EXPECT: 4
    printf("%d\n", order_count);

    /* quantum=1: pass1: all get -1 => t20 done(rem=0). pass2: t10-1=0 done, t30-1=1, t40-1=0 done. pass3: t30-1=0 done */
    // EXPECT: 20
    printf("%d\n", order[0]);

    // EXPECT: 10
    printf("%d\n", order[1]);

    // EXPECT: 40
    printf("%d\n", order[2]);

    // EXPECT: 30
    printf("%d\n", order[3]);

    // EXPECT: 8
    printf("%d\n", total_time);

    /* Test 3: Priority scheduling */
    init_task(&tasks[0], 100, 5, 3);
    init_task(&tasks[1], 200, 3, 1);
    init_task(&tasks[2], 300, 7, 2);

    total_time = run_priority_schedule(tasks, 3, order, &order_count);

    // EXPECT: 3
    printf("%d\n", order_count);

    /* Priority 1 (task 200) first, then priority 2 (task 300), then priority 3 (task 100) */
    // EXPECT: 200
    printf("%d\n", order[0]);

    // EXPECT: 300
    printf("%d\n", order[1]);

    // EXPECT: 100
    printf("%d\n", order[2]);

    // EXPECT: 15
    printf("%d\n", total_time);

    /* Test 4: Single task round-robin */
    init_task(&tasks[0], 42, 5, 1);

    total_time = run_round_robin(tasks, 1, 3, order, &order_count);

    // EXPECT: 42
    printf("%d\n", order[0]);

    // EXPECT: 5
    printf("%d\n", total_time);

    /* Test 5: All same time tasks */
    init_task(&tasks[0], 1, 2, 1);
    init_task(&tasks[1], 2, 2, 1);
    init_task(&tasks[2], 3, 2, 1);

    total_time = run_round_robin(tasks, 3, 2, order, &order_count);

    // EXPECT: 6
    printf("%d\n", total_time);

    // EXPECT: 1
    printf("%d\n", order[0]);

    // EXPECT: 2
    printf("%d\n", order[1]);

    // EXPECT: 3
    printf("%d\n", order[2]);

    return 0;
}
