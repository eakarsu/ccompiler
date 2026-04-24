int printf(const char *fmt, ...);

// Task Dependency Resolver (Topological Sort)
// Resolves execution order for tasks with dependencies

struct TaskGraph {
    int adj[12][12];
    int n;
    char *names[12];
};

void graph_init(struct TaskGraph *g, int n) {
    int i;
    int j;
    g->n = n;
    for (i = 0; i < n; i++) {
        g->names[i] = "unnamed";
        for (j = 0; j < n; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void graph_add_edge(struct TaskGraph *g, int from, int to) {
    g->adj[from][to] = 1;
}

int graph_has_cycle_util(struct TaskGraph *g, int node, int *visited, int *rec_stack) {
    int i;
    visited[node] = 1;
    rec_stack[node] = 1;
    for (i = 0; i < g->n; i++) {
        if (g->adj[node][i]) {
            if (visited[i] == 0) {
                if (graph_has_cycle_util(g, i, visited, rec_stack)) {
                    return 1;
                }
            } else if (rec_stack[i]) {
                return 1;
            }
        }
    }
    rec_stack[node] = 0;
    return 0;
}

int graph_has_cycle(struct TaskGraph *g) {
    int visited[12];
    int rec_stack[12];
    int i;
    for (i = 0; i < g->n; i++) {
        visited[i] = 0;
        rec_stack[i] = 0;
    }
    for (i = 0; i < g->n; i++) {
        if (visited[i] == 0) {
            if (graph_has_cycle_util(g, i, visited, rec_stack)) {
                return 1;
            }
        }
    }
    return 0;
}

void topo_sort_util(struct TaskGraph *g, int node, int *visited, int *stack, int *sp) {
    int i;
    visited[node] = 1;
    for (i = 0; i < g->n; i++) {
        if (g->adj[node][i] && visited[i] == 0) {
            topo_sort_util(g, i, visited, stack, sp);
        }
    }
    stack[*sp] = node;
    *sp = *sp + 1;
}

int topo_sort(struct TaskGraph *g, int *result) {
    int visited[12];
    int stack[12];
    int sp = 0;
    int i;

    if (graph_has_cycle(g)) return -1;

    for (i = 0; i < g->n; i++) {
        visited[i] = 0;
    }
    for (i = 0; i < g->n; i++) {
        if (visited[i] == 0) {
            topo_sort_util(g, i, visited, stack, &sp);
        }
    }
    for (i = 0; i < g->n; i++) {
        result[i] = stack[sp - 1 - i];
    }
    return sp;
}

int verify_order(struct TaskGraph *g, int *order, int n) {
    int pos[12];
    int i;
    int j;
    for (i = 0; i < n; i++) {
        pos[order[i]] = i;
    }
    for (i = 0; i < g->n; i++) {
        for (j = 0; j < g->n; j++) {
            if (g->adj[i][j]) {
                if (pos[i] >= pos[j]) return 0;
            }
        }
    }
    return 1;
}

int compute_depth(struct TaskGraph *g, int node, int *depths) {
    int i;
    int max_dep = 0;
    if (depths[node] >= 0) return depths[node];
    for (i = 0; i < g->n; i++) {
        if (g->adj[i][node]) {
            int d = compute_depth(g, i, depths) + 1;
            if (d > max_dep) max_dep = d;
        }
    }
    depths[node] = max_dep;
    return max_dep;
}

int main() {
    struct TaskGraph g;
    int order[12];
    int count;
    int i;

    // EXPECT: === Task Dependency Resolver ===
    printf("=== Task Dependency Resolver ===\n");

    graph_init(&g, 6);
    g.names[0] = "compile";
    g.names[1] = "link";
    g.names[2] = "test";
    g.names[3] = "package";
    g.names[4] = "deploy";
    g.names[5] = "notify";

    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 4);
    graph_add_edge(&g, 4, 5);

    // EXPECT: Has cycle: 0
    printf("Has cycle: %d\n", graph_has_cycle(&g));

    count = topo_sort(&g, order);
    // EXPECT: Topological order (6 tasks):
    printf("Topological order (%d tasks):\n", count);
    for (i = 0; i < count; i++) {
        printf("  %d: %s\n", i, g.names[order[i]]);
    }
    // EXPECT:   0: compile
    // EXPECT:   1: link
    // EXPECT:   2: test
    // EXPECT:   3: package
    // EXPECT:   4: deploy
    // EXPECT:   5: notify

    int valid = verify_order(&g, order, count);
    // EXPECT: Order valid: 1
    printf("Order valid: %d\n", valid);

    struct TaskGraph g2;
    graph_init(&g2, 4);
    graph_add_edge(&g2, 0, 2);
    graph_add_edge(&g2, 1, 2);
    graph_add_edge(&g2, 2, 3);
    count = topo_sort(&g2, order);
    // EXPECT: Diamond graph tasks: 4
    printf("Diamond graph tasks: %d\n", count);
    int valid2 = verify_order(&g2, order, count);
    // EXPECT: Diamond order valid: 1
    printf("Diamond order valid: %d\n", valid2);

    struct TaskGraph g3;
    graph_init(&g3, 3);
    graph_add_edge(&g3, 0, 1);
    graph_add_edge(&g3, 1, 2);
    graph_add_edge(&g3, 2, 0);
    // EXPECT: Cycle detected: 1
    printf("Cycle detected: %d\n", graph_has_cycle(&g3));
    int r = topo_sort(&g3, order);
    // EXPECT: Topo sort with cycle: -1
    printf("Topo sort with cycle: %d\n", r);

    int depths[12];
    for (i = 0; i < 6; i++) depths[i] = -1;
    int d = compute_depth(&g, 5, depths);
    // EXPECT: Critical path depth: 5
    printf("Critical path depth: %d\n", d);

    // EXPECT: All dependency resolver tests passed
    printf("All dependency resolver tests passed\n");
    return 0;
}
