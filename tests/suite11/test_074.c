int printf(const char *fmt, ...);

// Kosaraju's Algorithm for Strongly Connected Components
// Directed graph: 8 vertices

int adj[10][10];
int radj[10][10];
int adj_cnt[10];
int radj_cnt[10];
int N;

int visited[10];
int finish_order[10];
int finish_idx;
int comp[10];

void init(int n) {
    int i;
    N = n;
    finish_idx = 0;
    for (i = 0; i < n; i++) {
        adj_cnt[i] = 0;
        radj_cnt[i] = 0;
        visited[i] = 0;
        comp[i] = -1;
    }
}

void add_edge(int u, int v) {
    adj[u][adj_cnt[u]] = v;
    adj_cnt[u] = adj_cnt[u] + 1;
    radj[v][radj_cnt[v]] = u;
    radj_cnt[v] = radj_cnt[v] + 1;
}

// Iterative DFS for first pass (forward graph)
void dfs1(int start) {
    int stack[64];
    int state[64];
    int top;
    int u;
    int idx;
    int v;

    top = 0;
    stack[top] = start;
    state[top] = 0;

    while (top >= 0) {
        u = stack[top];
        idx = state[top];

        if (idx == 0) {
            if (visited[u]) {
                top = top - 1;
                continue;
            }
            visited[u] = 1;
        }

        if (idx < adj_cnt[u]) {
            state[top] = idx + 1;
            v = adj[u][idx];
            if (!visited[v]) {
                top = top + 1;
                stack[top] = v;
                state[top] = 0;
            }
        } else {
            finish_order[finish_idx] = u;
            finish_idx = finish_idx + 1;
            top = top - 1;
        }
    }
}

// Iterative DFS for second pass (reverse graph)
void dfs2(int start, int component) {
    int stack[64];
    int top;
    int u;
    int i;
    int v;

    top = 0;
    stack[top] = start;
    comp[start] = component;

    while (top >= 0) {
        u = stack[top];
        top = top - 1;

        for (i = 0; i < radj_cnt[u]; i++) {
            v = radj[u][i];
            if (comp[v] == -1) {
                comp[v] = component;
                top = top + 1;
                stack[top] = v;
            }
        }
    }
}

int kosaraju() {
    int i;
    int num_scc;
    int v;

    // First pass: DFS on forward graph
    for (i = 0; i < N; i++) {
        if (!visited[i]) {
            dfs1(i);
        }
    }

    // Second pass: DFS on reverse graph in reverse finish order
    num_scc = 0;
    for (i = finish_idx - 1; i >= 0; i--) {
        v = finish_order[i];
        if (comp[v] == -1) {
            dfs2(v, num_scc);
            num_scc = num_scc + 1;
        }
    }

    return num_scc;
}

int main() {
    int num_scc;
    int i;
    int j;
    int scc_size;

    init(8);

    // SCC1: {0, 1, 2} cycle: 0->1->2->0
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 0);

    // SCC2: {3, 4} cycle: 3->4->3
    add_edge(3, 4);
    add_edge(4, 3);

    // SCC3: {5, 6, 7} cycle: 5->6->7->5
    add_edge(5, 6);
    add_edge(6, 7);
    add_edge(7, 5);

    // Cross-SCC edges (do not create new SCCs)
    add_edge(2, 3);
    add_edge(4, 5);

    num_scc = kosaraju();
    printf("Number of SCCs: %d\n", num_scc); // EXPECT: Number of SCCs: 3

    // Print component assignment
    printf("Components:\n"); // EXPECT: Components:
    for (i = 0; i < N; i++) {
        printf("  vertex %d: SCC %d\n", i, comp[i]);
    }
    // EXPECT:   vertex 0: SCC 0
    // EXPECT:   vertex 1: SCC 0
    // EXPECT:   vertex 2: SCC 0
    // EXPECT:   vertex 3: SCC 1
    // EXPECT:   vertex 4: SCC 1
    // EXPECT:   vertex 5: SCC 2
    // EXPECT:   vertex 6: SCC 2
    // EXPECT:   vertex 7: SCC 2

    // Verify: vertices in same SCC have same component
    printf("0 and 1 same SCC: %d\n", comp[0] == comp[1]); // EXPECT: 0 and 1 same SCC: 1
    printf("0 and 2 same SCC: %d\n", comp[0] == comp[2]); // EXPECT: 0 and 2 same SCC: 1
    printf("3 and 4 same SCC: %d\n", comp[3] == comp[4]); // EXPECT: 3 and 4 same SCC: 1
    printf("5 and 7 same SCC: %d\n", comp[5] == comp[7]); // EXPECT: 5 and 7 same SCC: 1
    printf("0 and 3 same SCC: %d\n", comp[0] == comp[3]); // EXPECT: 0 and 3 same SCC: 0
    printf("3 and 5 same SCC: %d\n", comp[3] == comp[5]); // EXPECT: 3 and 5 same SCC: 0

    // Count SCC sizes
    for (i = 0; i < num_scc; i++) {
        scc_size = 0;
        for (j = 0; j < N; j++) {
            if (comp[j] == i) scc_size = scc_size + 1;
        }
        printf("SCC %d size: %d\n", i, scc_size);
    }
    // EXPECT: SCC 0 size: 3
    // EXPECT: SCC 1 size: 2
    // EXPECT: SCC 2 size: 3

    return 0;
}
