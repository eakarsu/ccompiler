
// EXPECT: === Graph DFS ===
// EXPECT: --- Graph (6 nodes) ---
// EXPECT: 0: 1 2
// EXPECT: 1: 0 3
// EXPECT: 2: 0 3 4
// EXPECT: 3: 1 2 5
// EXPECT: 4: 2 5
// EXPECT: 5: 3 4
// EXPECT: --- Recursive DFS from 0 ---
// EXPECT: DFS order: 0 1 3 2 4 5
// EXPECT: --- Iterative DFS from 0 ---
// EXPECT: DFS order: 0 1 3 2 4 5
// EXPECT: --- DFS Discovery/Finish Times ---
// EXPECT: Node 0: discover=0 finish=11
// EXPECT: Node 1: discover=1 finish=10
// EXPECT: Node 2: discover=3 finish=8
// EXPECT: Node 3: discover=2 finish=9
// EXPECT: Node 4: discover=4 finish=7
// EXPECT: Node 5: discover=5 finish=6
// EXPECT: --- Connected Components ---
// EXPECT: Graph with 8 nodes, 3 components
// EXPECT: Component of 0: 0 1 2
// EXPECT: Component of 3: 3 4 5
// EXPECT: Component of 6: 6 7
// EXPECT: --- Cycle Detection (undirected) ---
// EXPECT: Graph 1 (0-1-2-0): has_cycle=1
// EXPECT: Graph 2 (0-1, 2-3): has_cycle=0
// EXPECT: Graph 3 (0-1-2-3-4-2): has_cycle=1
// EXPECT: --- Path Existence ---
// EXPECT: path(0,5)=1
// EXPECT: path(0,3)=1
// EXPECT: path(0,4)=1
// EXPECT: --- DFS on Tree ---
// EXPECT: Tree DFS: 0 1 3 4 2 5 6
// EXPECT: === Done ===
int printf(const char *fmt, ...);

int adj[8][8];
int N;
int vis[8];
int order[8];
int ocnt;

