int printf(const char *fmt, ...);
// EXPECT: === Priority Scheduling ===\nT=0: Start task 3 (pri=5, dur=2)\nT=2: Finish task 3\nT=2: Start task 5 (pri=4, dur=1)\nT=3: Finish task 5\nT=3: Start task 1 (pri=3, dur=5)\nT=8: Finish task 1\nT=8: Start task 4 (pri=2, dur=4)\nT=12: Finish task 4\nT=12: Start task 2 (pri=1, dur=3)\nT=15: Finish task 2\nCompleted: 5\nTotal time: 15\nSum of durations: 15
// Task scheduler simulation with priority queue

struct Task {
    int id;
    int priority;
    int duration;
    int remaining;
    int start_time;
    int end_time;
};

struct Scheduler {
    struct Task tasks[16];
    int queue[16];
    int queue_size;
    int num_tasks;
    int current_time;
    int completed;
};

void sched_init(struct Scheduler *s) {
    s->queue_size = 0;
    s->num_tasks = 0;
    s->current_time = 0;
    s->completed = 0;
}

void sched_add_task(struct Scheduler *s, int id, int priority, int duration) {
    int idx = s->num_tasks;
    s->tasks[idx].id = id;
    s->tasks[idx].priority = priority;
    s->tasks[idx].duration = duration;
    s->tasks[idx].remaining = duration;
    s->tasks[idx].start_time = -1;
    s->tasks[idx].end_time = -1;
    s->num_tasks++;

    /* insert into priority queue */
    s->queue[s->queue_size] = idx;
    s->queue_size++;
    /* bubble up */
    {
        int i = s->queue_size - 1;
        int parent;
        int tmp;
        while (i > 0) {
            parent = (i - 1) / 2;
            if (s->tasks[s->queue[parent]].priority < s->tasks[s->queue[i]].priority) {
                tmp = s->queue[parent];
                s->queue[parent] = s->queue[i];
                s->queue[i] = tmp;
                i = parent;
            } else {
                break;
            }
        }
    }
}

int sched_pop_highest(struct Scheduler *s) {
    int top;
    int i;
    int left;
    int right;
    int largest;
    int tmp;
    if (s->queue_size == 0) return -1;
    top = s->queue[0];
    s->queue_size--;
    s->queue[0] = s->queue[s->queue_size];
    i = 0;
    while (1) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        largest = i;
        if (left < s->queue_size &&
            s->tasks[s->queue[left]].priority > s->tasks[s->queue[largest]].priority)
            largest = left;
        if (right < s->queue_size &&
            s->tasks[s->queue[right]].priority > s->tasks[s->queue[largest]].priority)
            largest = right;
        if (largest != i) {
            tmp = s->queue[i];
            s->queue[i] = s->queue[largest];
            s->queue[largest] = tmp;
            i = largest;
        } else {
            break;
        }
    }
    return top;
}

void sched_run(struct Scheduler *s) {
    int task_idx;
    while (s->queue_size > 0) {
        task_idx = sched_pop_highest(s);
        s->tasks[task_idx].start_time = s->current_time;
        printf("T=%d: Start task %d (pri=%d, dur=%d)\n",
            s->current_time, s->tasks[task_idx].id,
            s->tasks[task_idx].priority, s->tasks[task_idx].duration);
        s->current_time += s->tasks[task_idx].duration;
        s->tasks[task_idx].end_time = s->current_time;
        s->tasks[task_idx].remaining = 0;
        s->completed++;
        printf("T=%d: Finish task %d\n", s->current_time, s->tasks[task_idx].id);
    }
}

int main(void) {
    struct Scheduler s;
    int i;
    int total_time;
    sched_init(&s);

    sched_add_task(&s, 1, 3, 5);
    sched_add_task(&s, 2, 1, 3);
    sched_add_task(&s, 3, 5, 2);
    sched_add_task(&s, 4, 2, 4);
    sched_add_task(&s, 5, 4, 1);

    printf("=== Priority Scheduling ===\n");
    sched_run(&s);

    printf("Completed: %d\n", s.completed);
    printf("Total time: %d\n", s.current_time);

    total_time = 0;
    for (i = 0; i < s.num_tasks; i++) {
        total_time += s.tasks[i].end_time - s.tasks[i].start_time;
    }
    printf("Sum of durations: %d\n", total_time);

    return 0;
}
