int printf(const char *fmt, ...);

// Resource Contention Detector
// Detects deadlocks and contention in resource allocation graphs

struct ResNode {
    int id;
    int is_process;
    int holds[8];
    int hold_count;
    int waits_for;
};

struct ContentionGraph {
    struct ResNode nodes[12];
    int count;
};

void cg_init(struct ContentionGraph *g) {
    int i;
    int j;
    g->count = 0;
    for (i = 0; i < 12; i++) {
        g->nodes[i].id = 0;
        g->nodes[i].is_process = 0;
        g->nodes[i].hold_count = 0;
        g->nodes[i].waits_for = -1;
        for (j = 0; j < 8; j++) {
            g->nodes[i].holds[j] = -1;
        }
    }
}

int cg_add_node(struct ContentionGraph *g, int id, int is_process) {
    int idx = g->count;
    g->nodes[idx].id = id;
    g->nodes[idx].is_process = is_process;
    g->nodes[idx].hold_count = 0;
    g->nodes[idx].waits_for = -1;
    g->count = g->count + 1;
    return idx;
}

int cg_find_node(struct ContentionGraph *g, int id) {
    int i;
    for (i = 0; i < g->count; i++) {
        if (g->nodes[i].id == id) return i;
    }
    return -1;
}

void cg_assign_resource(struct ContentionGraph *g, int proc_id, int res_id) {
    int pi = cg_find_node(g, proc_id);
    if (pi < 0) return;
    int hc = g->nodes[pi].hold_count;
    g->nodes[pi].holds[hc] = res_id;
    g->nodes[pi].hold_count = hc + 1;
}

void cg_request_resource(struct ContentionGraph *g, int proc_id, int res_id) {
    int pi = cg_find_node(g, proc_id);
    if (pi < 0) return;
    g->nodes[pi].waits_for = res_id;
}

int cg_find_holder(struct ContentionGraph *g, int res_id) {
    int i;
    int j;
    for (i = 0; i < g->count; i++) {
        if (g->nodes[i].is_process) {
            for (j = 0; j < g->nodes[i].hold_count; j++) {
                if (g->nodes[i].holds[j] == res_id) {
                    return g->nodes[i].id;
                }
            }
        }
    }
    return -1;
}

int cg_detect_deadlock_from(struct ContentionGraph *g, int start_proc) {
    int visited[12];
    int i;
    int current;
    int steps = 0;

    for (i = 0; i < 12; i++) visited[i] = 0;

    current = start_proc;
    while (steps < 20) {
        int ci = cg_find_node(g, current);
        if (ci < 0) return 0;
        if (visited[ci]) {
            if (current == start_proc) return 1;
            return 0;
        }
        visited[ci] = 1;

        int wanted_res = g->nodes[ci].waits_for;
        if (wanted_res < 0) return 0;

        int holder = cg_find_holder(g, wanted_res);
        if (holder < 0) return 0;
        if (holder == current) return 0;

        current = holder;
        steps = steps + 1;
    }
    return 0;
}

int cg_detect_any_deadlock(struct ContentionGraph *g) {
    int i;
    for (i = 0; i < g->count; i++) {
        if (g->nodes[i].is_process && g->nodes[i].waits_for >= 0) {
            if (cg_detect_deadlock_from(g, g->nodes[i].id)) {
                return g->nodes[i].id;
            }
        }
    }
    return -1;
}

int cg_count_contention(struct ContentionGraph *g, int res_id) {
    int count = 0;
    int i;
    for (i = 0; i < g->count; i++) {
        if (g->nodes[i].is_process && g->nodes[i].waits_for == res_id) {
            count = count + 1;
        }
    }
    return count;
}

int cg_find_most_contended(struct ContentionGraph *g) {
    int best_res = -1;
    int best_count = 0;
    int i;
    for (i = 0; i < g->count; i++) {
        if (g->nodes[i].is_process == 0) {
            int c = cg_count_contention(g, g->nodes[i].id);
            if (c > best_count) {
                best_count = c;
                best_res = g->nodes[i].id;
            }
        }
    }
    return best_res;
}

int cg_count_blocked(struct ContentionGraph *g) {
    int count = 0;
    int i;
    for (i = 0; i < g->count; i++) {
        if (g->nodes[i].is_process && g->nodes[i].waits_for >= 0) {
            count = count + 1;
        }
    }
    return count;
}

