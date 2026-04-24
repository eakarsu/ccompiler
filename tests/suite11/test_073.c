int printf(const char *fmt, ...);

// Floyd-Warshall All-Pairs Shortest Path
// Graph: 5 vertices, directed weighted

int dist[8][8];
int next_hop[8][8];
int INF;
int N;

void init_fw(int n) {
    int i;
    int j;
    N = n;
    INF = 99999;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else {
                dist[i][j] = INF;
            }
            next_hop[i][j] = -1;
        }
    }
}

void add_directed_edge(int u, int v, int w) {
    dist[u][v] = w;
    next_hop[u][v] = v;
}

void floyd_warshall() {
    int k;
    int i;
    int j;
    int through_k;

    for (k = 0; k < N; k++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                if (dist[i][k] == INF || dist[k][j] == INF) continue;
                through_k = dist[i][k] + dist[k][j];
                if (through_k < dist[i][j]) {
                    dist[i][j] = through_k;
                    next_hop[i][j] = next_hop[i][k];
                }
            }
        }
    }
}

void print_path(int src, int dst) {
    int cur;
    if (dist[src][dst] == INF) {
        printf("No path from %d to %d\n", src, dst);
        return;
    }
    printf("Path %d->%d (cost %d): %d", src, dst, dist[src][dst], src);
    cur = src;
    while (cur != dst) {
        cur = next_hop[cur][dst];
        printf("->%d", cur);
    }
    printf("\n");
}

void print_dist_row(int row) {
    int j;
    for (j = 0; j < N; j++) {
        if (j > 0) printf(" ");
        if (dist[row][j] >= INF) {
            printf("INF");
        } else {
            printf("%d", dist[row][j]);
        }
    }
    printf("\n");
}

int main() {
    int i;

    init_fw(5);

    add_directed_edge(0, 1, 3);
    add_directed_edge(0, 2, 8);
    add_directed_edge(0, 4, -4);
    add_directed_edge(1, 3, 1);
    add_directed_edge(1, 4, 7);
    add_directed_edge(2, 1, 4);
    add_directed_edge(3, 0, 2);
    add_directed_edge(3, 2, -5);
    add_directed_edge(4, 3, 6);

    printf("Before Floyd-Warshall:\n"); // EXPECT: Before Floyd-Warshall:
    printf("dist[0][3]: %d\n", dist[0][3]); // EXPECT: dist[0][3]: 99999
    printf("dist[0][1]: %d\n", dist[0][1]); // EXPECT: dist[0][1]: 3

    floyd_warshall();

    printf("Distance matrix:\n"); // EXPECT: Distance matrix:
    for (i = 0; i < N; i++) {
        print_dist_row(i);
    }
    // EXPECT: 0 1 -3 2 -4
    // EXPECT: 3 0 -4 1 -1
    // EXPECT: 7 4 0 5 3
    // EXPECT: 2 -1 -5 0 -2
    // EXPECT: 8 5 1 6 0

    printf("Shortest distances:\n"); // EXPECT: Shortest distances:
    printf("0->1: %d\n", dist[0][1]); // EXPECT: 0->1: 1
    printf("0->2: %d\n", dist[0][2]); // EXPECT: 0->2: -3
    printf("0->3: %d\n", dist[0][3]); // EXPECT: 0->3: 2
    printf("0->4: %d\n", dist[0][4]); // EXPECT: 0->4: -4
    printf("1->0: %d\n", dist[1][0]); // EXPECT: 1->0: 3
    printf("2->0: %d\n", dist[2][0]); // EXPECT: 2->0: 7
    printf("3->2: %d\n", dist[3][2]); // EXPECT: 3->2: -5

    print_path(0, 2);
    print_path(0, 3);
    print_path(1, 0);
    // EXPECT: Path 0->2 (cost -3): 0->4->3->2
    // EXPECT: Path 0->3 (cost 2): 0->4->3
    // EXPECT: Path 1->0 (cost 3): 1->3->0

    return 0;
}
