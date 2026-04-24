int printf(const char *fmt, ...);

// Prim's MST algorithm using adjacency matrix
// Graph: 5 vertices, 7 edges (weighted undirected)

int adj[10][10];
int n_verts;

void init_graph(int n) {
    int i;
    int j;
    n_verts = n;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }
}

void add_edge(int u, int v, int w) {
    adj[u][v] = w;
    adj[v][u] = w;
}

struct PrimResult {
    int parent[10];
    int key[10];
    int total_weight;
};

void prim_mst(struct PrimResult *res) {
    int in_mst[10];
    int i;
    int count;
    int u;
    int v;
    int min_key;

    for (i = 0; i < n_verts; i++) {
        res->key[i] = 999999;
        in_mst[i] = 0;
        res->parent[i] = -1;
    }
    res->key[0] = 0;
    res->total_weight = 0;

    for (count = 0; count < n_verts; count++) {
        min_key = 999999;
        u = -1;
        for (i = 0; i < n_verts; i++) {
            if (!in_mst[i] && res->key[i] < min_key) {
                min_key = res->key[i];
                u = i;
            }
        }
        if (u == -1) break;

        in_mst[u] = 1;
        if (res->parent[u] != -1) {
            res->total_weight = res->total_weight + res->key[u];
        }

        for (v = 0; v < n_verts; v++) {
            if (adj[u][v] > 0 && !in_mst[v] && adj[u][v] < res->key[v]) {
                res->key[v] = adj[u][v];
                res->parent[v] = u;
            }
        }
    }
}

void print_row(int *row, int n) {
    int j;
    for (j = 0; j < n; j++) {
        if (j > 0) printf(" ");
        printf("%d", row[j]);
    }
    printf("\n");
}

int main() {
    struct PrimResult result;
    int i;

    init_graph(5);

    add_edge(0, 1, 2);
    add_edge(0, 2, 9);
    add_edge(0, 3, 6);
    add_edge(1, 2, 3);
    add_edge(1, 4, 5);
    add_edge(2, 3, 11);
    add_edge(3, 4, 7);

    printf("Adjacency matrix:\n"); // EXPECT: Adjacency matrix:
    for (i = 0; i < n_verts; i++) {
        print_row(adj[i], n_verts);
    }
    // EXPECT: 0 2 9 6 0
    // EXPECT: 2 0 3 0 5
    // EXPECT: 9 3 0 11 0
    // EXPECT: 6 0 11 0 7
    // EXPECT: 0 5 0 7 0

    prim_mst(&result);

    printf("Prim MST edges:\n"); // EXPECT: Prim MST edges:
    for (i = 1; i < n_verts; i++) {
        printf("  %d-%d w=%d\n", result.parent[i], i, result.key[i]);
    }
    // EXPECT:   0-1 w=2
    // EXPECT:   1-2 w=3
    // EXPECT:   0-3 w=6
    // EXPECT:   1-4 w=5
    printf("Total MST weight: %d\n", result.total_weight); // EXPECT: Total MST weight: 16

    printf("Parent of 0: %d\n", result.parent[0]); // EXPECT: Parent of 0: -1
    printf("Parent of 1: %d\n", result.parent[1]); // EXPECT: Parent of 1: 0
    printf("Parent of 2: %d\n", result.parent[2]); // EXPECT: Parent of 2: 1
    printf("Parent of 3: %d\n", result.parent[3]); // EXPECT: Parent of 3: 0
    printf("Parent of 4: %d\n", result.parent[4]); // EXPECT: Parent of 4: 1

    return 0;
}
