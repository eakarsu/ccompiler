int printf(const char *fmt, ...);

/* Simple shortest path using Dijkstra-like algorithm on weighted graph
   (adjacency matrix with weights, 0 = no edge) */

int weight[8][8];
int nv;

void init_graph(int n) {
    int i; int j;
    nv = n;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            weight[i][j] = 0;
}

void add_edge(int u, int v, int w) {
    weight[u][v] = w;
    weight[v][u] = w;
}

int dist[8];
int prev_node[8];
int visited[8];

void dijkstra(int src) {
    int i; int j;
    int u;
    int min_d;

    for (i = 0; i < nv; i++) {
        dist[i] = 99999;
        prev_node[i] = -1;
        visited[i] = 0;
    }
    dist[src] = 0;

    for (i = 0; i < nv; i++) {
        /* Find unvisited vertex with minimum distance */
        min_d = 99999;
        u = -1;
        for (j = 0; j < nv; j++) {
            if (visited[j] == 0 && dist[j] < min_d) {
                min_d = dist[j];
                u = j;
            }
        }
        if (u == -1) break;

        visited[u] = 1;

        /* Relax edges */
        for (j = 0; j < nv; j++) {
            if (weight[u][j] > 0 && visited[j] == 0) {
                int new_dist;
                new_dist = dist[u] + weight[u][j];
                if (new_dist < dist[j]) {
                    dist[j] = new_dist;
                    prev_node[j] = u;
                }
            }
        }
    }
}

void print_path(int src, int dst) {
    int path[8];
    int plen;
    int cur;
    int i;

    if (dist[dst] == 99999) {
        printf("no path %d->%d\n", src, dst);
        return;
    }

    plen = 0;
    cur = dst;
    while (cur != -1) {
        path[plen] = cur;
        plen = plen + 1;
        cur = prev_node[cur];
    }

    printf("path %d->%d (cost %d):", src, dst, dist[dst]);
    for (i = plen - 1; i >= 0; i--) {
        printf(" %d", path[i]);
    }
    printf("\n");
}

/* Bellman-Ford for graphs with potential negative edges */
int bf_dist[8];
int bf_prev[8];

struct Edge {
    int u;
    int v;
    int w;
};

struct Edge edges[20];
int edge_cnt;

void bf_add_edge(int u, int v, int w) {
    edges[edge_cnt].u = u;
    edges[edge_cnt].v = v;
    edges[edge_cnt].w = w;
    edge_cnt = edge_cnt + 1;
}

int bellman_ford(int src, int n) {
    int i; int j;
    int changed;

    for (i = 0; i < n; i++) {
        bf_dist[i] = 99999;
        bf_prev[i] = -1;
    }
    bf_dist[src] = 0;

    for (i = 0; i < n - 1; i++) {
        changed = 0;
        for (j = 0; j < edge_cnt; j++) {
            int u; int v; int w;
            u = edges[j].u;
            v = edges[j].v;
            w = edges[j].w;
            if (bf_dist[u] != 99999 && bf_dist[u] + w < bf_dist[v]) {
                bf_dist[v] = bf_dist[u] + w;
                bf_prev[v] = u;
                changed = 1;
            }
        }
        if (changed == 0) break;
    }

    /* Check for negative cycles */
    for (j = 0; j < edge_cnt; j++) {
        int u; int v; int w;
        u = edges[j].u;
        v = edges[j].v;
        w = edges[j].w;
        if (bf_dist[u] != 99999 && bf_dist[u] + w < bf_dist[v]) {
            return 0;  /* negative cycle */
        }
    }
    return 1;
}

int main(void) {
    int i;

    /*  Weighted graph:
        0 --(4)-- 1 --(8)-- 2
        |         |         |
       (8)       (11)      (7)
        |         |         |
        3 --(2)-- 4 --(4)-- 5
              \         /
              (6)     (2)
                \   /
                  6
    */
    init_graph(7);
    add_edge(0, 1, 4);
    add_edge(0, 3, 8);
    add_edge(1, 2, 8);
    add_edge(1, 4, 11);
    add_edge(2, 5, 7);
    add_edge(3, 4, 2);
    add_edge(4, 5, 4);
    add_edge(4, 6, 6);
    add_edge(5, 6, 2);

    dijkstra(0);

    // EXPECT: dist[0]=0
    printf("dist[0]=%d\n", dist[0]);
    // EXPECT: dist[1]=4
    printf("dist[1]=%d\n", dist[1]);
    // EXPECT: dist[2]=12
    printf("dist[2]=%d\n", dist[2]);
    // EXPECT: dist[3]=8
    printf("dist[3]=%d\n", dist[3]);
    // EXPECT: dist[4]=10
    printf("dist[4]=%d\n", dist[4]);
    // EXPECT: dist[5]=14
    printf("dist[5]=%d\n", dist[5]);
    // EXPECT: dist[6]=16
    printf("dist[6]=%d\n", dist[6]);

    // EXPECT: path 0->6 (cost 16): 0 3 4 6
    print_path(0, 6);
    // EXPECT: path 0->2 (cost 12): 0 1 2
    print_path(0, 2);
    // EXPECT: path 0->5 (cost 14): 0 3 4 5
    print_path(0, 5);

    /* Bellman-Ford test */
    edge_cnt = 0;
    bf_add_edge(0, 1, 4);
    bf_add_edge(0, 2, 5);
    bf_add_edge(1, 2, -3);
    bf_add_edge(2, 3, 4);

    i = bellman_ford(0, 4);
    // EXPECT: bf_ok=1
    printf("bf_ok=%d\n", i);
    // EXPECT: bf[0]=0
    printf("bf[0]=%d\n", bf_dist[0]);
    // EXPECT: bf[1]=4
    printf("bf[1]=%d\n", bf_dist[1]);
    // EXPECT: bf[2]=1
    printf("bf[2]=%d\n", bf_dist[2]);
    // EXPECT: bf[3]=5
    printf("bf[3]=%d\n", bf_dist[3]);

    return 0;
}
