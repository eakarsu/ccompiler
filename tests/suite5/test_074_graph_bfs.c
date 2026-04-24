int printf(const char *fmt, ...);

int adj[16][16];
int n_vertices;

int visited[16];
int dist[16];
int parent[16];
int bfs_order[16];
int bfs_count;

int queue[64];
int q_front;
int q_back;

void q_init(void) {
    q_front = 0;
    q_back = 0;
}

void q_push(int v) {
    queue[q_back] = v;
    q_back = q_back + 1;
}

int q_pop(void) {
    int v = queue[q_front];
    q_front = q_front + 1;
    return v;
}

int q_empty(void) {
    return q_front == q_back;
}

void init_graph(int n) {
    int i;
    int j;
    n_vertices = n;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            adj[i][j] = 0;
        }
    }
}

void add_edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

void bfs(int start) {
    int i;
    int u;
    int v;
    bfs_count = 0;
    for (i = 0; i < n_vertices; i = i + 1) {
        visited[i] = 0;
        dist[i] = -1;
        parent[i] = -1;
    }
    visited[start] = 1;
    dist[start] = 0;
    q_init();
    q_push(start);
    while (!q_empty()) {
        u = q_pop();
        bfs_order[bfs_count] = u;
        bfs_count = bfs_count + 1;
        for (v = 0; v < n_vertices; v = v + 1) {
            if (adj[u][v] && !visited[v]) {
                visited[v] = 1;
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q_push(v);
            }
        }
    }
}

int is_connected(void) {
    bfs(0);
    return bfs_count == n_vertices;
}

int main(void) {
    int i;

    init_graph(6);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(2, 3);
    add_edge(3, 4);
    add_edge(4, 5);

    bfs(0);

    // EXPECT: bfs from 0: 0 1 2 3 4 5
    printf("bfs from 0:");
    for (i = 0; i < bfs_count; i = i + 1) {
        printf(" %d", bfs_order[i]);
    }
    printf("\n");

    // EXPECT: dist[0]: 0
    printf("dist[0]: %d\n", dist[0]);
    // EXPECT: dist[1]: 1
    printf("dist[1]: %d\n", dist[1]);
    // EXPECT: dist[3]: 2
    printf("dist[3]: %d\n", dist[3]);
    // EXPECT: dist[5]: 4
    printf("dist[5]: %d\n", dist[5]);

    // EXPECT: parent[1]: 0
    printf("parent[1]: %d\n", parent[1]);
    // EXPECT: parent[3]: 1
    printf("parent[3]: %d\n", parent[3]);
    // EXPECT: parent[5]: 4
    printf("parent[5]: %d\n", parent[5]);

    // EXPECT: connected: 1
    printf("connected: %d\n", is_connected());

    // EXPECT: bfs_count: 6
    printf("bfs_count: %d\n", bfs_count);

    init_graph(4);
    add_edge(0, 1);
    add_edge(2, 3);
    bfs(0);

    // EXPECT: disconn bfs: 2
    printf("disconn bfs: %d\n", bfs_count);
    // EXPECT: disconn connected: 0
    printf("disconn connected: %d\n", is_connected());

    // EXPECT: dist to 2: -1
    printf("dist to 2: %d\n", dist[2]);
    // EXPECT: dist to 3: -1
    printf("dist to 3: %d\n", dist[3]);

    init_graph(5);
    add_edge(0, 1);
    add_edge(0, 4);
    add_edge(1, 2);
    add_edge(1, 3);
    add_edge(2, 3);
    add_edge(3, 4);
    bfs(0);
    // EXPECT: star dist[2]: 2
    printf("star dist[2]: %d\n", dist[2]);
    // EXPECT: star dist[3]: 2
    printf("star dist[3]: %d\n", dist[3]);

    return 0;
}
