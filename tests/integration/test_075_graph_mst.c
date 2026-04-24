int printf(const char *fmt, ...);
// EXPECT: === Graph 1 properties ===\nVertices: 5, Edges: 7\nTotal edge weight: 40\nConnected: 1\nComponents: 1\nMin edge: 2, Max edge: 9\nDegree of 0: 2\nDegree of 1: 4\nDegree of 3: 4\n=== Prim MST (Graph 1) ===\nPrim MST weight: 16\nPrim parent: -1 0 1 0 1\n=== Kruskal MST (Graph 1) ===\nKruskal MST weight: 16\nKruskal MST edges: 4\n=== Graph 2 properties ===\nVertices: 6, Edges: 8\nTotal edge weight: 26\nConnected: 1\n=== Prim MST (Graph 2) ===\nPrim MST weight: 12\n=== Kruskal MST (Graph 2) ===\nKruskal MST weight: 12\nKruskal MST edges: 5\n=== Graph 3 (complete K4) ===\nVertices: 4, Edges: 6\nTotal edge weight: 40\nPrim MST weight: 15\nKruskal MST weight: 15\n=== Graph 4 properties ===\nVertices: 7, Edges: 11\nConnected: 1\nPrim MST weight: 39\nKruskal MST weight: 39\n=== Dijkstra (Graph 1 from 0) ===\ndist[0]=0 dist[1]=2 dist[2]=5 dist[3]=6 dist[4]=7\n=== Dijkstra (Graph 2 from 0) ===\ndist[0]=0 dist[1]=4 dist[2]=4 dist[3]=7 dist[4]=10 dist[5]=9\n=== Union-Find tests ===\nUF: 0-1 connected=0\nAfter union(0,1): connected=1\nAfter union(2,3): 0-3 connected=0\nAfter union(1,2): 0-3 connected=1\nComponents after unions: 1\n=== Second MST ===\nGraph3 first MST: 15\nGraph3 second MST: 16\n=== MST verification ===\nG1 Prim==Kruskal: 1\nG2 Prim==Kruskal: 1\nG3 Prim==Kruskal: 1\nG4 Prim==Kruskal: 1\n=== Edge sorted order (Graph 1) ===\nSorted edges: 2 3 5 6 7 8 9\n=== MST edge details (Graph 3) ===\nMST edge: 1-2 w=4\nMST edge: 0-3 w=5\nMST edge: 0-2 w=6\nAll graph MST tests passed!
// ============================================================
// test_075_graph_mst.c
// Minimum spanning tree (Prim's and Kruskal's on small graphs
// with adjacency matrix), edge weight totals.
// ============================================================

// ---- Constants ----
// Maximum number of vertices
// We use 0 as "no edge" in adjacency matrix, so all weights > 0

// ---- Large value to represent infinity ----
int INF_VAL() {
    return 999999;
}

// ---- Graph as adjacency matrix ----
struct Graph {
    int adj[20][20];   // adjacency matrix (0 = no edge)
    int n;             // number of vertices
};

// ---- Initialize graph ----
void graph_init(struct Graph *g, int n) {
    g->n = n;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            g->adj[i][j] = 0;
        }
    }
}

// ---- Add undirected edge ----
void graph_add_edge(struct Graph *g, int u, int v, int w) {
    g->adj[u][v] = w;
    g->adj[v][u] = w;
}

// ---- Prim's MST algorithm ----
// Returns total weight of MST
// parent[] stores the MST edges (parent[i] = parent of i in MST)
int prim_mst(struct Graph *g, int parent[]) {
    int n = g->n;
    int in_mst[20];
    int key[20]; // minimum weight edge to connect to MST
    int i, j;

    for (i = 0; i < n; i++) {
        in_mst[i] = 0;
        key[i] = INF_VAL();
        parent[i] = -1;
    }

    key[0] = 0; // Start from vertex 0

    int total_weight = 0;
    int count;
    for (count = 0; count < n; count++) {
        // Find minimum key vertex not in MST
        int u = -1;
        int min_key = INF_VAL();
        for (i = 0; i < n; i++) {
            if (!in_mst[i] && key[i] < min_key) {
                min_key = key[i];
                u = i;
            }
        }

        if (u == -1) break; // Graph not connected

        in_mst[u] = 1;
        total_weight = total_weight + key[u];

        // Update keys of adjacent vertices
        for (j = 0; j < n; j++) {
            if (g->adj[u][j] != 0 && !in_mst[j] && g->adj[u][j] < key[j]) {
                key[j] = g->adj[u][j];
                parent[j] = u;
            }
        }
    }

    return total_weight;
}

