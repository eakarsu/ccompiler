int printf(const char *fmt, ...);

struct Edge {
    int to;
    int next;
};

struct Graph {
    int head[16];
    struct Edge edges[64];
    int num_nodes;
    int num_edges;
};

void graph_init(struct Graph *g, int n) {
    g->num_nodes = n;
    g->num_edges = 0;
    int i = 0;
    while (i < 16) {
        g->head[i] = -1;
        i = i + 1;
    }
}

void graph_add_edge(struct Graph *g, int from, int to) {
    int idx = g->num_edges;
    g->edges[idx].to = to;
    g->edges[idx].next = g->head[from];
    g->head[from] = idx;
    g->num_edges = g->num_edges + 1;
}

void graph_add_undirected(struct Graph *g, int a, int b) {
    graph_add_edge(g, a, b);
    graph_add_edge(g, b, a);
}

int degree(struct Graph *g, int node) {
    int count = 0;
    int e = g->head[node];
    while (e != -1) {
        count = count + 1;
        e = g->edges[e].next;
    }
    return count;
}

int bfs_order[16];
int bfs_count;

void bfs(struct Graph *g, int start) {
    int visited[16];
    int queue[16];
    int front = 0;
    int back = 0;
    int i = 0;
    while (i < 16) {
        visited[i] = 0;
        i = i + 1;
    }
    bfs_count = 0;
    visited[start] = 1;
    queue[back] = start;
    back = back + 1;

    while (front < back) {
        int node = queue[front];
        front = front + 1;
        bfs_order[bfs_count] = node;
        bfs_count = bfs_count + 1;

        int e = g->head[node];
        while (e != -1) {
            int to = g->edges[e].to;
            if (visited[to] == 0) {
                visited[to] = 1;
                queue[back] = to;
                back = back + 1;
            }
            e = g->edges[e].next;
        }
    }
}

int dfs_order[16];
int dfs_count;
int dfs_visited[16];

void dfs(struct Graph *g, int node) {
    dfs_visited[node] = 1;
    dfs_order[dfs_count] = node;
    dfs_count = dfs_count + 1;

    int e = g->head[node];
    while (e != -1) {
        int to = g->edges[e].to;
        if (dfs_visited[to] == 0) {
            dfs(g, to);
        }
        e = g->edges[e].next;
    }
}

void dfs_start(struct Graph *g, int start) {
    int i = 0;
    while (i < 16) {
        dfs_visited[i] = 0;
        i = i + 1;
    }
    dfs_count = 0;
    dfs(g, start);
}

int has_edge(struct Graph *g, int from, int to) {
    int e = g->head[from];
    while (e != -1) {
        if (g->edges[e].to == to) return 1;
        e = g->edges[e].next;
    }
    return 0;
}

int main(void) {
    struct Graph g;
    graph_init(&g, 6);

    graph_add_undirected(&g, 0, 1);
    graph_add_undirected(&g, 0, 2);
    graph_add_undirected(&g, 1, 3);
    graph_add_undirected(&g, 2, 4);
    graph_add_undirected(&g, 3, 5);
    graph_add_undirected(&g, 4, 5);

    // EXPECT: edges=12
    printf("edges=%d\n", g.num_edges);

    // EXPECT: deg0=2
    printf("deg0=%d\n", degree(&g, 0));
    // EXPECT: deg5=2
    printf("deg5=%d\n", degree(&g, 5));

    // EXPECT: has_01=1
    printf("has_01=%d\n", has_edge(&g, 0, 1));
    // EXPECT: has_03=0
    printf("has_03=%d\n", has_edge(&g, 0, 3));

    bfs(&g, 0);
    // EXPECT: bfs_count=6
    printf("bfs_count=%d\n", bfs_count);
    // EXPECT: bfs_start=0
    printf("bfs_start=%d\n", bfs_order[0]);

    int bfs_visited_all = 1;
    int check[6];
    int i = 0;
    while (i < 6) {
        check[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < bfs_count) {
        check[bfs_order[i]] = 1;
        i = i + 1;
    }
    i = 0;
    while (i < 6) {
        if (check[i] == 0) bfs_visited_all = 0;
        i = i + 1;
    }
    // EXPECT: bfs_all=1
    printf("bfs_all=%d\n", bfs_visited_all);

    dfs_start(&g, 0);
    // EXPECT: dfs_count=6
    printf("dfs_count=%d\n", dfs_count);
    // EXPECT: dfs_start=0
    printf("dfs_start=%d\n", dfs_order[0]);

    int dfs_all = 1;
    i = 0;
    while (i < 6) {
        check[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < dfs_count) {
        check[dfs_order[i]] = 1;
        i = i + 1;
    }
    i = 0;
    while (i < 6) {
        if (check[i] == 0) dfs_all = 0;
        i = i + 1;
    }
    // EXPECT: dfs_all=1
    printf("dfs_all=%d\n", dfs_all);

    struct Graph g2;
    graph_init(&g2, 4);
    graph_add_edge(&g2, 0, 1);
    graph_add_edge(&g2, 1, 2);
    graph_add_edge(&g2, 2, 3);
    graph_add_edge(&g2, 3, 0);

    // EXPECT: g2_edges=4
    printf("g2_edges=%d\n", g2.num_edges);

    bfs(&g2, 0);
    // EXPECT: g2_bfs_count=4
    printf("g2_bfs_count=%d\n", bfs_count);
    // EXPECT: g2_bfs0=0
    printf("g2_bfs0=%d\n", bfs_order[0]);
    // EXPECT: g2_bfs1=1
    printf("g2_bfs1=%d\n", bfs_order[1]);
    // EXPECT: g2_bfs2=2
    printf("g2_bfs2=%d\n", bfs_order[2]);
    // EXPECT: g2_bfs3=3
    printf("g2_bfs3=%d\n", bfs_order[3]);

    int total_deg = 0;
    i = 0;
    while (i < 6) {
        total_deg = total_deg + degree(&g, i);
        i = i + 1;
    }
    // EXPECT: total_deg=12
    printf("total_deg=%d\n", total_deg);

    return 0;
}
