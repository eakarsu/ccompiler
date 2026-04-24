// EXPECT: === Graph BFS ===
// EXPECT: --- Graph Structure (8 nodes) ---
// EXPECT: 0: 1 3
// EXPECT: 1: 0 2 4
// EXPECT: 2: 1 5
// EXPECT: 3: 0 4 6
// EXPECT: 4: 1 3 5 7
// EXPECT: 5: 2 4
// EXPECT: 6: 3 7
// EXPECT: 7: 4 6
// EXPECT: --- BFS from node 0 ---
// EXPECT: Visit order: 0 1 3 2 4 6 5 7
// EXPECT: --- BFS Levels ---
// EXPECT: Level 0: 0
// EXPECT: Level 1: 1 3
// EXPECT: Level 2: 2 4 6
// EXPECT: Level 3: 5 7
// EXPECT: --- Shortest Paths from 0 ---
// EXPECT: dist[0]=0
// EXPECT: dist[1]=1
// EXPECT: dist[2]=2
// EXPECT: dist[3]=1
// EXPECT: dist[4]=2
// EXPECT: dist[5]=3
// EXPECT: dist[6]=2
// EXPECT: dist[7]=3
// EXPECT: --- Shortest Path 0 -> 7 ---
// EXPECT: Path: 0 -> 1 -> 4 -> 7
// EXPECT: Length: 3
// EXPECT: --- Shortest Path 0 -> 5 ---
// EXPECT: Path: 0 -> 1 -> 2 -> 5
// EXPECT: Length: 3
// EXPECT: --- BFS from node 5 ---
// EXPECT: Visit order: 5 2 4 1 3 7 0 6
// EXPECT: dist[0]=3
// EXPECT: dist[7]=2
// EXPECT: --- Disconnected Graph ---
// EXPECT: BFS from 0: 0 1 2
// EXPECT: BFS from 3: 3 4
// EXPECT: Reachable from 0: 3
// EXPECT: Reachable from 3: 2
// EXPECT: === Done ===

int printf(const char *fmt, ...);

int adj[8][8];
int N;

void init_graph(int n) {
    N = n;
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            adj[i][j] = 0;
            j = j + 1;
        }
        i = i + 1;
    }
}

void add_edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

void print_graph(void) {
    int i = 0;
    while (i < N) {
        printf("%d:", i);
        int j = 0;
        while (j < N) {
            if (adj[i][j]) printf(" %d", j);
            j = j + 1;
        }
        printf("\n");
        i = i + 1;
    }
}

// BFS queue implemented with array
int queue[64];
int q_front;
int q_back;

void q_init(void) {
    q_front = 0;
    q_back = 0;
}

void q_push(int val) {
    queue[q_back] = val;
    q_back = q_back + 1;
}

int q_pop(void) {
    int val = queue[q_front];
    q_front = q_front + 1;
    return val;
}

int q_empty(void) {
    return q_front >= q_back;
}

int visited[8];
int dist[8];
int parent[8];

void bfs(int start) {
    int i = 0;
    while (i < N) {
        visited[i] = 0;
        dist[i] = -1;
        parent[i] = -1;
        i = i + 1;
    }
    q_init();
    visited[start] = 1;
    dist[start] = 0;
    q_push(start);

    while (!q_empty()) {
        int u = q_pop();
        int v = 0;
        while (v < N) {
            if (adj[u][v] && !visited[v]) {
                visited[v] = 1;
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q_push(v);
            }
            v = v + 1;
        }
    }
}

void print_path(int start, int end) {
    int path[8];
    int len = 0;
    int cur = end;
    while (cur != -1) {
        path[len] = cur;
        len = len + 1;
        cur = parent[cur];
    }
    printf("Path:");
    int i = len - 1;
    while (i >= 0) {
        if (i < len - 1) printf(" ->");
        printf(" %d", path[i]);
        i = i - 1;
    }
    printf("\n");
    printf("Length: %d\n", dist[end]);
}

void bfs_level_order(int start) {
    int i = 0;
    while (i < N) {
        visited[i] = 0;
        dist[i] = -1;
        i = i + 1;
    }
    q_init();
    visited[start] = 1;
    dist[start] = 0;
    q_push(start);

    int cur_level = 0;
    printf("Level %d:", cur_level);

    while (!q_empty()) {
        int u = q_pop();
        if (dist[u] > cur_level) {
            printf("\n");
            cur_level = dist[u];
            printf("Level %d:", cur_level);
        }
        printf(" %d", u);
        int v = 0;
        while (v < N) {
            if (adj[u][v] && !visited[v]) {
                visited[v] = 1;
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q_push(v);
            }
            v = v + 1;
        }
    }
    printf("\n");
}

