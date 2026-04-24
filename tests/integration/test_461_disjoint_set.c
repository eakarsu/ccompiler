int printf(const char *fmt, ...);
// EXPECT: Initial sets: 10\n0 and 1 connected: 0\nAfter 4 unions: 6 sets\n0 and 1 connected: 1\n0 and 2 connected: 0\nAfter merging pairs: 4 sets\nSet size of 0: 4\nSet size of 4: 4\nAfter more merges: 1 sets\nSet size of 0: 10\n3 and 7 connected: 1\n1 and 9 connected: 1\nAll same root: 1\nDone

// Union-Find with path compression and union by rank

struct DisjointSet {
    int parent[16];
    int rank[16];
    int size[16];
    int n;
    int num_sets;
};

void ds_init(struct DisjointSet *ds, int n) {
    ds->n = n;
    ds->num_sets = n;
    int i = 0;
    while (i < n) {
        ds->parent[i] = i;
        ds->rank[i] = 0;
        ds->size[i] = 1;
        i = i + 1;
    }
}

int ds_find(struct DisjointSet *ds, int x) {
    if (ds->parent[x] != x) {
        ds->parent[x] = ds_find(ds, ds->parent[x]);
    }
    return ds->parent[x];
}

void ds_union(struct DisjointSet *ds, int x, int y) {
    int rx = ds_find(ds, x);
    int ry = ds_find(ds, y);
    if (rx == ry) return;

    ds->num_sets = ds->num_sets - 1;

    if (ds->rank[rx] < ds->rank[ry]) {
        ds->parent[rx] = ry;
        ds->size[ry] = ds->size[ry] + ds->size[rx];
    } else if (ds->rank[rx] > ds->rank[ry]) {
        ds->parent[ry] = rx;
        ds->size[rx] = ds->size[rx] + ds->size[ry];
    } else {
        ds->parent[ry] = rx;
        ds->size[rx] = ds->size[rx] + ds->size[ry];
        ds->rank[rx] = ds->rank[rx] + 1;
    }
}

int ds_connected(struct DisjointSet *ds, int x, int y) {
    return ds_find(ds, x) == ds_find(ds, y);
}

int ds_set_size(struct DisjointSet *ds, int x) {
    return ds->size[ds_find(ds, x)];
}

int main(void) {
    struct DisjointSet ds;
    ds_init(&ds, 10);

    printf("Initial sets: %d\n", ds.num_sets);
    printf("0 and 1 connected: %d\n", ds_connected(&ds, 0, 1));

    // Build some sets
    ds_union(&ds, 0, 1);
    ds_union(&ds, 2, 3);
    ds_union(&ds, 4, 5);
    ds_union(&ds, 6, 7);

    printf("After 4 unions: %d sets\n", ds.num_sets);
    printf("0 and 1 connected: %d\n", ds_connected(&ds, 0, 1));
    printf("0 and 2 connected: %d\n", ds_connected(&ds, 0, 2));

    // Merge larger sets
    ds_union(&ds, 0, 2);
    ds_union(&ds, 4, 6);
    printf("After merging pairs: %d sets\n", ds.num_sets);
    printf("Set size of 0: %d\n", ds_set_size(&ds, 0));
    printf("Set size of 4: %d\n", ds_set_size(&ds, 4));

    // Merge everything
    ds_union(&ds, 0, 4);
    ds_union(&ds, 8, 9);
    ds_union(&ds, 0, 8);
    printf("After more merges: %d sets\n", ds.num_sets);
    printf("Set size of 0: %d\n", ds_set_size(&ds, 0));

    // All should be connected
    printf("3 and 7 connected: %d\n", ds_connected(&ds, 3, 7));
    printf("1 and 9 connected: %d\n", ds_connected(&ds, 1, 9));

    // Path compression check - all should have same root
    int root = ds_find(&ds, 0);
    int all_same = 1;
    int i = 1;
    while (i < 10) {
        if (ds_find(&ds, i) != root) {
            all_same = 0;
        }
        i = i + 1;
    }
    printf("All same root: %d\n", all_same);

    printf("Done\n");
    return 0;
}
