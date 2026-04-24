int printf(const char *fmt, ...);

// Kruskal's MST with Union-Find
// Graph: 6 vertices, 9 edges (weighted undirected)

struct Edge {
    int u;
    int v;
    int w;
};

int parent[64];
int rank_arr[64];

void uf_init(int n) {
    int i;
    for (i = 0; i < n; i++) {
        parent[i] = i;
        rank_arr[i] = 0;
    }
}

int uf_find(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

int uf_union(int a, int b) {
    int ra;
    int rb;
    ra = uf_find(a);
    rb = uf_find(b);
    if (ra == rb) return 0;
    if (rank_arr[ra] < rank_arr[rb]) {
        parent[ra] = rb;
    } else if (rank_arr[ra] > rank_arr[rb]) {
        parent[rb] = ra;
    } else {
        parent[rb] = ra;
        rank_arr[ra] = rank_arr[ra] + 1;
    }
    return 1;
}

void sort_edges(struct Edge *edges, int n) {
    int i;
    int j;
    struct Edge tmp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (edges[j].w > edges[j + 1].w) {
                tmp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = tmp;
            }
        }
    }
}

int main() {
    struct Edge edges[9];
    int num_edges;
    int num_vertices;
    int mst_weight;
    int mst_count;
    int i;
    int merged;

    num_vertices = 6;
    num_edges = 9;

    edges[0].u = 0; edges[0].v = 1; edges[0].w = 4;
    edges[1].u = 0; edges[1].v = 2; edges[1].w = 2;
    edges[2].u = 1; edges[2].v = 2; edges[2].w = 1;
    edges[3].u = 1; edges[3].v = 3; edges[3].w = 5;
    edges[4].u = 2; edges[4].v = 3; edges[4].w = 8;
    edges[5].u = 2; edges[5].v = 4; edges[5].w = 10;
    edges[6].u = 3; edges[6].v = 4; edges[6].w = 3;
    edges[7].u = 3; edges[7].v = 5; edges[7].w = 6;
    edges[8].u = 4; edges[8].v = 5; edges[8].w = 7;

    sort_edges(edges, num_edges);

    printf("Sorted edges:\n"); // EXPECT: Sorted edges:
    for (i = 0; i < num_edges; i++) {
        printf("  %d-%d w=%d\n", edges[i].u, edges[i].v, edges[i].w);
    }
    // EXPECT:   1-2 w=1
    // EXPECT:   0-2 w=2
    // EXPECT:   3-4 w=3
    // EXPECT:   0-1 w=4
    // EXPECT:   1-3 w=5
    // EXPECT:   3-5 w=6
    // EXPECT:   4-5 w=7
    // EXPECT:   2-3 w=8
    // EXPECT:   2-4 w=10

    uf_init(num_vertices);
    mst_weight = 0;
    mst_count = 0;

    printf("MST edges:\n"); // EXPECT: MST edges:
    for (i = 0; i < num_edges; i++) {
        if (mst_count == num_vertices - 1) break;
        merged = uf_union(edges[i].u, edges[i].v);
        if (merged) {
            printf("  %d-%d w=%d\n", edges[i].u, edges[i].v, edges[i].w);
            mst_weight = mst_weight + edges[i].w;
            mst_count = mst_count + 1;
        }
    }
    // EXPECT:   1-2 w=1
    // EXPECT:   0-2 w=2
    // EXPECT:   3-4 w=3
    // EXPECT:   1-3 w=5
    // EXPECT:   3-5 w=6
    printf("MST weight: %d\n", mst_weight); // EXPECT: MST weight: 17
    printf("MST edges count: %d\n", mst_count); // EXPECT: MST edges count: 5

    printf("Find 0: %d\n", uf_find(0)); // EXPECT: Find 0: 1
    printf("Same component 0,5: %d\n", uf_find(0) == uf_find(5)); // EXPECT: Same component 0,5: 1
    printf("Find 3: %d\n", uf_find(3)); // EXPECT: Find 3: 1
    printf("Find 5: %d\n", uf_find(5)); // EXPECT: Find 5: 1

    return 0;
}
