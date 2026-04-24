int printf(const char *fmt, ...);
// EXPECT: DFS from 0: 0 1 3 4 2 5 6 \nDFS iterative from 0: 0 1 3 4 2 5 6 \nVisited: 7\nComponents: 4\nComponents after merge: 2
// Test: DFS on adjacency matrix

int adj[10][10];
int visited[10];
int num_nodes;
int order[10];
int order_count;

void init_graph(int n) {
    num_nodes = n;
    int i, j;
    for (i = 0; i < n; i++) {
        visited[i] = 0;
        for (j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }
    order_count = 0;
}

void add_edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

void dfs(int node) {
    visited[node] = 1;
    order[order_count] = node;
    order_count++;
    int i;
    for (i = 0; i < num_nodes; i++) {
        if (adj[node][i] && !visited[i]) {
            dfs(i);
        }
    }
}

int dfs_iterative(int start) {
    int stack[64];
    int sp = 0;
    int count = 0;
    int i;
    for (i = 0; i < num_nodes; i++) visited[i] = 0;

    stack[sp] = start;
    sp++;
    while (sp > 0) {
        sp--;
        int node = stack[sp];
        if (visited[node]) continue;
        visited[node] = 1;
        count++;
        printf("%d ", node);
        for (i = num_nodes - 1; i >= 0; i--) {
            if (adj[node][i] && !visited[i]) {
                stack[sp] = i;
                sp++;
            }
        }
    }
    printf("\n");
    return count;
}

int count_components(void) {
    int i;
    int components = 0;
    for (i = 0; i < num_nodes; i++) visited[i] = 0;
    for (i = 0; i < num_nodes; i++) {
        if (!visited[i]) {
            order_count = 0;
            dfs(i);
            components++;
        }
    }
    return components;
}

int main(void) {
    init_graph(7);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(1, 4);
    add_edge(2, 5);
    add_edge(2, 6);

    printf("DFS from 0: ");
    int i;
    for (i = 0; i < num_nodes; i++) visited[i] = 0;
    order_count = 0;
    dfs(0);
    for (i = 0; i < order_count; i++) {
        printf("%d ", order[i]);
    }
    printf("\n");

    printf("DFS iterative from 0: ");
    int cnt = dfs_iterative(0);
    printf("Visited: %d\n", cnt);

    init_graph(8);
    add_edge(0, 1);
    add_edge(2, 3);
    add_edge(4, 5);
    add_edge(6, 7);
    printf("Components: %d\n", count_components());

    add_edge(1, 2);
    add_edge(5, 6);
    printf("Components after merge: %d\n", count_components());

    return 0;
}
