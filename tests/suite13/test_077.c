int printf(const char *fmt, ...);

// Floyd-Warshall all-pairs shortest path

struct APSPResult {
    int dist[8][8];
    int next[8][8];  // next[i][j] = next node on shortest path from i to j
    int n;
};

void fw_init(struct APSPResult *r, int n) {
    int i;
    int j;
    r->n = n;
    for (i = 0; i < 8; i = i + 1) {
        for (j = 0; j < 8; j = j + 1) {
            if (i == j) {
                r->dist[i][j] = 0;
            } else {
                r->dist[i][j] = 99999;
            }
            r->next[i][j] = -1;
        }
    }
}

void fw_add_edge(struct APSPResult *r, int from, int to, int w) {
    r->dist[from][to] = w;
    r->next[from][to] = to;
}

void fw_add_undirected(struct APSPResult *r, int u, int v, int w) {
    r->dist[u][v] = w;
    r->dist[v][u] = w;
    r->next[u][v] = v;
    r->next[v][u] = u;
}

void floyd_warshall(struct APSPResult *r) {
    int k;
    int i;
    int j;
    int through_k;

    for (k = 0; k < r->n; k = k + 1) {
        for (i = 0; i < r->n; i = i + 1) {
            for (j = 0; j < r->n; j = j + 1) {
                if (r->dist[i][k] < 99999 && r->dist[k][j] < 99999) {
                    through_k = r->dist[i][k] + r->dist[k][j];
                    if (through_k < r->dist[i][j]) {
                        r->dist[i][j] = through_k;
                        r->next[i][j] = r->next[i][k];
                    }
                }
            }
        }
    }
}

// Reconstruct path from u to v, return length (node count)
int fw_path(struct APSPResult *r, int u, int v, int *path) {
    int len;
    int cur;
    if (r->dist[u][v] >= 99999) return 0;
    len = 0;
    cur = u;
    path[len] = cur;
    len = len + 1;
    while (cur != v) {
        cur = r->next[cur][v];
        if (cur == -1) return 0;
        path[len] = cur;
        len = len + 1;
    }
    return len;
}

// Diameter: longest shortest path among all pairs
int graph_diameter(struct APSPResult *r) {
    int i;
    int j;
    int maxd;
    maxd = 0;
    for (i = 0; i < r->n; i = i + 1) {
        for (j = 0; j < r->n; j = j + 1) {
            if (r->dist[i][j] < 99999 && r->dist[i][j] > maxd) {
                maxd = r->dist[i][j];
            }
        }
    }
    return maxd;
}

int main() {
    struct APSPResult r;
    int path[8];
    int len;

    // Directed weighted graph, 4 nodes
    // 0->(3)->1, 0->(7)->2, 1->(1)->2, 1->(2)->3, 2->(2)->3
    fw_init(&r, 4);
    fw_add_edge(&r, 0, 1, 3);
    fw_add_edge(&r, 0, 2, 7);
    fw_add_edge(&r, 1, 2, 1);
    fw_add_edge(&r, 1, 3, 2);
    fw_add_edge(&r, 2, 3, 2);

    floyd_warshall(&r);

    // dist[0][0..3]: 0, 3, 4, 5
    // 0->0=0, 0->1=3, 0->2=min(7, 3+1)=4, 0->3=min(inf, 3+2, 4+2)=5
    printf("%d %d %d %d\n", r.dist[0][0], r.dist[0][1], r.dist[0][2], r.dist[0][3]);
    // EXPECT: 0 3 4 5

    // dist[1][0..3]: inf, 0, 1, 2
    printf("%d %d %d %d\n", r.dist[1][0] >= 99999 ? -1 : r.dist[1][0], r.dist[1][1], r.dist[1][2], r.dist[1][3]);
    // EXPECT: -1 0 1 2

    // dist[2][0..3]: inf, inf, 0, 2
    printf("%d %d %d %d\n", r.dist[2][0] >= 99999 ? -1 : r.dist[2][0], r.dist[2][1] >= 99999 ? -1 : r.dist[2][1], r.dist[2][2], r.dist[2][3]);
    // EXPECT: -1 -1 0 2

    // dist[3][0..3]: inf, inf, inf, 0
    printf("%d %d %d %d\n", r.dist[3][0] >= 99999 ? -1 : r.dist[3][0], r.dist[3][1] >= 99999 ? -1 : r.dist[3][1], r.dist[3][2] >= 99999 ? -1 : r.dist[3][2], r.dist[3][3]);
    // EXPECT: -1 -1 -1 0

    // Path from 0 to 3: 0->1->3 (cost 3+2=5)
    len = fw_path(&r, 0, 3, path);
    printf("%d\n", len);
    // EXPECT: 3
    printf("%d %d %d\n", path[0], path[1], path[2]);
    // EXPECT: 0 1 3

    // Path from 0 to 2: 0->1->2 (cost 3+1=4)
    len = fw_path(&r, 0, 2, path);
    printf("%d\n", len);
    // EXPECT: 3
    printf("%d %d %d\n", path[0], path[1], path[2]);
    // EXPECT: 0 1 2

    printf("%d\n", graph_diameter(&r));
    // EXPECT: 5

    // Undirected weighted graph, 5 nodes (triangle + branch)
    // 0--(2)--1--(3)--2
    //         |       |
    //        (1)     (4)
    //         |       |
    //         3       4
    fw_init(&r, 5);
    fw_add_undirected(&r, 0, 1, 2);
    fw_add_undirected(&r, 1, 2, 3);
    fw_add_undirected(&r, 1, 3, 1);
    fw_add_undirected(&r, 2, 4, 4);

    floyd_warshall(&r);

    // dist from 0: 0, 2, 5, 3, 9
    printf("%d %d %d %d %d\n", r.dist[0][0], r.dist[0][1], r.dist[0][2], r.dist[0][3], r.dist[0][4]);
    // EXPECT: 0 2 5 3 9

    // dist from 3: 3, 1, 4, 0, 8
    printf("%d %d %d %d %d\n", r.dist[3][0], r.dist[3][1], r.dist[3][2], r.dist[3][3], r.dist[3][4]);
    // EXPECT: 3 1 4 0 8

    // dist from 4: 9, 7, 4, 8, 0
    printf("%d %d %d %d %d\n", r.dist[4][0], r.dist[4][1], r.dist[4][2], r.dist[4][3], r.dist[4][4]);
    // EXPECT: 9 7 4 8 0

    printf("%d\n", graph_diameter(&r));
    // EXPECT: 9

    // Path from 0 to 4: 0->1->2->4 (2+3+4=9)
    len = fw_path(&r, 0, 4, path);
    printf("%d\n", len);
    // EXPECT: 4
    printf("%d %d %d %d\n", path[0], path[1], path[2], path[3]);
    // EXPECT: 0 1 2 4

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
