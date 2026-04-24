// EXPECT: === Round Robin Scheduling ===
// EXPECT: RR[0]: Init
// EXPECT: RR[1]: Network
// EXPECT: RR[2]: Disk
// EXPECT: RR[3]: Compute
// EXPECT: RR[4]: Init
// EXPECT: RR[5]: Network
// EXPECT: RR[6]: Disk
// EXPECT: RR[7]: Compute
// EXPECT: === Priority Scheduling ===
// EXPECT: PRI[0]: Compute (pri=10)
// EXPECT: PRI[1]: Init (pri=8)
// EXPECT: PRI[2]: Network (pri=5)
// EXPECT: PRI[3]: Disk (pri=3)
// EXPECT: === Aging Simulation ===
// EXPECT: After aging: Disk pri=6, Network pri=8, Init pri=11, Compute pri=13
// EXPECT: === Task Statistics ===
// EXPECT: Init: ran 3 times
// EXPECT: Network: ran 3 times
// EXPECT: Disk: ran 3 times
// EXPECT: Compute: ran 3 times
// EXPECT: All scheduler tests passed!

int printf(const char *fmt, ...);

enum TaskState {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_DONE
};

struct Task {
    char name[16];
    int priority;
    int state;
    int time_slice;
    int run_count;
    int id;
};

void str_copy(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void task_init(struct Task *t, const char *name, int priority, int id) {
    str_copy(t->name, name);
    t->priority = priority;
    t->state = TASK_READY;
    t->time_slice = 1;
    t->run_count = 0;
    t->id = id;
}

// Round-robin scheduler: cycle through tasks in order
void schedule_round_robin(struct Task *tasks, int ntasks, int cycles) {
    int step = 0;
    int c;
    int i;
    printf("=== Round Robin Scheduling ===\n");
    for (c = 0; c < cycles; c++) {
        for (i = 0; i < ntasks; i++) {
            if (tasks[i].state != TASK_DONE) {
                printf("RR[%d]: %s\n", step, tasks[i].name);
                tasks[i].run_count = tasks[i].run_count + 1;
                step = step + 1;
            }
        }
    }
}

// Find the highest priority task among ready tasks
int find_highest_priority(struct Task *tasks, int ntasks, int *done) {
    int best = -1;
    int best_pri = -1;
    int i;
    for (i = 0; i < ntasks; i++) {
        if (done[i] == 0 && tasks[i].priority > best_pri) {
            best_pri = tasks[i].priority;
            best = i;
        }
    }
    return best;
}

// Priority scheduler: always run the highest priority task first
void schedule_priority(struct Task *tasks, int ntasks) {
    int done[8];
    int step;
    int i;
    int idx;
    for (i = 0; i < ntasks; i++) {
        done[i] = 0;
    }
    printf("=== Priority Scheduling ===\n");
    for (step = 0; step < ntasks; step++) {
        idx = find_highest_priority(tasks, ntasks, done);
        if (idx >= 0) {
            printf("PRI[%d]: %s (pri=%d)\n", step, tasks[idx].name, tasks[idx].priority);
            tasks[idx].run_count = tasks[idx].run_count + 1;
            done[idx] = 1;
        }
    }
}

// Simulate priority aging: increase priority of waiting tasks
void simulate_aging(struct Task *tasks, int ntasks, int rounds) {
    int r;
    int i;
    for (r = 0; r < rounds; r++) {
        for (i = 0; i < ntasks; i++) {
            tasks[i].priority = tasks[i].priority + 1;
        }
    }
}

int main(void) {
    struct Task tasks[4];
    int i;

    task_init(&tasks[0], "Init", 8, 0);
    task_init(&tasks[1], "Network", 5, 1);
    task_init(&tasks[2], "Disk", 3, 2);
    task_init(&tasks[3], "Compute", 10, 3);

    // Round-robin: 2 cycles of 4 tasks = 8 steps
    schedule_round_robin(tasks, 4, 2);

    // Reset run counts for priority scheduling
    for (i = 0; i < 4; i++) {
        tasks[i].run_count = 0;
    }

    // Priority scheduling
    schedule_priority(tasks, 4);

    // Aging simulation: increase all priorities by 3
    printf("=== Aging Simulation ===\n");
    simulate_aging(tasks, 4, 3);
    printf("After aging: %s pri=%d, %s pri=%d, %s pri=%d, %s pri=%d\n",
           tasks[2].name, tasks[2].priority,
           tasks[1].name, tasks[1].priority,
           tasks[0].name, tasks[0].priority,
           tasks[3].name, tasks[3].priority);

    // Reset and run one more round-robin to get stats
    for (i = 0; i < 4; i++) {
        tasks[i].run_count = 0;
    }
    // Silent round: 3 cycles just to count
    int c;
    int j;
    for (c = 0; c < 3; c++) {
        for (j = 0; j < 4; j++) {
            tasks[j].run_count = tasks[j].run_count + 1;
        }
    }

    printf("=== Task Statistics ===\n");
    for (i = 0; i < 4; i++) {
        printf("%s: ran %d times\n", tasks[i].name, tasks[i].run_count);
    }

    printf("All scheduler tests passed!\n");
    return 0;
}