void cg_release_resource(struct ContentionGraph *g, int proc_id, int res_id) {
    int pi = cg_find_node(g, proc_id);
    if (pi < 0) return;
    int i;
    for (i = 0; i < g->nodes[pi].hold_count; i++) {
        if (g->nodes[pi].holds[i] == res_id) {
            int j;
            for (j = i; j < g->nodes[pi].hold_count - 1; j++) {
                g->nodes[pi].holds[j] = g->nodes[pi].holds[j + 1];
            }
            g->nodes[pi].hold_count = g->nodes[pi].hold_count - 1;
            return;
        }
    }
}

int main() {
    struct ContentionGraph g;
    int i;

    // EXPECT: === Resource Contention Detector ===
    printf("=== Resource Contention Detector ===\n");

    // Classic deadlock: P1 holds R1, wants R2; P2 holds R2, wants R1
    cg_init(&g);
    cg_add_node(&g, 1, 1);  // Process 1
    cg_add_node(&g, 2, 1);  // Process 2
    cg_add_node(&g, 101, 0); // Resource 1
    cg_add_node(&g, 102, 0); // Resource 2

    cg_assign_resource(&g, 1, 101);
    cg_assign_resource(&g, 2, 102);
    cg_request_resource(&g, 1, 102);
    cg_request_resource(&g, 2, 101);

    int dl = cg_detect_any_deadlock(&g);
    // EXPECT: Deadlock detected starting at process: 1
    printf("Deadlock detected starting at process: %d\n", dl);

    // EXPECT: Blocked processes: 2
    printf("Blocked processes: %d\n", cg_count_blocked(&g));

    // No deadlock scenario
    struct ContentionGraph g2;
    cg_init(&g2);
    cg_add_node(&g2, 1, 1);
    cg_add_node(&g2, 2, 1);
    cg_add_node(&g2, 3, 1);
    cg_add_node(&g2, 101, 0);
    cg_add_node(&g2, 102, 0);

    cg_assign_resource(&g2, 1, 101);
    cg_request_resource(&g2, 2, 101);
    cg_request_resource(&g2, 3, 101);

    int dl2 = cg_detect_any_deadlock(&g2);
    // EXPECT: No deadlock: -1
    printf("No deadlock: %d\n", dl2);

    int contended = cg_find_most_contended(&g2);
    // EXPECT: Most contended resource: 101
    printf("Most contended resource: %d\n", contended);

    int cont_count = cg_count_contention(&g2, 101);
    // EXPECT: Contention on R101: 2
    printf("Contention on R101: %d\n", cont_count);

    // 3-way deadlock: P1->R1, P2->R2, P3->R3, P1 wants R2, P2 wants R3, P3 wants R1
    struct ContentionGraph g3;
    cg_init(&g3);
    cg_add_node(&g3, 1, 1);
    cg_add_node(&g3, 2, 1);
    cg_add_node(&g3, 3, 1);
    cg_add_node(&g3, 201, 0);
    cg_add_node(&g3, 202, 0);
    cg_add_node(&g3, 203, 0);

    cg_assign_resource(&g3, 1, 201);
    cg_assign_resource(&g3, 2, 202);
    cg_assign_resource(&g3, 3, 203);
    cg_request_resource(&g3, 1, 202);
    cg_request_resource(&g3, 2, 203);
    cg_request_resource(&g3, 3, 201);

    int dl3 = cg_detect_any_deadlock(&g3);
    // EXPECT: 3-way deadlock at process: 1
    printf("3-way deadlock at process: %d\n", dl3);
    // EXPECT: Blocked in 3-way: 3
    printf("Blocked in 3-way: %d\n", cg_count_blocked(&g3));

    // Resolve deadlock by releasing
    cg_release_resource(&g3, 1, 201);
    g3.nodes[cg_find_node(&g3, 1)].waits_for = -1;

    int dl4 = cg_detect_any_deadlock(&g3);
    // EXPECT: After resolution: -1
    printf("After resolution: %d\n", dl4);

    // Check holder function
    int holder = cg_find_holder(&g3, 202);
    // EXPECT: Holder of R202: 2
    printf("Holder of R202: %d\n", holder);

    int holder2 = cg_find_holder(&g3, 201);
    // EXPECT: Holder of R201 after release: -1
    printf("Holder of R201 after release: %d\n", holder2);

    // EXPECT: All contention detector tests passed
    printf("All contention detector tests passed\n");
    return 0;
}
