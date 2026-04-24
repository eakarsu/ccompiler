int printf(const char *fmt, ...);

/* Graph represented as adjacency matrix (up to 8 nodes).
   BFS and DFS traversals, path finding, connectivity check. */

#define MAXN 8

typedef struct {
    int adj[MAXN][MAXN]; /* adj[i][j] = 1 if edge i->j */
    int n;               /* number of nodes */
} Graph;

void graph_init(Graph *g, int n) {
    int i, j;
    g->n = n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            g->adj[i][j] = 0;
}

void graph_add_edge(Graph *g, int u, int v) {
    g->adj[u][v] = 1;
    g->adj[v][u] = 1; /* undirected */
}

/* BFS from start; fills order[] with visit order, returns count visited */
int bfs(Graph *g, int start, int *order) {
    int visited[MAXN] = {0};
    int queue[MAXN];
    int head = 0, tail = 0;
    int count = 0;

    visited[start] = 1;
    queue[tail++] = start;

    while (head < tail) {
        int u = queue[head++];
        order[count++] = u;
        int v;
        for (v = 0; v < g->n; v++) {
            if (g->adj[u][v] && !visited[v]) {
                visited[v] = 1;
                queue[tail++] = v;
            }
        }
    }
    return count;
}

/* DFS from start (iterative); fills order[], returns count visited */
int dfs(Graph *g, int start, int *order) {
    int visited[MAXN] = {0};
    int stack[MAXN];
    int top = 0;
    int count = 0;

    stack[top++] = start;

    while (top > 0) {
        int u = stack[--top];
        if (visited[u]) continue;
        visited[u] = 1;
        order[count++] = u;
        /* Push neighbors in reverse order so lower-indexed visited first */
        int v;
        for (v = g->n - 1; v >= 0; v--) {
            if (g->adj[u][v] && !visited[v]) {
                stack[top++] = v;
            }
        }
    }
    return count;
}

/* Check if graph is fully connected */
int is_connected(Graph *g) {
    int order[MAXN];
    int visited = bfs(g, 0, order);
    return visited == g->n;
}

/* BFS shortest path from src to dst; returns distance or -1 if unreachable */
int bfs_shortest_path(Graph *g, int src, int dst) {
    if (src == dst) return 0;
    int visited[MAXN] = {0};
    int dist[MAXN];
    int queue[MAXN];
    int head = 0, tail = 0;
    int i;
    for (i = 0; i < g->n; i++) dist[i] = -1;

    visited[src] = 1;
    dist[src] = 0;
    queue[tail++] = src;

    while (head < tail) {
        int u = queue[head++];
        int v;
        for (v = 0; v < g->n; v++) {
            if (g->adj[u][v] && !visited[v]) {
                visited[v] = 1;
                dist[v] = dist[u] + 1;
                if (v == dst) return dist[v];
                queue[tail++] = v;
            }
        }
    }
    return -1;
}

/* Count connected components */
int count_components(Graph *g) {
    int visited[MAXN] = {0};
    int comps = 0;
    int i;
    for (i = 0; i < g->n; i++) {
        if (!visited[i]) {
            comps++;
            /* Mini-BFS to mark component */
            int queue[MAXN];
            int head = 0, tail = 0;
            visited[i] = 1;
            queue[tail++] = i;
            while (head < tail) {
                int u = queue[head++];
                int v;
                for (v = 0; v < g->n; v++) {
                    if (g->adj[u][v] && !visited[v]) {
                        visited[v] = 1;
                        queue[tail++] = v;
                    }
                }
            }
        }
    }
    return comps;
}

/* Print traversal order */
void print_order(int *order, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", order[i]);
    }
    printf("\n");
}

