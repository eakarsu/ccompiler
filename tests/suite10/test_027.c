int printf(const char *fmt, ...);

// Deadlock Detection: resource allocation graph cycle detection

struct ResourceGraph {
    int alloc[8][8];    // alloc[i][j] = 1 if process i holds resource j
    int request[8][8];  // request[i][j] = 1 if process i requests resource j
    int available[8];   // available resources
    int num_proc;
    int num_res;
};

void init_graph(struct ResourceGraph *g, int np, int nr) {
    int i;
    int j;
    g->num_proc = np;
    g->num_res = nr;
    for (i = 0; i < np; i = i + 1) {
        for (j = 0; j < nr; j = j + 1) {
            g->alloc[i][j] = 0;
            g->request[i][j] = 0;
        }
    }
    for (j = 0; j < nr; j = j + 1) {
        g->available[j] = 0;
    }
}

void set_available(struct ResourceGraph *g, int res, int count) {
    g->available[res] = count;
}

void allocate(struct ResourceGraph *g, int proc, int res) {
    g->alloc[proc][res] = 1;
}

void request_res(struct ResourceGraph *g, int proc, int res) {
    g->request[proc][res] = 1;
}

// Deadlock detection using banker's algorithm style
int detect_deadlock(struct ResourceGraph *g) {
    int work[8];
    int finish[8];
    int i;
    int j;
    int found;
    int deadlocked;
    int count;
    int can_finish;

    for (j = 0; j < g->num_res; j = j + 1) {
        work[j] = g->available[j];
    }
    for (i = 0; i < g->num_proc; i = i + 1) {
        finish[i] = 0;
        // If process has no allocations, mark finished
        can_finish = 1;
        for (j = 0; j < g->num_res; j = j + 1) {
            if (g->alloc[i][j]) {
                can_finish = 0;
            }
        }
        if (can_finish) finish[i] = 1;
    }

    // Try to find processes that can finish
    found = 1;
    while (found) {
        found = 0;
        for (i = 0; i < g->num_proc; i = i + 1) {
            if (finish[i] == 0) {
                can_finish = 1;
                for (j = 0; j < g->num_res; j = j + 1) {
                    if (g->request[i][j] > work[j]) {
                        can_finish = 0;
                    }
                }
                if (can_finish) {
                    finish[i] = 1;
                    found = 1;
                    for (j = 0; j < g->num_res; j = j + 1) {
                        work[j] = work[j] + g->alloc[i][j];
                    }
                    printf("P%d can finish\n", i);
                }
            }
        }
    }

    deadlocked = 0;
    for (i = 0; i < g->num_proc; i = i + 1) {
        if (finish[i] == 0) {
            printf("P%d is DEADLOCKED\n", i);
            deadlocked = deadlocked + 1;
        }
    }
    return deadlocked;
}

// Wait-for graph cycle detection (simplified)
int detect_cycle(int waitfor[8][8], int n) {
    int visited[8];
    int rec_stack[8];
    int stack[32];
    int sp;
    int i;
    int j;
    int cur;
    int has_cycle;

    has_cycle = 0;
    for (i = 0; i < n; i = i + 1) {
        visited[i] = 0;
        rec_stack[i] = 0;
    }

    for (i = 0; i < n; i = i + 1) {
        if (visited[i]) continue;
        // DFS using iteration
        sp = 0;
        stack[sp] = i;
        sp = sp + 1;
        while (sp > 0) {
            sp = sp - 1;
            cur = stack[sp];
            if (rec_stack[cur]) {
                has_cycle = 1;
            }
            if (visited[cur] == 0) {
                visited[cur] = 1;
                rec_stack[cur] = 1;
                for (j = 0; j < n; j = j + 1) {
                    if (waitfor[cur][j]) {
                        if (rec_stack[j]) {
                            has_cycle = 1;
                            printf("Cycle: P%d -> P%d\n", cur, j);
                        } else if (visited[j] == 0) {
                            stack[sp] = j;
                            sp = sp + 1;
                        }
                    }
                }
            }
        }
        // clear rec_stack for this traversal
        for (j = 0; j < n; j = j + 1) {
            rec_stack[j] = 0;
        }
    }
    return has_cycle;
}

int main() {
    struct ResourceGraph g;
    int result;
    int waitfor[8][8];
    int i;
    int j;

    printf("=== Deadlock Detection Test 1 (No Deadlock) ===\n"); // EXPECT: === Deadlock Detection Test 1 (No Deadlock) ===
    init_graph(&g, 3, 3);
    set_available(&g, 0, 1);
    set_available(&g, 1, 0);
    set_available(&g, 2, 1);

    allocate(&g, 0, 0);  // P0 holds R0
    allocate(&g, 1, 1);  // P1 holds R1
    request_res(&g, 0, 1); // P0 requests R1
    request_res(&g, 1, 2); // P1 requests R2

    result = detect_deadlock(&g);
    // EXPECT: P1 can finish
    // EXPECT: P0 can finish
    printf("Deadlocked processes: %d\n", result); // EXPECT: Deadlocked processes: 0

    printf("=== Deadlock Detection Test 2 (Deadlock) ===\n"); // EXPECT: === Deadlock Detection Test 2 (Deadlock) ===
    init_graph(&g, 3, 3);
    set_available(&g, 0, 0);
    set_available(&g, 1, 0);
    set_available(&g, 2, 0);

    allocate(&g, 0, 0);  // P0 holds R0
    allocate(&g, 1, 1);  // P1 holds R1
    allocate(&g, 2, 2);  // P2 holds R2
    request_res(&g, 0, 1); // P0 requests R1 (held by P1)
    request_res(&g, 1, 2); // P1 requests R2 (held by P2)
    request_res(&g, 2, 0); // P2 requests R0 (held by P0)

    result = detect_deadlock(&g);
    // EXPECT: P0 is DEADLOCKED
    // EXPECT: P1 is DEADLOCKED
    // EXPECT: P2 is DEADLOCKED
    printf("Deadlocked processes: %d\n", result); // EXPECT: Deadlocked processes: 3

    printf("=== Wait-For Graph Cycle ===\n"); // EXPECT: === Wait-For Graph Cycle ===
    for (i = 0; i < 8; i = i + 1) {
        for (j = 0; j < 8; j = j + 1) {
            waitfor[i][j] = 0;
        }
    }
    waitfor[0][1] = 1; // P0 waits for P1
    waitfor[1][2] = 1; // P1 waits for P2
    waitfor[2][0] = 1; // P2 waits for P0 (cycle!)

    result = detect_cycle(waitfor, 3);
    // EXPECT: Cycle: P2 -> P0
    printf("Has cycle: %d\n", result); // EXPECT: Has cycle: 1

    printf("Deadlock detection complete\n"); // EXPECT: Deadlock detection complete
    return 0;
}
