int printf(const char *fmt, ...);
// EXPECT: Topo sort: 4 5 0 2 3 1\nAll nodes: 1\nHas cycle: 0\nLinear: 0 1 2 3\nCycle: 1
// Test: Topological sort (Kahn's algorithm)

int adj[10][10];
int indegree[10];
int num_nodes;

void init_graph(int n) {
    num_nodes = n;
    int i, j;
    for (i = 0; i < n; i++) {
        indegree[i] = 0;
        for (j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }
}

void add_directed_edge(int u, int v) {
    adj[u][v] = 1;
    indegree[v]++;
}

int topological_sort(int result[]) {
    int queue[64];
    int qf = 0, qr = 0;
    int in_deg[10];
    int i, j;
    int count = 0;

    for (i = 0; i < num_nodes; i++) {
        in_deg[i] = indegree[i];
    }

    for (i = 0; i < num_nodes; i++) {
        if (in_deg[i] == 0) {
            queue[qr] = i;
            qr++;
        }
    }

    while (qf < qr) {
        int node = queue[qf];
        qf++;
        result[count] = node;
        count++;

        for (j = 0; j < num_nodes; j++) {
            if (adj[node][j]) {
                in_deg[j]--;
                if (in_deg[j] == 0) {
                    queue[qr] = j;
                    qr++;
                }
            }
        }
    }
    return count;
}

int has_cycle(void) {
    int result[10];
    int count = topological_sort(result);
    return count != num_nodes;
}

int main(void) {
    init_graph(6);
    add_directed_edge(5, 2);
    add_directed_edge(5, 0);
    add_directed_edge(4, 0);
    add_directed_edge(4, 1);
    add_directed_edge(2, 3);
    add_directed_edge(3, 1);

    int result[10];
    int n = topological_sort(result);
    printf("Topo sort: ");
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", result[i]);
    }
    printf("\n");
    printf("All nodes: %d\n", n == 6);
    printf("Has cycle: %d\n", has_cycle());

    init_graph(4);
    add_directed_edge(0, 1);
    add_directed_edge(1, 2);
    add_directed_edge(2, 3);
    n = topological_sort(result);
    printf("Linear: ");
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", result[i]);
    }
    printf("\n");

    init_graph(3);
    add_directed_edge(0, 1);
    add_directed_edge(1, 2);
    add_directed_edge(2, 0);
    printf("Cycle: %d\n", has_cycle());

    return 0;
}
