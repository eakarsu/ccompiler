int printf(const char *fmt, ...);

/* Breadth-First Search on a graph (adjacency matrix) */

int adj[10][10];
int n_verts;

void init_graph(int n) {
    int i; int j;
    n_verts = n;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            adj[i][j] = 0;
}

void edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

int visited[10];
int parent[10];
int dist[10];
int order[10];
int order_cnt;

int queue[20];
int qf;
int qb;

void bfs(int start) {
    int i;
    int cur;

    for (i = 0; i < n_verts; i++) {
        visited[i] = 0;
        parent[i] = -1;
        dist[i] = -1;
    }
    order_cnt = 0;
    qf = 0;
    qb = 0;

    visited[start] = 1;
    dist[start] = 0;
    queue[qb] = start;
    qb = qb + 1;

    while (qf < qb) {
        cur = queue[qf];
        qf = qf + 1;
        order[order_cnt] = cur;
        order_cnt = order_cnt + 1;

        for (i = 0; i < n_verts; i++) {
            if (adj[cur][i] && visited[i] == 0) {
                visited[i] = 1;
                parent[i] = cur;
                dist[i] = dist[cur] + 1;
                queue[qb] = i;
                qb = qb + 1;
            }
        }
    }
}

void print_path(int start, int end) {
    int path[10];
    int path_len;
    int cur;
    int i;

    if (dist[end] == -1) {
        printf("no path\n");
        return;
    }

    path_len = 0;
    cur = end;
    while (cur != -1) {
        path[path_len] = cur;
        path_len = path_len + 1;
        cur = parent[cur];
    }

    /* Reverse and print */
    printf("path %d->%d:", start, end);
    for (i = path_len - 1; i >= 0; i--) {
        printf(" %d", path[i]);
    }
    printf("\n");
}

int count_at_dist(int d) {
    int i;
    int cnt;
    cnt = 0;
    for (i = 0; i < n_verts; i++) {
        if (dist[i] == d) cnt = cnt + 1;
    }
    return cnt;
}

int main(void) {
    int i;

    /* Graph:
       0 -- 1 -- 2
       |    |    |
       3 -- 4 -- 5
            |
            6 -- 7
    */
    init_graph(8);
    edge(0, 1);
    edge(1, 2);
    edge(0, 3);
    edge(3, 4);
    edge(1, 4);
    edge(2, 5);
    edge(4, 5);
    edge(4, 6);
    edge(6, 7);

    bfs(0);

    // EXPECT: BFS order: 0 1 3 2 4 5 6 7
    printf("BFS order:");
    for (i = 0; i < order_cnt; i++) {
        printf(" %d", order[i]);
    }
    printf("\n");

    // EXPECT: dist[0]=0
    printf("dist[0]=%d\n", dist[0]);
    // EXPECT: dist[1]=1
    printf("dist[1]=%d\n", dist[1]);
    // EXPECT: dist[2]=2
    printf("dist[2]=%d\n", dist[2]);
    // EXPECT: dist[4]=2
    printf("dist[4]=%d\n", dist[4]);
    // EXPECT: dist[6]=3
    printf("dist[6]=%d\n", dist[6]);
    // EXPECT: dist[7]=4
    printf("dist[7]=%d\n", dist[7]);

    // EXPECT: path 0->7: 0 1 4 6 7
    print_path(0, 7);
    // EXPECT: path 0->5: 0 1 2 5
    print_path(0, 5);
    // EXPECT: path 0->0: 0
    print_path(0, 0);

    // EXPECT: at dist 0: 1
    printf("at dist 0: %d\n", count_at_dist(0));
    // EXPECT: at dist 1: 2
    printf("at dist 1: %d\n", count_at_dist(1));
    // EXPECT: at dist 2: 2
    printf("at dist 2: %d\n", count_at_dist(2));
    // EXPECT: at dist 3: 2
    printf("at dist 3: %d\n", count_at_dist(3));
    // EXPECT: at dist 4: 1
    printf("at dist 4: %d\n", count_at_dist(4));

    return 0;
}
