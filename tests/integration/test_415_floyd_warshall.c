int printf(const char *fmt, ...);
// EXPECT: Distance matrix:\n  0   3   4   6   7\n  6   0   1   3   4\n  5   8   0   2   3\n  3   6   7   0   1\n  2   5   6   8   0\nPath 0->3: 0->1->2->3 (cost=6)\nPath 4->2: 4->0->1->2 (cost=6)\nPath 3->1: 3->4->0->1 (cost=6)
// Test: Floyd-Warshall all-pairs shortest path

int dist[8][8];
int next_node[8][8];
int num_nodes;
int INF;

void init_graph(int n) {
    num_nodes = n;
    INF = 99999;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j)
                dist[i][j] = 0;
            else
                dist[i][j] = INF;
            next_node[i][j] = -1;
        }
    }
}

void add_edge(int u, int v, int w) {
    dist[u][v] = w;
    next_node[u][v] = v;
}

void floyd_warshall(void) {
    int k, i, j;
    for (k = 0; k < num_nodes; k++) {
        for (i = 0; i < num_nodes; i++) {
            for (j = 0; j < num_nodes; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next_node[i][j] = next_node[i][k];
                }
            }
        }
    }
}

void print_path(int u, int v) {
    if (next_node[u][v] == -1) {
        printf("no path");
        return;
    }
    printf("%d", u);
    while (u != v) {
        u = next_node[u][v];
        printf("->%d", u);
    }
}

int main(void) {
    init_graph(5);
    add_edge(0, 1, 3);
    add_edge(0, 4, 7);
    add_edge(1, 2, 1);
    add_edge(1, 3, 5);
    add_edge(2, 3, 2);
    add_edge(3, 4, 1);
    add_edge(4, 0, 2);

    floyd_warshall();

    printf("Distance matrix:\n");
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (j > 0) printf(" ");
            if (dist[i][j] >= INF)
                printf("INF");
            else
                printf("%3d", dist[i][j]);
        }
        printf("\n");
    }

    printf("Path 0->3: ");
    print_path(0, 3);
    printf(" (cost=%d)\n", dist[0][3]);

    printf("Path 4->2: ");
    print_path(4, 2);
    printf(" (cost=%d)\n", dist[4][2]);

    printf("Path 3->1: ");
    print_path(3, 1);
    printf(" (cost=%d)\n", dist[3][1]);

    return 0;
}
