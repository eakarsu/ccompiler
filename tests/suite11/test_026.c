int printf(const char *fmt, ...);

// Disjoint set (Union-Find) with union by rank and path compression

struct DSU {
    int parent[32];
    int rank[32];
    int size[32];
    int n;
    int num_sets;
};

void dsu_init(struct DSU *d, int n) {
    int i;
    d->n = n;
    d->num_sets = n;
    for (i = 0; i < n; i++) {
        d->parent[i] = i;
        d->rank[i] = 0;
        d->size[i] = 1;
    }
}

int dsu_find(struct DSU *d, int x) {
    if (d->parent[x] != x) {
        d->parent[x] = dsu_find(d, d->parent[x]);
    }
    return d->parent[x];
}

int dsu_union(struct DSU *d, int x, int y) {
    int rx = dsu_find(d, x);
    int ry = dsu_find(d, y);
    if (rx == ry) return 0;
    if (d->rank[rx] < d->rank[ry]) {
        d->parent[rx] = ry;
        d->size[ry] = d->size[ry] + d->size[rx];
    } else if (d->rank[rx] > d->rank[ry]) {
        d->parent[ry] = rx;
        d->size[rx] = d->size[rx] + d->size[ry];
    } else {
        d->parent[ry] = rx;
        d->size[rx] = d->size[rx] + d->size[ry];
        d->rank[rx] = d->rank[rx] + 1;
    }
    d->num_sets = d->num_sets - 1;
    return 1;
}

int dsu_connected(struct DSU *d, int x, int y) {
    return dsu_find(d, x) == dsu_find(d, y);
}

int dsu_set_size(struct DSU *d, int x) {
    return d->size[dsu_find(d, x)];
}

int dsu_largest_set(struct DSU *d) {
    int max_size = 0;
    int i;
    for (i = 0; i < d->n; i++) {
        if (d->parent[i] == i && d->size[i] > max_size) {
            max_size = d->size[i];
        }
    }
    return max_size;
}

int main() {
    struct DSU dsu;
    int edges_x[12];
    int edges_y[12];
    int num_edges;
    int i;
    int components;

    dsu_init(&dsu, 10);
    printf("initial sets: %d\n", dsu.num_sets);              // EXPECT: initial sets: 10

    dsu_union(&dsu, 0, 1);
    dsu_union(&dsu, 2, 3);
    dsu_union(&dsu, 4, 5);
    printf("after 3 unions: %d\n", dsu.num_sets);            // EXPECT: after 3 unions: 7

    printf("0-1 connected: %d\n", dsu_connected(&dsu, 0, 1));  // EXPECT: 0-1 connected: 1
    printf("0-2 connected: %d\n", dsu_connected(&dsu, 0, 2));  // EXPECT: 0-2 connected: 0

    dsu_union(&dsu, 1, 3);
    printf("0-3 connected: %d\n", dsu_connected(&dsu, 0, 3));  // EXPECT: 0-3 connected: 1
    printf("set size of 0: %d\n", dsu_set_size(&dsu, 0));      // EXPECT: set size of 0: 4
    printf("sets: %d\n", dsu.num_sets);                         // EXPECT: sets: 6

    dsu_union(&dsu, 5, 6);
    dsu_union(&dsu, 6, 7);
    dsu_union(&dsu, 7, 8);
    printf("set size of 4: %d\n", dsu_set_size(&dsu, 4));      // EXPECT: set size of 4: 5
    printf("sets: %d\n", dsu.num_sets);                         // EXPECT: sets: 3

    printf("largest set: %d\n", dsu_largest_set(&dsu));         // EXPECT: largest set: 5

    dsu_union(&dsu, 0, 4);
    printf("after big merge: %d\n", dsu.num_sets);              // EXPECT: after big merge: 2
    printf("set size of 0: %d\n", dsu_set_size(&dsu, 0));      // EXPECT: set size of 0: 9
    printf("largest: %d\n", dsu_largest_set(&dsu));             // EXPECT: largest: 9

    // redundant union should return 0
    printf("dup union: %d\n", dsu_union(&dsu, 0, 1));          // EXPECT: dup union: 0
    printf("sets unchanged: %d\n", dsu.num_sets);               // EXPECT: sets unchanged: 2

    // Kruskal-like: count components in a graph
    dsu_init(&dsu, 8);
    edges_x[0] = 0; edges_y[0] = 1;
    edges_x[1] = 1; edges_y[1] = 2;
    edges_x[2] = 3; edges_y[2] = 4;
    edges_x[3] = 5; edges_y[3] = 6;
    edges_x[4] = 6; edges_y[4] = 7;
    edges_x[5] = 2; edges_y[5] = 0;
    num_edges = 6;
    for (i = 0; i < num_edges; i++) {
        dsu_union(&dsu, edges_x[i], edges_y[i]);
    }
    printf("graph components: %d\n", dsu.num_sets);            // EXPECT: graph components: 3

    return 0;
}
