int printf(const char *fmt, ...);

struct Edge {
    int u;
    int v;
    int w;
};

struct Edge edges[64];
int edge_count;

int par[16];
int rnk[16];

int n_vertices;

void init_uf(int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        par[i] = i;
        rnk[i] = 0;
    }
}

int find(int x) {
    if (par[x] != x) {
        par[x] = find(par[x]);
    }
    return par[x];
}

int unite(int a, int b) {
    int ra;
    int rb;
    ra = find(a);
    rb = find(b);
    if (ra == rb) return 0;
    if (rnk[ra] < rnk[rb]) {
        par[ra] = rb;
    } else if (rnk[ra] > rnk[rb]) {
        par[rb] = ra;
    } else {
        par[rb] = ra;
        rnk[ra] = rnk[ra] + 1;
    }
    return 1;
}

void init_graph(int n) {
    n_vertices = n;
    edge_count = 0;
}

void add_edge(int u, int v, int w) {
    edges[edge_count].u = u;
    edges[edge_count].v = v;
    edges[edge_count].w = w;
    edge_count = edge_count + 1;
}

void sort_edges(void) {
    int i;
    int j;
    struct Edge tmp;
    for (i = 0; i < edge_count - 1; i = i + 1) {
        for (j = 0; j < edge_count - 1 - i; j = j + 1) {
            if (edges[j].w > edges[j + 1].w) {
                tmp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = tmp;
            }
        }
    }
}

struct Edge mst_edges[16];
int mst_count;
int mst_weight;

void kruskal(void) {
    int i;
    mst_count = 0;
    mst_weight = 0;
    init_uf(n_vertices);
    sort_edges();
    for (i = 0; i < edge_count; i = i + 1) {
        if (unite(edges[i].u, edges[i].v)) {
            mst_edges[mst_count] = edges[i];
            mst_count = mst_count + 1;
            mst_weight = mst_weight + edges[i].w;
            if (mst_count == n_vertices - 1) break;
        }
    }
}

int prim_adj[16][16];

int prim_mst(int n) {
    int in_mst[16];
    int key[16];
    int total;
    int i;
    int j;
    int u;
    int min_k;
    int min_u;

    for (i = 0; i < n; i = i + 1) {
        in_mst[i] = 0;
        key[i] = 99999;
    }
    key[0] = 0;
    total = 0;

    for (i = 0; i < n; i = i + 1) {
        min_k = 99999;
        min_u = -1;
        for (u = 0; u < n; u = u + 1) {
            if (!in_mst[u] && key[u] < min_k) {
                min_k = key[u];
                min_u = u;
            }
        }
        if (min_u == -1) break;
        in_mst[min_u] = 1;
        total = total + key[min_u];
        for (j = 0; j < n; j = j + 1) {
            if (prim_adj[min_u][j] > 0 && !in_mst[j]) {
                if (prim_adj[min_u][j] < key[j]) {
                    key[j] = prim_adj[min_u][j];
                }
            }
        }
    }
    return total;
}

int main(void) {
    int i;
    int j;
    int total;

    init_graph(6);
    add_edge(0, 1, 4);
    add_edge(0, 2, 2);
    add_edge(1, 2, 1);
    add_edge(1, 3, 5);
    add_edge(2, 3, 8);
    add_edge(2, 4, 10);
    add_edge(3, 4, 2);
    add_edge(3, 5, 6);
    add_edge(4, 5, 3);

    kruskal();
    // EXPECT: kruskal weight: 13
    printf("kruskal weight: %d\n", mst_weight);
    // EXPECT: kruskal edges: 5
    printf("kruskal edges: %d\n", mst_count);

    for (i = 0; i < 16; i = i + 1) {
        for (j = 0; j < 16; j = j + 1) {
            prim_adj[i][j] = 0;
        }
    }
    prim_adj[0][1] = 4; prim_adj[1][0] = 4;
    prim_adj[0][2] = 2; prim_adj[2][0] = 2;
    prim_adj[1][2] = 1; prim_adj[2][1] = 1;
    prim_adj[1][3] = 5; prim_adj[3][1] = 5;
    prim_adj[2][3] = 8; prim_adj[3][2] = 8;
    prim_adj[2][4] = 10; prim_adj[4][2] = 10;
    prim_adj[3][4] = 2; prim_adj[4][3] = 2;
    prim_adj[3][5] = 6; prim_adj[5][3] = 6;
    prim_adj[4][5] = 3; prim_adj[5][4] = 3;

    total = prim_mst(6);
    // EXPECT: prim weight: 13
    printf("prim weight: %d\n", total);

    init_graph(4);
    add_edge(0, 1, 10);
    add_edge(0, 2, 6);
    add_edge(0, 3, 5);
    add_edge(1, 3, 15);
    add_edge(2, 3, 4);

    kruskal();
    // EXPECT: kruskal2 weight: 19
    printf("kruskal2 weight: %d\n", mst_weight);
    // EXPECT: kruskal2 edges: 3
    printf("kruskal2 edges: %d\n", mst_count);

    for (i = 0; i < 16; i = i + 1) {
        for (j = 0; j < 16; j = j + 1) {
            prim_adj[i][j] = 0;
        }
    }
    prim_adj[0][1] = 10; prim_adj[1][0] = 10;
    prim_adj[0][2] = 6; prim_adj[2][0] = 6;
    prim_adj[0][3] = 5; prim_adj[3][0] = 5;
    prim_adj[1][3] = 15; prim_adj[3][1] = 15;
    prim_adj[2][3] = 4; prim_adj[3][2] = 4;

    total = prim_mst(4);
    // EXPECT: prim2 weight: 19
    printf("prim2 weight: %d\n", total);

    init_graph(3);
    add_edge(0, 1, 1);
    add_edge(1, 2, 2);
    add_edge(0, 2, 3);
    kruskal();
    // EXPECT: triangle weight: 3
    printf("triangle weight: %d\n", mst_weight);
    // EXPECT: triangle edges: 2
    printf("triangle edges: %d\n", mst_count);

    // EXPECT: first mst edge w: 1
    printf("first mst edge w: %d\n", mst_edges[0].w);
    // EXPECT: second mst edge w: 2
    printf("second mst edge w: %d\n", mst_edges[1].w);

    init_uf(5);
    // EXPECT: uf find 0: 0
    printf("uf find 0: %d\n", find(0));
    unite(0, 1);
    unite(2, 3);
    // EXPECT: uf same 0 1: 1
    printf("uf same 0 1: %d\n", find(0) == find(1));
    // EXPECT: uf same 0 2: 0
    printf("uf same 0 2: %d\n", find(0) == find(2));
    unite(1, 3);
    // EXPECT: uf same 0 3: 1
    printf("uf same 0 3: %d\n", find(0) == find(3));

    return 0;
}