void init_graph(int n) {
    N = n;
    int i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            adj[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
}

void add_edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

void print_graph(void) {
    int i = 0;
    while (i < N) {
        printf("%d:", i);
        int j = 0;
        while (j < N) {
            if (adj[i][j]) printf(" %d", j);
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }
}

// Recursive DFS
void dfs_rec(int u) {
    vis[u] = 1;
    order[ocnt] = u;
    ocnt = ocnt + 1;
    int v = 0;
    while (v < N) {
        if (adj[u][v] && !vis[v]) {
            dfs_rec(v);
        }
        v = v + 1;
    }
}

// Iterative DFS using stack
int stack[64];
int sp;

void stack_push(int val) {
    stack[sp] = val;
    sp = sp + 1;
}

int stack_pop(void) {
    sp = sp - 1;
    return stack[sp];
}

int stack_empty(void) {
    return sp <= 0;
}

void dfs_iter(int start) {
    int i = 0;
    while (i < N) {
        vis[i] = 0;
        i = i + 1;
    }
    sp = 0;
    ocnt = 0;
    stack_push(start);
    while (!stack_empty()) {
        int u = stack_pop();
        if (vis[u]) continue;
        vis[u] = 1;
        order[ocnt] = u;
        ocnt = ocnt + 1;
        // Push neighbors in reverse order for consistent ordering
        int v = N - 1;
        while (v >= 0) {
            if (adj[u][v] && !vis[v]) {
                stack_push(v);
            }
            v = v - 1;
        }
    }
}

// DFS with discovery and finish times
int disc[8];
int fin[8];
int timer_val;

void dfs_timed(int u) {
    vis[u] = 1;
    disc[u] = timer_val;
    timer_val = timer_val + 1;
    int v = 0;
    while (v < N) {
        if (adj[u][v] && !vis[v]) {
            dfs_timed(v);
        }
        v = v + 1;
    }
    fin[u] = timer_val;
    timer_val = timer_val + 1;
}

// Connected components
int comp[8];

void dfs_mark(int u, int c) {
    comp[u] = c;
    vis[u] = 1;
    int v = 0;
    while (v < N) {
        if (adj[u][v] && !vis[v]) {
            dfs_mark(v, c);
        }
        v = v + 1;
    }
}

int count_components(void) {
    int i = 0;
    while (i < N) {
        vis[i] = 0;
        comp[i] = -1;
        i = i + 1;
    }
    int c = 0;
    i = 0;
    while (i < N) {
        if (!vis[i]) {
            dfs_mark(i, c);
            c = c + 1;
        }
        i = i + 1;
    }
    return c;
}

// Cycle detection (undirected)
int has_cycle_flag;

void dfs_cycle(int u, int parent) {
    vis[u] = 1;
    int v = 0;
    while (v < N) {
        if (adj[u][v]) {
            if (!vis[v]) {
                dfs_cycle(v, u);
            } else if (v != parent) {
                has_cycle_flag = 1;
            }
        }
        v = v + 1;
    }
}

int detect_cycle(void) {
    int i = 0;
    while (i < N) {
        vis[i] = 0;
        i = i + 1;
    }
    has_cycle_flag = 0;
    i = 0;
    while (i < N) {
        if (!vis[i]) {
            dfs_cycle(i, -1);
        }
        i = i + 1;
    }
    return has_cycle_flag;
}

// Path existence
int path_exists(int src, int dst) {
    int i = 0;
    while (i < N) {
        vis[i] = 0;
        i = i + 1;
    }
    ocnt = 0;
    dfs_rec(src);
    return vis[dst];
}

int main(void) {
    printf("=== Graph DFS ===\n");

    // Build graph:
    // 0 - 1
    // |   |
    // 2 - 3 - 5
    // |       |
    // 4 ------+
    init_graph(6);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(2, 3);
    add_edge(2, 4);
    add_edge(3, 5);
    add_edge(4, 5);

    printf("--- Graph (6 nodes) ---\n");
    print_graph();

    printf("--- Recursive DFS from 0 ---\n");
    int i = 0;
    while (i < N) { vis[i] = 0; i = i + 1; }
    ocnt = 0;
    dfs_rec(0);
    printf("DFS order:");
    i = 0;
    while (i < ocnt) {
        printf(" %d", order[i]);
        i = i + 1;
    }
    printf("\n");

    printf("--- Iterative DFS from 0 ---\n");
    dfs_iter(0);
    printf("DFS order:");
    i = 0;
    while (i < ocnt) {
        printf(" %d", order[i]);
        i = i + 1;
    }
    printf("\n");

    printf("--- DFS Discovery/Finish Times ---\n");
    i = 0;
    while (i < N) { vis[i] = 0; i = i + 1; }
    timer_val = 0;
    dfs_timed(0);
    i = 0;
    while (i < N) {
        printf("Node %d: discover=%d finish=%d\n", i, disc[i], fin[i]);
        i = i + 1;
    }

    // Connected components test
    printf("--- Connected Components ---\n");
    init_graph(8);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(3, 4);
    add_edge(4, 5);
    add_edge(6, 7);
    int nc = count_components();
    printf("Graph with 8 nodes, %d components\n", nc);

    printf("Component of 0:");
    i = 0;
    while (i < 8) {
        if (comp[i] == comp[0]) printf(" %d", i);
        i = i + 1;
    }
    printf("\n");

    printf("Component of 3:");
    i = 0;
    while (i < 8) {
        if (comp[i] == comp[3]) printf(" %d", i);
        i = i + 1;
    }
    printf("\n");

    printf("Component of 6:");
    i = 0;
    while (i < 8) {
        if (comp[i] == comp[6]) printf(" %d", i);
        i = i + 1;
    }
    printf("\n");

    // Cycle detection
    printf("--- Cycle Detection (undirected) ---\n");

    // Triangle: 0-1-2-0
    init_graph(3);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 0);
    printf("Graph 1 (0-1-2-0): has_cycle=%d\n", detect_cycle());

    // No cycle: two disconnected edges
    init_graph(4);
    add_edge(0, 1);
    add_edge(2, 3);
    printf("Graph 2 (0-1, 2-3): has_cycle=%d\n", detect_cycle());

    // Cycle: 0-1-2-3-4-2
    init_graph(5);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 4);
    add_edge(4, 2);
    printf("Graph 3 (0-1-2-3-4-2): has_cycle=%d\n", detect_cycle());

    // Path existence
    printf("--- Path Existence ---\n");
    init_graph(6);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(2, 3);
    add_edge(2, 4);
    add_edge(3, 5);
    add_edge(4, 5);
    printf("path(0,5)=%d\n", path_exists(0, 5));
    printf("path(0,3)=%d\n", path_exists(0, 3));
    printf("path(0,4)=%d\n", path_exists(0, 4));

    // DFS on tree
    printf("--- DFS on Tree ---\n");
    init_graph(7);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(1, 4);
    add_edge(2, 5);
    add_edge(2, 6);
    i = 0;
    while (i < 7) { vis[i] = 0; i = i + 1; }
    ocnt = 0;
    dfs_rec(0);
    printf("Tree DFS:");
    i = 0;
    while (i < ocnt) {
        printf(" %d", order[i]);
        i = i + 1;
    }
    printf("\n");

    printf("=== Done ===\n");
    return 0;
}
