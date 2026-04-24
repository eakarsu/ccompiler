int printf(const char *fmt, ...);

// Test 090: Job scheduler with FIFO queue and completion tracking

struct Job {
    int id;
    int priority;
    int duration;
    int status;   // 0=pending, 1=running, 2=done
};

struct JobQueue {
    struct Job jobs[8];
    int head;
    int tail;
    int count;
};

struct Scheduler {
    struct JobQueue pending;
    struct Job completed[16];
    int completed_count;
    int next_id;
    int current_time;
};

void jq_init(struct JobQueue *jq) {
    int i;
    for (i = 0; i < 8; i++) {
        jq->jobs[i].id = 0;
        jq->jobs[i].priority = 0;
        jq->jobs[i].duration = 0;
        jq->jobs[i].status = 0;
    }
    jq->head = 0;
    jq->tail = 0;
    jq->count = 0;
}

void sched_init(struct Scheduler *s) {
    int i;
    jq_init(&s->pending);
    for (i = 0; i < 16; i++) {
        s->completed[i].id = 0;
        s->completed[i].priority = 0;
        s->completed[i].duration = 0;
        s->completed[i].status = 0;
    }
    s->completed_count = 0;
    s->next_id = 1;
    s->current_time = 0;
}

int sched_submit(struct Scheduler *s, int prio, int dur) {
    struct JobQueue *jq;
    int id;
    jq = &s->pending;
    if (jq->count >= 8) return -1;
    id = s->next_id;
    s->next_id = s->next_id + 1;
    jq->jobs[jq->tail].id = id;
    jq->jobs[jq->tail].priority = prio;
    jq->jobs[jq->tail].duration = dur;
    jq->jobs[jq->tail].status = 0;
    jq->tail = (jq->tail + 1) % 8;
    jq->count = jq->count + 1;
    return id;
}

int sched_run_next(struct Scheduler *s) {
    struct JobQueue *jq;
    struct Job job;
    int idx;
    jq = &s->pending;
    if (jq->count <= 0) return 0;
    job.id = jq->jobs[jq->head].id;
    job.priority = jq->jobs[jq->head].priority;
    job.duration = jq->jobs[jq->head].duration;
    job.status = 2;
    jq->head = (jq->head + 1) % 8;
    jq->count = jq->count - 1;
    s->current_time = s->current_time + job.duration;
    idx = s->completed_count;
    if (idx < 16) {
        s->completed[idx].id = job.id;
        s->completed[idx].priority = job.priority;
        s->completed[idx].duration = job.duration;
        s->completed[idx].status = 2;
        s->completed_count = s->completed_count + 1;
    }
    return job.id;
}

int sched_run_all(struct Scheduler *s) {
    int count;
    count = 0;
    while (s->pending.count > 0) {
        sched_run_next(s);
        count = count + 1;
    }
    return count;
}

int main() {
    struct Scheduler sched;
    int id;
    int ran;
    int total_dur;
    int i;

    sched_init(&sched);
    printf("Time: %d pending: %d done: %d\n", sched.current_time, sched.pending.count, sched.completed_count); // EXPECT: Time: 0 pending: 0 done: 0

    // Submit jobs
    id = sched_submit(&sched, 1, 10);
    printf("Submitted job %d\n", id); // EXPECT: Submitted job 1
    id = sched_submit(&sched, 2, 5);
    printf("Submitted job %d\n", id); // EXPECT: Submitted job 2
    id = sched_submit(&sched, 1, 15);
    printf("Submitted job %d\n", id); // EXPECT: Submitted job 3
    id = sched_submit(&sched, 3, 3);
    printf("Submitted job %d\n", id); // EXPECT: Submitted job 4
    printf("Pending: %d\n", sched.pending.count); // EXPECT: Pending: 4

    // Run first 2 jobs
    id = sched_run_next(&sched);
    printf("Ran job %d, time=%d\n", id, sched.current_time); // EXPECT: Ran job 1, time=10
    id = sched_run_next(&sched);
    printf("Ran job %d, time=%d\n", id, sched.current_time); // EXPECT: Ran job 2, time=15

    printf("Pending: %d done: %d\n", sched.pending.count, sched.completed_count); // EXPECT: Pending: 2 done: 2

    // Submit more while running
    sched_submit(&sched, 2, 8);
    sched_submit(&sched, 1, 12);
    printf("Pending: %d\n", sched.pending.count); // EXPECT: Pending: 4

    // Run all remaining
    ran = sched_run_all(&sched);
    printf("Ran %d jobs, time=%d\n", ran, sched.current_time); // EXPECT: Ran 4 jobs, time=53
    printf("Total completed: %d\n", sched.completed_count); // EXPECT: Total completed: 6

    // Review completed jobs
    total_dur = 0;
    for (i = 0; i < sched.completed_count; i++) {
        total_dur = total_dur + sched.completed[i].duration;
    }
    printf("Total duration: %d\n", total_dur); // EXPECT: Total duration: 53

    // Check completed job details
    printf("Job %d: prio=%d dur=%d\n", sched.completed[0].id, sched.completed[0].priority, sched.completed[0].duration); // EXPECT: Job 1: prio=1 dur=10
    printf("Job %d: prio=%d dur=%d\n", sched.completed[5].id, sched.completed[5].priority, sched.completed[5].duration); // EXPECT: Job 6: prio=1 dur=12

    // Submit a big batch
    for (i = 0; i < 8; i++) {
        sched_submit(&sched, i % 3, (i + 1) * 2);
    }
    printf("Batch pending: %d\n", sched.pending.count); // EXPECT: Batch pending: 8

    ran = sched_run_all(&sched);
    printf("Batch ran: %d time=%d\n", ran, sched.current_time); // EXPECT: Batch ran: 8 time=125

    printf("Grand total completed: %d\n", sched.completed_count); // EXPECT: Grand total completed: 14

    // Empty queue run
    ran = sched_run_all(&sched);
    printf("Empty run: %d\n", ran); // EXPECT: Empty run: 0

    printf("PASS\n"); // EXPECT: PASS
    return 0;
}