// ---- Union-Find (Disjoint Set) for Kruskal's ----
struct UnionFind {
    int parent[20];
    int rank[20];
    int n;
};

void uf_init(struct UnionFind *uf, int n) {
    uf->n = n;
    int i;
    for (i = 0; i < n; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
}

int uf_find(struct UnionFind *uf, int x) {
    while (uf->parent[x] != x) {
        uf->parent[x] = uf->parent[uf->parent[x]]; // path compression
        x = uf->parent[x];
    }
    return x;
}

int uf_union(struct UnionFind *uf, int x, int y) {
    int rx = uf_find(uf, x);
    int ry = uf_find(uf, y);
    if (rx == ry) return 0; // Already same set

    if (uf->rank[rx] < uf->rank[ry]) {
        uf->parent[rx] = ry;
    } else if (uf->rank[rx] > uf->rank[ry]) {
        uf->parent[ry] = rx;
    } else {
        uf->parent[ry] = rx;
        uf->rank[rx]++;
    }
    return 1;
}

int uf_connected(struct UnionFind *uf, int x, int y) {
    return (uf_find(uf, x) == uf_find(uf, y)) ? 1 : 0;
}

// ---- Edge structure for Kruskal's ----
struct Edge {
    int u;
    int v;
    int w;
};

// ---- Sort edges by weight (bubble sort) ----
void sort_edges(struct Edge edges[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (edges[j].w > edges[j + 1].w) {
                struct Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }
}

// ---- Extract edges from adjacency matrix ----
int extract_edges(struct Graph *g, struct Edge edges[]) {
    int count = 0;
    int i, j;
    for (i = 0; i < g->n; i++) {
        for (j = i + 1; j < g->n; j++) {
            if (g->adj[i][j] != 0) {
                edges[count].u = i;
                edges[count].v = j;
                edges[count].w = g->adj[i][j];
                count++;
            }
        }
    }
    return count;
}

// ---- Kruskal's MST algorithm ----
// Returns total weight and stores MST edges in mst_edges[]
int kruskal_mst(struct Graph *g, struct Edge mst_edges[], int *mst_count) {
    struct Edge edges[100];
    int ne = extract_edges(g, edges);
    sort_edges(edges, ne);

    struct UnionFind uf;
    uf_init(&uf, g->n);

    int total_weight = 0;
    *mst_count = 0;
    int i;

    for (i = 0; i < ne; i++) {
        if (uf_union(&uf, edges[i].u, edges[i].v)) {
            mst_edges[*mst_count] = edges[i];
            (*mst_count)++;
            total_weight = total_weight + edges[i].w;
            if (*mst_count == g->n - 1) break;
        }
    }

    return total_weight;
}

// ---- Count edges in graph ----
int count_edges(struct Graph *g) {
    int count = 0;
    int i, j;
    for (i = 0; i < g->n; i++) {
        for (j = i + 1; j < g->n; j++) {
            if (g->adj[i][j] != 0) count++;
        }
    }
    return count;
}

// ---- Total weight of all edges ----
int total_edge_weight(struct Graph *g) {
    int total = 0;
    int i, j;
    for (i = 0; i < g->n; i++) {
        for (j = i + 1; j < g->n; j++) {
            total = total + g->adj[i][j];
        }
    }
    return total;
}

// ---- Degree of a vertex ----
int vertex_degree(struct Graph *g, int v) {
    int deg = 0;
    int i;
    for (i = 0; i < g->n; i++) {
        if (g->adj[v][i] != 0) deg++;
    }
    return deg;
}

// ---- Check if graph is connected using BFS ----
int is_connected(struct Graph *g) {
    int visited[20];
    int queue[20];
    int front = 0, back = 0;
    int i;

    for (i = 0; i < g->n; i++) visited[i] = 0;

    visited[0] = 1;
    queue[back++] = 0;

    while (front < back) {
        int u = queue[front++];
        for (i = 0; i < g->n; i++) {
            if (g->adj[u][i] != 0 && !visited[i]) {
                visited[i] = 1;
                queue[back++] = i;
            }
        }
    }

    for (i = 0; i < g->n; i++) {
        if (!visited[i]) return 0;
    }
    return 1;
}

// ---- Dijkstra's shortest path from source ----
void dijkstra(struct Graph *g, int src, int dist[]) {
    int visited[20];
    int i, j;

    for (i = 0; i < g->n; i++) {
        dist[i] = INF_VAL();
        visited[i] = 0;
    }
    dist[src] = 0;

    int count;
    for (count = 0; count < g->n; count++) {
        int u = -1;
        int min_d = INF_VAL();
        for (i = 0; i < g->n; i++) {
            if (!visited[i] && dist[i] < min_d) {
                min_d = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        for (j = 0; j < g->n; j++) {
            if (g->adj[u][j] != 0 && !visited[j]) {
                int new_dist = dist[u] + g->adj[u][j];
                if (new_dist < dist[j]) {
                    dist[j] = new_dist;
                }
            }
        }
    }
}

// ---- Check if adding edge creates cycle (using BFS) ----
int creates_cycle(struct Graph *g, int u, int v) {
    // BFS from u, see if v is reachable without using edge u-v
    int visited[20];
    int queue[20];
    int front = 0, back = 0;
    int i;

    for (i = 0; i < g->n; i++) visited[i] = 0;

    visited[u] = 1;
    queue[back++] = u;

    while (front < back) {
        int curr = queue[front++];
        for (i = 0; i < g->n; i++) {
            if (g->adj[curr][i] != 0 && !visited[i]) {
                if (i == v) return 1; // v reachable from u
                visited[i] = 1;
                queue[back++] = i;
            }
        }
    }
    return 0;
}

// ---- Number of connected components ----
int count_components(struct Graph *g) {
    int visited[20];
    int queue[20];
    int i, j;
    int components = 0;

    for (i = 0; i < g->n; i++) visited[i] = 0;

    for (i = 0; i < g->n; i++) {
        if (!visited[i]) {
            components++;
            int front = 0, back = 0;
            visited[i] = 1;
            queue[back++] = i;
            while (front < back) {
                int u = queue[front++];
                for (j = 0; j < g->n; j++) {
                    if (g->adj[u][j] != 0 && !visited[j]) {
                        visited[j] = 1;
                        queue[back++] = j;
                    }
                }
            }
        }
    }
    return components;
}

// ---- Minimum weight edge in graph ----
int min_edge_weight(struct Graph *g) {
    int min_w = INF_VAL();
    int i, j;
    for (i = 0; i < g->n; i++) {
        for (j = i + 1; j < g->n; j++) {
            if (g->adj[i][j] != 0 && g->adj[i][j] < min_w) {
                min_w = g->adj[i][j];
            }
        }
    }
    return min_w;
}

// ---- Maximum weight edge in graph ----
int max_edge_weight(struct Graph *g) {
    int max_w = 0;
    int i, j;
    for (i = 0; i < g->n; i++) {
        for (j = i + 1; j < g->n; j++) {
            if (g->adj[i][j] > max_w) {
                max_w = g->adj[i][j];
            }
        }
    }
    return max_w;
}

// ---- Second MST weight (remove one MST edge, find next best) ----
int second_mst_weight(struct Graph *g) {
    struct Edge mst_edges[20];
    int mst_count;
    int first_mst = kruskal_mst(g, mst_edges, &mst_count);

    int min_second = INF_VAL();
    int e;

    for (e = 0; e < mst_count; e++) {
        // Temporarily remove this MST edge
        int u = mst_edges[e].u;
        int v = mst_edges[e].v;
        int w = g->adj[u][v];
        g->adj[u][v] = 0;
        g->adj[v][u] = 0;

        // Try to find MST without this edge
        struct Edge temp_mst[20];
        int temp_count;
        int temp_weight = kruskal_mst(g, temp_mst, &temp_count);

        if (temp_count == g->n - 1 && temp_weight < min_second) {
            min_second = temp_weight;
        }

        // Restore edge
        g->adj[u][v] = w;
        g->adj[v][u] = w;
    }

    return min_second;
}

// ---- Build test graph 1 (5 vertices) ----
//   0 --2-- 1
//   |       |
//   6       3
//   |       |
//   3 --7-- 2
//   |
//   9
//   |
//   4
void build_graph1(struct Graph *g) {
    graph_init(g, 5);
    graph_add_edge(g, 0, 1, 2);
    graph_add_edge(g, 0, 3, 6);
    graph_add_edge(g, 1, 2, 3);
    graph_add_edge(g, 1, 3, 8);
    graph_add_edge(g, 1, 4, 5);
    graph_add_edge(g, 2, 3, 7);
    graph_add_edge(g, 3, 4, 9);
}

// ---- Build test graph 2 (6 vertices) ----
void build_graph2(struct Graph *g) {
    graph_init(g, 6);
    graph_add_edge(g, 0, 1, 4);
    graph_add_edge(g, 0, 2, 4);
    graph_add_edge(g, 1, 2, 2);
    graph_add_edge(g, 2, 3, 3);
    graph_add_edge(g, 3, 4, 3);
    graph_add_edge(g, 4, 5, 1);
    graph_add_edge(g, 3, 5, 2);
    graph_add_edge(g, 1, 4, 7);
}

// ---- Build test graph 3 (4 vertices, complete) ----
void build_graph3(struct Graph *g) {
    graph_init(g, 4);
    graph_add_edge(g, 0, 1, 10);
    graph_add_edge(g, 0, 2, 6);
    graph_add_edge(g, 0, 3, 5);
    graph_add_edge(g, 1, 2, 4);
    graph_add_edge(g, 1, 3, 7);
    graph_add_edge(g, 2, 3, 8);
}

// ---- Build test graph 4 (7 vertices) ----
void build_graph4(struct Graph *g) {
    graph_init(g, 7);
    graph_add_edge(g, 0, 1, 7);
    graph_add_edge(g, 0, 3, 5);
    graph_add_edge(g, 1, 2, 8);
    graph_add_edge(g, 1, 3, 9);
    graph_add_edge(g, 1, 4, 7);
    graph_add_edge(g, 2, 4, 5);
    graph_add_edge(g, 3, 4, 15);
    graph_add_edge(g, 3, 5, 6);
    graph_add_edge(g, 4, 5, 8);
    graph_add_edge(g, 4, 6, 9);
    graph_add_edge(g, 5, 6, 11);
}


int main() {
    struct Graph g;
    int parent[20];

    // === Graph 1 ===
    printf("=== Graph 1 properties ===\n");
    build_graph1(&g);
    printf("Vertices: %d, Edges: %d\n", g.n, count_edges(&g));
    printf("Total edge weight: %d\n", total_edge_weight(&g));
    printf("Connected: %d\n", is_connected(&g));
    printf("Components: %d\n", count_components(&g));
    printf("Min edge: %d, Max edge: %d\n", min_edge_weight(&g), max_edge_weight(&g));
    printf("Degree of 0: %d\n", vertex_degree(&g, 0));
    printf("Degree of 1: %d\n", vertex_degree(&g, 1));
    printf("Degree of 3: %d\n", vertex_degree(&g, 3));

    printf("=== Prim MST (Graph 1) ===\n");
    build_graph1(&g);
    int prim_w = prim_mst(&g, parent);
    printf("Prim MST weight: %d\n", prim_w);
    printf("Prim parent:");
    {
        int i;
        for (i = 0; i < g.n; i++) {
            printf(" %d", parent[i]);
        }
    }
    printf("\n");

    printf("=== Kruskal MST (Graph 1) ===\n");
    build_graph1(&g);
    {
        struct Edge mst_edges[20];
        int mst_count;
        int kw = kruskal_mst(&g, mst_edges, &mst_count);
        printf("Kruskal MST weight: %d\n", kw);
        printf("Kruskal MST edges: %d\n", mst_count);
    }

    // === Graph 2 ===
    printf("=== Graph 2 properties ===\n");
    build_graph2(&g);
    printf("Vertices: %d, Edges: %d\n", g.n, count_edges(&g));
    printf("Total edge weight: %d\n", total_edge_weight(&g));
    printf("Connected: %d\n", is_connected(&g));

    printf("=== Prim MST (Graph 2) ===\n");
    build_graph2(&g);
    prim_w = prim_mst(&g, parent);
    printf("Prim MST weight: %d\n", prim_w);

    printf("=== Kruskal MST (Graph 2) ===\n");
    build_graph2(&g);
    {
        struct Edge mst_edges[20];
        int mst_count;
        int kw = kruskal_mst(&g, mst_edges, &mst_count);
        printf("Kruskal MST weight: %d\n", kw);
        printf("Kruskal MST edges: %d\n", mst_count);
    }

    // === Graph 3 ===
    printf("=== Graph 3 (complete K4) ===\n");
    build_graph3(&g);
    printf("Vertices: %d, Edges: %d\n", g.n, count_edges(&g));
    printf("Total edge weight: %d\n", total_edge_weight(&g));
    build_graph3(&g);
    printf("Prim MST weight: %d\n", prim_mst(&g, parent));
    build_graph3(&g);
    {
        struct Edge me[20]; int mc;
        printf("Kruskal MST weight: %d\n", kruskal_mst(&g, me, &mc));
    }

    // === Graph 4 ===
    printf("=== Graph 4 properties ===\n");
    build_graph4(&g);
    printf("Vertices: %d, Edges: %d\n", g.n, count_edges(&g));
    printf("Connected: %d\n", is_connected(&g));
    build_graph4(&g);
    printf("Prim MST weight: %d\n", prim_mst(&g, parent));
    build_graph4(&g);
    {
        struct Edge me[20]; int mc;
        printf("Kruskal MST weight: %d\n", kruskal_mst(&g, me, &mc));
    }

    // === Dijkstra ===
    printf("=== Dijkstra (Graph 1 from 0) ===\n");
    build_graph1(&g);
    {
        int dist[20];
        dijkstra(&g, 0, dist);
        int i;
        for (i = 0; i < g.n; i++) {
            printf("dist[%d]=%d", i, dist[i]);
            if (i < g.n - 1) printf(" ");
        }
        printf("\n");
    }

    printf("=== Dijkstra (Graph 2 from 0) ===\n");
    build_graph2(&g);
    {
        int dist[20];
        dijkstra(&g, 0, dist);
        int i;
        for (i = 0; i < g.n; i++) {
            printf("dist[%d]=%d", i, dist[i]);
            if (i < g.n - 1) printf(" ");
        }
        printf("\n");
    }

    // === Union-Find ===
    printf("=== Union-Find tests ===\n");
    {
        struct UnionFind uf;
        uf_init(&uf, 5);
        printf("UF: 0-1 connected=%d\n", uf_connected(&uf, 0, 1));
        uf_union(&uf, 0, 1);
        printf("After union(0,1): connected=%d\n", uf_connected(&uf, 0, 1));
        uf_union(&uf, 2, 3);
        printf("After union(2,3): 0-3 connected=%d\n", uf_connected(&uf, 0, 3));
        uf_union(&uf, 1, 2);
        printf("After union(1,2): 0-3 connected=%d\n", uf_connected(&uf, 0, 3));

        // Count components
        uf_union(&uf, 3, 4);
        int components = 0;
        int i;
        for (i = 0; i < 5; i++) {
            if (uf_find(&uf, i) == i) components++;
        }
        printf("Components after unions: %d\n", components);
    }

    // === Second MST ===
    printf("=== Second MST ===\n");
    build_graph3(&g);
    {
        struct Edge me[20]; int mc;
        int first = kruskal_mst(&g, me, &mc);
        printf("Graph3 first MST: %d\n", first);
    }
    build_graph3(&g);
    printf("Graph3 second MST: %d\n", second_mst_weight(&g));

    // === MST verification ===
    printf("=== MST verification ===\n");
    {
        struct Edge me[20]; int mc;
        int p1, k1, p2, k2, p3, k3, p4, k4;

        build_graph1(&g); p1 = prim_mst(&g, parent);
        build_graph1(&g); k1 = kruskal_mst(&g, me, &mc);
        printf("G1 Prim==Kruskal: %d\n", (p1 == k1) ? 1 : 0);

        build_graph2(&g); p2 = prim_mst(&g, parent);
        build_graph2(&g); k2 = kruskal_mst(&g, me, &mc);
        printf("G2 Prim==Kruskal: %d\n", (p2 == k2) ? 1 : 0);

        build_graph3(&g); p3 = prim_mst(&g, parent);
        build_graph3(&g); k3 = kruskal_mst(&g, me, &mc);
        printf("G3 Prim==Kruskal: %d\n", (p3 == k3) ? 1 : 0);

        build_graph4(&g); p4 = prim_mst(&g, parent);
        build_graph4(&g); k4 = kruskal_mst(&g, me, &mc);
        printf("G4 Prim==Kruskal: %d\n", (p4 == k4) ? 1 : 0);
    }

    // === Edge sorted order ===
    printf("=== Edge sorted order (Graph 1) ===\n");
    build_graph1(&g);
    {
        struct Edge edges[20];
        int ne = extract_edges(&g, edges);
        sort_edges(edges, ne);
        printf("Sorted edges:");
        int i;
        for (i = 0; i < ne; i++) {
            printf(" %d", edges[i].w);
        }
        printf("\n");
    }

    // === MST edge details ===
    printf("=== MST edge details (Graph 3) ===\n");
    build_graph3(&g);
    {
        struct Edge mst_edges[20];
        int mst_count;
        kruskal_mst(&g, mst_edges, &mst_count);
        int i;
        for (i = 0; i < mst_count; i++) {
            printf("MST edge: %d-%d w=%d\n",
                   mst_edges[i].u, mst_edges[i].v, mst_edges[i].w);
        }
    }

    printf("All graph MST tests passed!\n");
    return 0;
}
