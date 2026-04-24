int printf(const char *fmt, ...);

// Thread pool simulation (single-threaded)
// Simulates submitting tasks to a pool and executing them round-robin

struct Task {
    int id;
    int arg;
    int result;
    int status; // 0=pending, 1=running, 2=done
    int worker;
};

struct TaskQueue {
    struct Task tasks[16];
    int head;
    int tail;
    int count;
};

void queue_init(struct TaskQueue *q) {
    int i;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    for (i = 0; i < 16; i = i + 1) {
        q->tasks[i].id = 0;
        q->tasks[i].arg = 0;
        q->tasks[i].result = 0;
        q->tasks[i].status = 0;
        q->tasks[i].worker = -1;
    }
}

int queue_push(struct TaskQueue *q, int id, int arg) {
    if (q->count >= 16) return 0;
    q->tasks[q->tail].id = id;
    q->tasks[q->tail].arg = arg;
    q->tasks[q->tail].status = 0;
    q->tasks[q->tail].worker = -1;
    q->tasks[q->tail].result = 0;
    q->tail = (q->tail + 1) % 16;
    q->count = q->count + 1;
    return 1;
}

int queue_pop_index(struct TaskQueue *q) {
    int idx;
    if (q->count <= 0) return -1;
    idx = q->head;
    q->head = (q->head + 1) % 16;
    q->count = q->count - 1;
    return idx;
}

// Compute functions simulating different task types
int compute_square(int x) { return x * x; }
int compute_double(int x) { return x * 2; }
int compute_add10(int x) { return x + 10; }
int compute_factorial(int x) {
    int r;
    int i;
    r = 1;
    for (i = 1; i <= x; i = i + 1) {
        r = r * i;
    }
    return r;
}

struct Worker {
    int id;
    int tasks_done;
    int busy;
};

struct ThreadPool {
    struct Worker workers[4];
    int num_workers;
    struct TaskQueue queue;
    int total_submitted;
    int total_completed;
    int next_task_id;
};

void pool_init(struct ThreadPool *p, int nworkers) {
    int i;
    p->num_workers = nworkers;
    p->total_submitted = 0;
    p->total_completed = 0;
    p->next_task_id = 1;
    queue_init(&p->queue);
    for (i = 0; i < 4; i = i + 1) {
        p->workers[i].id = i;
        p->workers[i].tasks_done = 0;
        p->workers[i].busy = 0;
    }
}

int pool_submit(struct ThreadPool *p, int arg) {
    int tid;
    tid = p->next_task_id;
    p->next_task_id = p->next_task_id + 1;
    if (!queue_push(&p->queue, tid, arg)) return -1;
    p->total_submitted = p->total_submitted + 1;
    return tid;
}

// Execute one task on a worker (round-robin assignment)
int pool_execute_one(struct ThreadPool *p, int task_type) {
    int idx;
    int wid;
    int arg;
    int result;
    idx = queue_pop_index(&p->queue);
    if (idx < 0) return -1;

    // Round-robin worker assignment
    wid = p->total_completed % p->num_workers;

    arg = p->queue.tasks[idx].arg;
    if (task_type == 0) result = compute_square(arg);
    else if (task_type == 1) result = compute_double(arg);
    else if (task_type == 2) result = compute_add10(arg);
    else result = compute_factorial(arg);

    p->queue.tasks[idx].result = result;
    p->queue.tasks[idx].status = 2;
    p->queue.tasks[idx].worker = wid;
    p->workers[wid].tasks_done = p->workers[wid].tasks_done + 1;
    p->total_completed = p->total_completed + 1;
    return result;
}

int main() {
    struct ThreadPool pool;
    pool_init(&pool, 4);

    printf("workers: %d\n", pool.num_workers); // EXPECT: workers: 4
    printf("submitted: %d\n", pool.total_submitted); // EXPECT: submitted: 0

    // Submit tasks
    int t1 = pool_submit(&pool, 5);
    int t2 = pool_submit(&pool, 3);
    int t3 = pool_submit(&pool, 7);
    int t4 = pool_submit(&pool, 4);
    int t5 = pool_submit(&pool, 6);
    printf("task ids: %d %d %d %d %d\n", t1, t2, t3, t4, t5); // EXPECT: task ids: 1 2 3 4 5
    printf("submitted: %d\n", pool.total_submitted); // EXPECT: submitted: 5
    printf("queue count: %d\n", pool.queue.count); // EXPECT: queue count: 5

    // Execute tasks with square function
    int r1 = pool_execute_one(&pool, 0);
    printf("result1 (5*5): %d\n", r1); // EXPECT: result1 (5*5): 25

    int r2 = pool_execute_one(&pool, 0);
    printf("result2 (3*3): %d\n", r2); // EXPECT: result2 (3*3): 9

    int r3 = pool_execute_one(&pool, 1);
    printf("result3 (7*2): %d\n", r3); // EXPECT: result3 (7*2): 14

    int r4 = pool_execute_one(&pool, 2);
    printf("result4 (4+10): %d\n", r4); // EXPECT: result4 (4+10): 14

    int r5 = pool_execute_one(&pool, 3);
    printf("result5 (6!): %d\n", r5); // EXPECT: result5 (6!): 720

    printf("completed: %d\n", pool.total_completed); // EXPECT: completed: 5
    printf("queue empty: %d\n", pool.queue.count); // EXPECT: queue empty: 0

    // Check worker distribution
    printf("w0 tasks: %d\n", pool.workers[0].tasks_done); // EXPECT: w0 tasks: 2
    printf("w1 tasks: %d\n", pool.workers[1].tasks_done); // EXPECT: w1 tasks: 1
    printf("w2 tasks: %d\n", pool.workers[2].tasks_done); // EXPECT: w2 tasks: 1
    printf("w3 tasks: %d\n", pool.workers[3].tasks_done); // EXPECT: w3 tasks: 1

    // No more tasks
    int empty = pool_execute_one(&pool, 0);
    printf("no task: %d\n", empty); // EXPECT: no task: -1

    // Batch test: submit and execute 8 more tasks
    int i;
    for (i = 0; i < 8; i = i + 1) {
        pool_submit(&pool, i + 1);
    }
    printf("queue after batch: %d\n", pool.queue.count); // EXPECT: queue after batch: 8

    int sum = 0;
    for (i = 0; i < 8; i = i + 1) {
        sum = sum + pool_execute_one(&pool, 0);
    }
    printf("sum of squares 1-8: %d\n", sum); // EXPECT: sum of squares 1-8: 204
    printf("total completed: %d\n", pool.total_completed); // EXPECT: total completed: 13

    return 0;
}
