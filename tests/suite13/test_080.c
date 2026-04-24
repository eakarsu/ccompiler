int printf(const char *fmt, ...);

// Strongly connected components using Kosaraju's algorithm

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

// Build transpose graph
void transpose_graph(struct Graph *g, struct Graph *gt) {
    int i;
    int j;
    graph_init(gt, g->n);
    for (i = 0; i < g->n; i = i + 1) {
        for (j = 0; j < g->n; j = j + 1) {
            if (g->adj[i][j]) {
                gt->adj[j][i] = 1;
            }
        }
    }
}

// DFS on original graph to compute finish order
void dfs_finish(struct Graph *g, int v, int *visited, int *stack, int *top) {
    int i;
    visited[v] = 1;
    for (i = 0; i < g->n; i = i + 1) {
        if (g->adj[v][i] && !visited[i]) {
            dfs_finish(g, i, visited, stack, top);
        }
    }
    stack[*top] = v;
    *top = *top + 1;
}

// DFS on transposed graph to mark SCC
void dfs_assign(struct Graph *gt, int v, int *comp, int label) {
    int i;
    comp[v] = label;
    for (i = 0; i < gt->n; i = i + 1) {
        if (gt->adj[v][i] && comp[i] == -1) {
            dfs_assign(gt, i, comp, label);
        }
    }
}

// Kosaraju's algorithm: returns number of SCCs
// comp[i] = SCC label for node i
int kosaraju(struct Graph *g, int *comp) {
    struct Graph gt;
    int visited[8];
    int stack[8];
    int top;
    int i;
    int v;
    int scc_count;

    // Step 1: DFS on original graph, fill stack by finish time
    for (i = 0; i < g->n; i = i + 1) {
        visited[i] = 0;
    }
    top = 0;
    for (i = 0; i < g->n; i = i + 1) {
        if (!visited[i]) {
            dfs_finish(g, i, visited, stack, &top);
        }
    }

    // Step 2: Transpose graph
    transpose_graph(g, &gt);

    // Step 3: DFS on transposed graph in reverse finish order
    for (i = 0; i < g->n; i = i + 1) {
        comp[i] = -1;
    }
    scc_count = 0;
    for (i = top - 1; i >= 0; i = i - 1) {
        v = stack[i];
        if (comp[v] == -1) {
            dfs_assign(&gt, v, comp, scc_count);
            scc_count = scc_count + 1;
        }
    }

    return scc_count;
}

// Size of the SCC containing node v
int scc_size(int *comp, int n, int v) {
    int i;
    int cnt;
    int label;
    label = comp[v];
    cnt = 0;
    for (i = 0; i < n; i = i + 1) {
        if (comp[i] == label) cnt = cnt + 1;
    }
    return cnt;
}

// Find largest SCC size
int largest_scc(int *comp, int n, int num_scc) {
    int sizes[8];
    int i;
    int maxs;
    for (i = 0; i < num_scc; i = i + 1) {
        sizes[i] = 0;
    }
    for (i = 0; i < n; i = i + 1) {
        sizes[comp[i]] = sizes[comp[i]] + 1;
    }
    maxs = 0;
    for (i = 0; i < num_scc; i = i + 1) {
        if (sizes[i] > maxs) maxs = sizes[i];
    }
    return maxs;
}

// Check if two nodes are in the same SCC
int same_scc(int *comp, int u, int v) {
    return comp[u] == comp[v];
}

int main() {
    struct Graph g;
    int comp[8];
    int nscc;

    // Classic SCC example:
    // SCC1: {0, 1, 2} (cycle: 0->1->2->0)
    // SCC2: {3, 4} (cycle: 3->4->3)
    // Cross edges: 2->3
    graph_init(&g, 5);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 0);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 4);
    graph_add_edge(&g, 4, 3);

    nscc = kosaraju(&g, comp);
    printf("%d\n", nscc);
    // EXPECT: 2

    // Nodes 0,1,2 should be in same SCC, nodes 3,4 in another
    printf("%d\n", same_scc(comp, 0, 1));
    // EXPECT: 1
    printf("%d\n", same_scc(comp, 0, 2));
    // EXPECT: 1
    printf("%d\n", same_scc(comp, 3, 4));
    // EXPECT: 1
    printf("%d\n", same_scc(comp, 0, 3));
    // EXPECT: 0

    printf("%d\n", largest_scc(comp, 5, nscc));
    // EXPECT: 3
    printf("%d\n", scc_size(comp, 5, 0));
    // EXPECT: 3
    printf("%d\n", scc_size(comp, 5, 3));
    // EXPECT: 2

    // DAG: each node is its own SCC
    // 0->1->2->3
    graph_init(&g, 4);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);

    nscc = kosaraju(&g, comp);
    printf("%d\n", nscc);
    // EXPECT: 4
    printf("%d\n", largest_scc(comp, 4, nscc));
    // EXPECT: 1

    // All nodes in one SCC: 0->1->2->3->0
    graph_init(&g, 4);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 0);

    nscc = kosaraju(&g, comp);
    printf("%d\n", nscc);
    // EXPECT: 1
    printf("%d\n", same_scc(comp, 0, 3));
    // EXPECT: 1
    printf("%d\n", largest_scc(comp, 4, nscc));
    // EXPECT: 4

    // Three SCCs: {0,1}, {2}, {3,4,5}
    // 0->1->0, 1->2, 2->3, 3->4->5->3
    graph_init(&g, 6);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 0);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 4);
    graph_add_edge(&g, 4, 5);
    graph_add_edge(&g, 5, 3);

    nscc = kosaraju(&g, comp);
    printf("%d\n", nscc);
    // EXPECT: 3

    printf("%d\n", same_scc(comp, 0, 1));
    // EXPECT: 1
    printf("%d\n", same_scc(comp, 3, 5));
    // EXPECT: 1
    printf("%d\n", same_scc(comp, 0, 2));
    // EXPECT: 0
    printf("%d\n", same_scc(comp, 2, 3));
    // EXPECT: 0

    printf("%d\n", scc_size(comp, 6, 0));
    // EXPECT: 2
    printf("%d\n", scc_size(comp, 6, 2));
    // EXPECT: 1
    printf("%d\n", scc_size(comp, 6, 4));
    // EXPECT: 3
    printf("%d\n", largest_scc(comp, 6, nscc));
    // EXPECT: 3

    // Single node, self-loop
    graph_init(&g, 1);
    graph_add_edge(&g, 0, 0);
    nscc = kosaraju(&g, comp);
    printf("%d\n", nscc);
    // EXPECT: 1

    // Two nodes, no edges: each is own SCC
    graph_init(&g, 2);
    nscc = kosaraju(&g, comp);
    printf("%d\n", nscc);
    // EXPECT: 2

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
