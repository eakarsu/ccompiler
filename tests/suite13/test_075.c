int printf(const char *fmt, ...);

// Connected components counting in undirected graph

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

// DFS to mark all nodes in a component
void dfs_mark(struct Graph *g, int v, int *comp, int label) {
    int i;
    comp[v] = label;
    for (i = 0; i < g->n; i = i + 1) {
        if (g->adj[v][i] && comp[i] == -1) {
            dfs_mark(g, i, comp, label);
        }
    }
}

// Find connected components, returns count
// comp[i] = component label for node i
int find_components(struct Graph *g, int *comp) {
    int i;
    int count;
    for (i = 0; i < g->n; i = i + 1) {
        comp[i] = -1;
    }
    count = 0;
    for (i = 0; i < g->n; i = i + 1) {
        if (comp[i] == -1) {
            dfs_mark(g, i, comp, count);
            count = count + 1;
        }
    }
    return count;
}

// Size of the component containing node v
int component_size(struct Graph *g, int v) {
    int comp[8];
    int i;
    int cnt;
    int label;
    find_components(g, comp);
    label = comp[v];
    cnt = 0;
    for (i = 0; i < g->n; i = i + 1) {
        if (comp[i] == label) cnt = cnt + 1;
    }
    return cnt;
}

// Find the largest component size
int largest_component(struct Graph *g) {
    int comp[8];
    int sizes[8];
    int nc;
    int i;
    int maxs;

    nc = find_components(g, comp);
    for (i = 0; i < nc; i = i + 1) {
        sizes[i] = 0;
    }
    for (i = 0; i < g->n; i = i + 1) {
        sizes[comp[i]] = sizes[comp[i]] + 1;
    }
    maxs = 0;
    for (i = 0; i < nc; i = i + 1) {
        if (sizes[i] > maxs) maxs = sizes[i];
    }
    return maxs;
}

// Check if two nodes are in the same component
int same_component(struct Graph *g, int u, int v) {
    int comp[8];
    find_components(g, comp);
    return comp[u] == comp[v];
}

// Count edges in the graph
int count_edges(struct Graph *g) {
    int i;
    int j;
    int cnt;
    cnt = 0;
    for (i = 0; i < g->n; i = i + 1) {
        for (j = i + 1; j < g->n; j = j + 1) {
            if (g->adj[i][j]) cnt = cnt + 1;
        }
    }
    return cnt;
}

// Check if graph is connected
int is_connected(struct Graph *g) {
    int comp[8];
    return find_components(g, comp) == 1;
}

int main() {
    struct Graph g;
    int comp[8];
    int nc;

    // Three components: {0,1,2}, {3,4}, {5}
    graph_init(&g, 6);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 3, 4);

    nc = find_components(&g, comp);
    printf("%d\n", nc);
    // EXPECT: 3

    // Components: 0,1,2 -> label 0; 3,4 -> label 1; 5 -> label 2
    printf("%d %d %d %d %d %d\n", comp[0], comp[1], comp[2], comp[3], comp[4], comp[5]);
    // EXPECT: 0 0 0 1 1 2

    printf("%d\n", is_connected(&g));
    // EXPECT: 0
    printf("%d\n", largest_component(&g));
    // EXPECT: 3
    printf("%d\n", component_size(&g, 0));
    // EXPECT: 3
    printf("%d\n", component_size(&g, 3));
    // EXPECT: 2
    printf("%d\n", component_size(&g, 5));
    // EXPECT: 1

    printf("%d\n", same_component(&g, 0, 2));
    // EXPECT: 1
    printf("%d\n", same_component(&g, 0, 3));
    // EXPECT: 0
    printf("%d\n", same_component(&g, 3, 4));
    // EXPECT: 1
    printf("%d\n", same_component(&g, 4, 5));
    // EXPECT: 0

    printf("%d\n", count_edges(&g));
    // EXPECT: 3

    // Connect all: add 2-3 and 4-5
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 4, 5);

    nc = find_components(&g, comp);
    printf("%d\n", nc);
    // EXPECT: 1
    printf("%d\n", is_connected(&g));
    // EXPECT: 1
    printf("%d\n", largest_component(&g));
    // EXPECT: 6
    printf("%d\n", count_edges(&g));
    // EXPECT: 5

    // Single node
    graph_init(&g, 1);
    printf("%d\n", is_connected(&g));
    // EXPECT: 1

    // All isolated: 8 nodes, no edges
    graph_init(&g, 8);
    nc = find_components(&g, comp);
    printf("%d\n", nc);
    // EXPECT: 8
    printf("%d\n", largest_component(&g));
    // EXPECT: 1

    // Complete graph K4 (nodes 0-3): one component
    graph_init(&g, 4);
    graph_add_edge(&g, 0, 1);
    graph_add_edge(&g, 0, 2);
    graph_add_edge(&g, 0, 3);
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 1, 3);
    graph_add_edge(&g, 2, 3);

    printf("%d\n", is_connected(&g));
    // EXPECT: 1
    printf("%d\n", count_edges(&g));
    // EXPECT: 6

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
