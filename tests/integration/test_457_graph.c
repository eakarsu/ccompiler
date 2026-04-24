int printf(const char *fmt, ...);
// EXPECT: BFS from 0: 0 1 2 3 4 5 6 \nDFS from 0: 0 1 3 4 2 5 6 \nPath 0->6: 1\nPath 3->5: 1\nDegree of 1: 3\nDegree of 0: 2\nEdges: 6
// Graph with adjacency list (array-based), BFS, DFS

struct Graph {
    int adj[10][10];
    int num_vertices;
};

void graph_init(struct Graph *g, int n) {
    int i;
    int j;
    g->num_vertices = n;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void graph_add_edge(struct Graph *g, int u, int v) {
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void graph_bfs(struct Graph *g, int start) {
    int visited[10];
    int queue[10];
    int front;
    int back;
    int i;
    int cur;
    for (i = 0; i < 10; i++) visited[i] = 0;
    front = 0;
    back = 0;
    queue[back] = start;
    back++;
    visited[start] = 1;
    printf("BFS from %d: ", start);
    while (front < back) {
        cur = queue[front];
        front++;
        printf("%d ", cur);
        for (i = 0; i < g->num_vertices; i++) {
            if (g->adj[cur][i] && !visited[i]) {
                visited[i] = 1;
                queue[back] = i;
                back++;
            }
        }
    }
    printf("\n");
}

void graph_dfs_helper(struct Graph *g, int node, int *visited) {
    int i;
    visited[node] = 1;
    printf("%d ", node);
    for (i = 0; i < g->num_vertices; i++) {
        if (g->adj[node][i] && !visited[i]) {
            graph_dfs_helper(g, i, visited);
        }
    }
}

void graph_dfs(struct Graph *g, int start) {
    int visited[10];
    int i;
    for (i = 0; i < 10; i++) visited[i] = 0;
    printf("DFS from %d: ", start);
    graph_dfs_helper(g, start, visited);
    printf("\n");
}

int graph_has_path(struct Graph *g, int src, int dst) {
    int visited[10];
    int queue[10];
    int front;
    int back;
    int i;
    int cur;
    for (i = 0; i < 10; i++) visited[i] = 0;
    front = 0; back = 0;
    queue[back] = src; back++;
    visited[src] = 1;
    while (front < back) {
        cur = queue[front]; front++;
        if (cur == dst) return 1;
        for (i = 0; i < g->num_vertices; i++) {
            if (g->adj[cur][i] && !visited[i]) {
                visited[i] = 1;
                queue[back] = i; back++;
            }
        }
    }
    return 0;
}

int graph_degree(struct Graph *g, int v) {
    int d = 0;
    int i;
    for (i = 0; i < g->num_vertices; i++) {
        if (g->adj[v][i]) d++;
    }
    return d;
}

int graph_count_edges(struct Graph *g) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < g->num_vertices; i++) {
        for (j = i + 1; j < g->num_vertices; j++) {
            if (g->adj[i][j]) count++;
        }
    }
    return count;
}

int main(void) {
    struct Graph g;
    graph_init(&g, 7);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 0, 2);
    graph_add_edge(&g, 1, 3);
    graph_add_edge(&g, 1, 4);
    graph_add_edge(&g, 2, 5);
    graph_add_edge(&g, 2, 6);

    graph_bfs(&g, 0);
    graph_dfs(&g, 0);

    printf("Path 0->6: %d\n", graph_has_path(&g, 0, 6));
    printf("Path 3->5: %d\n", graph_has_path(&g, 3, 5));
    printf("Degree of 1: %d\n", graph_degree(&g, 1));
    printf("Degree of 0: %d\n", graph_degree(&g, 0));
    printf("Edges: %d\n", graph_count_edges(&g));

    return 0;
}
