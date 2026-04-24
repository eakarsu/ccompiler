int printf(const char *fmt, ...);
// EXPECT: Distances from 0:\n  to 0: 0\n  to 1: 3\n  to 2: 2\n  to 3: 8\n  to 4: 10\n  to 5: 13\nPath 0 to 5: 0 -> 2 -> 1 -> 3 -> 4 -> 5\nPath 0 to 4: 0 -> 2 -> 1 -> 3 -> 4\nDistances from 3:\n  to 0: 8\n  to 1: 5\n  to 2: 6\n  to 3: 0\n  to 4: 2\n  to 5: 5
// Test: Dijkstra's shortest path (small graph)

int adj[10][10];
int num_nodes;

void init_graph(int n) {
    num_nodes = n;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }
}

void add_edge(int u, int v, int w) {
    adj[u][v] = w;
    adj[v][u] = w;
}

void dijkstra(int src, int dist[], int prev[]) {
    int visited[10];
    int i, j;
    int INF = 999999;

    for (i = 0; i < num_nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (i = 0; i < num_nodes; i++) {
        int u = -1;
        int min_dist = INF;
        for (j = 0; j < num_nodes; j++) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                u = j;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        for (j = 0; j < num_nodes; j++) {
            if (adj[u][j] > 0 && !visited[j]) {
                int new_dist = dist[u] + adj[u][j];
                if (new_dist < dist[j]) {
                    dist[j] = new_dist;
                    prev[j] = u;
                }
            }
        }
    }
}

void print_path(int prev[], int dest) {
    if (prev[dest] != -1) {
        print_path(prev, prev[dest]);
        printf(" -> ");
    }
    printf("%d", dest);
}

int main(void) {
    init_graph(6);
    add_edge(0, 1, 4);
    add_edge(0, 2, 2);
    add_edge(1, 2, 1);
    add_edge(1, 3, 5);
    add_edge(2, 3, 8);
    add_edge(2, 4, 10);
    add_edge(3, 4, 2);
    add_edge(3, 5, 6);
    add_edge(4, 5, 3);

    int dist[10];
    int prev[10];

    dijkstra(0, dist, prev);

    printf("Distances from 0:\n");
    int i;
    for (i = 0; i < 6; i++) {
        printf("  to %d: %d\n", i, dist[i]);
    }

    printf("Path 0 to 5: ");
    print_path(prev, 5);
    printf("\n");

    printf("Path 0 to 4: ");
    print_path(prev, 4);
    printf("\n");

    dijkstra(3, dist, prev);
    printf("Distances from 3:\n");
    for (i = 0; i < 6; i++) {
        printf("  to %d: %d\n", i, dist[i]);
    }

    return 0;
}
