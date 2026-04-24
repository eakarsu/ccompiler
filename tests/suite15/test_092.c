int printf(const char *fmt, ...);

/* Maximum Flow - Ford-Fulkerson with BFS (Edmonds-Karp)
   on a small directed graph */

int cap[8][8];    /* capacity matrix */
int flow[8][8];   /* flow matrix */
int parent[8];    /* BFS parent array */
int visited[8];

int queue[64];
int qfront;
int qback;

void q_init(void) {
    qfront = 0;
    qback = 0;
}

void q_push(int v) {
    queue[qback] = v;
    qback = qback + 1;
}

int q_pop(void) {
    int v = queue[qfront];
    qfront = qfront + 1;
    return v;
}

int q_empty(void) {
    return qfront >= qback;
}

int bfs(int source, int sink, int n) {
    int i;
    int u;
    int v;
    for (i = 0; i < n; i++) {
        visited[i] = 0;
        parent[i] = -1;
    }
    visited[source] = 1;
    q_init();
    q_push(source);

    while (!q_empty()) {
        u = q_pop();
        for (v = 0; v < n; v++) {
            if (!visited[v] && (cap[u][v] - flow[u][v]) > 0) {
                visited[v] = 1;
                parent[v] = u;
                if (v == sink) return 1;
                q_push(v);
            }
        }
    }
    return 0;
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

int edmonds_karp(int source, int sink, int n) {
    int i;
    int j;
    int max_flow;
    int path_flow;
    int v;
    int u;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            flow[i][j] = 0;

    max_flow = 0;

    while (bfs(source, sink, n)) {
        /* Find bottleneck */
        path_flow = 999999;
        v = sink;
        while (v != source) {
            u = parent[v];
            path_flow = min(path_flow, cap[u][v] - flow[u][v]);
            v = u;
        }

        /* Update flow */
        v = sink;
        while (v != source) {
            u = parent[v];
            flow[u][v] = flow[u][v] + path_flow;
            flow[v][u] = flow[v][u] - path_flow;
            v = u;
        }

        max_flow = max_flow + path_flow;
    }

    return max_flow;
}

void clear_graph(int n) {
    int i;
    int j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            cap[i][j] = 0;
}

void test_simple(void) {
    /* Graph: 0->1 cap 10, 0->2 cap 10, 1->3 cap 10, 2->3 cap 10
       Source=0, Sink=3.  Max flow = 20 */
    int result;
    clear_graph(4);
    cap[0][1] = 10;
    cap[0][2] = 10;
    cap[1][3] = 10;
    cap[2][3] = 10;

    result = edmonds_karp(0, 3, 4);
    printf("=== Simple Flow ===\n");
    // EXPECT: === Simple Flow ===
    printf("Max flow: %d\n", result);
    // EXPECT: Max flow: 20
}

void test_bottleneck(void) {
    /* 0->1 cap 100, 1->2 cap 1, 2->3 cap 100
       Bottleneck at edge 1->2. Max flow = 1 */
    int result;
    clear_graph(4);
    cap[0][1] = 100;
    cap[1][2] = 1;
    cap[2][3] = 100;

    result = edmonds_karp(0, 3, 4);
    printf("=== Bottleneck ===\n");
    // EXPECT: === Bottleneck ===
    printf("Max flow: %d\n", result);
    // EXPECT: Max flow: 1
}

void test_diamond(void) {
    /*  0 -> 1 (cap 3)
        0 -> 2 (cap 3)
        1 -> 3 (cap 2)
        2 -> 3 (cap 2)
        1 -> 2 (cap 2)
        Source=0, Sink=3
        Path 0->1->3: flow 2
        Path 0->2->3: flow 2
        Path 0->1->2->3: flow 1 (1 remaining on 0->1, 1 remaining on 1->2, 0 remaining on 2->3... no)
        Actually: after 0->1->3 (2), 0->2->3 (2), total = 4.
        Remaining: 0->1 has 1 left, 1->2 has 2. But 2->3 is full.
        So max flow = 4. Wait let me re-check.
        0->1: cap 3, 0->2: cap 3, 1->3: cap 2, 2->3: cap 2, 1->2: cap 2
        BFS finds 0->1->3, bottleneck = min(3,2) = 2. Flow 0->1=2, 1->3=2.
        BFS finds 0->2->3, bottleneck = min(3,2) = 2. Flow 0->2=2, 2->3=2.
        BFS: 0->1 has 1 left, 1->2 has 2, but 2->3 is full (cap 2, flow 2).
        1->3 is full. Can we route 0->1->2->...? 2->3 is full. No path.
        Max flow = 4.
    */
    int result;
    clear_graph(4);
    cap[0][1] = 3;
    cap[0][2] = 3;
    cap[1][3] = 2;
    cap[2][3] = 2;
    cap[1][2] = 2;

    result = edmonds_karp(0, 3, 4);
    printf("=== Diamond ===\n");
    // EXPECT: === Diamond ===
    printf("Max flow: %d\n", result);
    // EXPECT: Max flow: 4
}

void test_complex(void) {
    /* 6-node graph:
       0->1 (16), 0->2 (13)
       1->2 (4), 1->3 (12)
       2->1 (10), 2->4 (14)
       3->2 (9), 3->5 (20)
       4->3 (7), 4->5 (4)
       Source=0, Sink=5
       Classic example: max flow = 23
    */
    int result;
    clear_graph(6);
    cap[0][1] = 16; cap[0][2] = 13;
    cap[1][2] = 4;  cap[1][3] = 12;
    cap[2][1] = 10; cap[2][4] = 14;
    cap[3][2] = 9;  cap[3][5] = 20;
    cap[4][3] = 7;  cap[4][5] = 4;

    result = edmonds_karp(0, 5, 6);
    printf("=== Complex 6-node ===\n");
    // EXPECT: === Complex 6-node ===
    printf("Max flow: %d\n", result);
    // EXPECT: Max flow: 23
}

void test_no_path(void) {
    /* 0->1 (5), 2->3 (5). No path from 0 to 3. */
    int result;
    clear_graph(4);
    cap[0][1] = 5;
    cap[2][3] = 5;

    result = edmonds_karp(0, 3, 4);
    printf("=== No Path ===\n");
    // EXPECT: === No Path ===
    printf("Max flow: %d\n", result);
    // EXPECT: Max flow: 0
}

int main(void) {
    test_simple();
    test_bottleneck();
    test_diamond();
    test_complex();
    test_no_path();

    printf("All max flow tests passed!\n");
    // EXPECT: All max flow tests passed!
    return 0;
}