int main(void) {
    /* Graph 1: 6-node connected graph
       Edges: 0-1, 0-2, 1-3, 1-4, 2-5, 3-5
       0 -- 1 -- 3
       |    |     \
       2    4      5
       |           |
       +-----------+
    */
    Graph g1;
    graph_init(&g1, 6);
    graph_add_edge(&g1, 0, 1);
    graph_add_edge(&g1, 0, 2);
    graph_add_edge(&g1, 1, 3);
    graph_add_edge(&g1, 1, 4);
    graph_add_edge(&g1, 2, 5);
    graph_add_edge(&g1, 3, 5);

    printf("%d\n", is_connected(&g1));  /* 1 */
    // EXPECT: 1

    int order[MAXN];
    int cnt;

    /* BFS from 0 */
    cnt = bfs(&g1, 0, order);
    printf("%d\n", cnt);                /* 6 */
    // EXPECT: 6
    print_order(order, cnt);
    // EXPECT: 0 1 2 3 4 5

    /* DFS from 0 */
    cnt = dfs(&g1, 0, order);
    printf("%d\n", cnt);                /* 6 */
    // EXPECT: 6
    print_order(order, cnt);
    // EXPECT: 0 1 3 5 2 4

    /* Shortest paths */
    printf("%d\n", bfs_shortest_path(&g1, 0, 5));  /* 2 (0->2->5) */
    // EXPECT: 2
    printf("%d\n", bfs_shortest_path(&g1, 0, 4));  /* 2 (0->1->4) */
    // EXPECT: 2
    printf("%d\n", bfs_shortest_path(&g1, 0, 0));  /* 0 */
    // EXPECT: 0
    printf("%d\n", bfs_shortest_path(&g1, 3, 4));  /* 3 (3->1->4 or 3->5->2->0->1->4... shortest=2: 3->1->4) */
    // EXPECT: 2
    printf("%d\n", bfs_shortest_path(&g1, 2, 4));  /* 3 (2->0->1->4) */
    // EXPECT: 3

    /* Graph 2: disconnected, 7 nodes, 3 components
       Component 1: 0-1-2
       Component 2: 3-4
       Component 3: 5, 6 (isolated nodes + one edge)
    */
    Graph g2;
    graph_init(&g2, 7);
    graph_add_edge(&g2, 0, 1);
    graph_add_edge(&g2, 1, 2);
    graph_add_edge(&g2, 3, 4);
    graph_add_edge(&g2, 5, 6);

    printf("%d\n", is_connected(&g2));       /* 0 */
    // EXPECT: 0
    printf("%d\n", count_components(&g2));   /* 3 */
    // EXPECT: 3

    /* BFS within component */
    cnt = bfs(&g2, 0, order);
    printf("%d\n", cnt);                     /* 3 */
    // EXPECT: 3
    print_order(order, cnt);
    // EXPECT: 0 1 2

    /* Unreachable path */
    printf("%d\n", bfs_shortest_path(&g2, 0, 4)); /* -1 */
    // EXPECT: -1
    printf("%d\n", bfs_shortest_path(&g2, 3, 6)); /* -1 */
    // EXPECT: -1

    /* Graph 3: complete graph K4 (4 nodes, all pairs connected) */
    Graph g3;
    graph_init(&g3, 4);
    graph_add_edge(&g3, 0, 1);
    graph_add_edge(&g3, 0, 2);
    graph_add_edge(&g3, 0, 3);
    graph_add_edge(&g3, 1, 2);
    graph_add_edge(&g3, 1, 3);
    graph_add_edge(&g3, 2, 3);

    printf("%d\n", is_connected(&g3));       /* 1 */
    // EXPECT: 1
    printf("%d\n", count_components(&g3));   /* 1 */
    // EXPECT: 1
    printf("%d\n", bfs_shortest_path(&g3, 0, 3)); /* 1 */
    // EXPECT: 1
    printf("%d\n", bfs_shortest_path(&g3, 1, 2)); /* 1 */
    // EXPECT: 1

    /* BFS from 0 on K4 */
    cnt = bfs(&g3, 0, order);
    print_order(order, cnt);
    // EXPECT: 0 1 2 3

    return 0;
}
