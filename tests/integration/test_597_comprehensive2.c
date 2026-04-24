int printf(const char *fmt, ...);
// EXPECT: All pending:\n  #1 [TODO] Design API (prio=5 effort=8)\n  #2 [TODO] Setup DB (prio=4 effort=5)\n  #3 [BLOCKED] Implement Auth (prio=3 effort=13)\n  #4 [BLOCKED] Build Frontend (prio=3 effort=21)\n  #5 [BLOCKED] Write Tests (prio=2 effort=8)\n  #6 [BLOCKED] Deploy (prio=1 effort=3)\n  (6 tasks)\n\nCompleting 'Design API' and 'Setup DB'...\nAfter completing deps:\n  #3 [TODO] Implement Auth (prio=3 effort=13)\n  #4 [TODO] Build Frontend (prio=3 effort=21)\n  #5 [BLOCKED] Write Tests (prio=2 effort=8)\n  #6 [BLOCKED] Deploy (prio=1 effort=3)\n  (4 tasks)\nHigh priority:\n  #1 [DONE] Design API (prio=5 effort=8)\n  #2 [DONE] Setup DB (prio=4 effort=5)\n  #3 [TODO] Implement Auth (prio=3 effort=13)\n  #4 [TODO] Build Frontend (prio=3 effort=21)\n  (4 tasks)\n\nProgress: 13/58 effort (22%)\nPriority order: #1 #2 #3 #4 #5 #6 
#define MAX_NODES 10
#define MAX_TASKS 8

enum TaskStatus { TODO = 0, IN_PROGRESS = 1, DONE = 2, BLOCKED = 3 };

struct Task {
    int id;
    char *title;
    int priority;
    int status;
    int deps[4];
    int num_deps;
    int effort;
};

typedef int (*FilterFn)(struct Task *);

struct TaskList {
    struct Task tasks[MAX_TASKS];
    int count;
};

void tl_init(struct TaskList *tl) { tl->count = 0; }

int tl_add(struct TaskList *tl, char *title, int prio, int effort) {
    if (tl->count >= MAX_TASKS) return -1;
    int i = tl->count;
    tl->tasks[i].id = i + 1;
    tl->tasks[i].title = title;
    tl->tasks[i].priority = prio;
    tl->tasks[i].status = TODO;
    tl->tasks[i].num_deps = 0;
    tl->tasks[i].effort = effort;
    tl->count++;
    return tl->tasks[i].id;
}

void tl_add_dep(struct TaskList *tl, int task_idx, int dep_id) {
    struct Task *t = &tl->tasks[task_idx];
    if (t->num_deps < 4) {
        t->deps[t->num_deps] = dep_id;
        t->num_deps++;
    }
}

char *status_str(int s) {
    switch (s) {
        case TODO: return "TODO";
        case IN_PROGRESS: return "IN_PROG";
        case DONE: return "DONE";
        case BLOCKED: return "BLOCKED";
    }
    return "?";
}

int all_deps_done(struct TaskList *tl, int task_idx) {
    struct Task *t = &tl->tasks[task_idx];
    int i;
    int j;
    for (i = 0; i < t->num_deps; i++) {
        for (j = 0; j < tl->count; j++) {
            if (tl->tasks[j].id == t->deps[i]) {
                if (tl->tasks[j].status != DONE) return 0;
            }
        }
    }
    return 1;
}

void tl_update_blocked(struct TaskList *tl) {
    int i;
    for (i = 0; i < tl->count; i++) {
        if (tl->tasks[i].status == TODO && tl->tasks[i].num_deps > 0) {
            if (!all_deps_done(tl, i)) {
                tl->tasks[i].status = BLOCKED;
            }
        }
        if (tl->tasks[i].status == BLOCKED && all_deps_done(tl, i)) {
            tl->tasks[i].status = TODO;
        }
    }
}

int filter_high_priority(struct Task *t) { return t->priority >= 3; }
int filter_not_done(struct Task *t) { return t->status != DONE; }

void tl_print_filtered(struct TaskList *tl, FilterFn filter, char *label) {
    printf("%s:\n", label);
    int i;
    int count = 0;
    for (i = 0; i < tl->count; i++) {
        if (filter(&tl->tasks[i])) {
            printf("  #%d [%s] %s (prio=%d effort=%d)\n",
                   tl->tasks[i].id, status_str(tl->tasks[i].status),
                   tl->tasks[i].title, tl->tasks[i].priority,
                   tl->tasks[i].effort);
            count++;
        }
    }
    printf("  (%d tasks)\n", count);
}

int tl_total_effort(struct TaskList *tl) {
    int total = 0;
    int i;
    for (i = 0; i < tl->count; i++) total = total + tl->tasks[i].effort;
    return total;
}

int tl_done_effort(struct TaskList *tl) {
    int total = 0;
    int i;
    for (i = 0; i < tl->count; i++) {
        if (tl->tasks[i].status == DONE) total = total + tl->tasks[i].effort;
    }
    return total;
}

int main(void) {
    struct TaskList tl;
    tl_init(&tl);

    int t1 = tl_add(&tl, "Design API", 5, 8);
    int t2 = tl_add(&tl, "Setup DB", 4, 5);
    int t3 = tl_add(&tl, "Implement Auth", 3, 13);
    int t4 = tl_add(&tl, "Build Frontend", 3, 21);
    int t5 = tl_add(&tl, "Write Tests", 2, 8);
    int t6 = tl_add(&tl, "Deploy", 1, 3);

    tl_add_dep(&tl, 2, t1);
    tl_add_dep(&tl, 2, t2);
    tl_add_dep(&tl, 3, t1);
    tl_add_dep(&tl, 4, t3);
    tl_add_dep(&tl, 4, t4);
    tl_add_dep(&tl, 5, t5);

    tl_update_blocked(&tl);

    tl_print_filtered(&tl, filter_not_done, "All pending");

    printf("\nCompleting 'Design API' and 'Setup DB'...\n");
    tl.tasks[0].status = DONE;
    tl.tasks[1].status = DONE;
    tl_update_blocked(&tl);

    tl_print_filtered(&tl, filter_not_done, "After completing deps");
    tl_print_filtered(&tl, filter_high_priority, "High priority");

    int total = tl_total_effort(&tl);
    int done = tl_done_effort(&tl);
    printf("\nProgress: %d/%d effort (%d%%)\n", done, total, done * 100 / total);

    int sort_order[MAX_TASKS];
    int i;
    int j;
    for (i = 0; i < tl.count; i++) sort_order[i] = i;
    for (i = 0; i < tl.count - 1; i++) {
        for (j = 0; j < tl.count - 1 - i; j++) {
            if (tl.tasks[sort_order[j]].priority < tl.tasks[sort_order[j+1]].priority) {
                int tmp = sort_order[j];
                sort_order[j] = sort_order[j + 1];
                sort_order[j + 1] = tmp;
            }
        }
    }
    printf("Priority order: ");
    for (i = 0; i < tl.count; i++) {
        printf("#%d ", tl.tasks[sort_order[i]].id);
    }
    printf("\n");

    return 0;
}
