int printf(const char *fmt, ...);

int adj[16][16];
int n_vertices;
int visited[16];
int dfs_order[16];
int dfs_count;

int color[16];
int has_cycle;

int topo_order[16];
int topo_count;

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

void add_directed(int u, int v) {
    adj[u][v] = 1;
}

void dfs(int u) {
    int v;
    visited[u] = 1;
    dfs_order[dfs_count] = u;
    dfs_count = dfs_count + 1;
    for (v = 0; v < n_vertices; v = v + 1) {
        if (adj[u][v] && !visited[v]) {
            dfs(v);
        }
    }
}

void dfs_start(int start) {
    int i;
    dfs_count = 0;
    for (i = 0; i < n_vertices; i = i + 1) {
        visited[i] = 0;
    }
    dfs(start);
}

int count_components(void) {
    int i;
    int comp = 0;
    for (i = 0; i < n_vertices; i = i + 1) {
        visited[i] = 0;
    }
    dfs_count = 0;
    for (i = 0; i < n_vertices; i = i + 1) {
        if (!visited[i]) {
            dfs(i);
            comp = comp + 1;
        }
    }
    return comp;
}

void cycle_dfs(int u) {
    int v;
    color[u] = 1;
    for (v = 0; v < n_vertices; v = v + 1) {
        if (adj[u][v]) {
            if (color[v] == 1) {
                has_cycle = 1;
            } else if (color[v] == 0) {
                cycle_dfs(v);
            }
        }
    }
    color[u] = 2;
}

int detect_cycle_directed(void) {
    int i;
    has_cycle = 0;
    for (i = 0; i < n_vertices; i = i + 1) {
        color[i] = 0;
    }
    for (i = 0; i < n_vertices; i = i + 1) {
        if (color[i] == 0) {
            cycle_dfs(i);
        }
    }
    return has_cycle;
}

int topo_visited[16];

void topo_dfs(int u) {
    int v;
    topo_visited[u] = 1;
    for (v = 0; v < n_vertices; v = v + 1) {
        if (adj[u][v] && !topo_visited[v]) {
            topo_dfs(v);
        }
    }
    topo_order[topo_count] = u;
    topo_count = topo_count + 1;
}

void topological_sort(void) {
    int i;
    topo_count = 0;
    for (i = 0; i < n_vertices; i = i + 1) {
        topo_visited[i] = 0;
    }
    for (i = 0; i < n_vertices; i = i + 1) {
        if (!topo_visited[i]) {
            topo_dfs(i);
        }
    }
}

int validate_topo(void) {
    int pos[16];
    int i;
    int u;
    int v;
    for (i = 0; i < topo_count; i = i + 1) {
        pos[topo_order[topo_count - 1 - i]] = i;
    }
    for (u = 0; u < n_vertices; u = u + 1) {
        for (v = 0; v < n_vertices; v = v + 1) {
            if (adj[u][v]) {
                if (pos[u] > pos[v]) {
                    return 0;
                }
            }
        }
    }
    return 1;
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

    dfs_start(0);
    // EXPECT: dfs count: 6
    printf("dfs count: %d\n", dfs_count);
    // EXPECT: dfs start: 0
    printf("dfs start: %d\n", dfs_order[0]);

    // EXPECT: components: 1
    printf("components: %d\n", count_components());

    init_graph(6);
    add_edge(0, 1);
    add_edge(2, 3);
    add_edge(4, 5);
    // EXPECT: three components: 3
    printf("three components: %d\n", count_components());

    init_graph(4);
    add_directed(0, 1);
    add_directed(1, 2);
    add_directed(2, 3);
    // EXPECT: dag no cycle: 0
    printf("dag no cycle: %d\n", detect_cycle_directed());

    init_graph(4);
    add_directed(0, 1);
    add_directed(1, 2);
    add_directed(2, 0);
    // EXPECT: cycle found: 1
    printf("cycle found: %d\n", detect_cycle_directed());

    init_graph(6);
    add_directed(5, 2);
    add_directed(5, 0);
    add_directed(4, 0);
    add_directed(4, 1);
    add_directed(2, 3);
    add_directed(3, 1);

    // EXPECT: dag6 no cycle: 0
    printf("dag6 no cycle: %d\n", detect_cycle_directed());

    topological_sort();
    // EXPECT: topo count: 6
    printf("topo count: %d\n", topo_count);
    // EXPECT: topo valid: 1
    printf("topo valid: %d\n", validate_topo());

    init_graph(3);
    add_directed(0, 1);
    add_directed(1, 2);
    topological_sort();
    // EXPECT: small topo valid: 1
    printf("small topo valid: %d\n", validate_topo());
    // EXPECT: small topo count: 3
    printf("small topo count: %d\n", topo_count);

    init_graph(1);
    // EXPECT: single no cycle: 0
    printf("single no cycle: %d\n", detect_cycle_directed());

    return 0;
}
