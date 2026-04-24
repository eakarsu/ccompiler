int printf(const char *fmt, ...);
// EXPECT: Chromatic number: 3\nValid: 1\nColors: 1 2 3 2 1\nCycle-4 chromatic: 2\nTriangle chromatic: 3\nEmpty graph chromatic: 1
// Test: Graph coloring (small graph)

int adj[10][10];
int color[10];
int num_nodes;

void init_graph(int n) {
    num_nodes = n;
    int i, j;
    for (i = 0; i < n; i++) {
        color[i] = 0;
        for (j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }
}

void add_edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

int is_safe_color(int node, int c) {
    int i;
    for (i = 0; i < num_nodes; i++) {
        if (adj[node][i] && color[i] == c) return 0;
    }
    return 1;
}

int graph_coloring(int node, int num_colors) {
    if (node == num_nodes) return 1;
    int c;
    for (c = 1; c <= num_colors; c++) {
        if (is_safe_color(node, c)) {
            color[node] = c;
            if (graph_coloring(node + 1, num_colors)) return 1;
            color[node] = 0;
        }
    }
    return 0;
}

int chromatic_number(void) {
    int c;
    for (c = 1; c <= num_nodes; c++) {
        int i;
        for (i = 0; i < num_nodes; i++) color[i] = 0;
        if (graph_coloring(0, c)) return c;
    }
    return num_nodes;
}

int verify_coloring(void) {
    int i, j;
    for (i = 0; i < num_nodes; i++) {
        if (color[i] == 0) return 0;
        for (j = i + 1; j < num_nodes; j++) {
            if (adj[i][j] && color[i] == color[j]) return 0;
        }
    }
    return 1;
}

int main(void) {
    init_graph(5);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(0, 3);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 4);

    int chi = chromatic_number();
    printf("Chromatic number: %d\n", chi);
    printf("Valid: %d\n", verify_coloring());
    printf("Colors:");
    int i;
    for (i = 0; i < 5; i++) printf(" %d", color[i]);
    printf("\n");

    init_graph(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 0);
    chi = chromatic_number();
    printf("Cycle-4 chromatic: %d\n", chi);

    init_graph(3);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 0);
    chi = chromatic_number();
    printf("Triangle chromatic: %d\n", chi);

    init_graph(4);
    chi = chromatic_number();
    printf("Empty graph chromatic: %d\n", chi);

    return 0;
}
