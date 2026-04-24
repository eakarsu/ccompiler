int printf(const char *fmt, ...);

// Maximum Flow using Ford-Fulkerson (BFS-based / Edmonds-Karp)
// Finds maximum flow from source to sink in a directed graph

int cap[10][10];
int flow[10][10];
int N;

void init(int n) {
    int i;
    int j;
    N = n;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            cap[i][j] = 0;
            flow[i][j] = 0;
        }
    }
}

void add_cap(int u, int v, int c) {
    cap[u][v] = c;
}

// BFS to find augmenting path, returns bottleneck capacity (0 if no path)
int bfs_augment(int source, int sink, int *parent) {
    int visited[10];
    int queue[10];
    int front;
    int back;
    int u;
    int v;
    int i;
    int bottleneck;
    int residual;

    for (i = 0; i < N; i++) {
        visited[i] = 0;
        parent[i] = -1;
    }

    front = 0;
    back = 0;
    queue[back] = source;
    back = back + 1;
    visited[source] = 1;

    while (front < back) {
        u = queue[front];
        front = front + 1;

        for (v = 0; v < N; v++) {
            residual = cap[u][v] - flow[u][v];
            if (!visited[v] && residual > 0) {
                visited[v] = 1;
                parent[v] = u;
                queue[back] = v;
                back = back + 1;
                if (v == sink) {
                    // Compute bottleneck
                    bottleneck = 999999;
                    while (v != source) {
                        u = parent[v];
                        residual = cap[u][v] - flow[u][v];
                        if (residual < bottleneck) bottleneck = residual;
                        v = u;
                    }
                    return bottleneck;
                }
            }
        }
    }
    return 0;
}

int max_flow(int source, int sink) {
    int parent[10];
    int total_flow;
    int path_flow;
    int v;
    int u;
    int iterations;

    total_flow = 0;
    iterations = 0;

    while (1) {
        path_flow = bfs_augment(source, sink, parent);
        if (path_flow == 0) break;

        // Update flow along the path
        v = sink;
        while (v != source) {
            u = parent[v];
            flow[u][v] = flow[u][v] + path_flow;
            flow[v][u] = flow[v][u] - path_flow;
            v = u;
        }

        total_flow = total_flow + path_flow;
        iterations = iterations + 1;
    }

    printf("Iterations: %d\n", iterations);
    return total_flow;
}

void print_flow_matrix() {
    int i;
    int j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (j > 0) printf(" ");
            if (flow[i][j] > 0) {
                printf("%d", flow[i][j]);
            } else {
                printf("0");
            }
        }
        printf("\n");
    }
}

int main() {
    int mf;
    int i;
    int j;

    // Test 1: Classic max-flow graph
    //   0 -> 1 (cap 16)
    //   0 -> 2 (cap 13)
    //   1 -> 2 (cap 4)
    //   1 -> 3 (cap 12)
    //   2 -> 1 (cap 10)
    //   2 -> 4 (cap 14)
    //   3 -> 2 (cap 9)
    //   3 -> 5 (cap 20)
    //   4 -> 3 (cap 7)
    //   4 -> 5 (cap 4)
    printf("Test 1: 6-vertex flow network\n"); // EXPECT: Test 1: 6-vertex flow network
    init(6);
    add_cap(0, 1, 16);
    add_cap(0, 2, 13);
    add_cap(1, 2, 4);
    add_cap(1, 3, 12);
    add_cap(2, 1, 10);
    add_cap(2, 4, 14);
    add_cap(3, 2, 9);
    add_cap(3, 5, 20);
    add_cap(4, 3, 7);
    add_cap(4, 5, 4);

    mf = max_flow(0, 5);
    // EXPECT: Iterations: 3
    printf("Max flow 0->5: %d\n", mf); // EXPECT: Max flow 0->5: 23

    printf("Flow matrix:\n"); // EXPECT: Flow matrix:
    print_flow_matrix();
    // EXPECT: 0 12 11 0 0 0
    // EXPECT: 0 0 0 12 0 0
    // EXPECT: 0 0 0 0 11 0
    // EXPECT: 0 0 0 0 0 19
    // EXPECT: 0 0 0 7 0 4
    // EXPECT: 0 0 0 0 0 0

    // Test 2: Simple 2-path graph
    //   0 -> 1 (5), 1 -> 3 (5)
    //   0 -> 2 (3), 2 -> 3 (3)
    printf("Test 2: Two paths\n"); // EXPECT: Test 2: Two paths
    init(4);
    add_cap(0, 1, 5);
    add_cap(0, 2, 3);
    add_cap(1, 3, 5);
    add_cap(2, 3, 3);

    mf = max_flow(0, 3);
    // EXPECT: Iterations: 2
    printf("Max flow 0->3: %d\n", mf); // EXPECT: Max flow 0->3: 8

    // Test 3: Bottleneck in middle
    //   0->1(10), 1->2(1), 2->3(10)
    printf("Test 3: Bottleneck\n"); // EXPECT: Test 3: Bottleneck
    init(4);
    add_cap(0, 1, 10);
    add_cap(1, 2, 1);
    add_cap(2, 3, 10);

    mf = max_flow(0, 3);
    // EXPECT: Iterations: 1
    printf("Max flow 0->3: %d\n", mf); // EXPECT: Max flow 0->3: 1

    // Test 4: No path from source to sink
    printf("Test 4: No path\n"); // EXPECT: Test 4: No path
    init(4);
    add_cap(0, 1, 5);
    add_cap(2, 3, 5);
    // No edge from {0,1} to {2,3}

    mf = max_flow(0, 3);
    // EXPECT: Iterations: 0
    printf("Max flow 0->3: %d\n", mf); // EXPECT: Max flow 0->3: 0

    return 0;
}
