int printf(const char *fmt, ...);

// Disjoint Set (Union-Find) with path compression and union by rank

struct DSU {
    int parent[20];
    int rank[20];
    int size;
};

void dsu_init(struct DSU *d, int n) {
    int i;
    d->size = n;
    for (i = 0; i < n; i++) {
        d->parent[i] = i;
        d->rank[i] = 0;
    }
}

int dsu_find(struct DSU *d, int x) {
    if (d->parent[x] != x) {
        d->parent[x] = dsu_find(d, d->parent[x]);
    }
    return d->parent[x];
}

void dsu_union(struct DSU *d, int a, int b) {
    int ra;
    int rb;
    ra = dsu_find(d, a);
    rb = dsu_find(d, b);
    if (ra == rb) return;
    if (d->rank[ra] < d->rank[rb]) {
        d->parent[ra] = rb;
    } else if (d->rank[ra] > d->rank[rb]) {
        d->parent[rb] = ra;
    } else {
        d->parent[rb] = ra;
        d->rank[ra] = d->rank[ra] + 1;
    }
}

int dsu_connected(struct DSU *d, int a, int b) {
    return dsu_find(d, a) == dsu_find(d, b);
}

int dsu_count_components(struct DSU *d) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < d->size; i++) {
        if (dsu_find(d, i) == i) {
            count = count + 1;
        }
    }
    return count;
}

int dsu_component_size(struct DSU *d, int x) {
    int root;
    int count;
    int i;
    root = dsu_find(d, x);
    count = 0;
    for (i = 0; i < d->size; i++) {
        if (dsu_find(d, i) == root) {
            count = count + 1;
        }
    }
    return count;
}

int main() {
    struct DSU d;
    int comps;
    int sz;

    dsu_init(&d, 10);

    comps = dsu_count_components(&d);
    printf("Initial components: %d\n", comps);
    // EXPECT: Initial components: 10

    dsu_union(&d, 0, 1);
    dsu_union(&d, 2, 3);
    dsu_union(&d, 4, 5);

    comps = dsu_count_components(&d);
    printf("After 3 unions: %d\n", comps);
    // EXPECT: After 3 unions: 7

    printf("0-1 connected: %d\n", dsu_connected(&d, 0, 1));
    // EXPECT: 0-1 connected: 1

    printf("0-2 connected: %d\n", dsu_connected(&d, 0, 2));
    // EXPECT: 0-2 connected: 0

    dsu_union(&d, 1, 3);
    printf("After union(1,3), 0-2 connected: %d\n", dsu_connected(&d, 0, 2));
    // EXPECT: After union(1,3), 0-2 connected: 1

    printf("0-3 connected: %d\n", dsu_connected(&d, 0, 3));
    // EXPECT: 0-3 connected: 1

    comps = dsu_count_components(&d);
    printf("Components now: %d\n", comps);
    // EXPECT: Components now: 6

    sz = dsu_component_size(&d, 0);
    printf("Component size of 0: %d\n", sz);
    // EXPECT: Component size of 0: 4

    dsu_union(&d, 5, 6);
    dsu_union(&d, 6, 7);
    dsu_union(&d, 7, 8);
    dsu_union(&d, 8, 9);

    comps = dsu_count_components(&d);
    printf("After chaining 4-9: %d\n", comps);
    // EXPECT: After chaining 4-9: 2

    sz = dsu_component_size(&d, 9);
    printf("Component size of 9: %d\n", sz);
    // EXPECT: Component size of 9: 6

    dsu_union(&d, 0, 9);
    comps = dsu_count_components(&d);
    printf("Final components: %d\n", comps);
    // EXPECT: Final components: 1

    sz = dsu_component_size(&d, 5);
    printf("Final component size: %d\n", sz);
    // EXPECT: Final component size: 10

    // Verify path compression: after finds, parents should point to root
    dsu_find(&d, 9);
    dsu_find(&d, 3);
    printf("Path compression works: %d\n", d.parent[9] == dsu_find(&d, 0));
    // EXPECT: Path compression works: 1

    printf("All same root: %d\n", dsu_find(&d, 0) == dsu_find(&d, 9));
    // EXPECT: All same root: 1

    return 0;
}
