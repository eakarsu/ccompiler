int printf(const char *fmt, ...);

int adj[16][16];
int n_vertices;

int dist[16];
int visited[16];
int parent[16];

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

void add_edge(int u, int v, int w) {
    adj[u][v] = w;
    adj[v][u] = w;
}

void add_directed(int u, int v, int w) {
    adj[u][v] = w;
}

void dijkstra(int src) {
    int i;
    int u;
    int v;
    int min_d;
    int min_u;

    for (i = 0; i < n_vertices; i = i + 1) {
        dist[i] = 99999;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[src] = 0;

    for (i = 0; i < n_vertices; i = i + 1) {
        min_d = 99999;
        min_u = -1;
        for (u = 0; u < n_vertices; u = u + 1) {
            if (!visited[u] && dist[u] < min_d) {
                min_d = dist[u];
                min_u = u;
            }
        }
        if (min_u == -1) break;
        visited[min_u] = 1;
        for (v = 0; v < n_vertices; v = v + 1) {
            if (adj[min_u][v] > 0 && !visited[v]) {
                if (dist[min_u] + adj[min_u][v] < dist[v]) {
                    dist[v] = dist[min_u] + adj[min_u][v];
                    parent[v] = min_u;
                }
            }
        }
    }
}

int path_buf[16];
int path_len;

void get_path(int dest) {
    int cur;
    int i;
    int j;
    int tmp;
    path_len = 0;
    cur = dest;
    while (cur != -1) {
        path_buf[path_len] = cur;
        path_len = path_len + 1;
        cur = parent[cur];
    }
    for (i = 0; i < path_len / 2; i = i + 1) {
        j = path_len - 1 - i;
        tmp = path_buf[i];
        path_buf[i] = path_buf[j];
        path_buf[j] = tmp;
    }
}

int main(void) {
    int i;

    init_graph(6);
    add_edge(0, 1, 4);
    add_edge(0, 2, 2);
    add_edge(1, 3, 5);
    add_edge(2, 1, 1);
    add_edge(2, 3, 8);
    add_edge(3, 4, 2);
    add_edge(4, 5, 6);

    dijkstra(0);

    // EXPECT: dist[0]: 0
    printf("dist[0]: %d\n", dist[0]);
    // EXPECT: dist[1]: 3
    printf("dist[1]: %d\n", dist[1]);
    // EXPECT: dist[2]: 2
    printf("dist[2]: %d\n", dist[2]);
    // EXPECT: dist[3]: 8
    printf("dist[3]: %d\n", dist[3]);
    // EXPECT: dist[4]: 10
    printf("dist[4]: %d\n", dist[4]);
    // EXPECT: dist[5]: 16
    printf("dist[5]: %d\n", dist[5]);

    get_path(5);
    // EXPECT: path to 5: 0 2 1 3 4 5
    printf("path to 5:");
    for (i = 0; i < path_len; i = i + 1) {
        printf(" %d", path_buf[i]);
    }
    printf("\n");

    // EXPECT: path len to 5: 6
    printf("path len to 5: %d\n", path_len);

    get_path(3);
    // EXPECT: path to 3: 0 2 1 3
    printf("path to 3:");
    for (i = 0; i < path_len; i = i + 1) {
        printf(" %d", path_buf[i]);
    }
    printf("\n");

    init_graph(5);
    add_directed(0, 1, 10);
    add_directed(0, 3, 5);
    add_directed(1, 2, 1);
    add_directed(1, 3, 2);
    add_directed(2, 4, 4);
    add_directed(3, 1, 3);
    add_directed(3, 2, 9);
    add_directed(3, 4, 2);
    add_directed(4, 2, 6);

    dijkstra(0);
    // EXPECT: dir dist[1]: 8
    printf("dir dist[1]: %d\n", dist[1]);
    // EXPECT: dir dist[2]: 9
    printf("dir dist[2]: %d\n", dist[2]);
    // EXPECT: dir dist[3]: 5
    printf("dir dist[3]: %d\n", dist[3]);
    // EXPECT: dir dist[4]: 7
    printf("dir dist[4]: %d\n", dist[4]);

    get_path(4);
    // EXPECT: dir path to 4: 0 3 4
    printf("dir path to 4:");
    for (i = 0; i < path_len; i = i + 1) {
        printf(" %d", path_buf[i]);
    }
    printf("\n");

    // EXPECT: dir path len: 3
    printf("dir path len: %d\n", path_len);

    return 0;
}
