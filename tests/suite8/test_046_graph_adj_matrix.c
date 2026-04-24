int printf(const char *fmt, ...);

/* Graph represented as adjacency matrix */

int adj[10][10];
int num_vertices;

void graph_init(int n) {
    int i; int j;
    num_vertices = n;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            adj[i][j] = 0;
        }
    }
}

void add_edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;   /* undirected */
}

void add_directed_edge(int u, int v) {
    adj[u][v] = 1;
}

int has_edge(int u, int v) {
    return adj[u][v];
}

int degree(int v) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < num_vertices; i++) {
        if (adj[v][i]) count = count + 1;
    }
    return count;
}

int count_edges(void) {
    int i; int j;
    int total;
    total = 0;
    for (i = 0; i < num_vertices; i++) {
        for (j = i + 1; j < num_vertices; j++) {
            if (adj[i][j]) total = total + 1;
        }
    }
    return total;
}

void print_neighbors(int v) {
    int i;
    int first;
    printf("neighbors of %d:", v);
    first = 1;
    for (i = 0; i < num_vertices; i++) {
        if (adj[v][i]) {
            printf(" %d", i);
        }
    }
    printf("\n");
}

void print_matrix(void) {
    int i; int j;
    for (i = 0; i < num_vertices; i++) {
        for (j = 0; j < num_vertices; j++) {
            if (j > 0) printf(" ");
            printf("%d", adj[i][j]);
        }
        printf("\n");
    }
}

int is_connected_undirected(void) {
    /* BFS from vertex 0, check if all vertices reached */
    int visited[10];
    int queue[10];
    int qf; int qb;
    int i;
    int cur;

    for (i = 0; i < 10; i++) visited[i] = 0;
    qf = 0; qb = 0;
    visited[0] = 1;
    queue[qb] = 0; qb = qb + 1;

    while (qf < qb) {
        cur = queue[qf]; qf = qf + 1;
        for (i = 0; i < num_vertices; i++) {
            if (adj[cur][i] && visited[i] == 0) {
                visited[i] = 1;
                queue[qb] = i; qb = qb + 1;
            }
        }
    }

    for (i = 0; i < num_vertices; i++) {
        if (visited[i] == 0) return 0;
    }
    return 1;
}

int main(void) {
    /* Undirected graph:
       0 -- 1 -- 2
       |         |
       3 -- 4 -- 5
    */
    graph_init(6);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(0, 3);
    add_edge(3, 4);
    add_edge(4, 5);
    add_edge(2, 5);

    // EXPECT: edge 0-1: 1
    printf("edge 0-1: %d\n", has_edge(0, 1));
    // EXPECT: edge 1-0: 1
    printf("edge 1-0: %d\n", has_edge(1, 0));
    // EXPECT: edge 0-2: 0
    printf("edge 0-2: %d\n", has_edge(0, 2));
    // EXPECT: edge 3-4: 1
    printf("edge 3-4: %d\n", has_edge(3, 4));

    // EXPECT: degree 0: 2
    printf("degree 0: %d\n", degree(0));
    // EXPECT: degree 1: 2
    printf("degree 1: %d\n", degree(1));
    // EXPECT: degree 5: 2
    printf("degree 5: %d\n", degree(5));

    // EXPECT: edges: 6
    printf("edges: %d\n", count_edges());

    // EXPECT: neighbors of 0: 1 3
    print_neighbors(0);
    // EXPECT: neighbors of 2: 1 5
    print_neighbors(2);
    // EXPECT: neighbors of 4: 3 5
    print_neighbors(4);

    // EXPECT: connected: 1
    printf("connected: %d\n", is_connected_undirected());

    // EXPECT: matrix:
    printf("matrix:\n");
    // EXPECT: 0 1 0 1 0 0
    // EXPECT: 1 0 1 0 0 0
    // EXPECT: 0 1 0 0 0 1
    // EXPECT: 1 0 0 0 1 0
    // EXPECT: 0 0 0 1 0 1
    // EXPECT: 0 0 1 0 1 0
    print_matrix();

    return 0;
}
