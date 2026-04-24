int printf(const char *fmt, ...);

// Dijkstra's shortest path (simplified, array-based priority queue)

struct Graph {
    int weight[8][8];  // 0 means no edge
    int n;
};

void graph_init(struct Graph *g, int n) {
    int i;
    int j;
    g->n = n;
    for (i = 0; i < 8; i = i + 1) {
        for (j = 0; j < 8; j = j + 1) {
            g->weight[i][j] = 0;
        }
    }
}

void graph_add_edge(struct Graph *g, int from, int to, int w) {
    g->weight[from][to] = w;
}

void graph_add_undirected(struct Graph *g, int u, int v, int w) {
    g->weight[u][v] = w;
    g->weight[v][u] = w;
}

// Dijkstra: compute shortest distances from src
// dist[i] = shortest distance, -1 if unreachable
// prev[i] = predecessor on shortest path, -1 if none
void dijkstra(struct Graph *g, int src, int *dist, int *prev) {
    int visited[8];
    int i;
    int u;
    int v;
    int min_dist;
    int iter;

    for (i = 0; i < g->n; i = i + 1) {
        dist[i] = 999999;
        prev[i] = -1;
        visited[i] = 0;
    }
    dist[src] = 0;

    for (iter = 0; iter < g->n; iter = iter + 1) {
        // Find unvisited node with minimum distance
        u = -1;
        min_dist = 999999;
        for (i = 0; i < g->n; i = i + 1) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }
        if (u == -1) break;

        visited[u] = 1;

        // Relax edges from u
        for (v = 0; v < g->n; v = v + 1) {
            if (g->weight[u][v] > 0 && !visited[v]) {
                if (dist[u] + g->weight[u][v] < dist[v]) {
                    dist[v] = dist[u] + g->weight[u][v];
                    prev[v] = u;
                }
            }
        }
    }

    // Mark unreachable as -1
    for (i = 0; i < g->n; i = i + 1) {
        if (dist[i] == 999999) dist[i] = -1;
    }
}

// Reconstruct path from src to dst, return path length (node count)
int get_path(int *prev, int src, int dst, int *path) {
    int stack[8];
    int top;
    int v;
    int i;

    if (src == dst) {
        path[0] = src;
        return 1;
    }

    top = 0;
    v = dst;
    while (v != -1) {
        stack[top] = v;
        top = top + 1;
        if (v == src) break;
        v = prev[v];
    }

    if (stack[top - 1] != src) return 0;

    for (i = 0; i < top; i = i + 1) {
        path[i] = stack[top - 1 - i];
    }
    return top;
}

int main() {
    struct Graph g;
    int dist[8];
    int prev[8];
    int path[8];
    int len;

    // Undirected weighted graph, 6 nodes
    //   0 --(4)-- 1 --(1)-- 3
    //   |         |         |
    //  (2)       (3)       (5)
    //   |         |         |
    //   2 --(6)-- 4 --(2)-- 5
    graph_init(&g, 6);
    graph_add_undirected(&g, 0, 1, 4);
    graph_add_undirected(&g, 0, 2, 2);
    graph_add_undirected(&g, 1, 3, 1);
    graph_add_undirected(&g, 1, 4, 3);
    graph_add_undirected(&g, 2, 4, 6);
    graph_add_undirected(&g, 3, 5, 5);
    graph_add_undirected(&g, 4, 5, 2);

    // Dijkstra from 0
    dijkstra(&g, 0, dist, prev);
    printf("%d %d %d %d %d %d\n", dist[0], dist[1], dist[2], dist[3], dist[4], dist[5]);
    // EXPECT: 0 4 2 5 7 9

    // Path from 0 to 5: 0->1->3->5 (cost 4+1+5=10?)
    // Wait let me recheck: dist[5]=9. How?
    // 0->1 (4), 1->4 (3) = 7, 4->5 (2) = 9. So path 0->1->4->5.
    len = get_path(prev, 0, 5, path);
    printf("%d\n", len);
    // EXPECT: 4
    printf("%d %d %d %d\n", path[0], path[1], path[2], path[3]);
    // EXPECT: 0 1 4 5

    // Path from 0 to 3: 0->1->3 (cost 5)
    len = get_path(prev, 0, 3, path);
    printf("%d\n", len);
    // EXPECT: 3
    printf("%d %d %d\n", path[0], path[1], path[2]);
    // EXPECT: 0 1 3

    // Dijkstra from node 5
    dijkstra(&g, 5, dist, prev);
    printf("%d %d %d %d %d %d\n", dist[0], dist[1], dist[2], dist[3], dist[4], dist[5]);
    // EXPECT: 9 5 8 5 2 0

    // Directed graph with different paths
    // 0->(1)->1, 0->(10)->2, 1->(1)->2, 1->(6)->3, 2->(1)->3
    graph_init(&g, 4);
    graph_add_edge(&g, 0, 1, 1);
    graph_add_edge(&g, 0, 2, 10);
    graph_add_edge(&g, 1, 2, 1);
    graph_add_edge(&g, 1, 3, 6);
    graph_add_edge(&g, 2, 3, 1);

    dijkstra(&g, 0, dist, prev);
    // 0->0=0, 0->1=1, 0->1->2=2, 0->1->2->3=3
    printf("%d %d %d %d\n", dist[0], dist[1], dist[2], dist[3]);
    // EXPECT: 0 1 2 3

    // Path 0 to 3: 0->1->2->3
    len = get_path(prev, 0, 3, path);
    printf("%d\n", len);
    // EXPECT: 4
    printf("%d %d %d %d\n", path[0], path[1], path[2], path[3]);
    // EXPECT: 0 1 2 3

    // From node 3: nothing reachable (directed)
    dijkstra(&g, 3, dist, prev);
    printf("%d %d %d %d\n", dist[0], dist[1], dist[2], dist[3]);
    // EXPECT: -1 -1 -1 0

    // Disconnected: 2 components with weights
    graph_init(&g, 4);
    graph_add_undirected(&g, 0, 1, 3);
    graph_add_undirected(&g, 2, 3, 5);

    dijkstra(&g, 0, dist, prev);
    printf("%d %d %d %d\n", dist[0], dist[1], dist[2], dist[3]);
    // EXPECT: 0 3 -1 -1

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
