int printf(const char *fmt, ...);
// EXPECT: RR:proc=4,fin= 4,cs=8,compl=15,12,6,13,wait=9,8,4,10,tat= 15,12,6,13,avgwait=7,avgt=11,maxwait=10,tburst=15,endt=15,pcs=3,2,1,2|PRI:cs= 4,compl=12,3,8,14,wait=8,0,3,12,avgwait=5,avgt=9|FCFS:cs=4,compl=5,8,10,14,wait=0,4,6,7,avgwait=4,avgt=7|RR3:cs=6,compl=13,12,7,wait=6,7,6,avgwait=6,endt=13|PRI2:compl= 3,7,9,wait=0,1,6|done

/* Process scheduling simulation */

enum ProcessState {
    PROC_READY,
    PROC_RUNNING,
    PROC_BLOCKED,
    PROC_FINISHED
};

struct Process {
    int pid;
    int state;
    int priority;       /* lower number = higher priority */
    int burst_time;     /* total CPU time needed */
    int remaining;      /* remaining CPU time */
    int arrival;        /* arrival time */
    int completion;     /* completion time */
    int wait_time;      /* total wait time */
    int turnaround;     /* turnaround time */
    int context_switches;
};

struct Process procs[16];
int proc_count;
int current_time;
int total_context_switches;
int quantum; /* for round-robin */

/* Ready queue (circular buffer indices) */
int ready_queue[16];
int rq_head;
int rq_tail;
int rq_size;

void init_scheduler(void) {
    int i;
    proc_count = 0;
    current_time = 0;
    total_context_switches = 0;
    quantum = 2;
    rq_head = 0;
    rq_tail = 0;
    rq_size = 0;
    i = 0;
    while (i < 16) {
        procs[i].pid = -1;
        procs[i].state = PROC_FINISHED;
        procs[i].priority = 0;
        procs[i].burst_time = 0;
        procs[i].remaining = 0;
        procs[i].arrival = 0;
        procs[i].completion = 0;
        procs[i].wait_time = 0;
        procs[i].turnaround = 0;
        procs[i].context_switches = 0;
        i = i + 1;
    }
}

int create_process(int pid, int priority, int burst, int arrival) {
    int idx;
    idx = proc_count;
    if (idx >= 16) return -1;
    procs[idx].pid = pid;
    procs[idx].state = PROC_READY;
    procs[idx].priority = priority;
    procs[idx].burst_time = burst;
    procs[idx].remaining = burst;
    procs[idx].arrival = arrival;
    procs[idx].completion = 0;
    procs[idx].wait_time = 0;
    procs[idx].turnaround = 0;
    procs[idx].context_switches = 0;
    proc_count = proc_count + 1;
    return idx;
}

void enqueue(int idx) {
    ready_queue[rq_tail] = idx;
    rq_tail = (rq_tail + 1) % 16;
    rq_size = rq_size + 1;
}

int dequeue(void) {
    int val;
    if (rq_size <= 0) return -1;
    val = ready_queue[rq_head];
    rq_head = (rq_head + 1) % 16;
    rq_size = rq_size - 1;
    return val;
}

int queue_empty(void) {
    return (rq_size == 0);
}

/* Round-robin scheduler: run one time quantum */
int rr_step(void) {
    int idx;
    int run;
    if (queue_empty()) return -1;
    idx = dequeue();
    procs[idx].state = PROC_RUNNING;
    total_context_switches = total_context_switches + 1;
    procs[idx].context_switches = procs[idx].context_switches + 1;
    run = quantum;
    if (procs[idx].remaining < run) {
        run = procs[idx].remaining;
    }
    procs[idx].remaining = procs[idx].remaining - run;
    current_time = current_time + run;
    /* Update wait times for queued processes */
    if (procs[idx].remaining <= 0) {
        procs[idx].state = PROC_FINISHED;
        procs[idx].completion = current_time;
        procs[idx].turnaround = procs[idx].completion - procs[idx].arrival;
        procs[idx].wait_time = procs[idx].turnaround - procs[idx].burst_time;
    } else {
        procs[idx].state = PROC_READY;
        enqueue(idx);
    }
    return idx;
}

