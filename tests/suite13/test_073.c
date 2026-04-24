int printf(const char *fmt, ...);

// Topological sort using Kahn's algorithm (BFS-based)

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

// Compute in-degree for each node
void compute_indegree(struct Graph *g, int *indeg) {
    int i;
    int j;
    for (i = 0; i < g->n; i = i + 1) {
        indeg[i] = 0;
    }
    for (i = 0; i < g->n; i = i + 1) {
        for (j = 0; j < g->n; j = j + 1) {
            if (g->adj[i][j]) {
                indeg[j] = indeg[j] + 1;
            }
        }
    }
}

// Kahn's algorithm: returns number of nodes in topological order
// If < g->n, the graph has a cycle
int topo_sort(struct Graph *g, int *result) {
    int indeg[8];
    int queue[64];
    int front;
    int back;
    int i;
    int u;
    int v;
    int cnt;

    compute_indegree(g, indeg);

    front = 0;
    back = 0;
    // Enqueue nodes with 0 in-degree
    for (i = 0; i < g->n; i = i + 1) {
        if (indeg[i] == 0) {
            queue[back] = i;
            back = back + 1;
        }
    }

    cnt = 0;
    while (front < back) {
        u = queue[front];
        front = front + 1;
        result[cnt] = u;
        cnt = cnt + 1;

        for (v = 0; v < g->n; v = v + 1) {
            if (g->adj[u][v]) {
                indeg[v] = indeg[v] - 1;
                if (indeg[v] == 0) {
                    queue[back] = v;
                    back = back + 1;
                }
            }
        }
    }
    return cnt;
}

// Verify topological order: for each edge u->v, u appears before v
int verify_topo_order(struct Graph *g, int *order, int cnt) {
    int pos[8];
    int i;
    int u;
    int v;

    if (cnt != g->n) return 0;

    for (i = 0; i < cnt; i = i + 1) {
        pos[order[i]] = i;
    }

    for (u = 0; u < g->n; u = u + 1) {
        for (v = 0; v < g->n; v = v + 1) {
            if (g->adj[u][v]) {
                if (pos[u] >= pos[v]) return 0;
            }
        }
    }
    return 1;
}

// Compute the longest path in a DAG using topo order
int longest_path_dag(struct Graph *g) {
    int order[8];
    int cnt;
    int dist[8];
    int i;
    int u;
    int v;
    int maxd;

    cnt = topo_sort(g, order);
    if (cnt != g->n) return -1;

    for (i = 0; i < g->n; i = i + 1) {
        dist[i] = 0;
    }

    for (i = 0; i < cnt; i = i + 1) {
        u = order[i];
        for (v = 0; v < g->n; v = v + 1) {
            if (g->adj[u][v]) {
                if (dist[u] + 1 > dist[v]) {
                    dist[v] = dist[u] + 1;
                }
            }
        }
    }

    maxd = 0;
    for (i = 0; i < g->n; i = i + 1) {
        if (dist[i] > maxd) maxd = dist[i];
    }
    return maxd;
}

int main() {
    struct Graph g;
    int result[8];
    int cnt;
    int indeg[8];

    // DAG with 6 nodes:
    // 0->2, 0->3, 1->3, 1->4, 2->5, 3->5, 4->5
    graph_init(&g, 6);
    graph_add_edge(&g, 0, 2);
    graph_add_edge(&g, 0, 3);
    graph_add_edge(&g, 1, 3);
    graph_add_edge(&g, 1, 4);
    graph_add_edge(&g, 2, 5);
    graph_add_edge(&g, 3, 5);
    graph_add_edge(&g, 4, 5);

    // In-degrees: 0:0, 1:0, 2:1, 3:2, 4:1, 5:3
    compute_indegree(&g, indeg);
    printf("%d %d %d %d %d %d\n", indeg[0], indeg[1], indeg[2], indeg[3], indeg[4], indeg[5]);
    // EXPECT: 0 0 1 2 1 3

    // Topo sort: queue starts with [0,1]
    // Pop 0: decrease indeg of 2(->0),3(->1). Enqueue 2.
    // Pop 1: decrease indeg of 3(->0),4(->0). Enqueue 3,4.
    // Pop 2: decrease indeg of 5(->2).
    // Pop 3: decrease indeg of 5(->1).
    // Pop 4: decrease indeg of 5(->0). Enqueue 5.
    // Pop 5.
    // Order: 0 1 2 3 4 5
    cnt = topo_sort(&g, result);
    printf("%d\n", cnt);
    // EXPECT: 6
    printf("%d %d %d %d %d %d\n", result[0], result[1], result[2], result[3], result[4], result[5]);
    // EXPECT: 0 1 2 3 4 5

    // Verify order
    printf("%d\n", verify_topo_order(&g, result, cnt));
    // EXPECT: 1

    // Longest path in this DAG: 0->3->5 or 1->4->5 = length 2
    // Actually 0->2->5 also length 2. Let's check: max dist[5]=2.
    printf("%d\n", longest_path_dag(&g));
    // EXPECT: 2

    // Linear chain: 0->1->2->3->4
    graph_init(&g, 5);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 4);

    cnt = topo_sort(&g, result);
    printf("%d\n", cnt);
    // EXPECT: 5
    printf("%d %d %d %d %d\n", result[0], result[1], result[2], result[3], result[4]);
    // EXPECT: 0 1 2 3 4
    printf("%d\n", verify_topo_order(&g, result, cnt));
    // EXPECT: 1
    printf("%d\n", longest_path_dag(&g));
    // EXPECT: 4

    // Graph with cycle: 0->1->2->0
    graph_init(&g, 3);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 0);

    cnt = topo_sort(&g, result);
    // Cycle: no valid topological order, cnt < n
    printf("%d\n", cnt);
    // EXPECT: 0

    // Diamond DAG: 0->1, 0->2, 1->3, 2->3
    graph_init(&g, 4);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 0, 2);
    graph_add_edge(&g, 1, 3);
    graph_add_edge(&g, 2, 3);

    cnt = topo_sort(&g, result);
    printf("%d\n", cnt);
    // EXPECT: 4
    printf("%d %d %d %d\n", result[0], result[1], result[2], result[3]);
    // EXPECT: 0 1 2 3
    printf("%d\n", verify_topo_order(&g, result, cnt));
    // EXPECT: 1
    printf("%d\n", longest_path_dag(&g));
    // EXPECT: 2

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
