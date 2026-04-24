int printf(const char *fmt, ...);

// BFS shortest path on unweighted directed graph

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

void graph_add_undirected(struct Graph *g, int u, int v) {
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

// BFS from src, fills dist[] with shortest distances (-1 if unreachable)
void bfs(struct Graph *g, int src, int *dist) {
    int queue[64];
    int front;
    int back;
    int i;
    int u;
    int v;

    for (i = 0; i < g->n; i = i + 1) {
        dist[i] = -1;
    }

    dist[src] = 0;
    front = 0;
    back = 0;
    queue[back] = src;
    back = back + 1;

    while (front < back) {
        u = queue[front];
        front = front + 1;

        for (v = 0; v < g->n; v = v + 1) {
            if (g->adj[u][v] && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                queue[back] = v;
                back = back + 1;
            }
        }
    }
}

// BFS with parent tracking to reconstruct path
// Returns path length, fills path[] with nodes from src to dst
int bfs_path(struct Graph *g, int src, int dst, int *path) {
    int dist[8];
    int parent[8];
    int queue[64];
    int front;
    int back;
    int i;
    int u;
    int v;
    int len;

    for (i = 0; i < g->n; i = i + 1) {
        dist[i] = -1;
        parent[i] = -1;
    }

    dist[src] = 0;
    front = 0;
    back = 0;
    queue[back] = src;
    back = back + 1;

    while (front < back) {
        u = queue[front];
        front = front + 1;
        if (u == dst) break;

        for (v = 0; v < g->n; v = v + 1) {
            if (g->adj[u][v] && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                parent[v] = u;
                queue[back] = v;
                back = back + 1;
            }
        }
    }

    if (dist[dst] == -1) return 0;

    // Reconstruct path backwards
    len = dist[dst] + 1;
    v = dst;
    i = len - 1;
    while (i >= 0) {
        path[i] = v;
        v = parent[v];
        i = i - 1;
    }
    return len;
}

// Count nodes at a given distance from source
int count_at_distance(struct Graph *g, int src, int d) {
    int dist[8];
    int cnt;
    int i;
    bfs(g, src, dist);
    cnt = 0;
    for (i = 0; i < g->n; i = i + 1) {
        if (dist[i] == d) cnt = cnt + 1;
    }
    return cnt;
}

int main() {
    struct Graph g;
    int dist[8];
    int path[8];
    int len;

    // Undirected graph, 7 nodes
    //   0 -- 1 -- 3 -- 5
    //   |    |         |
    //   2    4         6
    graph_init(&g, 7);
    graph_add_undirected(&g, 0, 1);
    graph_add_undirected(&g, 0, 2);
    graph_add_undirected(&g, 1, 3);
    graph_add_undirected(&g, 1, 4);
    graph_add_undirected(&g, 3, 5);
    graph_add_undirected(&g, 5, 6);

    // BFS from 0
    bfs(&g, 0, dist);
    printf("%d %d %d %d %d %d %d\n", dist[0], dist[1], dist[2], dist[3], dist[4], dist[5], dist[6]);
    // EXPECT: 0 1 1 2 2 3 4

    // BFS from 3
    bfs(&g, 3, dist);
    printf("%d %d %d %d %d %d %d\n", dist[0], dist[1], dist[2], dist[3], dist[4], dist[5], dist[6]);
    // EXPECT: 2 1 3 0 2 1 2

    // BFS from 6
    bfs(&g, 6, dist);
    printf("%d %d %d %d %d %d %d\n", dist[0], dist[1], dist[2], dist[3], dist[4], dist[5], dist[6]);
    // EXPECT: 4 3 5 2 4 1 0

    // Path from 0 to 6: 0->1->3->5->6
    len = bfs_path(&g, 0, 6, path);
    printf("%d\n", len);
    // EXPECT: 5
    printf("%d %d %d %d %d\n", path[0], path[1], path[2], path[3], path[4]);
    // EXPECT: 0 1 3 5 6

    // Path from 2 to 4: 2->0->1->4
    len = bfs_path(&g, 2, 4, path);
    printf("%d\n", len);
    // EXPECT: 4
    printf("%d %d %d %d\n", path[0], path[1], path[2], path[3]);
    // EXPECT: 2 0 1 4

    // Path from 0 to 0: just [0]
    len = bfs_path(&g, 0, 0, path);
    printf("%d\n", len);
    // EXPECT: 1
    printf("%d\n", path[0]);
    // EXPECT: 0

    // Nodes at distance 1 from 0: nodes 1,2
    printf("%d\n", count_at_distance(&g, 0, 1));
    // EXPECT: 2
    // Nodes at distance 2 from 0: nodes 3,4
    printf("%d\n", count_at_distance(&g, 0, 2));
    // EXPECT: 2
    // Nodes at distance 3 from 0: node 5
    printf("%d\n", count_at_distance(&g, 0, 3));
    // EXPECT: 1
    // Nodes at distance 4 from 0: node 6
    printf("%d\n", count_at_distance(&g, 0, 4));
    // EXPECT: 1
    // No nodes at distance 5
    printf("%d\n", count_at_distance(&g, 0, 5));
    // EXPECT: 0

    // Directed graph test
    graph_init(&g, 5);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 0, 2);
    graph_add_edge(&g, 1, 3);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 4);

    bfs(&g, 0, dist);
    printf("%d %d %d %d %d\n", dist[0], dist[1], dist[2], dist[3], dist[4]);
    // EXPECT: 0 1 1 2 3

    // From 4, nothing reachable (directed)
    bfs(&g, 4, dist);
    printf("%d %d %d %d %d\n", dist[0], dist[1], dist[2], dist[3], dist[4]);
    // EXPECT: -1 -1 -1 -1 0

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