/* Run round-robin until all done */
void run_round_robin(void) {
    int safety;
    safety = 0;
    while (!queue_empty() && safety < 200) {
        rr_step();
        safety = safety + 1;
    }
}

/* Priority scheduler (non-preemptive) */
/* Pick the ready process with lowest priority number (highest priority) */
int find_highest_priority(void) {
    int i;
    int best;
    int best_pri;
    best = -1;
    best_pri = 999;
    i = 0;
    while (i < proc_count) {
        if (procs[i].state == PROC_READY && procs[i].arrival <= current_time) {
            if (procs[i].priority < best_pri) {
                best_pri = procs[i].priority;
                best = i;
            }
        }
        i = i + 1;
    }
    return best;
}

void run_priority_scheduler(void) {
    int idx;
    int all_done;
    int safety;
    int i;
    safety = 0;
    while (safety < 200) {
        /* Check arrivals */
        i = 0;
        while (i < proc_count) {
            if (procs[i].state == PROC_READY && procs[i].arrival <= current_time) {
                /* already READY, nothing to do */
            }
            i = i + 1;
        }
        idx = find_highest_priority();
        if (idx < 0) {
            /* Check if all done */
            all_done = 1;
            i = 0;
            while (i < proc_count) {
                if (procs[i].state != PROC_FINISHED) {
                    all_done = 0;
                }
                i = i + 1;
            }
            if (all_done) break;
            current_time = current_time + 1; /* idle */
            safety = safety + 1;
            continue;
        }
        procs[idx].state = PROC_RUNNING;
        total_context_switches = total_context_switches + 1;
        procs[idx].context_switches = procs[idx].context_switches + 1;
        current_time = current_time + procs[idx].remaining;
        procs[idx].remaining = 0;
        procs[idx].state = PROC_FINISHED;
        procs[idx].completion = current_time;
        procs[idx].turnaround = procs[idx].completion - procs[idx].arrival;
        procs[idx].wait_time = procs[idx].turnaround - procs[idx].burst_time;
        safety = safety + 1;
    }
}

