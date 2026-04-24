int printf(const char *fmt, ...);

// DFS traversal on directed graph using adjacency matrix

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

// DFS recursive helper
void dfs_helper(struct Graph *g, int v, int *visited, int *order, int *cnt) {
    int i;
    if (visited[v]) return;
    visited[v] = 1;
    order[*cnt] = v;
    *cnt = *cnt + 1;
    for (i = 0; i < g->n; i = i + 1) {
        if (g->adj[v][i] && !visited[i]) {
            dfs_helper(g, i, visited, order, cnt);
        }
    }
}

void dfs(struct Graph *g, int start, int *order, int *cnt) {
    int visited[8];
    int i;
    for (i = 0; i < 8; i = i + 1) {
        visited[i] = 0;
    }
    *cnt = 0;
    dfs_helper(g, start, visited, order, cnt);
}

// DFS iterative using explicit stack
void dfs_iter(struct Graph *g, int start, int *order, int *cnt) {
    int visited[8];
    int stack[64];
    int top;
    int i;
    int v;

    for (i = 0; i < 8; i = i + 1) {
        visited[i] = 0;
    }

    top = 0;
    stack[top] = start;
    top = top + 1;
    *cnt = 0;

    while (top > 0) {
        top = top - 1;
        v = stack[top];
        if (visited[v]) continue;
        visited[v] = 1;
        order[*cnt] = v;
        *cnt = *cnt + 1;
        // Push in reverse order so lowest neighbor visited first
        for (i = g->n - 1; i >= 0; i = i - 1) {
            if (g->adj[v][i] && !visited[i]) {
                stack[top] = i;
                top = top + 1;
            }
        }
    }
}

// Count reachable nodes from start
int count_reachable(struct Graph *g, int start) {
    int order[8];
    int cnt;
    dfs(g, start, order, &cnt);
    return cnt;
}

// Check if path exists from src to dst
int has_path(struct Graph *g, int src, int dst) {
    int order[8];
    int cnt;
    int i;
    dfs(g, src, order, &cnt);
    for (i = 0; i < cnt; i = i + 1) {
        if (order[i] == dst) return 1;
    }
    return 0;
}

// Compute sum of DFS order node indices
int dfs_order_sum(struct Graph *g, int start) {
    int order[8];
    int cnt;
    int sum;
    int i;
    dfs(g, start, order, &cnt);
    sum = 0;
    for (i = 0; i < cnt; i = i + 1) {
        sum = sum + order[i];
    }
    return sum;
}

int main() {
    struct Graph g;
    int order[8];
    int cnt;

    // Build directed graph: 6 nodes
    // 0->1, 0->2, 1->3, 2->4, 3->5, 4->5
    graph_init(&g, 6);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 0, 2);
    graph_add_edge(&g, 1, 3);
    graph_add_edge(&g, 2, 4);
    graph_add_edge(&g, 3, 5);
    graph_add_edge(&g, 4, 5);

    // Recursive DFS from 0: 0->1->3->5, back, 2->4 (5 already visited)
    // Order: 0 1 3 5 2 4
    dfs(&g, 0, order, &cnt);
    printf("%d\n", cnt);
    // EXPECT: 6
    printf("%d %d %d %d %d %d\n", order[0], order[1], order[2], order[3], order[4], order[5]);
    // EXPECT: 0 1 3 5 2 4

    // Iterative DFS from 0: pop 0, push 2,1. Pop 1, push 3. Pop 3, push 5. Pop 5.
    // Pop 2, push 4. Pop 4 (5 visited).
    // Order: 0 1 3 5 2 4
    dfs_iter(&g, 0, order, &cnt);
    printf("%d\n", cnt);
    // EXPECT: 6
    printf("%d %d %d %d %d %d\n", order[0], order[1], order[2], order[3], order[4], order[5]);
    // EXPECT: 0 1 3 5 2 4

    // Reachability
    printf("%d\n", count_reachable(&g, 0));
    // EXPECT: 6
    // From node 2: can reach 2, 4, 5
    printf("%d\n", count_reachable(&g, 2));
    // EXPECT: 3
    // From node 5: only itself (no outgoing edges)
    printf("%d\n", count_reachable(&g, 5));
    // EXPECT: 1

    // Path checks
    printf("%d\n", has_path(&g, 0, 5));
    // EXPECT: 1
    printf("%d\n", has_path(&g, 5, 0));
    // EXPECT: 0
    printf("%d\n", has_path(&g, 2, 5));
    // EXPECT: 1
    printf("%d\n", has_path(&g, 3, 2));
    // EXPECT: 0
    printf("%d\n", has_path(&g, 1, 4));
    // EXPECT: 0

    // DFS order sum from 0: 0+1+3+5+2+4 = 15
    printf("%d\n", dfs_order_sum(&g, 0));
    // EXPECT: 15
    // From 2: 2+4+5 = 11
    printf("%d\n", dfs_order_sum(&g, 2));
    // EXPECT: 11

    // DFS from node 1: visits 1, 3, 5
    dfs(&g, 1, order, &cnt);
    printf("%d\n", cnt);
    // EXPECT: 3
    printf("%d %d %d\n", order[0], order[1], order[2]);
    // EXPECT: 1 3 5

    // Add more edges: 5->0 (creates cycle), 4->1
    graph_add_edge(&g, 5, 0);
    graph_add_edge(&g, 4, 1);

    // Now from 5: 5->0->1->3->5(visited), 2->4->1(visited), 5(visited)
    // Order: 5 0 1 3 2 4
    dfs(&g, 5, order, &cnt);
    printf("%d\n", cnt);
    // EXPECT: 6
    printf("%d %d %d %d %d %d\n", order[0], order[1], order[2], order[3], order[4], order[5]);
    // EXPECT: 5 0 1 3 2 4

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
