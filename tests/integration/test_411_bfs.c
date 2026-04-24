int printf(const char *fmt, ...);
// EXPECT: BFS from 0: 0 1 2 3 4 5 6 \nBFS from 3: 3 1 0 4 2 5 6 \nDist 0->6: 2\nDist 3->5: 4\nDist 0->0: 0
// Test: BFS on adjacency matrix

int adj[10][10];
int visited[10];
int queue[100];
int qfront;
int qrear;
int num_nodes;

void init_graph(int n) {
    num_nodes = n;
    int i, j;
    for (i = 0; i < n; i++) {
        visited[i] = 0;
        for (j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }
}

void add_edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

void bfs(int start) {
    qfront = 0;
    qrear = 0;
    int i;
    for (i = 0; i < num_nodes; i++) visited[i] = 0;

    visited[start] = 1;
    queue[qrear] = start;
    qrear++;

    while (qfront < qrear) {
        int node = queue[qfront];
        qfront++;
        printf("%d ", node);

        for (i = 0; i < num_nodes; i++) {
            if (adj[node][i] && !visited[i]) {
                visited[i] = 1;
                queue[qrear] = i;
                qrear++;
            }
        }
    }
    printf("\n");
}

int bfs_distance(int start, int end) {
    int dist[10];
    int i;
    for (i = 0; i < num_nodes; i++) {
        visited[i] = 0;
        dist[i] = -1;
    }
    qfront = 0;
    qrear = 0;
    visited[start] = 1;
    dist[start] = 0;
    queue[qrear] = start;
    qrear++;

    while (qfront < qrear) {
        int node = queue[qfront];
        qfront++;
        for (i = 0; i < num_nodes; i++) {
            if (adj[node][i] && !visited[i]) {
                visited[i] = 1;
                dist[i] = dist[node] + 1;
                queue[qrear] = i;
                qrear++;
            }
        }
    }
    return dist[end];
}

int main(void) {
    init_graph(7);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(1, 4);
    add_edge(2, 5);
    add_edge(2, 6);

    printf("BFS from 0: ");
    bfs(0);

    printf("BFS from 3: ");
    bfs(3);

    printf("Dist 0->6: %d\n", bfs_distance(0, 6));
    printf("Dist 3->5: %d\n", bfs_distance(3, 5));
    printf("Dist 0->0: %d\n", bfs_distance(0, 0));

    return 0;
}