/* FCFS scheduler */
void run_fcfs(void) {
    int i;
    int j;
    int order[16];
    int tmp;
    int n;
    n = proc_count;
    i = 0;
    while (i < n) {
        order[i] = i;
        i = i + 1;
    }
    /* Sort by arrival time */
    i = 0;
    while (i < n - 1) {
        j = 0;
        while (j < n - 1 - i) {
            if (procs[order[j]].arrival > procs[order[j + 1]].arrival) {
                tmp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    current_time = 0;
    i = 0;
    while (i < n) {
        if (current_time < procs[order[i]].arrival) {
            current_time = procs[order[i]].arrival;
        }
        procs[order[i]].state = PROC_RUNNING;
        total_context_switches = total_context_switches + 1;
        procs[order[i]].context_switches = procs[order[i]].context_switches + 1;
        current_time = current_time + procs[order[i]].burst_time;
        procs[order[i]].remaining = 0;
        procs[order[i]].state = PROC_FINISHED;
        procs[order[i]].completion = current_time;
        procs[order[i]].turnaround = procs[order[i]].completion - procs[order[i]].arrival;
        procs[order[i]].wait_time = procs[order[i]].turnaround - procs[order[i]].burst_time;
        i = i + 1;
    }
}

int avg_wait(void) {
    int i;
    int s;
    s = 0;
    i = 0;
    while (i < proc_count) {
        s = s + procs[i].wait_time;
        i = i + 1;
    }
    if (proc_count == 0) return 0;
    return s / proc_count;
}

int avg_turnaround(void) {
    int i;
    int s;
    s = 0;
    i = 0;
    while (i < proc_count) {
        s = s + procs[i].turnaround;
        i = i + 1;
    }
    if (proc_count == 0) return 0;
    return s / proc_count;
}

int max_wait(void) {
    int i;
    int m;
    m = 0;
    i = 0;
    while (i < proc_count) {
        if (procs[i].wait_time > m) {
            m = procs[i].wait_time;
        }
        i = i + 1;
    }
    return m;
}

int total_burst(void) {
    int i;
    int s;
    s = 0;
    i = 0;
    while (i < proc_count) {
        s = s + procs[i].burst_time;
        i = i + 1;
    }
    return s;
}

int count_finished(void) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < proc_count) {
        if (procs[i].state == PROC_FINISHED) {
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

int main(void) {
    int i;

    /* ============ Test Round-Robin ============ */
    init_scheduler();
    quantum = 2;

    /* P1: burst=6, P2: burst=4, P3: burst=2, P4: burst=3 */
    create_process(1, 0, 6, 0);
    create_process(2, 0, 4, 0);
    create_process(3, 0, 2, 0);
    create_process(4, 0, 3, 0);

    printf("%c%c", 'R', 'R');
    printf("%c%c%c%c%c%c", ':', 'p', 'r', 'o', 'c', '=');
    printf("%d", proc_count);

    /* Enqueue all */
    i = 0;
    while (i < proc_count) {
        enqueue(i);
        i = i + 1;
    }

    run_round_robin();

    printf("%c%c%c%c%c%c", ',', 'f', 'i', 'n', '=', ' ');
    printf("%d", count_finished());

    printf("%c%c%c%c", ',', 'c', 's', '=');
    printf("%d", total_context_switches);

    /* Print completion times */
    printf("%c%c%c%c%c%c%c", ',', 'c', 'o', 'm', 'p', 'l', '=');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].completion);
        i = i + 1;
    }

    /* Print wait times */
    printf("%c%c%c%c%c", ',', 'w', 'a', 'i', 't');
    printf("%c", '=');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].wait_time);
        i = i + 1;
    }

    /* Print turnaround */
    printf("%c%c%c%c%c%c", ',', 't', 'a', 't', '=', ' ');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].turnaround);
        i = i + 1;
    }

    printf("%c%c%c%c%c%c%c%c%c", ',', 'a', 'v', 'g', 'w', 'a', 'i', 't', '=');
    printf("%d", avg_wait());
    printf("%c%c%c%c%c%c", ',', 'a', 'v', 'g', 't', '=');
    printf("%d", avg_turnaround());
    printf("%c%c%c%c%c%c%c%c%c", ',', 'm', 'a', 'x', 'w', 'a', 'i', 't', '=');
    printf("%d", max_wait());
    printf("%c%c%c%c%c%c%c", ',', 't', 'b', 'u', 'r', 's', 't');
    printf("%c", '=');
    printf("%d", total_burst());
    printf("%c%c%c%c%c%c", ',', 'e', 'n', 'd', 't', '=');
    printf("%d", current_time);

    /* Per-process context switches */
    printf("%c%c%c%c%c", ',', 'p', 'c', 's', '=');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].context_switches);
        i = i + 1;
    }

    /* ============ Test Priority Scheduling ============ */
    init_scheduler();
    /* P1: pri=3, burst=4, arr=0 */
    /* P2: pri=1, burst=3, arr=0 */
    /* P3: pri=2, burst=5, arr=0 */
    /* P4: pri=4, burst=2, arr=0 */
    create_process(10, 3, 4, 0);
    create_process(11, 1, 3, 0);
    create_process(12, 2, 5, 0);
    create_process(13, 4, 2, 0);

    run_priority_scheduler();

    printf("%c", '|');
    printf("%c%c%c%c", 'P', 'R', 'I', ':');
    printf("%c%c%c%c", 'c', 's', '=', ' ');
    printf("%d", total_context_switches);
    printf("%c%c%c%c%c%c%c", ',', 'c', 'o', 'm', 'p', 'l', '=');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].completion);
        i = i + 1;
    }
    printf("%c%c%c%c%c%c", ',', 'w', 'a', 'i', 't', '=');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].wait_time);
        i = i + 1;
    }
    printf("%c%c%c%c%c%c%c%c%c", ',', 'a', 'v', 'g', 'w', 'a', 'i', 't', '=');
    printf("%d", avg_wait());
    printf("%c%c%c%c%c%c", ',', 'a', 'v', 'g', 't', '=');
    printf("%d", avg_turnaround());

    /* ============ Test FCFS ============ */
    init_scheduler();
    /* P1: burst=5, arr=0 */
    /* P2: burst=3, arr=1 */
    /* P3: burst=2, arr=2 */
    /* P4: burst=4, arr=3 */
    create_process(20, 0, 5, 0);
    create_process(21, 0, 3, 1);
    create_process(22, 0, 2, 2);
    create_process(23, 0, 4, 3);

    run_fcfs();

    printf("%c", '|');
    printf("%c%c%c%c%c", 'F', 'C', 'F', 'S', ':');
    printf("%c%c", 'c', 's');
    printf("%c", '=');
    printf("%d", total_context_switches);
    printf("%c%c%c%c%c%c%c", ',', 'c', 'o', 'm', 'p', 'l', '=');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].completion);
        i = i + 1;
    }
    printf("%c%c%c%c%c%c", ',', 'w', 'a', 'i', 't', '=');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].wait_time);
        i = i + 1;
    }
    printf("%c%c%c%c%c%c%c%c%c", ',', 'a', 'v', 'g', 'w', 'a', 'i', 't', '=');
    printf("%d", avg_wait());
    printf("%c%c%c%c%c%c", ',', 'a', 'v', 'g', 't', '=');
    printf("%d", avg_turnaround());

    /* ============ Test Round-Robin with quantum=3 ============ */
    init_scheduler();
    quantum = 3;
    /* P1: burst=7, P2: burst=5, P3: burst=1 */
    create_process(30, 0, 7, 0);
    create_process(31, 0, 5, 0);
    create_process(32, 0, 1, 0);

    i = 0;
    while (i < proc_count) {
        enqueue(i);
        i = i + 1;
    }
    run_round_robin();

    printf("%c", '|');
    printf("%c%c%c%c", 'R', 'R', '3', ':');
    printf("%c%c", 'c', 's');
    printf("%c", '=');
    printf("%d", total_context_switches);
    printf("%c%c%c%c%c%c%c", ',', 'c', 'o', 'm', 'p', 'l', '=');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].completion);
        i = i + 1;
    }
    printf("%c%c%c%c%c%c", ',', 'w', 'a', 'i', 't', '=');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].wait_time);
        i = i + 1;
    }
    printf("%c%c%c%c%c%c%c%c%c", ',', 'a', 'v', 'g', 'w', 'a', 'i', 't', '=');
    printf("%d", avg_wait());
    printf("%c%c%c%c%c%c", ',', 'e', 'n', 'd', 't', '=');
    printf("%d", current_time);

    /* ============ Test Priority with different arrivals ============ */
    init_scheduler();
    /* P1: pri=2, burst=3, arr=0 */
    /* P2: pri=1, burst=4, arr=2 */
    /* P3: pri=3, burst=2, arr=1 */
    create_process(40, 2, 3, 0);
    create_process(41, 1, 4, 2);
    create_process(42, 3, 2, 1);

    run_priority_scheduler();

    printf("%c", '|');
    printf("%c%c%c%c%c", 'P', 'R', 'I', '2', ':');
    printf("%c%c%c%c%c%c%c", 'c', 'o', 'm', 'p', 'l', '=', ' ');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].completion);
        i = i + 1;
    }
    printf("%c%c%c%c%c%c", ',', 'w', 'a', 'i', 't', '=');
    i = 0;
    while (i < proc_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", procs[i].wait_time);
        i = i + 1;
    }

    printf("%c%c%c%c%c", '|', 'd', 'o', 'n', 'e');

    printf("%c", '\n');

    return 0;
}
