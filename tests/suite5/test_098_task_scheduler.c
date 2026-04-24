int printf(const char *fmt, ...);

struct Task {
    int id;
    char name[16];
    int done;
    int dep_count;
    int deps[8];
};

struct Scheduler {
    struct Task tasks[32];
    int count;
    int order[32];
    int order_count;
};

void copy_str(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(const char *a, const char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void sched_init(struct Scheduler *s) {
    s->count = 0;
    s->order_count = 0;
}

int sched_add_task(struct Scheduler *s, int id, const char *name) {
    int idx = s->count;
    s->tasks[idx].id = id;
    copy_str(s->tasks[idx].name, name);
    s->tasks[idx].done = 0;
    s->tasks[idx].dep_count = 0;
    s->count = s->count + 1;
    return idx;
}

int find_task_idx(struct Scheduler *s, int id) {
    int i = 0;
    while (i < s->count) {
        if (s->tasks[i].id == id) return i;
        i = i + 1;
    }
    return -1;
}

void sched_add_dep(struct Scheduler *s, int task_id, int dep_id) {
    int idx = find_task_idx(s, task_id);
    if (idx < 0) return;
    int di = s->tasks[idx].dep_count;
    s->tasks[idx].deps[di] = dep_id;
    s->tasks[idx].dep_count = s->tasks[idx].dep_count + 1;
}

int deps_satisfied(struct Scheduler *s, int idx) {
    int i = 0;
    while (i < s->tasks[idx].dep_count) {
        int dep_id = s->tasks[idx].deps[i];
        int di = find_task_idx(s, dep_id);
        if (di < 0 || !s->tasks[di].done) return 0;
        i = i + 1;
    }
    return 1;
}

int sched_run(struct Scheduler *s) {
    s->order_count = 0;
    int total_done = 0;
    int progress = 1;
    while (progress && total_done < s->count) {
        progress = 0;
        int i = 0;
        while (i < s->count) {
            if (!s->tasks[i].done && deps_satisfied(s, i)) {
                s->tasks[i].done = 1;
                s->order[s->order_count] = s->tasks[i].id;
                s->order_count = s->order_count + 1;
                total_done = total_done + 1;
                progress = 1;
            }
            i = i + 1;
        }
    }
    return total_done;
}

int count_done(struct Scheduler *s) {
    int c = 0;
    int i = 0;
    while (i < s->count) {
        if (s->tasks[i].done) c = c + 1;
        i = i + 1;
    }
    return c;
}

int count_ready(struct Scheduler *s) {
    int c = 0;
    int i = 0;
    while (i < s->count) {
        if (!s->tasks[i].done && deps_satisfied(s, i)) {
            c = c + 1;
        }
        i = i + 1;
    }
    return c;
}

int count_blocked(struct Scheduler *s) {
    int c = 0;
    int i = 0;
    while (i < s->count) {
        if (!s->tasks[i].done && !deps_satisfied(s, i)) {
            c = c + 1;
        }
        i = i + 1;
    }
    return c;
}

int has_cycle(struct Scheduler *s) {
    int i = 0;
    while (i < s->count) {
        s->tasks[i].done = 0;
        i = i + 1;
    }
    int done = sched_run(s);
    return done < s->count;
}

int dep_chain_len(struct Scheduler *s, int id) {
    int idx = find_task_idx(s, id);
    if (idx < 0) return 0;
    if (s->tasks[idx].dep_count == 0) return 0;
    int mx = 0;
    int i = 0;
    while (i < s->tasks[idx].dep_count) {
        int d = dep_chain_len(s, s->tasks[idx].deps[i]);
        if (d + 1 > mx) mx = d + 1;
        i = i + 1;
    }
    return mx;
}

int main(void) {
    struct Scheduler s;
    sched_init(&s);

    sched_add_task(&s, 1, "init");
    sched_add_task(&s, 2, "parse");
    sched_add_task(&s, 3, "analyze");
    sched_add_task(&s, 4, "optimize");
    sched_add_task(&s, 5, "codegen");
    sched_add_task(&s, 6, "link");
    sched_add_task(&s, 7, "test");

    sched_add_dep(&s, 2, 1);
    sched_add_dep(&s, 3, 2);
    sched_add_dep(&s, 4, 3);
    sched_add_dep(&s, 5, 4);
    sched_add_dep(&s, 6, 5);
    sched_add_dep(&s, 7, 6);

    // EXPECT: task_count=7
    printf("task_count=%d\n", s.count);

    // EXPECT: ready_before=1
    printf("ready_before=%d\n", count_ready(&s));

    // EXPECT: blocked_before=6
    printf("blocked_before=%d\n", count_blocked(&s));

    int done = sched_run(&s);
    // EXPECT: completed=7
    printf("completed=%d\n", done);

    // EXPECT: order_0=1
    printf("order_0=%d\n", s.order[0]);
    // EXPECT: order_1=2
    printf("order_1=%d\n", s.order[1]);
    // EXPECT: order_2=3
    printf("order_2=%d\n", s.order[2]);
    // EXPECT: order_6=7
    printf("order_6=%d\n", s.order[6]);

    // EXPECT: all_done=7
    printf("all_done=%d\n", count_done(&s));

    struct Scheduler s2;
    sched_init(&s2);
    sched_add_task(&s2, 10, "A");
    sched_add_task(&s2, 20, "B");
    sched_add_task(&s2, 30, "C");
    sched_add_task(&s2, 40, "D");
    sched_add_dep(&s2, 30, 10);
    sched_add_dep(&s2, 30, 20);
    sched_add_dep(&s2, 40, 30);

    // EXPECT: chain_40=2
    printf("chain_40=%d\n", dep_chain_len(&s2, 40));

    // EXPECT: chain_30=1
    printf("chain_30=%d\n", dep_chain_len(&s2, 30));

    // EXPECT: chain_10=0
    printf("chain_10=%d\n", dep_chain_len(&s2, 10));

    int done2 = sched_run(&s2);
    // EXPECT: done2=4
    printf("done2=%d\n", done2);

    // EXPECT: s2_order_0=10
    printf("s2_order_0=%d\n", s2.order[0]);

    // EXPECT: s2_order_1=20
    printf("s2_order_1=%d\n", s2.order[1]);

    // EXPECT: s2_order_3=40
    printf("s2_order_3=%d\n", s2.order[3]);

    return 0;
}
