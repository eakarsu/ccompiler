int printf(const char *fmt, ...);

// Graph coloring using greedy algorithm

struct Graph {
    int adj[8][8];
    int n;
};

void graph_init(struct Graph *g, int n) {
    int i;
    int j;
    g->n = n;
    for (i = 0; i < 8; i = i + 1) {
        for (j = 0; j < 8; j = j + 1) {
            g->adj[i][j] = 0;
        }
    }
}

void graph_add_edge(struct Graph *g, int u, int v) {
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

// Greedy graph coloring: assigns smallest available color to each node
// color[i] = color assigned to node i (0-indexed)
// Returns number of colors used (chromatic number upper bound)
int greedy_color(struct Graph *g, int *color) {
    int available[8];  // available[c] = 1 if color c is available
    int i;
    int v;
    int nb;
    int c;
    int num_colors;

    for (i = 0; i < g->n; i = i + 1) {
        color[i] = -1;
    }

    num_colors = 0;

    for (v = 0; v < g->n; v = v + 1) {
        // Mark all colors as available
        for (c = 0; c < g->n; c = c + 1) {
            available[c] = 1;
        }

        // Mark colors of adjacent colored vertices as unavailable
        for (nb = 0; nb < g->n; nb = nb + 1) {
            if (g->adj[v][nb] && color[nb] != -1) {
                available[color[nb]] = 0;
            }
        }

        // Pick the smallest available color
        for (c = 0; c < g->n; c = c + 1) {
            if (available[c]) {
                color[v] = c;
                break;
            }
        }

        if (color[v] + 1 > num_colors) {
            num_colors = color[v] + 1;
        }
    }

    return num_colors;
}

// Verify coloring: no two adjacent vertices have same color
int verify_coloring(struct Graph *g, int *color) {
    int u;
    int v;
    for (u = 0; u < g->n; u = u + 1) {
        for (v = u + 1; v < g->n; v = v + 1) {
            if (g->adj[u][v] && color[u] == color[v]) {
                return 0;
            }
        }
    }
    return 1;
}

// Get degree of a node
int degree(struct Graph *g, int v) {
    int i;
    int d;
    d = 0;
    for (i = 0; i < g->n; i = i + 1) {
        if (g->adj[v][i]) d = d + 1;
    }
    return d;
}

// Max degree in graph
int max_degree(struct Graph *g) {
    int i;
    int d;
    int maxd;
    maxd = 0;
    for (i = 0; i < g->n; i = i + 1) {
        d = degree(g, i);
        if (d > maxd) maxd = d;
    }
    return maxd;
}

// Greedy coloring with specific order (given as permutation)
int greedy_color_order(struct Graph *g, int *order, int *color) {
    int available[8];
    int i;
    int v;
    int nb;
    int c;
    int num_colors;

    for (i = 0; i < g->n; i = i + 1) {
        color[i] = -1;
    }
    num_colors = 0;

    for (i = 0; i < g->n; i = i + 1) {
        v = order[i];
        for (c = 0; c < g->n; c = c + 1) {
            available[c] = 1;
        }
        for (nb = 0; nb < g->n; nb = nb + 1) {
            if (g->adj[v][nb] && color[nb] != -1) {
                available[color[nb]] = 0;
            }
        }
        for (c = 0; c < g->n; c = c + 1) {
            if (available[c]) {
                color[v] = c;
                break;
            }
        }
        if (color[v] + 1 > num_colors) {
            num_colors = color[v] + 1;
        }
    }
    return num_colors;
}

int main() {
    struct Graph g;
    int color[8];
    int nc;
    int order[8];

    // Path graph: 0--1--2--3--4
    // Bipartite, needs 2 colors
    graph_init(&g, 5);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 4);

    nc = greedy_color(&g, color);
    printf("%d\n", nc);
    // EXPECT: 2
    printf("%d %d %d %d %d\n", color[0], color[1], color[2], color[3], color[4]);
    // EXPECT: 0 1 0 1 0
    printf("%d\n", verify_coloring(&g, color));
    // EXPECT: 1

    // Cycle of length 3 (odd): needs 3 colors
    // 0--1--2--0
    graph_init(&g, 3);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 0);

    nc = greedy_color(&g, color);
    // 0->color 0, 1->color 1 (0 adj), 2->color 2 (0 and 1 adj)
    printf("%d\n", nc);
    // EXPECT: 3
    printf("%d %d %d\n", color[0], color[1], color[2]);
    // EXPECT: 0 1 2
    printf("%d\n", verify_coloring(&g, color));
    // EXPECT: 1

    // Cycle of length 4 (even): bipartite, 2 colors
    // 0--1--2--3--0
    graph_init(&g, 4);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 0);

    nc = greedy_color(&g, color);
    // 0->0, 1->1(0 adj), 2->0(1 adj), 3->1(0,2 adj: colors 0,0 -> 1 unavail? No. adj to 0(color0) and 2(color0), so 0 unavail, pick 1)
    printf("%d\n", nc);
    // EXPECT: 2
    printf("%d %d %d %d\n", color[0], color[1], color[2], color[3]);
    // EXPECT: 0 1 0 1
    printf("%d\n", verify_coloring(&g, color));
    // EXPECT: 1

    // Complete graph K4: needs 4 colors
    graph_init(&g, 4);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 0, 2);
    graph_add_edge(&g, 0, 3);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 1, 3);
    graph_add_edge(&g, 2, 3);

    nc = greedy_color(&g, color);
    printf("%d\n", nc);
    // EXPECT: 4
    printf("%d %d %d %d\n", color[0], color[1], color[2], color[3]);
    // EXPECT: 0 1 2 3
    printf("%d\n", verify_coloring(&g, color));
    // EXPECT: 1
    printf("%d\n", max_degree(&g));
    // EXPECT: 3

    // Star graph: center 0 connected to 1,2,3,4
    graph_init(&g, 5);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 0, 2);
    graph_add_edge(&g, 0, 3);
    graph_add_edge(&g, 0, 4);

    nc = greedy_color(&g, color);
    // 0->0, 1->1(adj 0), 2->1(adj 0 only), 3->1(adj 0 only), 4->1(adj 0 only)
    printf("%d\n", nc);
    // EXPECT: 2
    printf("%d %d %d %d %d\n", color[0], color[1], color[2], color[3], color[4]);
    // EXPECT: 0 1 1 1 1
    printf("%d\n", verify_coloring(&g, color));
    // EXPECT: 1

    // Independent set: no edges, 1 color
    graph_init(&g, 4);
    nc = greedy_color(&g, color);
    printf("%d\n", nc);
    // EXPECT: 1
    printf("%d %d %d %d\n", color[0], color[1], color[2], color[3]);
    // EXPECT: 0 0 0 0

    // Petersen-like subgraph: 0--1, 0--2, 1--3, 2--3, 2--4, 3--4
    graph_init(&g, 5);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 0, 2);
    graph_add_edge(&g, 1, 3);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 2, 4);
    graph_add_edge(&g, 3, 4);

    nc = greedy_color(&g, color);
    // 0->0, 1->1(adj 0), 2->1(adj 0), 3->2(adj 1,2: colors 1,1 -> 0 avail? adj to 1(c=1) and 2(c=1), 0 is avail. Wait: color 0 not used by any neighbor of 3? Neighbors of 3: 1(c=1), 2(c=1), 4(uncolored). So 0 is available.
    // Actually: 3's neighbors are 1(c=1) and 2(c=1). So available[1]=0, available[0]=1. So color[3]=0.
    // 4: neighbors 2(c=1), 3(c=0). available[0]=0, available[1]=0, pick 2.
    printf("%d\n", nc);
    // EXPECT: 3
    printf("%d %d %d %d %d\n", color[0], color[1], color[2], color[3], color[4]);
    // EXPECT: 0 1 1 0 2
    printf("%d\n", verify_coloring(&g, color));
    // EXPECT: 1

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