int main(void) {
    printf("=== Graph BFS ===\n");

    // Build a grid-like graph:
    // 0 - 1 - 2
    // |   |   |
    // 3 - 4 - 5
    // |   |
    // 6 - 7
    init_graph(8);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(0, 3);
    add_edge(1, 4);
    add_edge(2, 5);
    add_edge(3, 4);
    add_edge(3, 6);
    add_edge(4, 5);
    add_edge(4, 7);
    add_edge(6, 7);

    printf("--- Graph Structure (8 nodes) ---\n");
    print_graph();

    printf("--- BFS from node 0 ---\n");
    bfs(0);
    printf("Visit order:");
    // Print BFS order from queue
    // Re-run to capture order
    int order[8];
    int oc = 0;
    {
        int i = 0;
        while (i < N) {
            visited[i] = 0;
            dist[i] = -1;
            parent[i] = -1;
            i = i + 1;
        }
        q_init();
        visited[0] = 1;
        dist[0] = 0;
        q_push(0);
        while (!q_empty()) {
            int u = q_pop();
            order[oc] = u;
            oc = oc + 1;
            int v = 0;
            while (v < N) {
                if (adj[u][v] && !visited[v]) {
                    visited[v] = 1;
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q_push(v);
                }
                v = v + 1;
            }
        }
    }
    int k = 0;
    while (k < oc) {
        printf(" %d", order[k]);
        k = k + 1;
    }
    printf("\n");

    printf("--- BFS Levels ---\n");
    bfs_level_order(0);

    printf("--- Shortest Paths from 0 ---\n");
    bfs(0);
    int i = 0;
    while (i < N) {
        printf("dist[%d]=%d\n", i, dist[i]);
        i = i + 1;
    }

    printf("--- Shortest Path 0 -> 7 ---\n");
    print_path(0, 7);

    printf("--- Shortest Path 0 -> 5 ---\n");
    bfs(0);
    print_path(0, 5);

    printf("--- BFS from node 5 ---\n");
    bfs(5);
    // print visit order
    {
        int ii = 0;
        while (ii < N) {
            visited[ii] = 0;
            dist[ii] = -1;
            parent[ii] = -1;
            ii = ii + 1;
        }
        q_init();
        visited[5] = 1;
        dist[5] = 0;
        q_push(5);
        oc = 0;
        while (!q_empty()) {
            int u = q_pop();
            order[oc] = u;
            oc = oc + 1;
            int v = 0;
            while (v < N) {
                if (adj[u][v] && !visited[v]) {
                    visited[v] = 1;
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q_push(v);
                }
                v = v + 1;
            }
        }
    }
    printf("Visit order:");
    k = 0;
    while (k < oc) {
        printf(" %d", order[k]);
        k = k + 1;
    }
    printf("\n");
    printf("dist[0]=%d\n", dist[0]);
    printf("dist[7]=%d\n", dist[7]);

    // Disconnected graph test
    printf("--- Disconnected Graph ---\n");
    init_graph(5);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(3, 4);

    // BFS from 0
    bfs(0);
    printf("BFS from 0:");
    i = 0;
    while (i < 5) {
        if (dist[i] >= 0) printf(" %d", i);
        i = i + 1;
    }
    printf("\n");

    // BFS from 3
    bfs(3);
    printf("BFS from 3:");
    i = 0;
    while (i < 5) {
        if (dist[i] >= 0) printf(" %d", i);
        i = i + 1;
    }
    printf("\n");

    // Count reachable
    bfs(0);
    int reach0 = 0;
    i = 0;
    while (i < 5) {
        if (dist[i] >= 0) reach0 = reach0 + 1;
        i = i + 1;
    }
    printf("Reachable from 0: %d\n", reach0);

    bfs(3);
    int reach3 = 0;
    i = 0;
    while (i < 5) {
        if (dist[i] >= 0) reach3 = reach3 + 1;
        i = i + 1;
    }
    printf("Reachable from 3: %d\n", reach3);

    printf("=== Done ===\n");
    return 0;
}
