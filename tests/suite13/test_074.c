int printf(const char *fmt, ...);

// Cycle detection in directed graph using DFS coloring

struct Graph {
    int adj[8][8];
    int n;
};

void graph_init(struct Graph *g, int n) {
    int i;
    int j;
    g->n = n;
    for (i = 0; i < 8; i = i + 1) {
        for (j = 0; j < 8; j = j + 1) {
            g->adj[i][j] = 0;
        }
    }
}

void graph_add_edge(struct Graph *g, int from, int to) {
    g->adj[from][to] = 1;
}

// Color states: 0=WHITE (unvisited), 1=GRAY (in stack), 2=BLACK (done)
// Returns 1 if cycle found from node u
int dfs_cycle(struct Graph *g, int u, int *color, int *cycle_node) {
    int v;
    color[u] = 1; // GRAY
    for (v = 0; v < g->n; v = v + 1) {
        if (g->adj[u][v]) {
            if (color[v] == 1) {
                // Back edge: cycle detected
                *cycle_node = v;
                return 1;
            }
            if (color[v] == 0) {
                if (dfs_cycle(g, v, color, cycle_node)) {
                    return 1;
                }
            }
        }
    }
    color[u] = 2; // BLACK
    return 0;
}

// Detect if graph has any cycle, returns 1 if cycle exists
int has_cycle(struct Graph *g) {
    int color[8];
    int i;
    int dummy;
    for (i = 0; i < g->n; i = i + 1) {
        color[i] = 0;
    }
    for (i = 0; i < g->n; i = i + 1) {
        if (color[i] == 0) {
            if (dfs_cycle(g, i, color, &dummy)) {
                return 1;
            }
        }
    }
    return 0;
}

// Find a node that is part of a cycle (returns -1 if no cycle)
int find_cycle_node(struct Graph *g) {
    int color[8];
    int i;
    int node;
    node = -1;
    for (i = 0; i < g->n; i = i + 1) {
        color[i] = 0;
    }
    for (i = 0; i < g->n; i = i + 1) {
        if (color[i] == 0) {
            if (dfs_cycle(g, i, color, &node)) {
                return node;
            }
        }
    }
    return -1;
}

// Count number of back edges (cycles indicators)
int count_back_edges_helper(struct Graph *g, int u, int *color) {
    int v;
    int cnt;
    color[u] = 1;
    cnt = 0;
    for (v = 0; v < g->n; v = v + 1) {
        if (g->adj[u][v]) {
            if (color[v] == 1) {
                cnt = cnt + 1;
            }
            if (color[v] == 0) {
                cnt = cnt + count_back_edges_helper(g, v, color);
            }
        }
    }
    color[u] = 2;
    return cnt;
}

int count_back_edges(struct Graph *g) {
    int color[8];
    int i;
    int cnt;
    cnt = 0;
    for (i = 0; i < g->n; i = i + 1) {
        color[i] = 0;
    }
    for (i = 0; i < g->n; i = i + 1) {
        if (color[i] == 0) {
            cnt = cnt + count_back_edges_helper(g, i, color);
        }
    }
    return cnt;
}

// Check if removing an edge breaks a cycle
int removing_edge_breaks_cycle(struct Graph *g, int from, int to) {
    int had_cycle;
    int result;
    had_cycle = has_cycle(g);
    g->adj[from][to] = 0;
    result = !has_cycle(g);
    g->adj[from][to] = 1;  // restore
    if (!had_cycle) return 0;
    return result;
}

int main() {
    struct Graph g;

    // Acyclic graph: 0->1->2->3
    graph_init(&g, 4);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);

    printf("%d\n", has_cycle(&g));
    // EXPECT: 0
    printf("%d\n", find_cycle_node(&g));
    // EXPECT: -1
    printf("%d\n", count_back_edges(&g));
    // EXPECT: 0

    // Add back edge 3->1: creates cycle 1->2->3->1
    graph_add_edge(&g, 3, 1);
    printf("%d\n", has_cycle(&g));
    // EXPECT: 1
    printf("%d\n", find_cycle_node(&g));
    // EXPECT: 1
    printf("%d\n", count_back_edges(&g));
    // EXPECT: 1

    // Removing 3->1 breaks the cycle
    printf("%d\n", removing_edge_breaks_cycle(&g, 3, 1));
    // EXPECT: 1
    // Removing 0->1 does not break the 1->2->3->1 cycle
    printf("%d\n", removing_edge_breaks_cycle(&g, 0, 1));
    // EXPECT: 0

    // Self-loop: 0->0
    graph_init(&g, 3);
    graph_add_edge(&g, 0, 0);
    graph_add_edge(&g, 1, 2);
    printf("%d\n", has_cycle(&g));
    // EXPECT: 1
    printf("%d\n", find_cycle_node(&g));
    // EXPECT: 0
    printf("%d\n", count_back_edges(&g));
    // EXPECT: 1

    // Two separate cycles: 0->1->0 and 2->3->4->2
    graph_init(&g, 5);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 0);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 4);
    graph_add_edge(&g, 4, 2);
    printf("%d\n", has_cycle(&g));
    // EXPECT: 1
    printf("%d\n", count_back_edges(&g));
    // EXPECT: 2

    // Complex DAG with no cycle: 0->1, 0->2, 1->3, 2->3, 3->4, 2->4
    graph_init(&g, 5);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 0, 2);
    graph_add_edge(&g, 1, 3);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 4);
    graph_add_edge(&g, 2, 4);
    printf("%d\n", has_cycle(&g));
    // EXPECT: 0
    printf("%d\n", count_back_edges(&g));
    // EXPECT: 0

    // Add edge 4->0: creates cycle
    graph_add_edge(&g, 4, 0);
    printf("%d\n", has_cycle(&g));
    // EXPECT: 1
    printf("%d\n", find_cycle_node(&g));
    // EXPECT: 0
    printf("%d\n", count_back_edges(&g));
    // EXPECT: 1

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
