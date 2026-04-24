int printf(const char *fmt, ...);
// EXPECT: Initial sets: 10\nAfter 3 unions: 7 sets\nAfter union(0,2): 6 sets\nConnected 1,3: 1\nConnected 1,5: 0\nAfter more unions: 2 sets\nSet size of 0: 8\nSet size of 4: 2\nSets: {0,1,2,3,6,7,8,9} {4,5} 
// Disjoint set (union-find) with path compression and union by rank

struct UnionFind {
    int parent[20];
    int rank[20];
    int size[20];
    int num_sets;
    int n;
};

void uf_init(struct UnionFind *uf, int n) {
    int i;
    uf->n = n;
    uf->num_sets = n;
    for (i = 0; i < n; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
        uf->size[i] = 1;
    }
}

int uf_find(struct UnionFind *uf, int x) {
    int root = x;
    int tmp;
    while (uf->parent[root] != root) {
        root = uf->parent[root];
    }
    while (uf->parent[x] != root) {
        tmp = uf->parent[x];
        uf->parent[x] = root;
        x = tmp;
    }
    return root;
}

void uf_union(struct UnionFind *uf, int x, int y) {
    int rx = uf_find(uf, x);
    int ry = uf_find(uf, y);
    if (rx == ry) return;
    if (uf->rank[rx] < uf->rank[ry]) {
        uf->parent[rx] = ry;
        uf->size[ry] = uf->size[ry] + uf->size[rx];
    } else if (uf->rank[rx] > uf->rank[ry]) {
        uf->parent[ry] = rx;
        uf->size[rx] = uf->size[rx] + uf->size[ry];
    } else {
        uf->parent[ry] = rx;
        uf->size[rx] = uf->size[rx] + uf->size[ry];
        uf->rank[rx]++;
    }
    uf->num_sets--;
}

int uf_connected(struct UnionFind *uf, int x, int y) {
    return uf_find(uf, x) == uf_find(uf, y);
}

int uf_set_size(struct UnionFind *uf, int x) {
    return uf->size[uf_find(uf, x)];
}

void uf_print_sets(struct UnionFind *uf) {
    int i;
    int j;
    int printed[20];
    int root;
    int first;
    for (i = 0; i < uf->n; i++) printed[i] = 0;
    for (i = 0; i < uf->n; i++) {
        root = uf_find(uf, i);
        if (!printed[root]) {
            printed[root] = 1;
            printf("{");
            first = 1;
            for (j = 0; j < uf->n; j++) {
                if (uf_find(uf, j) == root) {
                    if (!first) printf(",");
                    printf("%d", j);
                    first = 0;
                }
            }
            printf("} ");
        }
    }
    printf("\n");
}

int main(void) {
    struct UnionFind uf;
    uf_init(&uf, 10);

    printf("Initial sets: %d\n", uf.num_sets);

    uf_union(&uf, 0, 1);
    uf_union(&uf, 2, 3);
    uf_union(&uf, 4, 5);
    printf("After 3 unions: %d sets\n", uf.num_sets);

    uf_union(&uf, 0, 2);
    printf("After union(0,2): %d sets\n", uf.num_sets);
    printf("Connected 1,3: %d\n", uf_connected(&uf, 1, 3));
    printf("Connected 1,5: %d\n", uf_connected(&uf, 1, 5));

    uf_union(&uf, 6, 7);
    uf_union(&uf, 8, 9);
    uf_union(&uf, 6, 8);
    uf_union(&uf, 0, 6);
    printf("After more unions: %d sets\n", uf.num_sets);

    printf("Set size of 0: %d\n", uf_set_size(&uf, 0));
    printf("Set size of 4: %d\n", uf_set_size(&uf, 4));

    printf("Sets: ");
    uf_print_sets(&uf);

    return 0;
}
